/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/03 18:27:52 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/08 08:33:54 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/internal.h"

/*
** The printf() family of functions produces output according to a format as
** described below.  The printf() and vprintf() functions write output to
** stdout, the standard output stream;
**
** RETURN VALUES
** These functions return the number of characters printed (not including
** the trailing `\0' used to end output to strings)
** These functions return a negative value if an error occurs.
*/

static int		yield_fd(t_buf *b, void *p)
{
	if (!b)
		return (0);
	return (write(*(int*)p, (char*)(b->buf), b->current));
}

int				ft_vprintf(const char *format, va_list ap)
{
	int		result;
	int		fd;

	fd = 1;
	result = vfmt_buffer(yield_fd, (void*)&fd, (char*)format, ap);
	return (result);
}
