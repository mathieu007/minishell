#include "minishell.h"



// exec the function right away because it is a sequential cmd.
// No need to fork.
// Because t_process is stored inside static variable no need 
// to initilized a new one, each forked process will received it's
// own t_process struct
static int32_t	exec(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	if (proc->errnum > 0)
		return (proc->errnum);
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
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
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

int32_t	exec_or(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->stop_exec = false;
	proc->errnum = 0;
	build_token_environement(cmd->token);
	if (contains_parentheses(cmd->token))
		proc->errnum = exec_sequence(cmd->child);
	cmd = parse_at_execution(cmd);
	if (!cmd)
		return (-1);
	if (cmd->is_builtin)
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
	proc->errnum = exec_or(cmd);
	if (proc->errnum == -1)
		return (NULL);
	cmd = cmd->next;
	if (proc->errnum > 0 && cmd && cmd->cmd_seq_type == CMD_LOG_AND)
		cmd = exec_logical_and(cmd);
	else if (proc->errnum > 0 && cmd && cmd->cmd_seq_type == CMD_LOG_OR)
	{
		while (cmd && cmd->cmd_seq_type == CMD_LOG_OR && proc->errnum > 0)
		{
			proc->errnum = 0;
			proc->errnum = exec_or(cmd);
			cmd = cmd->next;
			if (proc->errnum > 0 && cmd->cmd_seq_type != CMD_LOG_OR)
			{
				cmd = cmd->next;
				while (cmd && cmd->cmd_seq_type != CMD_LOG_OR)
					cmd = cmd->next;
				proc->errnum = exec_sequence(cmd);
				return (NULL);
			}
			else if (proc->errnum == 0)	
				break ;
			cmd = cmd->next;			
		}
	}
	else if (proc->errnum == 0 && cmd && cmd->cmd_seq_type == CMD_LOG_AND)
		cmd = exec_logical_and(cmd);
	return (cmd);
}

t_token	*contains_parentheses(t_token *token)
{
	t_token	*child;

	child = token->child_tokens;
	while (child)
	{
		if (child->type == TK_PARENTHESE_OPEN)
			return (child);
		child = child->next;
	}
	return (NULL);
}

int32_t exec_and(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	build_token_environement(cmd->token);
	if (contains_parentheses(cmd->token))
		proc->errnum = exec_sequence(cmd->child);
	proc->errnum = 0;
	cmd = parse_at_execution(cmd);
	if (!cmd)
		return (-1);
	if (cmd->is_builtin && proc->errnum == 0)
		proc->errnum = exec(cmd);
	else if (proc->errnum == 0)
		fork_exec(cmd);
	proc->stop_exec = false;
	return (proc->errnum);
}

t_cmd	*exec_logical_and(t_cmd *cmd)
{
	t_process	*proc;
	int32_t		ret;

	proc = get_process();
	ret = exec_and(cmd);
	if (ret == -1)
		return (NULL);
	if (proc->errnum == 0)
	{
		if (cmd->next && cmd->next->cmd_seq_type == CMD_LOG_AND)
		{
			while (cmd && cmd->next)
			{
				if (cmd->next->cmd_seq_type == CMD_LOG_AND && proc->errnum == 0)
				{
					proc->errnum = 0;
					proc->errnum = exec_and(cmd->next);					
					if (proc->errnum > 0)
						proc->stop_exec = true;
					cmd = cmd->next;
				}
				else if (proc->errnum > 0 && cmd->next->cmd_seq_type == CMD_LOG_OR)
					cmd = exec_logical_or(cmd->next);
				else if (proc->errnum == 0 && cmd->next->cmd_seq_type == CMD_LOG_OR)
					cmd = cmd->next;
				else
					break;
			}
		}			
		else if (cmd->next && cmd->next->cmd_seq_type == CMD_LOG_OR)
			cmd = cmd->next;
	}
	else if (cmd->next->cmd_seq_type == CMD_LOG_OR)
		cmd = exec_logical_or(cmd->next);
	return (cmd);
}
