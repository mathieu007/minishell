#include "minishell.h"

/// @brief group are essentially parentheses: (export VAR='123' && echo $VAR && (export VAR='456' && echo $VAR))
/// @param cmd 
void	fork_group(t_cmd *cmd)
{
	pid_t		pid;
	t_process 	*proc;
	int32_t		status;

	proc = get_process();
	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		get_process()->env_cpy = copy_env_from(proc);
		exit(cmd->func(cmd));
	}
	cmd->pid = pid;
	waitpid(cmd->pid, &status, 0);
}
