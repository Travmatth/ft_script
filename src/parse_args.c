/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 14:03:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/17 17:12:49 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

static void		log_command(t_context *ctx)
{
	int		i;
	char	*log;
	char	*key;

	i = 0;
	log = ctx->flags & LOG_KEYS ? "-k" : "";
	key = ctx->flags & LOG_TIME ? "-t" : "";
	DEBUG_LOG("./ft_script %s %s", log, key);
	while (ctx->command && ctx->command[i])
	{
		DEBUG_LOG(" %s", ctx->command[i]);
		i += 1;
	}
	DEBUG_PRINT("\n");
}

static int		parse_command(t_context *ctx, char *argv[], char *envp[], int i)
{
	ctx->ts_name = argv[i] ? argv[i++] : "typescript";
	if ((ctx->typescript = open(ctx->ts_name, TS_PERMS, 0666)) == ERROR)
	{
		ft_dprintf(STDERR_FILENO, OPEN_ERR, ctx->ts_name, strerror(errno));
		return (EXIT_FAILURE);
	}
	if (argv[i])
	{
		ctx->command = &argv[i];
	}
	else
	{
		i = 0;
		while (envp[i] && ft_strncmp("SHELL", envp[i], 5))
			i += 1;
		argv[0] = !ft_strncmp("SHELL", envp[i], 5) ? &envp[i][6] : "/bin/sh";
		ctx->command = argv;
	}
	return (EXIT_SUCCESS);
}

int				parse_args(t_context *ctx, int argc, char *argv[], char *envp[])
{
	int		i;
	int		ret;

	i = 1;
	if (argc > 1)
		while (i <= 2)
		{
			if (ft_strequ("-t", argv[i]) || ft_strequ("-t", argv[i]))
				ctx->flags |= LOG_TIME;
			else if (ft_strequ("-k", argv[i]) || ft_strequ("-k", argv[i]))
				ctx->flags |= LOG_KEYS;
			else if (argv[i] && argv[i][0] == '-' && argv[i][1])
			{
				ft_printf("ft_script: illegal option -- %c\n", argv[i][1]);
				return (EXIT_FAILURE);
			}
			else
				break ;
			i += 1;
		}
	ret = parse_command(ctx, argv, envp, i);
	log_command(ctx);
	return (ret);
}
