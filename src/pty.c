/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 12:36:37 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/11 13:01:53 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/ft_script.h"

void	dup_close_stdio(int fd)
{
	int		duped;

	errno = 0;
	if ((duped = dup2(fd, STDIN_FILENO)) == -1)
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

int		open_pty_slave(char *slave_name)
{
	int		slave_fd;

	errno = 0;
	if ((slave_fd = open(slave_name, O_RDWR)) == -1)
	{
		DEBUG_LOG("Failed to open slave pty: %s\n", strerror(errno));
		return (-1);
	}
	return (slave_fd);
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
		if (setsid() == -1)
		{
			DEBUG_LOG("setsid failed: %s", strerror(errno));
			_exit(1);
		}
		else if ((slave_fd = open_pty_slave(slave_name)))
		{
			DEBUG_LOG("open_pty_slave failed: %s", strerror(errno));
			_exit(1);
		}
		close(master_fd);
		dup_close_stdio(slave_fd);
		return (0);
	}
	*fd = master_fd;
	return (pid);
}
