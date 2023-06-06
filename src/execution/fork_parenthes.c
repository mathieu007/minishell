#include "minishell.h"

static int32_t	fork_group(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;
	int32_t		ret;
	int32_t		status;

	pid = fork();
	proc = get_process();
	ret = 0;
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		get_process()->env_cpy = proc->env_cpy;
		ret = exec_sequence(cmd->child);
		exit(ret);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		proc->errnum = WEXITSTATUS(status);
	return (proc->errnum);
}

t_cmd	*exec_group(t_cmd *cmd)
{
	t_process			*proc;
	
	proc = get_process();
	proc->errnum = fork_group(cmd);
	return (cmd);
}
