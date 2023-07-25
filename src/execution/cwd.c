/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 13:17:35 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	dir_exist(const char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (true);
	}
	else
		return (false);
}

char	*get_home(void)
{
	char	*env_value;

	env_value = get_env_value("HOME");
	if (!env_value)
		return (NULL);
	return (env_value);
}

/// @brief All path must have been malloced
/// @param path1
/// @param path2
/// @return
char	*join_path(char *path1, char *path2)
{
	char	*path;

	path = ft_strjoin(path1, "/");
	path = ft_strjoinfree(path, path2);
	return (path);
}

ino_t	get_dir_id(char *dir)
{
	struct stat	file_stat;

	if (dir_exist(dir))
	{
		if (stat(dir, &file_stat) != 0)
			free_all_and_exit2(errno, "stat error");
		return (file_stat.st_ino);
	}
	return (0);
}
