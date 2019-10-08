/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/03 19:43:32 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/08 08:32:01 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../../libft/libft.h"

int		ft_asprintf(char **ret, const char *format, ...);
int		ft_dprintf(int fd, const char *format, ...);
int		ft_fprintf(FILE *stream, const char *format, ...);
int		ft_printf(const char *format, ...);
int		ft_snprintf(char *str, size_t size, const char *format, ...);
int		ft_sprintf(char *str, const char *format, ...);
int		ft_vasprintf(char **ret, const char *format, va_list ap);
int		ft_vdprintf(int fd, const char *format, va_list ap);
int		ft_vfprintf(FILE *stream, const char *format, va_list ap);
int		ft_vprintf(const char *format, va_list ap);
int		ft_vsnprintf(char *str, size_t size, const char *format, va_list ap);
int		ft_vsprintf(char *str, const char *format, va_list ap);
#endif
