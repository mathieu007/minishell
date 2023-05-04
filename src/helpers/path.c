/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/03 18:34:22 by math             ###   ########.fr       */
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

/// @brief return true if the file path is executable,
/// else false.
/// @param path
/// @return
static bool	file_is_exec(char *path_file)
{
	if (path_file[0] == '/' && access(path_file, F_OK | X_OK) == 0)
		return (true);
	return (false);
}

/// @brief counting prev dir occurence ex: ../../bin/test = 2
/// @return 
static int32_t	count_prev_dir(char *relative_path)
{
	int32_t	count;

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

static char	*remove_dir(char *path, int32_t dir_count)
{
	char	*new_path;
	int32_t	i;
	int32_t	last_index;
	int32_t	len;

	i = 0;
	len = ft_strlen(path);
	last_index = len - 1;
	while (i < dir_count)
	{
		while (last_index >= 0)
			last_index--;
		i++;
	}
	new_path = ft_substr(path, 0, last_index + 1);
	return (free(path), new_path);
}

/// @brief handling ./path/to/file and ../../path/to/file
/// @param cmd_name 
/// @return 
char	*get_full_path(char *cmd_name)
{
	static char	buffer[PATH_MAX + 1];
	char		*path;
	int32_t		count;

	path = getcwd(&buffer[0], PATH_MAX + 1);
	if (path == NULL)
		perror("An error occur while triying to get the current working dir.");
	path = join(path, &cmd_name[1]);
	if (*cmd_name && cmd_name[0] == '.' && cmd_name[1] && cmd_name[1] == '/'
		&& access(path, F_OK | X_OK) == 0)
		return (path);
	free(path);
	count = count_prev_dir(cmd_name);
	path = getcwd(&buffer[0], PATH_MAX + 1);
	path = join(path, cmd_name);
	path = remove_dir(path, count);
	if (access(path, F_OK | X_OK) == 0)
		return (path);
	return (free(path), NULL);
}