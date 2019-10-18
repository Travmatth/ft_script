/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 23:29:01 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/17 17:10:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

int		manage_exec(t_context *ctx, char *envp[])
{
	time_t	cur;

	errno = 0;
	time(&cur);
	ft_dprintf(ctx->typescript, "Script started on %s", ctime(&cur));
	execve(ctx->command[0], ctx->command, envp);
	ft_printf("ft_script: %s: %s\n", ctx->command[0], strerror(errno));
	return (-1);
}

void	prep_pty(t_context *ctx, int fd)
{
	char			*err;
	struct termios	slave_term;

	errno = 0;
	if (tcgetattr(fd, &ctx->original_tty))
	{
		err = strerror(errno);
		DEBUG_LOG("Error getting slave terminal settings: %s\n", err);
		script_exit(ctx, EXIT_FAILURE);
	}
	slave_term = ctx->original_tty;
	slave_term.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP);
	slave_term.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON);
	slave_term.c_oflag &= ~(OPOST);
	slave_term.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	slave_term.c_cflag &= ~(CSIZE | PARENB);
	slave_term.c_cflag |= CS8;
	if (tcsetattr(fd, TCSANOW, &slave_term))
	{
		err = strerror(errno);
		DEBUG_LOG("Error setting slave terminal settings: %s\n", err);
		script_exit(ctx, EXIT_FAILURE);
	}
}

int		read_terminal_input(int master_fd, char buf[BUFSIZ])
{
	ssize_t	bytes;

	if ((bytes = read(STDIN_FILENO, buf, BUFSIZ)) == -1)
		return (1);
	else if (bytes == 0)
		return (1);
	else if (write(master_fd, buf, bytes) != bytes)
		return (1);
	return (0);
}

int		write_pty_output(t_context *ctx, int master_fd, char buf[BUFSIZ])
{
	ssize_t	bytes;

	if ((bytes = read(master_fd, buf, BUFSIZ - 2)) == -1)
		return (1);
	else if (bytes == 0)
		return (1);
	buf[bytes] = '\n';
	buf[bytes + 1] = '\0';
	if (write(STDOUT_FILENO, buf, bytes) != bytes)
		return (1);
	else if (write(ctx->typescript, buf, bytes) != bytes)
		return (1);
	return (0);
}

void	manage_pty(t_context *ctx, int master_fd)
{
	fd_set	fds;
	char	buf[BUFSIZ];
	time_t	cur;

	ft_printf("Script started, output file is %s\n", ctx->ts_name);
	prep_pty(ctx, STDIN_FILENO);
	while (1)
	{
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
		FD_SET(master_fd, &fds);
		if (select(master_fd + 1, &fds, NULL, NULL, NULL) == -1)
			script_exit(ctx, EXIT_FAILURE);
		if (FD_ISSET(STDIN_FILENO, &fds) && read_terminal_input(master_fd, buf))
			break ;
		if (FD_ISSET(master_fd, &fds) && write_pty_output(ctx, master_fd, buf))
			break ;
	}
	time(&cur);
	ft_dprintf(ctx->typescript, "Script done on %s", ctime(&cur));
	tcsetattr(STDIN_FILENO, TCSANOW, &ctx->original_tty);
	ft_printf("\nScript done, output file is %s\n", ctx->ts_name);
}
