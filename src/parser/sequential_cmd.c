
#include "minishell.h"

t_cmd	*sequential_cmd(t_token_group *group)
{
	t_cmd	*cmd;

	cmd = add_cmd();
	if (cmd->name == NULL)
		return (NULL);
	cmd->token_group = group;
	cmd->cmd_seq_type = get_sequence_type(TK_CMD_SEQ_END);
	return (cmd);
}

/// @brief if commands are executed sequentialy then we will need to parse
/// again the next command, ex: export FLAGS=-la && ls $FLAGS
/// if we parse the whole command once we will not be able to 
/// get updated $FLAGS so we excute first command then parse the next and exec.
/// @param group 
/// @return 
t_cmd	*sequential_cmds(t_token_group *group)
{
	if (!group)
		return (NULL);
	while (group)
	{
		sequential_cmd(group);
		group = group->next;
	}
	return (get_first_cmd());
}
