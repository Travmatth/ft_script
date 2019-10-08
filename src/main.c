/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 15:52:13 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/08 11:56:11 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

int		init_context(t_context *ctx, int argc, char *argv[])
{
	char	*file;

	if (argc < 3)
	{
		ft_putendl("Usage: ./ft_script [-k] [-t] <TYPESCRIPT> <COMMAND>");
		return (0);
	}
	file = argv[argc - 2];
	if (ft_strequ("-t", file) || ft_strequ("-k", file))
		ft_putendl("ft_script error: no typescript file specified");
	else if ((ctx->typescript = open(file, TYPESCRIPT_PERMS, 0666)) == -1)
		ft_printf(FD_ERR, file, strerror(errno));
	else
	{
		ctx->command = argv[argc - 1];
		if (ft_strequ("-t", argv[1]) || ft_strequ("-t", argv[2]))
			ctx->flags |= LOG_TIME;
		if (ft_strequ("-k", argv[1]) || ft_strequ("-k", argv[2]))
			ctx->flags |= LOG_KEYS;
		return (1);
	}
	return (0);
}

int		main(int argc, char *argv[])
{
	t_context	ctx;

	if (!init_context(&ctx, argc, argv))
		return (1);
	return (0);
}
