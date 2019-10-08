/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/03 18:27:52 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/08 08:34:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/internal.h"

/*
** DESCRIPTION
** The printf() family of functions produces output according to a format as
** described below.  dprintf() and vdprintf() write output to the
** given file descriptor
**
** RETURN VALUES
** These functions return the number of characters printed (not including
** the trailing `\0' used to end output to strings)
** These functions return a negative value if an error occurs.
*/

int		ft_dprintf(int fd, const char *format, ...)
{
	va_list ap;
	int		result;

	va_start(ap, format);
	result = ft_vdprintf(fd, (char*)format, ap);
	va_end(ap);
	return (result);
}
