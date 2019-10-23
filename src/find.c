/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 18:45:18 by tmatthew          #+#    #+#             */
/*   Updated: 2019/10/23 13:45:22 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_script.h"

/*
** use path variable to visit specified folders and search for executable
** @param {char*} buf - buffer to contain full executable path if found
** @param {char*} exec - executable file to locate
** @param {char*} path - path env var containing strings to walk
** @return {int} 0 if found, 1 otherwise
*/

int		walk_paths(char buf[BUFSIZ], char *exec, char *path)
{
	char	*start;
	char	*end;

	start = path;
	while (1)
	{
		if (!start || !start[0])
			return (EXIT_FAILURE);
		buf[0] = '\0';
		if (!(end = ft_strchr(start, ':')))
			ft_strcat(buf, start);
		else
		{
			ft_memcpy(buf, start, end - start);
			buf[end - start] = '\0';
		}
		ft_strcat(buf, "/");
		ft_strcat(buf, exec);
		if (!access(buf, X_OK))
			return (EXIT_SUCCESS);
		start = end ? end + 1 : end;
	}
}

/*
** attempt to locate specified executable and verify said file has exec rights
** @param {char*} buf - buffer to contain full executable path if found
** @param {char*} exec - executable file to locate
** @param {char**} envp - environment variables given to program
** @return {int} 0 if found, 1 otherwise
*/

int		find_executable(char buf[BUFSIZ], char *exec, char *const envp[])
{
	char	*path;

	if (!exec)
		return (EXIT_FAILURE);
	else if (exec[0] == '/' || exec[0] == '.')
	{
		path = ft_strnequ("./", exec, 2) ? &exec[2] : &exec[0];
		if (access(path, X_OK))
			return (EXIT_FAILURE);
		ft_memcpy(buf, exec, ft_strlen(exec));
		return (EXIT_SUCCESS);
	}
	if (!(path = get_env_var(envp, "PATH")))
		return (EXIT_FAILURE);
	return (walk_paths(buf, exec, path));
}
