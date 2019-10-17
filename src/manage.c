/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 23:29:01 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/16 17:02:48 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

int		manage_exec(t_context *ctx, char *envp[])
{
	errno = 0;
	DEBUG_LOG("execve: %s\n", ctx->command[0]);
	execve(ctx->command[0], ctx->command, envp);
	ft_printf("ft_script: %s: %s", ctx->command[0], , strerror(errno));
	return (-1);
}

void	prep_pty(t_context *ctx, int fd)
{
	struct termios	slave_term;

	errno = 0;
	if (tcgetattr(fd, &ctx->original_tty))
	{
		ft_printf("Error getting slave terminal settings: %s", strerror(errno));
		script_exit(ctx, EXIT_FAILURE);
	}
	slave_term = ctx->original_tty;
	slave_term.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	slave_term.c_iflag &= ~(BRKINT | ICRNL | IGNBRK | IGNCR | INLCR | INPCK | ISTRIP | IXON | PARMRK);
	slave_term.c_oflag &= ~(OPOST);
	slave_term.c_cc[VMIN] = 1;
	slave_term.c_cc[VTIME] = 0;
	if (tcsetattr(fd, TCSANOW, &slave_term))
	{
		ft_printf("Error getting slave terminal settings: %s", strerror(errno));
		script_exit(ctx, EXIT_FAILURE);
	}
}

void	read_terminal_input(t_context *ctx, int master_fd, char buf[BUFSIZ])
{
	ssize_t	bytes;

	if ((bytes = read(STDIN_FILENO, buf, BUFSIZ)) == -1)
		script_exit(ctx, EXIT_FAILURE);
	else if (bytes == 0)
		script_exit(ctx, EXIT_SUCCESS);
	else if (write(master_fd, buf, bytes) != bytes)
		script_exit(ctx, EXIT_FAILURE);
}

void	write_pty_output(t_context *ctx, int master_fd, char buf[BUFSIZ])
{
	ssize_t	bytes;

	if ((bytes = read(master_fd, buf, BUFSIZ)) == -1)
		script_exit(ctx, EXIT_SUCCESS);
	else if (bytes == 0)
		script_exit(ctx, EXIT_SUCCESS);
	buf[bytes] = '\n';
	buf[bytes + 1] = '\0';
	if (write(STDOUT_FILENO, buf, bytes) != bytes)
		script_exit(ctx, EXIT_FAILURE);
	else if (write(ctx->typescript, buf, bytes) != bytes)
		script_exit(ctx, EXIT_FAILURE);
}

void	manage_pty(t_context *ctx, int master_fd)
{
	fd_set	fds;
	char	buf[BUFSIZ];

	prep_pty(ctx, STDIN_FILENO);
	while (1)
	{
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
		FD_SET(master_fd, &fds);
		if (select(master_fd + 1, &fds, NULL, NULL, NULL) == -1)
			script_exit(ctx, EXIT_FAILURE);
		if (FD_ISSET(STDIN_FILENO, &fds))
			read_terminal_input(ctx, master_fd, buf);
		if (FD_ISSET(master_fd, &fds))
			write_pty_output(ctx, master_fd, buf);
	}
}
