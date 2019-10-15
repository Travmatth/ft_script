/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 23:29:01 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/14 18:20:47 by tmatthew         ###   ########.fr       */
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

void	manage_pty_parent(int pid, int master_fd, int typescript)
{
	int		current;
	int		bytes_read;
	char	buf[READ_BUF];
	char	c;

	errno = 0;
	current = 0;
	bytes_read = 0;
	while (1)
	{
		current = buffered_read(master_fd, &c/*buf*/, &bytes_read);
		if (write(STDOUT_FILENO, buf, current) != current)
			DEBUG_LOG("write to dst failed: %s\n", strerror(errno));
		if (write(typescript, buf, current) != current)
			DEBUG_LOG("write to typescript failed: %s\n", strerror(errno));
		else
			continue ;
		_exit(1);
	}
	kill(pid, SIGTERM);
}

void	manage_pty_child(int master_fd, int typescript)
{
	int		bytes_read;
	char	buf[READ_BUF];
	char	c;

	(void)master_fd;
	errno = 0;
	while (1)
	{
		if ((bytes_read = read(STDIN_FILENO, &c, 1)) == -1)
			DEBUG_LOG("read from src failed: %s\n", strerror(errno));
		else if (bytes_read == 0)
		{
			DEBUG_PRINT("bytes_read == 0, breaking\n");
			break ;
		}
		// else if (write(master_fd, buf, bytes_read) != bytes_read)
		// 	DEBUG_LOG("write to dst failed: %s\n", strerror(errno));
		else if (write(typescript, buf, bytes_read) != bytes_read)
			DEBUG_LOG("write to typescript failed: %s\n", strerror(errno));
		else
		{
			ft_printf("manage_pty_child: %c\n", c);
			continue ;
		}
		kill(getppid(), SIGTERM);
		_exit(1);
	}
	_exit(0);
}

void	manage_pty(int master_fd, int typescript)
{
	pid_t	pid;

	errno = 0;
	if ((pid = fork()) == -1)
	{
		DEBUG_LOG("pty fork failed: %s\n", strerror(errno));
		_exit(1);
	}
	else if (pid == 0)
		manage_pty_child(pid, master_fd);
	else
		manage_pty_parent(pid, master_fd, typescript);
}
