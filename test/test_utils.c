#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../includes/ft_script.h"

int		test_get_env_var(char *const envp[]) {
	char	*val;

	if ((val = get_env_var(envp, "FOO")) && !ft_strequ(NULL, val))
		return (EXIT_FAILURE);
	else if ((val = get_env_var(envp, "SHELL")) && !ft_strequ(getenv("SHELL"), val))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		test_find_exec(char *const envp[]) {
	char	buf[BUFSIZ];

	if (find_executable(buf, "/bin/ls", envp) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	else if (find_executable(buf, "norminette", envp) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	else if (find_executable(buf, "/usr/ls", envp) == EXIT_SUCCESS)
		return (EXIT_FAILURE);
	else if (find_executable(buf, "./ft_script", envp) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	else if (find_executable(buf, "./foo", envp) == EXIT_SUCCESS)
		return (EXIT_FAILURE);
	else if (find_executable(buf, "foo", envp) == EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		verify_context(t_context *ctx, char **commands, char *file, int flags) {
	int		i = 0;

	for (; commands[i]; i++) {
		if (!ft_strequ(ctx->command[i], commands[i]))
			return (EXIT_FAILURE);
	}
	if ((ctx->command[i]))
		return (EXIT_FAILURE);
	if ((ctx->flags & FLAG_QUIET) != (flags & FLAG_QUIET))
		return (EXIT_FAILURE);
	if ((ctx->flags & FLAG_APPEND) != (flags & FLAG_APPEND))
		return (EXIT_FAILURE);
	if (!ft_strequ(file, ctx->ts_name))
		return (EXIT_FAILURE);
	close(ctx->typescript);
	return (EXIT_SUCCESS);
}

int		test_parse_args(char *const envp[]) {
	t_context	ctx;

	char	*single_arg[] = { "./ft_script", NULL };
	char	*single_arg_cmd[] = { getenv("SHELL"), NULL };
	ft_bzero(&ctx, sizeof(t_context));
	if (parse_args(&ctx, 1, single_arg, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (verify_context(&ctx, single_arg_cmd, "typescript", 0) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	unlink("typescript");

	char	*wrong_flag[] = { "./ft_script", "-z",  NULL };
	int		orig = dup(STDOUT_FILENO);
	close(STDOUT_FILENO);
	ft_bzero(&ctx, sizeof(t_context));
	if (parse_args(&ctx, 2, wrong_flag, envp) != EXIT_FAILURE)
		return (EXIT_FAILURE);
	dup2(orig, STDOUT_FILENO);
	close(orig);

	char	*typescript_arg[] = { "./ft_script", "ts_file", NULL };
	char	*typescript_arg_cmd[] = { getenv("SHELL"), NULL };
	ft_bzero(&ctx, sizeof(t_context));
	if (parse_args(&ctx, 2, typescript_arg, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (verify_context(&ctx, typescript_arg_cmd, "ts_file", 0) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	unlink("ts_file");

	char	*ts_prog[] = { "./ft_script", "ts_file", "ls", NULL };
	char	*ts_prog_cmd[] = { "ls", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	if (parse_args(&ctx, 3, ts_prog, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (verify_context(&ctx, ts_prog_cmd, "ts_file", 0) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	unlink("ts_file");

	char	*ts_single_flag_args[] = { "./ft_script", "-q", "ts_file", "ls", NULL };
	char	*ts_single_flag_args_cmd[] = { "ls", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	if (parse_args(&ctx, 4, ts_single_flag_args, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (verify_context(&ctx, ts_single_flag_args_cmd, "ts_file", FLAG_QUIET) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	unlink("ts_file");

	char	*ts_multi_flag_args[] = { "./ft_script", "ts_file", "ls", NULL };
	char	*ts_multi_flag_args_cmd[] = { "ls", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	if (parse_args(&ctx, 3, ts_multi_flag_args, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (verify_context(&ctx, ts_multi_flag_args_cmd, "ts_file", 0) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	unlink("ts_file");

	char	*ts_multi_opt_args[] = { "./ft_script", "ts_file", "ls", "-la", NULL };
	char	*ts_multi_opt_args_cmd[] = { "ls", "-la", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	if (parse_args(&ctx, 4, ts_multi_opt_args, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (verify_context(&ctx, ts_multi_opt_args_cmd, "ts_file", 0) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	unlink("ts_file");

	char	*ts_multi_flag_opts_args[] = { "./ft_script", "-q", "-a", "ts_file", "ls", "-la", NULL };
	char	*ts_multi_flag_opts_args_cmd[] = { "ls", "-la", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	if (parse_args(&ctx, 6, ts_multi_flag_opts_args, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (verify_context(&ctx, ts_multi_flag_opts_args_cmd, "ts_file", FLAG_QUIET|FLAG_APPEND) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	unlink("ts_file");
	return (EXIT_SUCCESS);
}

int		test_append_flag(char *const envp[]) {
	t_context	ctx;

	char	*args_1[] = { "./ft_script", "-a", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	if (parse_args(&ctx, 2, args_1, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (!(fcntl(ctx.typescript, F_GETFL) & O_APPEND))
		return (EXIT_FAILURE);
	close(ctx.typescript);
	unlink("typescript");

	char	*args_2[] = { "./ft_script", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	if (parse_args(&ctx, 1, args_2, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if ((fcntl(ctx.typescript, F_GETFL) & O_APPEND))
		return (EXIT_FAILURE);
	close(ctx.typescript);
	unlink("typescript");
	return (EXIT_SUCCESS);
}

int		test_quiet_flag(void) {
	t_context	ctx;
	char		buf[10];
	int			filedes[2], orig_stdin, orig_stdout;

	ft_bzero(&ctx, sizeof(t_context));
	ctx.flags |= FLAG_QUIET;
	pipe(filedes);
	fcntl(filedes[0], F_SETFL, O_NONBLOCK);
	orig_stdin = dup(STDIN_FILENO);
	orig_stdout = dup(STDOUT_FILENO);
	dup2(filedes[1], STDIN_FILENO);
	dup2(filedes[0], STDOUT_FILENO);
	ctx.typescript = STDIN_FILENO;
	script_prologue(&ctx);
	script_epilogue(&ctx);
	errno = 0;
	int		bytes = read(filedes[0], &buf, sizeof(buf));
	int	status = errno;
	close(ctx.typescript);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	dup2(orig_stdin, STDIN_FILENO);
	dup2(orig_stdout, STDOUT_FILENO);
	if (bytes == -1 && status == EAGAIN)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;

	if (test_get_env_var(envp) == EXIT_FAILURE)
		printf("test_get_env_var failed\n");
	else if (test_find_exec(envp) == EXIT_FAILURE)
		printf("test_find_exec failed\n");
	else if (test_parse_args(envp) == EXIT_FAILURE)
		printf("test_parse_args failed\n");
	else if (test_append_flag(envp))
		printf("test_append_flag failed\n");
	else if (test_quiet_flag())
		printf("test_quiet_flag failed\n");
	else
	{
		printf("Tests passing\n");
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
