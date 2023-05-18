
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
	char		**args;	
	
	cmd = add_cmd();
	parse_env(group);
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
