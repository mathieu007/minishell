/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/01 10:39:55 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_path_is_execve(char *full_path_name)
{
	if (!full_path_name)
		return (false);
	if (full_path_name && is_a_directory(full_path_name))
		return (false);
	if (access(full_path_name, F_OK) != 0)
		return (false);
	if (access(full_path_name, X_OK) != 0)
		return (false);
	return (true);
}

bool	check_path_exist(char *full_path_name)
{
	if (!full_path_name)
		return (false);
	if (full_path_name && is_a_directory(full_path_name))
		return (false);
	if (access(full_path_name, F_OK) != 0)
		return (false);
	return (true);
}

char	*get_shell_path(char *name)
{
	char	*path;

	path = get_path(name);
	if (name && name[0] == '/')
	{
		free_ptr(path);
		path = ft_strdup(name);
	}
	if (path && is_a_directory(path))
		return (free(path), NULL);
	if (path && path[0] == '/' && access(path, F_OK) != 0)
		return (free(path), NULL);
	else if (path && path[0] == '/' && access(path, X_OK) != 0)
		return (free(path), NULL);
	if (path)
		return (path);
	return (NULL);
}
