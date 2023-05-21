
#include "minishell.h"

/// @brief if commands are executed sequentialy then we will need to parse
/// again the next command, ex: export FLAGS=-la && ls $FLAGS
/// if we parse the whole command once we will not be able to 
/// get updated $FLAGS so we excute first command then parse the next and exec.
/// @param group 
/// @return 
t_cmd	*get_seq_cmds(t_token_group *group)
{
	t_cmd		*cmd;

	parse_env(group);
	free_t_token_groups(group);
	group = get_data()->token_groups;
	cmd = add_cmd();
	cmd->args = parse_args(group);
	cmd->name = cmd->args[0];
	if (cmd->name == NULL)
		return (NULL);
	cmd->full_path_name = get_full_path(cmd->name);
	cmd->is_builtin = is_builtins(cmd->name);
	cmd->options = get_options(group);
	cmd->cmd_seq_type = group->cmd_seq_type;
	return (cmd);
}

t_cmd	*add_cmd_pipe(t_token_group *token_group)
{
	tokenize(token_group);
	parse_env(token_group);
}

t_cmd	*get_cmd(char *str)
{
	t_cmd			*cmd;
	t_token_group	*token_group;

	token_group = tokenize_groups(str);
	while (token_group)
	{
		if (token_group->cmd_seq_type == CMD_PIPE)
		{
			tokenize(token_group);
			parse_env(token_group);
		}
		else if (token_group->cmd_seq_type == CMD_NONE
			|| token_group->cmd_seq_type == CMD_SEQUENTIAL)
		{
			tokenize(token_group);
			parse_env(token_group);
			str = group_to_str(token_group);
			reset_token_group(token_group);
			token_group->str = str;
		}
		token_group = token_group->next;
	}
	token_group = get_data()->token_groups;
	cmd = add_cmd();
	cmd->args = parse_args(token_group);
	cmd->name = cmd->args[0];
	if (cmd->name == NULL)
		return (NULL);
	cmd->full_path_name = get_full_path(cmd->name);
	cmd->is_builtin = is_builtins(cmd->name);
	cmd->options = get_options(token_group);
	cmd->cmd_seq_type = token_group->cmd_seq_type;
	return (cmd);
}
