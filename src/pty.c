/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 12:36:37 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/15 15:33:49 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/ft_script.h"

void	dup_close_stdio(int master_fd, int fd)
{
	int		duped;

	errno = 0;
	duped = -1;
	if (close(master_fd) == -1)
		DEBUG_LOG("error closing master_fd: %s", strerror(errno));
	else if ((duped = dup2(fd, STDIN_FILENO)) == -1)
		DEBUG_LOG("error duping slave to stdin: %s", strerror(errno));
	else if ((duped = dup2(fd, STDOUT_FILENO)) == -1)
		DEBUG_LOG("error duping slave to stdout: %s", strerror(errno));
	else if ((duped = dup2(fd, STDERR_FILENO)) == -1)
		DEBUG_LOG("error duping slave to stderr: %s", strerror(errno));
	if (duped == -1)
		_exit(1);
	else if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		close(fd);
}

int		open_pty_master(char *slave_name)
{
	int		master_pty;
	char	*slave;

	errno = 0;
	if ((master_pty = posix_openpt(O_RDWR)) == -1)
	{
		DEBUG_LOG("posix_openpt failed: %s", strerror(errno));
		return (-1);
	}
	else if (grantpt(master_pty) == -1)
		DEBUG_LOG("grantpt failed: %s", strerror(errno));
	else if (unlockpt(master_pty) == -1)
		DEBUG_LOG("unlockpt failed: %s", strerror(errno));
	else if (!(slave = ptsname(master_pty)))
		DEBUG_LOG("ptsname failed: %s", strerror(errno));
	else
	{
		slave_name[0] = '\0';
		ft_strcat(slave_name, slave);
		return (master_pty);
	}
	close(master_pty);
	return (-1);
}

int		open_pty(int *fd, char *slave_name)
{
	int		pid;
	int		master_fd;
	int		slave_fd;

	if (((master_fd = open_pty_master(slave_name)) == -1)
		|| ((pid = fork()) == -1))
		return (EXIT_FAILURE);
	else if (pid == 0)
	{
		errno = 0;
		if (setsid() == -1)
			DEBUG_LOG("setsid failed: %s\n", strerror(errno));
		else if ((slave_fd = open(slave_name, O_RDWR)) == -1)
			DEBUG_LOG("Failed to open slave pty: %s\n", strerror(errno));
		else
		{
			dup_close_stdio(master_fd, slave_fd);
			return (0);
		}
		_exit(1);
	}
	DEBUG_LOG("open pty opened master at: %d\n", master_fd);
	*fd = master_fd;
	return (pid);
}
