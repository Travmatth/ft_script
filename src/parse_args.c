/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 14:03:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/16 13:09:51 by tmatthew         ###   ########.fr       */
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
		DEBUG_LOG(" %s", ctx->command[i++]);
	DEBUG_PRINT("\n");
}

static int		parse_command(t_context *ctx, char *argv[], char *envp[], int i)
{
	char	*file;
	int		j;

	file = argv[i] ? argv[i++] : "typescript";
	if ((ctx->typescript = open(file, TYPESCRIPT_PERMS, 0666)) == -1)
	{
		ft_dprintf(STDERR_FILENO, OPEN_ERR, file, strerror(errno));
		return (EXIT_FAILURE);
	}
	if (argv[i])
	{
		ctx->command = &argv[i];
		return (EXIT_SUCCESS);
	}
	else
		i -= 1;
	j = 0;
	while (envp[j] && ft_strncmp("SHELL", envp[j], 5))
		j += 1;
	argv[0] = !ft_strncmp("SHELL", envp[j], 5) ? &envp[j][6] : "/bin/sh";
	ctx->command = argv;
	ctx->flags |= DFLT_SHELL;
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
