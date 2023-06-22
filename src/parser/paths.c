
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
static char	*try_get_relative_dir(t_cmd *cmd)
{
	char	*path;

	path = get_cwd(cmd);
	if (path == NULL)
		perror("An error occur while triying to get the current working dir.");
	path = ft_strjoin(path, cmd->name);
	if (cmd->name && cmd->name[0] == '.' && cmd->name[1] == '/' && access(path,
			F_OK | X_OK) == 0)
		return (path);
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
	path = get_cwd(cmd);
	path = join(path, cmd->name);
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

char	*try_get_full_path_from_env_path(t_cmd *cmd)
{
	char	*path;
	char	**paths;
	char	**dup_paths;
	char	*path_free1;


	paths = get_env_path();
	path = ft_strdup("");
	dup_paths = paths;
	if (!paths)
		return (NULL);
	while (*paths)
	{
		path_free1 = path;
		path = ft_strjoin(*paths, "/");
		path = ft_strjoinfree(path, cmd->name);
		if (access(path, F_OK | X_OK) == 0)
			return (free_split(dup_paths), free(path_free1), path);
		if(path_free1)
			free(path_free1);
		paths++;
	}
	if (path)
		free(path);
	return (free_split(dup_paths), NULL);
}

/// @brief handling ./path/to/file and ../../path/to/file
/// @param cmd_name
/// @return
char	*get_full_path(t_cmd *cmd)
{
	char		*path;
	t_process	*proc;

	proc = get_process();
	path = try_get_relative_dir(cmd);
	if (path)
		return (path);
	path = try_get_relative_dir2(cmd);
	if (path)
		return (path);
	path = try_get_full_path_from_env_path(cmd);
	if (path)
		return (path);
	proc->errnum = 127;
	write_err2(127, cmd->name, ": command not found\n");
	return (NULL);
}
