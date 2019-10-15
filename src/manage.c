/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 23:29:01 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/15 15:23:14 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

int		manage_exec(t_context *ctx, char *envp[])
{
	errno = 0;
	DEBUG_LOG("execve: %s\n", ctx->command[0]);
	execve(ctx->command[0], ctx->command, envp);
	DEBUG_LOG("execve failed: %s\n", strerror(errno));
	return (-1);
}

void	prep_pty(int fd)
{
	struct termios	slave_term;

	errno = 0;
	if (tcgetattr(fd, &slave_term))
	{
		ft_printf("Error getting slave terminal settings: %s", strerror(errno));
		_exit(1);
	}
	slave_term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
	slave_term.c_oflag &= ~(ONLCR);
	slave_term.c_cc[VMIN] = 1;
	slave_term.c_cc[VTIME] = 0;
	if (tcsetattr(fd, TCSANOW, &slave_term))
	{
		ft_printf("Error getting slave terminal settings: %s", strerror(errno));
		_exit(1);
	}
}

void	manage_pty(int master_fd, int typescript)
{
	(void)master_fd;
	(void)typescript;
}
