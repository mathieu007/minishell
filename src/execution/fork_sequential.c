#include "minishell.h"

t_cmd	*last_in_redir(t_cmd *cmd)
{
	t_cmd	*last;

	last = NULL;
	while (cmd && is_redirection(cmd->type))
	{
		if (cmd->in_redir)
			last = cmd;
		cmd = cmd->next;
	}
	return (last);
}

t_cmd	*last_out_redir(t_cmd *cmd)
{
	t_cmd	*last;

	last = NULL;
	while (cmd && is_redirection(cmd->type))
	{
		if (cmd->out_redir)
			last = cmd;
		cmd = cmd->next;
	}
	return (last);
}

static int32_t	exec(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	if (proc->errnum > 0)
		return (proc->errnum);
	proc->errnum = cmd->func(cmd);
	return (proc->errnum);
}

static int32_t	fork_exec(t_cmd	*cmd)
{
	pid_t		pid;
	t_process	*proc;
	int32_t		ret;
	int32_t		status;
	t_cmd		*redir;

	pid = fork();
	ret = 0;
	proc = get_process();
	redir = cmd->next;
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		get_process()->env_cpy = proc->env_cpy;
		if (cmd->has_redirection)
		{
			create_fd_redir(cmd, redir);
			file_redirection(cmd);
		}
		ret = exec_commands(cmd, false);
		exit(ret);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	proc->errnum = ret;
	return (ret);
}

int32_t	exec_sequential(t_cmd *group_seq)
{
	t_process	*proc;
	t_cmd		*cmd;

	cmd = group_seq->child;
	proc = get_process();
	proc->errnum = 0;
	proc->stop_exec = false;
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (!cmd)
		return (proc->errnum);
	if (cmd->next && cmd->next->has_redirection)
		proc->errnum = fork_exec(cmd);
	else if (cmd->is_builtin)
		proc->errnum = exec(cmd);
	else if (proc->errnum == 0)
	 	proc->errnum = fork_exec(cmd);
	return (proc->errnum);
}
