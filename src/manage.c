/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 23:29:01 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/23 16:26:26 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

/*
** in the child fork of the program, execute the specified program
** @param {t_context*} ctx - program context
** @param {char *const[]} envp - environment variables given to program
** @return {int} none on success, 1 otherwise
*/

int		manage_exec(t_context *ctx, char *const envp[])
{
	char	path[BUFSIZ];

	errno = 0;
	if (find_executable(path, ctx->command[0], envp) == EXIT_SUCCESS)
		execve(path, ctx->command, envp);
	else if (!(ctx->flags & FLAG_QUIET))
	{
		ft_putstr_fd("ft_script: ", ctx->typescript);
		ft_putstr_fd(ctx->command[0], ctx->typescript);
		ft_putstr_fd(": No such file or directory\n", ctx->typescript);
		ft_putstr("ft_script: ");
		ft_putstr(ctx->command[0]);
		ft_putstr(": No such file or directory\n");
	}
	if (!(ctx->flags & FLAG_QUIET))
	{
		ft_putstr_fd("ft_script: ", ctx->typescript);
		ft_putstr_fd(ctx->command[0], ctx->typescript);
		ft_putstr_fd(" : ", ctx->typescript);
		ft_putendl_fd(strerror(errno), ctx->typescript);
	}
	return (EXIT_FAILURE);
}

/*
** in the parent fork of the program, set terminal to raw mode, ignoring signals
** in order to pass to the pty
** @param {t_context*} ctx - program context
** @param {int}_fd - fd to set in raw mode
** @return {int} 0 on success, 1 otherwise
*/

int		prep_pty(t_context *ctx, int fd)
{
	char			*err;
	struct termios	slave_term;

	errno = 0;
	if (ioctl(fd, TIOCGETA, &ctx->original_tty))
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
	if (ioctl(fd, TIOCSETA, &slave_term))
	{
		err = strerror(errno);
		DEBUG_LOG("Error setting slave terminal settings: %s\n", err);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/*
** when user has written to stdin, data is copied to pty master fd
** @param {int} master_fd - master fd of the pty pair
** @param {char[BUFSIZ]} buf - buffer to use in copying data
** @return {int} 0 on success, 1 otherwise
*/

int		read_terminal_input(int master_fd, char buf[BUFSIZ])
{
	ssize_t	bytes;

	if ((bytes = read(STDIN_FILENO, buf, BUFSIZ)) == ERROR)
		return (EXIT_FAILURE);
	else if (bytes == 0)
		return (EXIT_FAILURE);
	else if (write(master_fd, buf, bytes) != bytes)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
** when pty has data on master pty fd, data is written to stdout and typescript
** @param {t_context*} ctx - program context
** @param {int} master_fd - master fd of the pty pair
** @param {char[BUFSIZ]} buf - buffer to use in copying data
** @return {int} 0 on success, 1 otherwise
*/

int		write_pty_output(t_context *ctx, int master_fd, char buf[BUFSIZ])
{
	ssize_t	bytes;

	if ((bytes = read(master_fd, buf, BUFSIZ - 2)) == ERROR)
		return (EXIT_FAILURE);
	else if (bytes == 0)
		return (EXIT_FAILURE);
	buf[bytes] = '\n';
	buf[bytes + 1] = '\0';
	if (write(STDOUT_FILENO, buf, bytes) != bytes)
		return (EXIT_FAILURE);
	else if (write(ctx->typescript, buf, bytes) != bytes)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
** print stdin/typescript notices if specified, then watch pty master and slave,
** writing to pty slave (the specified program or shell) if user entered data,
** or the stdout and typescript if pty has data available. print notice of
** stdout/typescript on exit
** @param {t_context*} ctx - program context
** @param {int} master_fd - master fd of the pty pair
*/

void	manage_pty(t_context *ctx, int master_fd)
{
	fd_set	fds;
	char	buf[BUFSIZ];

	script_prologue(ctx);
	if (prep_pty(ctx, STDIN_FILENO) == EXIT_FAILURE)
	{
		ioctl(STDIN_FILENO, TIOCSETA, &ctx->original_tty);
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
	if (ioctl(STDIN_FILENO, TIOCSETA, &ctx->original_tty))
		return ;
	script_epilogue(ctx);
}
