/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifecycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 12:31:28 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/21 16:40:18 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

char	*get_time(t_context *ctx)
{
	struct timeval	timestamp;

	if (gettimeofday(&timestamp, NULL) == ERROR)
		script_exit(ctx, EXIT_FAILURE);
	return (ctime(&timestamp.tv_sec));
}

void	script_exit(t_context *ctx, int status)
{
	ioctl(STDIN_FILENO, TIOCSETA, &ctx->original_tty);
	_exit(status);
}

void		script_prologue(t_context *ctx)
{
	int		i;

	i = 0;
	if (!(ctx->flags & FLAG_QUIET))
	{
		ft_putstr_fd("Script started on ", ctx->typescript);
		ft_putstr_fd(get_time(ctx), ctx->typescript);
		ft_putstr_fd("command:", ctx->typescript);
		ft_putstr("Script started, output file is ");
		ft_putendl(ctx->ts_name);
		while (ctx->command && ctx->command[i])
		{
			ft_putstr_fd(" ", ctx->typescript);
			ft_putstr_fd(ctx->command[i++], ctx->typescript);
		}
		ft_putstr_fd("\n", ctx->typescript);
	}
}

void	script_epilogue(t_context *ctx)
{
	ioctl(STDIN_FILENO, TIOCSETA, &ctx->original_tty);
	if (!(ctx->flags & FLAG_QUIET))
	{
		ft_putstr_fd("\nScript done on ", ctx->typescript);
		ft_putstr_fd(get_time(ctx), ctx->typescript);
		ft_putstr("\nScript done, output file is ");
		ft_putstr(ctx->ts_name);
		ft_putstr("\n");
	}
}
