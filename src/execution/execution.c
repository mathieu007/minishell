
#include "minishell.h"

int32_t	execve_cmd(t_cmd *cmd)
{
	if (get_process()->parent->stop_exec)
		return (get_process()->parent->errnum);
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
t_token	*pipes_cmds(t_token *token)
{
	t_cmd	*cmd;
	t_cmd	*start;

	cmd = new_cmd();
	cmd->token = token;
	start = cmd;
	pipe_cmd(cmd);
	token = token->next;
	while (token && token->cmd_seq_type == CMD_PIPE)
	{
		cmd->next = new_cmd();
		cmd->next->token = token;
		pipe_cmd(cmd->next);
		token = token->next;
	}
	cmd->next = new_cmd();
	cmd->next->token = token;
	fork_pipes(start);
	return (token->next);
}

int32_t	exec_sequence(t_token *token)
{
	t_process	*proc;
	int32_t		ret;

	ret = 0;
	proc = get_process();
	while (token && token->type != TK_END)
	{
		if (token->cmd_seq_type == CMD_PIPE)
			token = pipes_cmds(token);
		else if (token->cmd_seq_type == CMD_SEQUENTIAL
			|| token->cmd_seq_type == CMD_NONE)
			exec_sequential(token);
		else if (token->cmd_seq_type == CMD_LOG_AND)
			token = exec_logical_and(token);
		else if (token->cmd_seq_type == CMD_LOG_OR)
			token = exec_logical_or(token);
		else if (token->cmd_seq_type == CMD_FILEOUT_APPPEND)
			token = exec_logical_or(token);
		else if (token->cmd_seq_type == CMD_FILEOUT)
			token = exec_logical_or(token);
		else if (token->cmd_seq_type == CMD_FILEIN)
			token = exec_logical_or(token);
		else if (token->cmd_seq_type == CMD_FILEIN_APPPEND)
			token = exec_logical_or(token);
		else if (token->type == TK_PARENTHESE_OPEN)
			token = exec_group(token);
		else if (token->type == TK_COMMANDSUBSTITUTION_OPEN)
			token = exec_logical_or(token);
		ret = proc->errnum;
		if (proc->stop_exec)
			return (ret);
		if (token)
			token = token->next;
	}
	return (ret);
}

int32_t	exec_cmds(char *str)
{
	t_token	*token;
	int32_t	ret;

	token = tokenize(str);
	get_process()->tokens = token->child_tokens;
	ret = exec_sequence(token->child_tokens);
	return (ret);
}
