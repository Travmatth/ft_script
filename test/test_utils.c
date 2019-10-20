#include <stdio.h>
#include "../includes/ft_script.h"

int		test_get_env_var(char *const envp[])
{
	char	*val;

	if ((val = get_env_var(envp, "FOO")) && !ft_strequ(NULL, val))
		return (EXIT_FAILURE);
	else if ((val = get_env_var(envp, "SHELL")) && !ft_strequ("/bin/zsh", val))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		test_find_exec(char *const envp[])
{
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

int		test_parse_args(char *const envp[])
{
	t_context	ctx;

	char	*single_arg[] = { "./ft_script", NULL };
	parse_args(&ctx, 1, single_arg, envp);

	char	*typescript_arg[] = { "./ft_script", "ts_file", NULL };
	parse_args(&ctx, 2, typescript_arg, envp);

	char	*ts_prog_args[] = { "./ft_script", "ts_file", "ls", NULL };
	parse_args(&ctx, 2, ts_prog_args, envp);
	return (EXIT_SUCCESS);
}

int		main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;

	if (test_get_env_var(envp) == EXIT_FAILURE)
	{
		printf("test_get_env_var failed\n");
		return (EXIT_FAILURE);
	}
	else if (test_find_exec(envp) == EXIT_FAILURE)
	{
		printf("test_find_exec failed\n");
		return (EXIT_FAILURE);
	}
	else if (test_parse_args(envp) == EXIT_FAILURE)
	{
		printf("test_parse_args failed\n");
		return (EXIT_FAILURE);
	}
	printf("Tests passing\n");
	return (EXIT_SUCCESS);
}
