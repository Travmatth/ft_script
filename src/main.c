/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 15:52:13 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/19 13:37:02 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

int		main(int argc, char *argv[], char *envp[])
{
	t_context	ctx;
	char		slave_name[20];
	int			pid;
	int			master_fd;

	(void)argc;
	if (parse_args(&ctx, argv, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if ((pid = open_pty(&ctx, &master_fd, slave_name)) == ERROR)
		return (EXIT_FAILURE);
	else if (pid == 0 && manage_exec(&ctx, envp) == ERROR)
		return (EXIT_FAILURE);
	else
		manage_pty(&ctx, master_fd);
	return (EXIT_SUCCESS);
}
