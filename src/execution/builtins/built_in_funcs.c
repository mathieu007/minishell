/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/06/26 15:41:18 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_built_in_func(t_cmd *cmd)
{
	size_t	len;

	len = ft_strlen(cmd->name) + 1;
	if (len == 0)
		return (0);
	if (ft_strnstr(cmd->name, "echo", len) == cmd->name)
		return (cmd->func = &echo_cmd, 1);
	else if (ft_strnstr(cmd->name, "env", len) == cmd->name)
		return (cmd->func = &env_cmd, 1);
	else if (ft_strnstr(cmd->name, "pwd", len) == cmd->name)
		return (cmd->func = &pwd_cmd, 1);
	else if (ft_strnstr(cmd->name, "cd", len) == cmd->name)
		return (cmd->func = &cd_cmd, 1);
	else if (ft_strnstr(cmd->name, "export", len) == cmd->name)
		return (cmd->func = &export_cmd, 1);
	else if (ft_strnstr(cmd->name, "unset", len) == cmd->name)
		return (cmd->func = &unset_cmd, 1);
	else if (ft_strnstr(cmd->name, "exit", len) == cmd->name)
		return (cmd->func = &exit_cmd, 1);
	return (0);
}
