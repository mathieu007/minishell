/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 21:12:03 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char *name)
{
	char	*path;

	path = try_get_relative_dir(name);
	if (!path)
		path = try_get_relative_dir2(name);
	if (!path)
		path = try_get_full_path_from_env_path(name);
	return (path);
}

char	*get_file_path_name(char *name)
{
	char	*path;

	path = try_get_relative_dir(name);
	if (!path)
		path = try_get_relative_dir2(name);
	if (!path && name[0] != '/')
		path = ft_strjoinfree(get_cwd_with_backslash(), name);
	if (path[0] == '/')
		return (path);
	return (ft_strdup(name));
}

char	*get_file_path(char *name)
{
	char	*path;

	path = get_file_path_name(name);
	ft_printf("name : %s\n", path);
	if (name && name[0] == '/')
	{
		free_ptr(path);
		path = ft_strdup(name);
	}
	if (path && is_a_directory(path))
	{
		write_err2(126, name, ": Is a directory\n");
		return (free(path), NULL);
	}
	if (path && path[0] == '/' && access(path, F_OK) != 0)
		write_err2(127, name, ": No such file or directory\n");
	else if (path && path[0] == '/' && access(path, X_OK) != 0)
		write_err2(126, name, ": Permission denied\n");
	if (path)
		return (path);
	if (ft_strchr(name, '/'))
		write_err2(127, name, ": No such file or directory\n");
	else
		write_err2(127, name, ": command not found\n");
	return (NULL);
}

char	*get_full_path(char *name)
{
	char	*path;

	path = get_path(name);
	if (name && name[0] == '/')
	{
		free_ptr(path);
		path = ft_strdup(name);
	}
	if (path && is_a_directory(path))
	{
		write_err2(126, name, ": Is a directory\n");
		return (free(path), NULL);
	}
	if (path && path[0] == '/' && access(path, F_OK) != 0)
		write_err2(127, name, ": No such file or directory\n");
	else if (path && path[0] == '/' && access(path, X_OK) != 0)
		write_err2(126, name, ": Permission denied\n");
	if (path)
		return (path);
	if (ft_strchr(name, '/'))
		write_err2(127, name, ": No such file or directory\n");
	else
		write_err2(127, name, ": command not found\n");
	return (NULL);
}
