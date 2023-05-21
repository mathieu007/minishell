/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/21 12:43:50 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief counting prev dir occurence ex: ../../bin/test = 2
/// @return 
static int32_t	count_prev_dir(char *relative_path)
{
	int32_t	count;

	count = 0;
	if (!relative_path)
		return (0);
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
/// @brief try get the relative dir ex: ./mydir/mycmd
/// @param cmd_name 
/// @return 
static char	*try_get_relative_dir(char *cmd_name)
{
	static char	buffer[PATH_MAX + 1];
	char		*path;

	path = getcwd(&buffer[0], PATH_MAX + 1);
	if (path == NULL)
		perror("An error occur while triying to get the current working dir.");
	path = ft_strjoin(path, &cmd_name[1]);
	if (*cmd_name && *cmd_name == '.' && cmd_name[1] && cmd_name[1] == '/'
		&& access(path, F_OK | X_OK) == 0)
		return (path);
	free(path);
	return (NULL);
}

/// @brief try get the relative dir ex: ../../mydir/mycmd
/// @param cmd_name 
/// @return 
static char	*try_get_relative_dir2(char *cmd_name)
{
	static char	buffer[PATH_MAX + 1];
	char		*path;
	int32_t		count;

	count = count_prev_dir(cmd_name);
	if (count == 0)
		return (NULL);
	path = getcwd(&buffer[0], PATH_MAX + 1);
	path = join(path, cmd_name);
	path = remove_dir(path, count);
	if (access(path, F_OK | X_OK) == 0)
		return (path);
	return (NULL);
}

void	*free_split(char **split)
{
	char	**dup;

	if (!split)
		return (NULL);
	dup = split;
	while (*split)
	{
		free(*split);
		split++;
	}
	return (free(dup), NULL);
}

char	*try_get_ful_path_from_env_path(t_cmd *cmd)
{
	char		*path;
	char		*path2;
	char		**paths;
	char		**dup_paths;

	paths = parse_env_path(cmd->env_cpy);
	dup_paths = paths;
	path2 = NULL;
	while (*paths)
	{
		path = ft_strjoin(*paths, "/");
		path2 = ft_strjoin(path, cmd->name);
		free(path);
		if (access(path2, F_OK | X_OK) == 0)
			return (free_split(dup_paths), path2);
		paths++;
	}
	return (free_split(dup_paths), NULL);
}

/// @brief handling ./path/to/file and ../../path/to/file
/// @param cmd_name 
/// @return 
char	*get_full_path(t_cmd *cmd)
{
	//static char	buffer[PATH_MAX + 1];
	char		*path;
	char		*cmd_name;

	cmd_name = cmd->name;
	path = try_get_relative_dir(cmd_name);
	if (path)
		return (path);
	path = try_get_relative_dir2(cmd_name);
	if (path)
		return (path);
	path = try_get_ful_path_from_env_path(cmd);
	if (path)
		return (path);
	perror("Error while trying to get the path of the command.");
	return (free(path), NULL);
}
