/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 14:03:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/11 13:01:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

static int		parse_command(t_context *ctx, char *argv[], char *envp[], int i)
{
	char	*file;

	file = argv[i] ? argv[i++] : "typescript";
	if ((ctx->typescript = open(file, TYPESCRIPT_PERMS, 0666)) == -1)
	{
		DEBUG_LOG(OPEN_ERR, file, strerror(errno));
		return (EXIT_FAILURE);
	}
	if (argv[i])
	{
		ctx->command = argv[i++];
		ctx->args = &argv[i];
		return (EXIT_SUCCESS);
	}
	i = 0;
	while (envp[i] && ft_strncmp("SHELL", envp[i], 5))
		i += 1;
	if (!ft_strncmp("SHELL", envp[i], 5))
		ctx->command = &envp[i][6];
	else
		ctx->command = "/bin/sh";
	ctx->args = NULL;
	ctx->flags |= DFLT_SHELL;
	return (EXIT_SUCCESS);
}

static void		log_command(t_context *ctx)
{
	int		i;
	char	*log;
	char	*key;

	log = ctx->flags & LOG_KEYS ? "-k" : "";
	key = ctx->flags & LOG_TIME ? "-t" : "";
	DEBUG_LOG("./ft_script %s %s %s", log, key, ctx.command);
	i = 0;
	while (ctx->args && ctx->args[i])
		DEBUG_LOG(" %s", ctx->args[i]);
	DEBUG_PRINT("\n");
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
				ft_printf("ft_script: illegal option -- %c", argv[i][1]);
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
