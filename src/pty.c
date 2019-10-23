/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 12:36:37 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/23 14:22:34 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

/*
** in the child process set slave pty fd as stdin/out/err, close master fd
** @param {char*} slave_name - location to save slave pty name
** @param {int} master_fd - master fd to close in the child
** @param {int} fd - fd to set as stdin/out/err in the child process
*/

void	dup_close_stdio(t_context *ctx, int master_fd, int fd)
{
	int		duped;

	errno = 0;
	duped = -1;
	if (close(master_fd) == ERROR)
		DEBUG_LOG("error closing master_fd: %s\n", strerror(errno));
	else if ((duped = dup2(fd, STDIN_FILENO)) == ERROR)
		DEBUG_LOG("error duping slave to stdin: %s\n", strerror(errno));
	else if ((duped = dup2(fd, STDOUT_FILENO)) == ERROR)
		DEBUG_LOG("error duping slave to stdout: %s\n", strerror(errno));
	else if ((duped = dup2(fd, STDERR_FILENO)) == ERROR)
		DEBUG_LOG("error duping slave to stderr: %s\n", strerror(errno));
	if (duped == ERROR)
		script_exit(ctx, EXIT_FAILURE);
	else if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		close(fd);
}

/*
** open, unlock a master pty, obtain slave pty name
** @param {char*} slave_name - location to save slave pty name
** @return {int} master pyt fd if opened successfully, 1 otherwise
*/

int		open_pty_master(char *slave_name)
{
	int		master_pty;
	char	slave[BUFSIZ];

	errno = 0;
	if ((master_pty = open("/dev/ptmx", O_RDWR)) == ERROR)
	{
		DEBUG_LOG("posix_openpt failed: %s\n", strerror(errno));
		return (EXIT_FAILURE);
	}
	else if (ioctl(master_pty, TIOCPTYGRANT) == ERROR)
		DEBUG_LOG("grantpt failed: %s\n", strerror(errno));
	else if (ioctl(master_pty, TIOCPTYUNLK) == ERROR)
		DEBUG_LOG("unlockpt failed: %s\n", strerror(errno));
	else if (ioctl(master_pty, TIOCPTYGNAME, slave) == ERROR)
		DEBUG_LOG("ptsname failed: %s\n", strerror(errno));
	else
	{
		slave_name[0] = '\0';
		ft_strcat(slave_name, slave);
		return (master_pty);
	}
	close(master_pty);
	return (EXIT_FAILURE);
}

/*
** attempts to set up pseudo-terminal and save slave pty name, master fd
** @param {t_context*} ctx - program context
** @param {int*} fd - location to save master fd
** @param {char*} slave_name - location to save slave pty name
** @return {int} child pid if launch successful, 1 otherwise
*/

int		open_pty(t_context *ctx, int *fd, char *slave_name)
{
	int		pid;
	int		master_fd;
	int		slave_fd;

	errno = 0;
	if (((master_fd = open_pty_master(slave_name)) == EXIT_FAILURE)
		|| ((pid = fork()) == ERROR))
		return (EXIT_FAILURE);
	else if (pid == 0)
	{
		if (setsid() == ERROR)
			DEBUG_LOG("setsid failed: %s\n", strerror(errno));
		else if ((slave_fd = open(slave_name, O_RDWR)) == ERROR)
			DEBUG_LOG("Failed to open slave pty: %s\n", strerror(errno));
		else
		{
			dup_close_stdio(ctx, master_fd, slave_fd);
			return (EXIT_SUCCESS);
		}
		script_exit(ctx, EXIT_FAILURE);
	}
	DEBUG_LOG("open pty opened master at: %d\n", master_fd);
	*fd = master_fd;
	return (pid);
}
