/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 14:03:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/23 13:23:21 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

char			*get_env_var(char *const envp[], const char *name)
{
	int		i;
	int		found;
	int		len;
	char	*value;

	i = 0;
	found = 0;
	value = NULL;
	len = LEN(name, 0);
	while (envp[i])
	{
		if (!ft_strncmp(name, envp[i], len))
		{
			found = 1;
			break ;
		}
		else
			i += 1;
	}
	if (found)
		value = (char*)&envp[i][len + 1];
	return (value);
}

static int		parse_command(t_context *ctx
							, char *argv[]
							, char *const envp[]
							, int i)
{
	char	*val;
	int		perms;

	perms = ctx->flags & FLAG_APPEND ? TS_PERMS_APP : TS_PERMS_NEW;
	if (argv[i])
	{
		ctx->ts_name = argv[i];
		argv[i++] = NULL;
	}
	else
		ctx->ts_name = "typescript";
	if ((ctx->typescript = open(ctx->ts_name, perms, 0666)) == ERROR)
		ft_dprintf(STDERR_FILENO, OPEN_ERR, ctx->ts_name, strerror(errno));
	else if (argv[i])
		ctx->command = &argv[i];
	else
	{
		val = get_env_var(envp, "SHELL");
		ctx->command = NULL;
		argv[0] = val ? val : "/bin/sh";
		ctx->command = argv;
	}
	return (ctx->typescript == ERROR ? EXIT_FAILURE : EXIT_SUCCESS);
}

int				parse_args(t_context *ctx
							, int argc
							, char *argv[]
							, char *const envp[])
{
	int		i;

	i = 1;
	if (argc > 1)
		while (i <= 2)
		{
			if (ft_strequ("-a", argv[i]) || ft_strequ("-a", argv[i]))
				ctx->flags |= FLAG_APPEND;
			else if (ft_strequ("-q", argv[i]) || ft_strequ("-q", argv[i]))
				ctx->flags |= FLAG_QUIET;
			else if (argv[i] && argv[i][0] == '-' && argv[i][1])
			{
				ft_printf("ft_script: illegal option -- %c\n", argv[i][1]);
				return (EXIT_FAILURE);
			}
			else
				break ;
			argv[i++] = NULL;
		}
	return (parse_command(ctx, argv, envp, i));
}
