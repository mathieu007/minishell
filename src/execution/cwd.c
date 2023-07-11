
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
	char	*path;

	path = ft_strjoin(path1, "/");
	path = ft_strjoin(path, path2);
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
		return (NULL);
	entry = readdir(dir);
	if (!entry || (entry->d_name[0] == '.' && entry->d_name[1] == '.'))
		return (closedir(dir), NULL);
	new_path = NULL;
	proc = get_process();
	while (entry)
	{
		new_path = join_path(path, entry->d_name);
		if (entry->d_ino == ino)
		{
			proc->cwd = new_path;
			return (closedir(dir), free(path), proc->cwd);
		}
		if (recursive_search_dir(ft_strdup(new_path), ino))
			return (closedir(dir), free(path), proc->cwd);
		free(new_path);
		entry = readdir(dir);
		if (!entry || (entry->d_name[0] == '.' && entry->d_name[1] == '.'))
			break ;
	}
	return (closedir(dir), free(path), NULL);
}

/// @brief this function automatically get the updated current working directory
/// even if an other process modify the current working directory location
/// ex: rename, move, or delete the current workin dir.
/// There is multiple fall back layer, don't know if bash do it that way.
/// @param cmd
/// @return
char	*get_cwd()
{
	static char	buffer[PATH_MAX + 1];
	struct stat	file_stat;
	char		*home;
	t_process	*proc;
	char		*cur_dir;

	proc = get_process();
	if (proc->cwd == NULL)
	{
		cur_dir = getcwd(&buffer[0], PATH_MAX + 1);
		if (!cur_dir)
			free_all_and_exit2(1, "An error occur while trying to get the current working dir.");
		proc->cwd = ft_strdup(cur_dir);
		if (proc->cwd[ft_strlen(proc->cwd) - 1] != '/')
			proc->cwd = ft_strjoinfree(proc->cwd, "/");
		return (proc->cwd);
	}
	if (stat(proc->cwd, &file_stat) != 0)
		free_all_and_exit2(errno, "stat error");
	if (proc->dir_id == 0)
		proc->dir_id = file_stat.st_ino;
	if (file_stat.st_ino == proc->dir_id)
		return (proc->cwd);
	home = get_home();
	if (home)
		proc->cwd = recursive_search_dir(ft_strdup(home), file_stat.st_ino);
	if (!proc->cwd)
	{
		cur_dir = getcwd(&buffer[0], PATH_MAX + 1);
		if (!cur_dir)
			free_all_and_exit2(1,
					"An error occur while trying to get the current working dir.");
		proc->cwd = ft_strdup(cur_dir);
	}
	if (proc->cwd[ft_strlen(proc->cwd) - 1] != '/')
		proc->cwd = ft_strjoinfree(proc->cwd, "/");
	return (proc->cwd);
}
