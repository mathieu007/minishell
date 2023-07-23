/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/23 18:27:19 by math             ###   ########.fr       */
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

bool	set_cwd(char *cwd)
{
	t_process	*proc;
	ino_t		dir_id;

	if (!cwd)
		return (false);
	proc = get_process();
	dir_id = get_dir_id(cwd);
	if (dir_id != 0)
	{
		proc->cwd = free_ptr(proc->cwd);
		proc->dir_id = dir_id;
		proc->cwd = cwd;
		return (true);
	}
	return (free_ptr(cwd), false);
}

/// @brief this function automatically get the updated current working directory
/// even if an other process modify the current working directory location
/// ex: rename, move, or delete the current workin dir.
/// There is multiple fall back layer, don't know if bash do it that way.
/// @param cmd
/// @return
char	*get_cwd(void)
{
	static char	buffer[PATH_MAX + 1];
	ino_t		dir_id;
	char		*home;
	t_process	*proc;
	char		*cur_dir;

	proc = get_process();
	if (!proc->cwd)
	{
		cur_dir = getcwd(&buffer[0], PATH_MAX + 1);
		if (!cur_dir)
			free_all_and_exit2(1,
								"An error occur while trying to get the current working dir.");
		set_cwd(ft_strdup(cur_dir));
		return (ft_strdup(proc->cwd));
	}
	dir_id = get_dir_id(proc->cwd);
	if (dir_id != 0 && proc->dir_id == dir_id)
		return (ft_strdup(proc->cwd));
	home = get_home();
	if (!home)
		free_all_and_exit2(1, "Could not find home dir.");
	proc->cwd = free_ptr(proc->cwd);
	proc->cwd = recursive_search_dir(ft_strdup(home), proc->dir_id);
	if (!proc->cwd)
	{
		free(home);
		free_all_and_exit2(1, "Could not find working dir.");
	}
	set_cwd(ft_strdup(proc->cwd));
	return (free(home), ft_strdup(proc->cwd));
}

char	*get_cwd_with_backslash(void)
{
	char	*cwd;

	cwd = get_cwd();
	if (cwd && !ft_strisempty(cwd) && cwd[ft_strlen(cwd) - 1] != '/')
		cwd = ft_strjoinfree(cwd, "/");
	return (cwd);
}
