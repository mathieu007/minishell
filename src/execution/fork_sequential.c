#include "minishell.h"

static t_cmd	*parse_seq_cmd(t_cmd *cmd)
{
	cmd = parse_cmd(cmd);
	if (cmd->is_builtin)
		add_built_in_func(cmd);
	else
		add_execve_func(cmd);
	return (cmd);
}

// exec the function right away because it is a sequential cmd.
// No need to fork.
// Because t_process is stored inside static variable no need 
// to initilized a new one, each forked process will received it's
// own t_process struct
static int32_t	exec(t_cmd *cmd)
{
	t_process	*proc;
	int32_t		ret;

	ret = 0;
	proc = get_process();
	if (proc->errnum > 0)
		return (proc->errnum);
	ret = cmd->func(cmd);
	return (ret);
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
	proc->errnum = ret;
	return (ret);
}

int32_t	exec_sequential(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	proc->stop_exec = false;
	cmd = parse_seq_cmd(cmd);
	if (!cmd)
		return (proc->errnum);
	if (cmd->is_builtin)
		proc->errnum = exec(cmd);
	else
		proc->errnum = fork_exec(cmd);
	return (proc->errnum);
}
