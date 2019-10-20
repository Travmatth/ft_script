/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifecycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 12:31:28 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/20 15:14:48 by tmatthew         ###   ########.fr       */
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
	int		i;
	time_t	cur;

	i = 0;
	time(&cur);
	ft_putstr_fd("Script started on ", ctx->typescript);
	ft_putstr_fd(ctime(&cur), ctx->typescript);
	ft_putstr_fd("command:", ctx->typescript);
	ft_putstr("Script started, output file is ");
	ft_putendl(ctx->ts_name);
	while (ctx->command && ctx->command[i])
	{
		ft_putstr_fd(" ", ctx->typescript);
		ft_putstr_fd(ctx->command[i++], ctx->typescript);
	}
	ft_putstr_fd("\n", ctx->typescript);
	return (prep_pty(ctx, STDIN_FILENO));
}

void	script_epilogue(t_context *ctx)
{
	time_t	cur;

	tcsetattr(STDIN_FILENO, TCSANOW, &ctx->original_tty);
	time(&cur);
	ft_putstr_fd("\nScript done on ", ctx->typescript);
	ft_putstr_fd(ctime(&cur), ctx->typescript);
	ft_putstr("\nScript done, output file is ");
	ft_putstr(ctx->ts_name);
	ft_putstr("\n");
}
