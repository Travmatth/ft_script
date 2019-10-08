/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 15:52:13 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/08 08:37:29 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

int		init_program(t_context *ctx, int argc, char *argv[], char *envp[])
{
	(void)ctx;
	(void)argv;
	(void)envp;
	if (argc < 3)
	{
		ft_putendl("Usage: ./ft_script <TYPESCRIPT> <COMMAND>");
		return (0);
	}
	return (1);
}

int		main(int argc, char *argv[], char *envp[])
{
	t_context	ctx;

	if (!init_program(&ctx, argc, argv, envp))
		return (1);
	return (0);
}
