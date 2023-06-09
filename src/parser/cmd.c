
#include "minishell.h"

/// @brief if commands are executed sequentialy then we will need to parse
/// again the next command, ex: export FLAGS=-la && ls $FLAGS
/// if we parse the whole command once we will not be able to 
/// get updated $FLAGS so we excute first command then parse the next and exec.
/// @param group 
/// @return 
t_cmd	*parse_cmd(t_cmd *cmd)
{
	cmd->args = parse_args(cmd->token);
	cmd->name = ft_strdup(cmd->args[0]);
	if (cmd->name == NULL)
		return (NULL);
	cmd->is_builtin = is_builtins(cmd->name);
	if (!cmd->is_builtin)
		cmd->full_path_name = get_full_path(cmd);
	cmd->options = get_options(cmd->token);
	return (cmd);
}

t_cmd	*parse_redirect(t_cmd *cmd)
{
	cmd->args = parse_args(cmd->token);
	cmd->name = ft_strdup(cmd->args[0]);
	if (cmd->args && cmd->args[0] && cmd->args[1])

	if (cmd->name == NULL)
		return (NULL);
	return (cmd);
}

t_cmd	*parse_cmd2(t_cmd *cmd)
{
	cmd->args = parse_args(cmd->token);
	cmd->name =  ft_strdup(cmd->args[0]);
	if (cmd->name == NULL)
		return (NULL);
	cmd->is_builtin = is_builtins(cmd->name);
	if (!cmd->is_builtin)
		cmd->full_path_name = get_full_path(cmd);
	cmd->options = get_options(cmd->token);
	return (cmd);
}
