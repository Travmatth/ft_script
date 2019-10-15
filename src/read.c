/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 17:43:06 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/14 18:20:34 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

// int		buffered_read(int master_fd, char buf[READ_BUF], int *bytes_read)
int		buffered_read(int master_fd, char *c, int *bytes_read)
{
	int		current;

	// if ((current = read(master_fd, buf, READ_BUF)) == -1)
	if ((current = read(master_fd, c, 1)) == -1)
	{
		DEBUG_LOG("read from src failed: %s\n", strerror(errno));
		_exit(1);
	}
	ft_printf("buffered_read: %c\n", c);
	*bytes_read += current;
	return (current);
}
