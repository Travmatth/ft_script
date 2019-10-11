/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 15:52:13 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/11 13:05:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

void	move_fd_contents(int src, int dst, int typescript)
{
	int		bytes_read;
	char	buf[BUFSIZ];

	while (1)
	{
		errno = 0;
		if ((bytes_read = read(src, buf, BUFSIZ)) == -1)
		{
			DEBUG_LOG("read from src failed: %s\n", strerror(errno));
			_exit(1);
		}
		else if (bytes_read == 0)
			break ;
		if (write(dst, buf, bytes_read) != bytes_read)
		{
			DEBUG_LOG("write to dst failed: %s\n", strerror(errno));
			_exit(1);
		}
		else if (write(typescript, buf, bytes_read) != bytes_read)
		{
			DEBUG_LOG("write to typescript failed: %s\n", strerror(errno));
			_exit(1);
		}
	}
	DEBUG_PRINT("EOF detected, ended reading from stdin\n");
}

int		manage_exec(t_context *ctx, char *envp[])
{
	errno = 0;
	execve(ctx->command, ctx->args, envp);
	DEBUG_LOG("execve failed: %s\n", strerror(errno));
	return (-1);
}

void	manage_pty(t_context *ctx, int master_fd)
{
	pid_t	pid;

	errno = 0;
	if ((pid = fork()) == -1)
	{
		DEBUG_LOG("pty fork failed: %s\n", strerror(errno));
		_exit(1);
	}
	else if (pid == 0)
	{
		move_fd_contents(STDIN_FILENO, master_fd, ctx->typescript);
		exit(0);
	}
	move_fd_contents(master_fd, STDOUT_FILENO, ctx->typescript);
}

int		main(int argc, char *argv[], char *envp[])
{
	t_context	ctx;
	char		slave_name[20];
	int			pid;
	int			master_fd;

	if (parse_args(&ctx, argc, argv, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if ((pid = open_pty(&master_fd, slave_name)) == -1)
		return (EXIT_FAILURE);
	else if (pid == 0 && manage_exec(&ctx, envp) == -1)
		return (EXIT_FAILURE);
	manage_pty(&ctx, master_fd);
	return (EXIT_SUCCESS);
}
