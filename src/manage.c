/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 23:29:01 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/20 15:14:25 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

int		manage_exec(t_context *ctx, char *const envp[])
{
	char	path[BUFSIZ];

	errno = 0;
	if (find_executable(path, ctx->command[0], envp) == EXIT_SUCCESS)
		execve(path, ctx->command, envp);
	else
	{
		ft_putstr_fd("ft_script: ", ctx->typescript);
		ft_putstr_fd(ctx->command[0], ctx->typescript);
		ft_putstr_fd(": No such file or directory\n", ctx->typescript);
		ft_putstr("ft_script: ");
		ft_putstr(ctx->command[0]);
		ft_putstr(": No such file or directory\n");
	}
	ft_putstr_fd("ft_script: ", ctx->typescript);
	ft_putstr_fd(ctx->command[0], ctx->typescript);
	ft_putstr_fd(" : ", ctx->typescript);
	ft_putendl_fd(strerror(errno), ctx->typescript);
	return (ERROR);
}

int		prep_pty(t_context *ctx, int fd)
{
	char			*err;
	struct termios	slave_term;

	errno = 0;
	if (tcgetattr(fd, &ctx->original_tty))
	{
		err = strerror(errno);
		DEBUG_LOG("Error getting slave terminal settings: %s\n", err);
		return (EXIT_FAILURE);
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
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int		read_terminal_input(int master_fd, char buf[BUFSIZ])
{
	ssize_t	bytes;

	if ((bytes = read(STDIN_FILENO, buf, BUFSIZ)) == ERROR)
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

	if ((bytes = read(master_fd, buf, BUFSIZ - 2)) == ERROR)
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

	if (script_prologue(ctx) == EXIT_FAILURE)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &ctx->original_tty);
		return ;
	}
	while (1)
	{
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
		FD_SET(master_fd, &fds);
		if (select(master_fd + 1, &fds, NULL, NULL, NULL) == ERROR)
			script_exit(ctx, EXIT_FAILURE);
		if (FD_ISSET(STDIN_FILENO, &fds) && read_terminal_input(master_fd, buf))
			break ;
		if (FD_ISSET(master_fd, &fds) && write_pty_output(ctx, master_fd, buf))
			break ;
	}
	script_epilogue(ctx);
}
