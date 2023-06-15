
#include "minishell.h"

int32_t	execve_cmd(t_cmd *cmd)
{
	if (execve(cmd->full_path_name, cmd->args, get_env_path()) == -1)
		perror("execve failed\n");
	return (errno);
}

int32_t	add_execve_func(t_cmd *cmd)
{
	cmd->func = &execve_cmd;
	return (1);
}

/// @brief fork all piped command and execute then waitpid for all command to complete
/// @param token_group 
/// @return we return the last pipe command
t_cmd	*pipes_cmds(t_cmd *cmd)
{
	t_cmd	*start;

	start = cmd;
	pipe_cmd(cmd);
	cmd = cmd->next;
	while (cmd && cmd->cmd_seq_type == CMD_PIPE)
	{
		pipe_cmd(cmd);
		cmd = cmd->next;
	}
	fork_pipes(start);
	if (!cmd)
		return (cmd);
	return (cmd->next);
}

int32_t	exec_sequence(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	while (cmd && cmd->cmd_seq_type != CMD_NONE)
	{
		if (cmd->cmd_seq_type == CMD_PIPE)
			cmd = pipes_cmds(cmd);
		else if (cmd->cmd_seq_type == CMD_SEQUENTIAL)
			exec_sequential(cmd);
		else if (cmd->cmd_seq_type == CMD_LOG_AND)
			cmd = exec_logical_and(cmd);
		else if (cmd->cmd_seq_type == CMD_LOG_OR)
			cmd = exec_logical_or(cmd);
		else if (cmd->cmd_seq_type == CMD_GROUPING)
			cmd = exec_group(cmd);
		if (proc->stop_exec)
			return (proc->errnum);
		if (cmd)
			cmd = cmd->next;
	}
	return (proc->errnum);
}

/// @brief we will create cmds from the tokens.
/// the commands are not yet parsed with the whole data.
/// it is just a hierarchical cmd representation of the cmd execution order.
/// @param token 
/// @return 
void	create_nested_cmds(t_cmd *cmd)
{
	t_token	*childs;
	t_cmd	*child_cmd;

	while (cmd)
	{
		childs = cmd->token->child_tokens;
		if (childs && get_sequence_type(childs) != CMD_NONE)
		{
			child_cmd = add_child_cmd(cmd, childs);
			create_nested_cmds(child_cmd);
			childs = childs->next;
		}
		cmd = cmd->next;
	}
}

t_cmd	*create_cmds_tree(t_token *token)
{
	t_cmd	*start;

	if (!token)
		return (NULL);
	start = add_root_cmd_token(token);
	token = token->next;
	while (token && token->next)
	{
		add_root_cmd_token(token);
		token = token->next;
	}
	create_nested_cmds(start);
	return (start);
}

t_cmd	*parse_at_execution(t_cmd *cmd)
{
	cmd = parse_cmd(cmd);
	if (cmd->is_builtin)
		add_built_in_func(cmd);
	else
		add_execve_func(cmd);
	return (cmd);
}

int32_t	exec_cmds(char *str)
{
	t_token	*token;
	int32_t	ret;
	t_cmd	*root_cmd;

	token = tokenize(str);
	get_process()->tokens = token->child_tokens;
	root_cmd = create_cmds_tree(token->child_tokens);
	get_process()->cmds = root_cmd;
	ret = exec_sequence(root_cmd);
	return (ret);
}
