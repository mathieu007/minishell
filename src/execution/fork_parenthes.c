#include "minishell.h"

static int32_t	fork_group(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;
	int32_t		ret;
	int32_t		status;
	t_cmd		*redir;

	redir = cmd->next;
	proc = get_process();
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (cmd->has_redirection)
		create_fd_redir(cmd, redir->child);
	pid = fork();
	ret = 0;
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		get_process()->env_cpy = proc->env_cpy;
		file_redirection(cmd);
		ret = exec_commands(cmd->child, false);
		close_files_redirections(cmd);
		exit(ret);
	}
	close_files_redirections(cmd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		proc->errnum = WEXITSTATUS(status);
	return (proc->errnum);
}

int32_t	exec_group(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = fork_group(cmd);
	return (proc->errnum);
}
