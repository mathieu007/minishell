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

static int32_t	fork_exec(t_cmd	*cmd)
{
	pid_t		pid;
	t_process	*proc;
	int32_t		ret;
	int32_t		status;

	pid = fork();
	ret = 0;
	proc = get_process();
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
	proc->errnum = ret;
	return (ret);
}

int32_t	exec_sequential(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	proc->stop_exec = false;
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (!cmd)
		return (proc->errnum);
	create_redir(cmd);
	if (cmd->is_builtin && cmd->next && !is_redirection(cmd->next->cmd_seq_type))
		proc->errnum = exec(cmd);
	else if (proc->errnum == 0)
		proc->errnum = fork_exec(cmd);
	return (proc->errnum);
}
