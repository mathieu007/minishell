
#include "minishell.h"

char	*get_home(void)
{
	char	*env_value;

	env_value = get_env_value("HOME");
	if (!env_value)
		return (NULL);
	return (env_value);
}

// int32_t	init_cwd_fd(char *cwd)
// {
// 	static char	buffer[PATH_MAX + 1];
// 	char		*path;
// 	int32_t		fd;

// 	path = getcwd(&buffer[0], PATH_MAX + 1);
// 	fd = open(path, O_RDONLY, 0);
// 	return (fd);
// }

/// @brief All path must have been malloced
/// @param path1 
/// @param path2 
/// @return 
char	*join_path(char *path1, char *path2)
{
	char	*temp;

	temp = path1;
	path1 = ft_strjoin(temp, "/");
	free(temp);
	temp = ft_strjoin(path1, path2);
	free(path1);
	free(path2);
	return (temp);
}

char	*recursive_search_dir(t_cmd *cmd, char *path, ino_t ino)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*new_path;
	t_process		*proc;

	if (path == NULL)
		return (NULL);
	dir = opendir(path);
	entry = readdir(dir);
	new_path = NULL;
	proc = get_process();	
	while (entry)
	{
		new_path = join_path(path, entry->d_name);
		if (entry->d_ino == ino)
		{
			free(proc->cwd);
			proc->cwd = new_path;
			return (free(path), proc->cwd);
		}
		if (recursive_search_dir(cmd, new_path, ino))
			return (free(path), proc->cwd);
	}
	closedir(dir);
	if (new_path)
		free(new_path);
	return (free(path), NULL);
}

// void	set_cwd(t_cmd *cmd, char *cwd)
// {
	
// }

/// @brief this function automatically get the updated current working directory
/// even if an other process modify the current working directory location
/// ex: rename, move, or delete the current workin dir. 
/// There is multiple fall back layer, don't know if bash do it that way.
/// TODO need to also add the trash bin dir
/// @param cmd 
/// @return 
char	*get_cwd(t_cmd *cmd)
{
	static char		buffer[PATH_MAX + 1];
	struct stat		file_stat;
	char			*home;
	t_process		*proc;

	proc = get_process();
	if (cmd == NULL || proc->cwd == NULL)
	{
		proc->cwd = getcwd(&buffer[0], PATH_MAX + 1);
		return (proc->cwd);
	}
	if (stat(proc->cwd, &file_stat) != 0)
	{
		perror("fstat() error");
		return (free_all_and_exit(EXIT_FAILURE));
	}
	if (proc->dir_id == 0)
		proc->dir_id = file_stat.st_ino;
	if (file_stat.st_ino == proc->dir_id)
		return (proc->cwd);
	home = get_home();
	if (home)
		proc->cwd = recursive_search_dir(cmd, ft_strdup(home), file_stat.st_ino);
	//TODO if (!cwd) search trash bin or return an error msg do the same but for trash bin.
	if (!proc->cwd)
		proc->cwd = getcwd(&buffer[0], PATH_MAX + 1);
	return (proc->cwd);
}
