/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/17 16:32:04 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**parse_args(t_token_group *group)
{
	int32_t	args_len;
	char	**split;

	args_len = get_args_len(group);
	split = malloc((args_len + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	split[args_len] = NULL;
	get_args(group, split);
	return (split);
}

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
