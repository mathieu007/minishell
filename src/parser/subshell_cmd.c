/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/16 08:36:50 by mroy             ###   ########.fr       */
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


t_cmd_seq	get_sequence_type(t_token_group *group)
{
	t_token_type	type;

	type = group->last_token->type;
	if (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE
		|| type == TK_CMD_SEQ_END)
		return ((t_cmd_seq)type);
	return (CMD_SEQUENTIAL);
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
	cmd->cmd_seq_type = get_sequence_type(group);
	cmd->args = &args[0];
	return (cmd);
}
