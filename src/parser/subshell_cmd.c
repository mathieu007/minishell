/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/18 14:16:13 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_cmd(t_token_group *group)
{
	t_cmd		*cmd;
	char		**args;
	
	
	cmd = add_cmd();
	args = parse_args(group);
	cmd->name = args[0];
	if (cmd->name == NULL)
		return (NULL);
	cmd->full_path_name = get_full_path(cmd->name);
	cmd->is_builtin = is_builtins(cmd->name);
	cmd->options = get_options(group);
	cmd->cmd_seq_type = group->cmd_seq_type;
	cmd->args = &args[0];
	return (cmd);
}
