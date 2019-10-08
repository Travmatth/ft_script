/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 15:51:03 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/08 11:56:12 by tmatthew         ###   ########.fr       */
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
# define FD_ERR "ft_script error: cannot open typescript file %s with error: %s"

enum	e_script_flags {
	LOG_TIME = (1u << 0),
	LOG_KEYS = (1u << 1),
};

typedef	struct	s_context {
	unsigned	flags;
	int			typescript;
	char		*command;
}				t_context;

#endif
