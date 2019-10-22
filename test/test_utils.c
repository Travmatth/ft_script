#include <stdio.h>
#include <fcntl.h>
#include "../includes/ft_script.h"

int		test_get_env_var(char *const envp[]) {
	char	*val;

	if ((val = get_env_var(envp, "FOO")) && !ft_strequ(NULL, val))
		return (EXIT_FAILURE);
	else if ((val = get_env_var(envp, "SHELL")) && !ft_strequ("/bin/zsh", val))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		test_find_exec(char *const envp[]) {
	char	buf[BUFSIZ];

	if (find_executable(buf, "/bin/ls", envp) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	else if (find_executable(buf, "supervisor", envp) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	else if (find_executable(buf, "/usr/ls", envp) == EXIT_SUCCESS)
		return (EXIT_FAILURE);
	else if (find_executable(buf, "./ft_script", envp) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	else if (find_executable(buf, "./foo", envp) == EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		verify_context(t_context *ctx, char **commands, char *file, int flags) {
	int		current;
	int		at_test;
	int		i = 0;

	for (; commands[i]; i++) {
		if (!ft_strequ(ctx->command[i], commands[i]))
			return (EXIT_FAILURE);
	}
	if ((ctx->command[i]))
		return (EXIT_FAILURE);
	at_test = (flags & FLAG_QUIET);
	current = (ctx->flags & FLAG_QUIET);
	if (current != at_test)
		return (EXIT_FAILURE);
	at_test = (flags & FLAG_APPEND);
	current = (ctx->flags & FLAG_APPEND);
	if (current != at_test)
		return (EXIT_FAILURE);
	if (!ft_strequ(file, ctx->ts_name))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		test_parse_args(char *const envp[]) {
	t_context	ctx;

	char	*single_arg[] = { "./ft_script", NULL };
	char	*single_arg_cmd[] = { "/bin/zsh", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	parse_args(&ctx, 1, single_arg, envp);
	if (verify_context(&ctx, single_arg_cmd, "typescript", 0) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	char	*typescript_arg[] = { "./ft_script", "ts_file", NULL };
	char	*typescript_arg_cmd[] = { "/bin/zsh", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	parse_args(&ctx, 2, typescript_arg, envp);
	if (verify_context(&ctx, typescript_arg_cmd, "ts_file", 0) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	char	*ts_prog[] = { "./ft_script", "ts_file", "ls", NULL };
	char	*ts_prog_cmd[] = { "ls", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	parse_args(&ctx, 3, ts_prog, envp);
	if (verify_context(&ctx, ts_prog_cmd, "ts_file", 0) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	char	*ts_single_flag_args[] = { "./ft_script", "-q", "ts_file", "ls", NULL };
	char	*ts_single_flag_args_cmd[] = { "ls", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	parse_args(&ctx, 4, ts_single_flag_args, envp);
	if (verify_context(&ctx, ts_single_flag_args_cmd, "ts_file", FLAG_QUIET) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	char	*ts_multi_flag_args[] = { "./ft_script", "ts_file", "ls", NULL };
	char	*ts_multi_flag_args_cmd[] = { "ls", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	parse_args(&ctx, 3, ts_multi_flag_args, envp);
	if (verify_context(&ctx, ts_multi_flag_args_cmd, "ts_file", 0) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	char	*ts_multi_opt_args[] = { "./ft_script", "ts_file", "ls", "-la", NULL };
	char	*ts_multi_opt_args_cmd[] = { "ls", "-la", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	parse_args(&ctx, 4, ts_multi_opt_args, envp);
	if (verify_context(&ctx, ts_multi_opt_args_cmd, "ts_file", 0) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	char	*ts_multi_flag_opts_args[] = { "./ft_script", "-q", "-a", "ts_file", "ls", "-la", NULL };
	char	*ts_multi_flag_opts_args_cmd[] = { "ls", "-la", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	parse_args(&ctx, 6, ts_multi_flag_opts_args, envp);
	if (verify_context(&ctx, ts_multi_flag_opts_args_cmd, "ts_file", FLAG_QUIET|FLAG_APPEND) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		test_append_flag(char *const envp[]) {
	t_context	ctx;

	char	*args[] = { "./ft_script", "-a", NULL };
	ft_bzero(&ctx, sizeof(t_context));
	parse_args(&ctx, 2, args, envp);
	if (fcntl(ctx.typescript, F_GETFL) & O_APPEND)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_quiet_flag(char *const envp[]) {
	t_context	ctx;
	char		buf[10];
	int			filedes[2];

	ft_bzero(&ctx, sizeof(t_context));
	ctx.flags |= FLAG_QUIET;
	if ((ctx.typescript = pipe(filedes)) == ERROR)
		return (EXIT_FAILURE);
	else if (dup2(filedes[], STDIN_FILENO) == ERROR)
		return (EXIT_FAILURE);
	else if (dup2(filedes[], STDOUT_FILENO) == ERROR)
		return (EXIT_FAILURE);
	script_prologue(&ctx);
	int		bytes = read(STDIN_FILENO, &buf, sizeof(buf));
	if (bytes != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
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
	else
	{
		printf("Tests passing\n");
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
