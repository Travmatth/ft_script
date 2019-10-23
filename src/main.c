/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 15:52:13 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/23 14:28:40 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

/*
** parse input, open pty pairs, copy data from pty to terminal & vice versa
** @param {int} argc - number of arguments passed to program
** @param {*char[]} argv - the arguments passed to program
** @param {*const envp[]} envp - the environment variables passed to program
*/

int		main(int argc, char *argv[], char *envp[])
{
	t_context	ctx;
	char		slave_name[20];
	int			pid;
	int			master_fd;

	ctx.flags = 0;
	if (parse_args(&ctx, argc, argv, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if ((pid = open_pty(&ctx, &master_fd, slave_name)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	else if (pid == 0 && manage_exec(&ctx, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	else
		manage_pty(&ctx, master_fd);
	return (EXIT_SUCCESS);
}
