/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/25 14:40:08 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	*get_path_to_change(t_cmd *cmd)
{
	char	*path;
	char	*path_to_change;

	path = NULL;
	path_to_change = NULL;
	if (cmd->args[1] == NULL || ft_strisempty(cmd->args[1]))
	{
		path_to_change = get_home();
		if (!path_to_change)
			write_err(1, "cd: HOME not set");
	}
	else if (cmd->args[1][0] == '/')
		path_to_change = ft_strdup(cmd->args[1]);
	else
	{
		path = get_cwd();
		if (!path)
			return (ft_printf("minishell: cd: %s: No such file or directory\n",
					path_to_change), NULL);
		path = ft_strjoinfree(path, "/");
		path_to_change = ft_strjoinfree(path, cmd->args[1]);
		path = NULL;
	}
	return (path_to_change);
}

int	cd_cmd(t_cmd *cmd)
{
	char	buffer[1024];
	int		result;
	char	*path_to_change;

	if (cmd->options != NULL)
		return (ft_printf("Error: Option \"%s\" not supported.\n",
				cmd->options[0]), 1);
	path_to_change = get_path_to_change(cmd);
	if (!path_to_change)
		return (1);
	result = chdir(path_to_change);
	if (result == 0)
		set_cwd(ft_strdup(getcwd(buffer, 1024)));
	free_ptr(path_to_change);
	if (result != 0)
	{
		ft_printf("minishell: cd: %s: No such file or directory\n",
			cmd->args[1]);
		return (1);
	}
	return (0);
}

// int	cd_cmd(t_cmd *cmd)
// {
// 	char	*path;
// 	char	*path_to_change;
// 	char	buffer[1024];
// 	int		result;

// 	path = NULL;
// 	path_to_change = NULL;
// 	if (cmd->options != NULL)
// 		return (ft_printf("Error: Option \"%s\" not supported.\n",
// 				cmd->options[0]), 1);
// 	if (cmd->args[1] == NULL || ft_strisempty(cmd->args[1]))
// 	{
// 		path_to_change = get_home();
// 		if (!path_to_change)
// 			write_err(1, "cd: HOME not set");
// 	}
// 	else if (cmd->args[1][0] == '/')
// 		path_to_change = ft_strdup(cmd->args[1]);
// 	else
// 	{
// 		path = get_cwd();
// 		if (!path)
// 		{
// 			ft_printf("minishell: cd: %s: No such file or directory\n",
// 				path_to_change);
// 			return (1);
// 		}
// 		path = ft_strjoinfree(path, "/");
// 		path_to_change = ft_strjoinfree(path, cmd->args[1]);
// 		path = NULL;
// 	}
// 	result = chdir(path_to_change);
// 	if (result == 0)
// 		set_cwd(ft_strdup(getcwd(buffer, 1024)));
// 	free_ptr(path);
// 	free_ptr(path_to_change);
// 	if (result != 0)
// 		return (ft_printf("minishell: cd: %s: No such file or directory\n",
// 				cmd->args[1]), 1);
// 	return (0);
// }
