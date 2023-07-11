#include "minishell.h"

static int32_t	execve_subshell(t_cmd *cmd)
{
	t_process	*proc;
	char		**subshell_args;

	proc = get_process();
	subshell_args = malloc(3 * sizeof(char *));
	subshell_args[2] = NULL;
	subshell_args[1] = cmd->token->str;
	subshell_args[0] = proc->full_program_name;
	if (execve(proc->full_program_name, subshell_args, get_env()) == -1)
		free_all_and_exit2(errno, "execve error");
	return (errno);
}

static int32_t	fork_subshell(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;
	int32_t		status;
	t_cmd		*redirection;

	redirection = cmd->next;
	proc = get_process();
	build_token_environement(cmd->token);
	if (has_error())
		return (proc->errnum);
	pid = fork();
	proc->errnum = 0;
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		if (cmd->has_redirection)
			create_fd_redir(cmd, redirection);
		file_redirection(cmd);
		close_files_redirections(cmd);
		proc->errnum = execve_subshell(cmd);
		exit(proc->errnum);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		proc->errnum = WEXITSTATUS(status);
	return (proc->errnum);
}

int32_t	exec_subshell(t_cmd *parenthese_cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = fork_subshell(parenthese_cmd);
	return (proc->errnum);
}
