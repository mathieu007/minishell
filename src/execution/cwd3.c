/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 18:52:58 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*recursive_search_helper(DIR *dir, ino_t ino, char *path)
{
	struct dirent	*entry;
	char			*new_path;
	t_process		*proc;

	proc = get_process();
	entry = readdir(dir);
	if (!entry || (entry->d_name[0] == '.' && entry->d_name[1] == '.'))
		return (closedir(dir), free(path), NULL);
	while (entry)
	{
		new_path = join_path(path, entry->d_name);
		if (entry->d_ino == ino)
		{
			proc->cwd = free_ptr(proc->cwd);
			proc->cwd = ft_strdup(new_path);
			return (closedir(dir), free(path), free(new_path), proc->cwd);
		}
		if (recursive_search_helper(dir, ino, new_path))
			return (closedir(dir), free(path), free(new_path), proc->cwd);
		free(new_path);
		entry = readdir(dir);
		if (!entry || (entry->d_name[0] == '.' && entry->d_name[1] == '.'))
			break ;
	}
	return (closedir(dir), free(path), NULL);
}

char	*recursive_search_dir(char *path, ino_t ino)
{
	DIR		*dir;
	char	*result;

	if (path == NULL)
		return (NULL);
	dir = opendir(path);
	if (!dir)
		return (free(path), NULL);
	result = recursive_search_helper(dir, ino, path);
	return (result);
}

// char	*recursive_search_dir(char *path, ino_t ino)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	char			*new_path;
// 	t_process		*proc;

// 	if (path == NULL)
// 		return (NULL);
// 	dir = opendir(path);
// 	if (!dir)
// 		return (free(path), NULL);
// 	entry = readdir(dir);
// 	if (!entry || (entry->d_name[0] == '.' && entry->d_name[1] == '.'))
// 		return (closedir(dir), free(path), NULL);
// 	new_path = NULL;
// 	proc = get_process();
// 	while (entry)
// 	{
// 		new_path = join_path(path, entry->d_name);
// 		if (entry->d_ino == ino)
// 		{
// 			proc->cwd = free_ptr(proc->cwd);
// 			proc->cwd = ft_strdup(new_path);
// 			return (closedir(dir), free(path), free(new_path), proc->cwd);
// 		}
// 		if (recursive_search_dir(ft_strdup(new_path), ino))
// 			return (closedir(dir), free(path), free(new_path), proc->cwd);
// 		free(new_path);
// 		entry = readdir(dir);
// 		if (!entry || (entry->d_name[0] == '.' && entry->d_name[1] == '.'))
// 			break ;
// 	}
// 	return (closedir(dir), free(path), NULL);
// }