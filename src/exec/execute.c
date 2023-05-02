/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/02 13:04:29 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*free_paths(char **paths)
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

char	*get_full_path_cmd(t_cmd *cmd)
{
	char	**paths;
	char	*cmd_name;
	char	*full_name;

	cmd_name = cmd->name;
	paths = parse_env(get_data()->envp, "PATH");
	if (paths == NULL)
		return (NULL);
	if (cmd_name[0] == '/' && access(cmd_name, F_OK | X_OK) == 0)
		return (ft_strdup(cmd_name));
	else
		return (NULL);
	while (*paths)
	{
		full_name = ft_strjoin_temp(*paths, "/");
		full_name = ft_strjoin(full_name, cmd_name);
		if (access(full_name, F_OK | X_OK) == 0)
		{
			free(cmd->name);
			cmd->name = full_name;
			return (full_name);
		}
		paths++;
	}
	return (free_paths(paths));
}