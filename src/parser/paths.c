
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
		if (*relative_path++ == '.' && *relative_path && *relative_path++ == '.'
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

	len = ft_strlen(path);
	last_index = len - 1;
	i = last_index;
	if (path[i] == '/')
		i--;
	while (i >= 0 && path[i])
	{
		if (path[i] == '/')
		{
			dir_count--;
			if (dir_count == 0)
			{
				last_index = i;
				break ;
			}
		}
		i--;
	}
	new_path = ft_substr(path, 0, last_index + 1);
	return (free(path), new_path);
}
/// @brief try get the relative dir ex: ./mydir/mycmd
/// @param cmd_name
/// @return
static char	*try_get_relative_dir(t_cmd *cmd)
{
	char	*path;

	path = get_cwd();
	if (path == NULL)
		perror("An error occur while triying to get the current working dir.");
	if (cmd->name && cmd->name[0] == '.' && cmd->name[1] == '/' && access(path,
			F_OK | X_OK) == 0)
	{
		path = ft_strjoinfree(path, &cmd->name[2]);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
	}
	free(path);
	return (NULL);
}

/// @brief try get the relative dir ex: ../../mydir/mycmd
/// @param cmd_name
/// @return
static char	*try_get_relative_dir2(t_cmd *cmd)
{
	char	*path;
	int32_t	count;

	count = count_prev_dir(cmd->name);
	if (count == 0)
		return (NULL);
	path = get_cwd();
	path = remove_dir(path, count);
	if (access(path, F_OK | X_OK) == 0)
	{
		if (ft_strlen(path) > 0 && path[ft_strlen(path) - 1] != '/')
			path = ft_strjoinfree(path, "/");
		path = ft_strjoinfree(path, &cmd->name[count * 3]);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
	}
	return (free(path), NULL);
}

char	*try_get_full_path_from_env_path(t_cmd *cmd)
{
	char	*path;
	char	**paths;
	char	**dup_paths;

	paths = get_env_path();
	path = NULL;
	dup_paths = paths;
	if (!paths)
		return (NULL);
	while (*paths)
	{
		path = ft_strjoin(*paths, "/");
		path = ft_strjoinfree(path, cmd->name);
		if (access(path, F_OK | X_OK) == 0)
			return (free_2d_char_array(dup_paths), path);
		free(path);
		paths++;
	}
	return (free_2d_char_array(dup_paths), NULL);
}

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
	if (path && is_a_directory(path))
	{
		write_err2(126, cmd->name, ": Is a directory\n");
		return (free(path), NULL);
	}
	if (path)
		return (path);
	if (ft_strchr(cmd->name, '/'))
		write_err2(127, cmd->name, ":  No such file or directory\n");
	else
		write_err2(127, cmd->name, ": command not found\n");
	return (NULL);
}
