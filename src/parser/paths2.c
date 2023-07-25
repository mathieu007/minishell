/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 08:21:59 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_dir(char *path, t_cmd *cmd)
{
	if (is_a_directory(path))
	{
		write_err2(126, cmd->name, ": Is a directory\n");
		return (free(path), NULL);
	}
	return (path);
}

/// @brief handling ./path/to/file and ../../path/to/file
/// @param cmd_name
/// @return
char	*get_full_path(t_cmd *cmd)
{
	char	*path;

	path = try_get_relative_dir(cmd);
	if (!path)
		path = try_get_relative_dir2(cmd);
	if (!path)
		path = try_get_full_path_from_env_path(cmd);
	if (cmd->name && cmd->name[0] == '/')
	{
		free_ptr(path);
		path = ft_strdup(cmd->name);
	}
	if (path && is_a_directory(path))
	{
		write_err2(126, cmd->name, ": Is a directory\n");
		return (free(path), NULL);
	}
	if (path && path[0] == '/' && access(path, F_OK) != 0)
		write_err2(127, cmd->name, ":  No such file or directory\n");
	else if (path && path[0] == '/' && access(path, X_OK) != 0)
		write_err2(126, cmd->name, ":  Permission denied\n");
	if (path)
		return (path);
	if (ft_strchr(cmd->name, '/'))
		write_err2(127, cmd->name, ":  No such file or directory\n");
	else
		write_err2(127, cmd->name, ": command not found\n");
	return (NULL);
}
