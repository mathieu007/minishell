/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directories.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/03 14:31:32 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*free_paths(char **paths)
{
	if (paths)
	{
		while (*paths)
		{
			free(*paths);
			*paths = NULL;
		}
	}
	if (paths != NULL)
		free(paths);
	return (NULL);
}

/// @brief If the file and path to file exist and is executable,
/// return true or false if not.
/// @param path
/// @return
bool	file_is_exec(char *path_file)
{
	if (path_file[0] == '/' && access(path_file, F_OK | X_OK) == 0)
		return (true);
	return (false);
}

/// @brief counting prev dir occurence ex: ../../bin/test = 2
/// @return 
static int32_t count_prev_dir(char *relative_path)
{
	int32_t count;
	
	count = 0;
	if (!relative_path)
		return (NULL);
	while (*relative_path)
	{
		if (*relative_path++ == '.'
			&& *relative_path && *relative_path++ == '.'
			&& *relative_path && *relative_path++ == '/')
			count++;						
		else
			break ;	
	}
	return (count);
}

char *get_full_path(char *rel_path)
{
	static char buffer[PATH_MAX + 1];
	char		*path;
	int32_t		count;
	
	path = getcwd(&buffer[0], PATH_MAX + 1);
	if (path == NULL)
		perror("An error occur while triying to get the current working dir.");
	path = join(path, &rel_path[1]);
	if (*rel_path && rel_path[0] == '.' && rel_path[1] && rel_path[1] == '/'
		&& access(path, F_OK | X_OK) == 0)
		return (path);
	free(path);	
	count = count_prev_dir(rel_path);
	path = getcwd(&buffer[0], PATH_MAX + 1);
	path = join_free2(path, ft_substr(path, count * 3, ft_strlen(rel_path) - count * 3));
	if (access(path, F_OK | X_OK) == 0)
		return (ft_strdup(path));
	return (free(path), NULL);
}