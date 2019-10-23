/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifecycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 12:31:28 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/23 12:57:07 by tmatthew         ###   ########.fr       */
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

int		str_ends_with(char *str)
{
	int		len;

	if (!(len = LEN(str, 0)))
		return (0);
	return (ft_strequ(&str[len > 2 ? len - 2 : len], "sh"));
}

void	script_prologue(t_context *ctx)
{
	int		i;
	int		is_shell;

	i = 0;
	if (!(ctx->flags & FLAG_QUIET))
	{
		ft_putstr_fd("Script started on ", ctx->typescript);
		ft_putstr_fd(get_time(ctx), ctx->typescript);
		if (!(is_shell = str_ends_with(ctx->command[0])))
			ft_putstr_fd("command:", ctx->typescript);
		ft_putstr("Script started, output file is ");
		ft_putendl(ctx->ts_name);
		if (!is_shell)
		{
			while (ctx->command && ctx->command[i])
			{
				ft_putstr_fd(" ", ctx->typescript);
				ft_putstr_fd(ctx->command[i++], ctx->typescript);
			}
			ft_putstr_fd("\n", ctx->typescript);
		}
	}
}

void	script_epilogue(t_context *ctx)
{
	if (!(ctx->flags & FLAG_QUIET))
	{
		ft_putstr_fd("\nScript done on ", ctx->typescript);
		ft_putstr_fd(get_time(ctx), ctx->typescript);
		ft_putstr("\nScript done, output file is ");
		ft_putstr(ctx->ts_name);
		ft_putstr("\n");
	}
}
