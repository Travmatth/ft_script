/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 14:03:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/20 14:42:56 by tmatthew         ###   ########.fr       */
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

int				parse_args(t_context *ctx, char *argv[], char *const envp[])
{
	char	*val;
	int		i;

	i = 1;
	ctx->ts_name = argv[i] ? argv[i++] : "typescript";
	if ((ctx->typescript = open(ctx->ts_name, TS_PERMS, 0666)) == ERROR)
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
