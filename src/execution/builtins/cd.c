#include "minishell.h"
#include <stddef.h>

char	*ft_strcat(char *dest, const char *src)
{
	size_t	dest_len;
	size_t	i;

	dest_len = 0;
	while (dest[dest_len] != '\0')
		dest_len++;
	i = 0;
	while (src[i] != '\0')
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

int	handle_cd_options(t_cmd *cmd)
{
	if (cmd->options != NULL)
	{
		printf("Error: Option \"%s\" not supported.\n", cmd->options[0]);
		return (1);
	}
	return (0);
}

int	change_directory(t_cmd *cmd, char *path_to_change)
{
	int	result;

	result = chdir(path_to_change);
	if (result != 0)
	{
		printf("minishell: cd: %s: No such file or directory\n", cmd->args[1]);
		return (1);
	}
	return (0);
}

int	cd_cmd(t_cmd *cmd)
{
	char	*path_to_change;
	char	*path;

	path_to_change = cmd->args[1];
	if (handle_cd_options(cmd) != 0)
		return (1);
	if (cmd->args[1] == NULL)
	{
		printf("Error: No directory specified.\n");
		return (1);
	}
	if (path_to_change[0] != '/')
	{
		path = get_cwd();
		if (!path)
		{
			printf("minishell: cd: %s: No such file or directory\n",
				cmd->args[1]);
			return (1);
		}
		ft_strcat(path, "/");
		ft_strcat(path, path_to_change);
		path_to_change = path;
	}
	return (change_directory(cmd, path_to_change));
}

// int	cd_cmd(t_cmd *cmd)
// {
// 	char *path;
// 	char *path_to_change;

// 	path_to_change = cmd->args[1];
// 	if (cmd->options != NULL)
// 		return (printf("Error: Option \"%s\" not supported.\n",
// 				cmd->options[0]), 1);
// 	if (cmd->args[1] == NULL)
// 		return (printf("Error: No directory specified.\n"), 1);
// 	if (path_to_change[0] != '/')
// 	{
// 		path = get_cwd();
// 		if (!path)
// 		{
// 			printf("minishell: cd: %s: No such file or directory\n",
// 					cmd->args[1]);
// 			return (1);
// 		}
// 		ft_strcat(path, "/");
// 		ft_strcat(path, path_to_change);
// 		path_to_change = path;
// 	}
// 	int result = chdir(path_to_change);
// 	if (result != 0)
// 		return (printf("minishell: cd: %s: No such file or directory\n",
// 				cmd->args[1]), 1);
// 	return (0);
// }