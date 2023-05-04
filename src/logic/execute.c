/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/03 18:29:56 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_full_path_cmd(t_cmd *cmd)
{
	char	**env_paths;
	char	*cmd_name;
	char	*full_name;

	if (!cmd || !cmd->name)
		return (NULL);
	env_paths = parse_env(get_data()->envp, "PATH");
	if (!env_paths)
		return (perror("Environement path not set."), free_all_and_exit());
	cmd_name = cmd->name;
	while (*env_paths)
	{	
		full_name = ft_strjoin_temp(*env_paths, "/");
		full_name = ft_strjoin(full_name, cmd_name);
		if (access(full_name, F_OK | X_OK) == 0)
		{
			free(cmd->name);
			cmd->name = full_name;
			return (full_name);
		}
		env_paths++;
	}
	return (free_paths(env_paths));
}