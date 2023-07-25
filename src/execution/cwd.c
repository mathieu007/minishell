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

char	*recursive_search_dir(char *path, ino_t ino)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*new_path;
	t_process		*proc;

	if (path == NULL)
		return (NULL);
	dir = opendir(path);
	if (!dir)
		return (free(path), NULL);
	entry = readdir(dir);
	if (!entry || (entry->d_name[0] == '.' && entry->d_name[1] == '.'))
		return (closedir(dir), free(path), NULL);
	new_path = NULL;
	proc = get_process();
	while (entry)
	{
		new_path = join_path(path, entry->d_name);
		if (entry->d_ino == ino)
		{
			proc->cwd = free_ptr(proc->cwd);
			proc->cwd = ft_strdup(new_path);
			return (closedir(dir), free(path), free(new_path), proc->cwd);
		}
		if (recursive_search_dir(ft_strdup(new_path), ino))
			return (closedir(dir), free(path), free(new_path), proc->cwd);
		free(new_path);
		entry = readdir(dir);
		if (!entry || (entry->d_name[0] == '.' && entry->d_name[1] == '.'))
			break ;
	}
	return (closedir(dir), free(path), NULL);
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
