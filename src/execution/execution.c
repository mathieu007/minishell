
#include "minishell.h"

int32_t	execve_cmd(t_cmd *cmd)
{
	if (get_process()->parent->stop_process)
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

static t_cmd	*parse(t_token_sequence *token_group)
{
	char	*str;
	t_cmd	*cmd;

	tokenize(token_group);
	str = parse_env(token_group);
	reset_token_group(token_group);
	token_group->str = str;
	tokenize(token_group);
	cmd = parse_cmd(token_group);
	if (cmd->is_builtin)
		add_built_in_func(cmd);
	else
		add_execve_func(cmd);
	return (cmd);
}

/// @brief fork all piped command and execute then waitpid for all command to complete
/// @param token_group 
/// @return we return the last pipe command
t_token_sequence	*pipes_fork_cmds(t_token_sequence *token_seq)
{
	t_cmd	*cmd;
	t_cmd	*start;

	cmd = parse(token_seq);
	start = cmd;
	pipe_cmd(cmd);
	token_seq = token_seq->next;
	while (token_seq && token_seq->cmd_seq_type == CMD_PIPE)
	{
		cmd = parse(token_seq);
		pipe_cmd(cmd);
		token_seq = token_seq->next;
	}
	cmd = parse(token_seq);
	fork_pipes(start);
	return (token_seq->next);
}

int32_t	exec_process_sequence(t_token_sequence *token_seq)
{
	int32_t		ret;
	t_process	*proc;
	int32_t		i;

	i = 0;
	proc = get_process();
	while (token_seq)
	{
		if (token_seq->cmd_seq_type == CMD_PIPE)
			token_seq = pipes_fork_cmds(token_seq);
		else if (token_seq->cmd_seq_type == CMD_SEQUENTIAL)
			ret = exec_sequential(token_seq);
		else if (token_seq->cmd_seq_type == CMD_LOG_AND)
			token_seq = exec_logical(token_seq);
		else if (token_seq->cmd_seq_type == CMD_LOG_OR)
			token_seq = exec_logical(token_seq);
		// else if (token_seq->cmd_seq_type == CMD_GROUPING)
		// 	token_seq = exec_sequential(token_seq);
		if (proc->stop_process)
			return (proc->errnum);
		if (token_seq)	
			token_seq = token_seq->next;
	}
	return (proc->errnum);
}

int32_t	exec_cmds(char *str)
{
	t_token_sequence	*token_seq;
	int32_t				ret;

	token_seq = tokenize_sequences(str);
	ret = exec_process_sequence(token_seq);
	return (ret);
}
