/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 15:52:13 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/14 17:46:16 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

int		main(int argc, char *argv[], char *envp[])
{
	t_context	ctx;
	char		slave_name[20];
	int			pid;
	int			master_fd;

	if (parse_args(&ctx, argc, argv, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if ((pid = open_pty(&master_fd, slave_name)) == -1)
		return (EXIT_FAILURE);
	else if (pid == 0 && manage_exec(&ctx, envp) == -1)
		return (EXIT_FAILURE);
	else
		manage_pty(master_fd, ctx.typescript);
	return (EXIT_SUCCESS);
}
