
#include "minishell.h"

/// @brief if commands are executed sequentialy then we will need to parse
/// again the next command, ex: export FLAGS=-la && ls $FLAGS
/// if we parse the whole command once we will not be able to 
/// get updated $FLAGS so we excute first command then parse the next and exec.
/// @param group 
/// @return 
t_cmd	*parse_cmd(t_token_group *token_group)
{
	t_cmd		*cmd;

	cmd = add_cmd();
	cmd->args = parse_args(token_group);
	cmd->name = cmd->args[0];
	if (cmd->name == NULL)
		return (NULL);
	cmd->full_path_name = get_full_path(cmd);
	cmd->is_builtin = is_builtins(cmd->name);
	cmd->options = get_options(token_group);
	cmd->cmd_seq_type = token_group->cmd_seq_type;
	if (cmd->cmd_seq_type == CMD_PIPE)
	{
		
	}
	return (cmd);
}
