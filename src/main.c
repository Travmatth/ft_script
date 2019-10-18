/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 15:52:13 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/17 17:56:02 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

void	script_exit(t_context *ctx, int status)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &ctx->original_tty);
	_exit(status);
}

int		script_prologue(t_context *ctx)
{
	ft_putstr("Script started, output file is ");
	ft_putendl(ctx->ts_name);
	return (prep_pty(ctx, STDIN_FILENO));
}

void	script_epilogue(t_context *ctx)
{
	time_t	cur;

	time(&cur);
	ft_putstr_fd("Script done on ", ctx->typescript);
	ft_putendl_fd(ctime(&cur), ctx->typescript);
	tcsetattr(STDIN_FILENO, TCSANOW, &ctx->original_tty);
	ft_putstr("\nScript done, output file is ");
	ft_putendl(ctx->ts_name);
}

int		main(int argc, char *argv[], char *envp[])
{
	t_context	ctx;
	char		slave_name[20];
	int			pid;
	int			master_fd;

	if (parse_args(&ctx, argc, argv, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if ((pid = open_pty(&ctx, &master_fd, slave_name)) == ERROR)
		return (EXIT_FAILURE);
	else if (pid == 0 && manage_exec(&ctx, envp) == ERROR)
		return (EXIT_FAILURE);
	else
		manage_pty(&ctx, master_fd);
	return (EXIT_SUCCESS);
}
