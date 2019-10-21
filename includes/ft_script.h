/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 15:51:03 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/21 01:18:00 by tmatthew         ###   ########.fr       */
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
# include <signal.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <sys/time.h>

# define TS_PERMS (O_WRONLY | O_CREAT | O_TRUNC)
# define OPEN_ERR "ft_script: error opening %s due to error: %s\n"
# define PASS (void)0

enum				e_script_flags {
	LOG_TIME = (1u << 0),
	LOG_KEYS = (1u << 1),
	DFLT_SHELL = (1u << 2),
};

typedef	struct		s_context {
	unsigned		flags;
	int				typescript;
	char			**command;
	struct termios	original_tty;
	char			*ts_name;
}					t_context;

/*
** parse_args.c
*/

char				*get_env_var(char *const envp[], const char *name);
int					parse_args(t_context *ctx, char *argv[], char *const envp[]);

/*
** pty.c
*/

int					open_pty(t_context *ctx, int *fd, char *slave_name);
int					manage_exec(t_context *ctx, char *const envp[]);

/*
** manage.c
*/

int					prep_pty(t_context *ctx, int fd);
void				manage_pty(t_context *ctx, int master_fd);

/*
** find.c
*/

int					find_executable(char path[BUFSIZ], char *exec, char *const envp[]);

/*
** lifecycle.c
*/

void				script_exit(t_context *ctx, int status);
int					script_prologue(t_context *ctx);
void				script_epilogue(t_context *ctx);

/*
** Debug statements used when compiled with __DEBUG__ variable defined
*/

# ifdef __DEBUG__
#  define DEBUG_LOG(fmt, ...) ft_dprintf(STDERR_FILENO, fmt, __VA_ARGS__)
#  define DEBUG_PRINT(str) ft_dprintf(STDERR_FILENO, str)
# else
#  define DEBUG_LOG(fmt, ...) PASS
#  define DEBUG_PRINT(str) PASS
# endif

#endif
