#include "minishell.h"

static int32_t	exec(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	if (proc->errnum > 0)
		return (proc->errnum);
	redirect_output(cmd);
	proc->errnum = cmd->func(cmd);	
	return (proc->errnum);
}

static void	fork_exec(t_cmd	*cmd)
{
	pid_t		pid;
	t_process	*proc;
	int32_t		ret;
	int32_t		status;

	pid = fork();
	proc = get_process();
	ret = proc->errnum;
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		get_process()->env_cpy = proc->env_cpy;
		ret = exec(cmd);
		exit(ret);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	if (ret != 0)
		proc->errnum = ret;
}

int32_t	execute(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->stop_exec = false;
	proc->errnum = 0;
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (!cmd)
		return (-1);
	create_fd_redir(cmd, cmd->next);
	if (cmd->is_builtin && cmd->next && !is_redirection(cmd->next->cmd_seq_type))
		proc->errnum = exec(cmd);
	else if (proc->errnum == 0)
		fork_exec(cmd);
	return (proc->errnum);
}

/// @brief logical operator such as && || stop the execution of the program
/// if an error occur.
/// @param cmd 
t_cmd	*exec_logical_or(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 1;
	while (cmd && cmd->cmd_seq_type == CMD_LOG_OR && proc->errnum > 0)
	{
		proc->errnum = execute(cmd);
		if (proc->errnum == -1)
			return (NULL);
		cmd = cmd->next;
		if (proc->errnum == 0 && cmd)
		{
			if (cmd->cmd_seq_type == CMD_LOG_AND)
				return (exec_logical_and(cmd));
			else if (cmd->next && cmd->next->cmd_seq_type == CMD_LOG_AND)
				return (exec_logical_and(cmd->next));
			else if (cmd->next)
				return (cmd->next);
			else
				return (cmd);
		}
		while (proc->errnum > 0 && cmd && cmd->cmd_seq_type == CMD_LOG_AND)
			cmd = cmd->next;
	}
	return (cmd);
}

// t_token	*contains_parentheses(t_token *token)
// {
// 	t_token	*child;

// 	child = token->child_tokens;
// 	while (child)
// 	{
// 		if (child->type == TK_PARENTHESE_OPEN)
// 			return (child);
// 		child = child->next;
// 	}
// 	return (NULL);
// }

t_cmd	*exec_logical_and(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	while (cmd && cmd->cmd_seq_type == CMD_LOG_AND && proc->errnum == 0)
	{
		proc->errnum = execute(cmd);
		if (proc->errnum == -1)
			return (NULL);
		cmd = cmd->next;
		if (proc->errnum > 0 && cmd)
		{
			while (cmd && cmd->cmd_seq_type == CMD_LOG_AND)
				cmd = cmd->next;
			if (cmd && cmd->cmd_seq_type == CMD_LOG_OR)
				return (exec_logical_or(cmd));
			else if (cmd)
				return (cmd);
			else
				return (NULL);
		}
	}
	return (NULL);
}
