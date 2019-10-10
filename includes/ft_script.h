/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 15:51:03 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/10 12:10:03 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCRIPT_H
# define FT_SCRIPT_H

# ifndef LIBFT_H
#  include "../libftprintf/srcs/includes/ft_printf.h"
# endif

# include <fcntl.h>
# include <errno.h>
# include <string.h>

# define TYPESCRIPT_PERMS (O_WRONLY | O_CREAT | O_APPEND)
# define OPEN_ERR "ft_script: error opening %s due to error: %s\n"

enum	e_script_flags {
	LOG_TIME = (1u << 0),
	LOG_KEYS = (1u << 1),
	DFLT_SHELL = (1u << 2),
};

typedef	struct	s_context {
	unsigned	flags;
	int			typescript;
	char		*command;
	char		**args;
}				t_context;

/*
** parse_args.c
*/

int				parse_args(t_context *ctx
							, int argc
							, char *argv[]
							, char *envp[]);
#endif
