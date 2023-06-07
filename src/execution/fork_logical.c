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

/// @brief logical operator such as && || stop the execution of the program
/// if an error occur.
/// @param cmd 
t_cmd	*exec_logical_or(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	build_token_environement(cmd->token);
	if (contains_parentheses(cmd->token))
		proc->errnum = exec_sequence(cmd->child);
	cmd = parse_at_execution(cmd);
	if (!cmd)
		return (cmd);
	if (cmd->is_builtin && proc->errnum == 0)
		proc->errnum = exec(cmd);
	else if (proc->errnum == 0)
		fork_exec(cmd);
	proc->stop_exec = false;
	if (proc->errnum == 0)
		cmd = cmd->next;
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

t_cmd	*exec_logical_and(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	build_token_environement(cmd->token);
	if (contains_parentheses(cmd->token))
		proc->errnum = exec_sequence(cmd->child);
	proc->errnum = 0;
	cmd = parse_at_execution(cmd);
	if (!cmd)
		return (cmd);
	if (cmd->is_builtin && proc->errnum == 0)
		proc->errnum = exec(cmd);
	else if (proc->errnum == 0)
		fork_exec(cmd);
	proc->stop_exec = false;
	if (proc->errnum > 0)
		proc->stop_exec = true;
	return (cmd);
}
