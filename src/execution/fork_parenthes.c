#include "minishell.h"

static int32_t	execve_subshell(t_cmd *cmd)
{
	t_process	*proc;
	char		**subshell_args;
	char		**env;

	proc = get_process();
	env = get_env();
	subshell_args = malloc(3 * sizeof(char *));
	subshell_args[2] = NULL;
	// subshell_args[3] = ft_itoa(cmd->out_redir->fd);
	// subshell_args[2] = ft_itoa(cmd->in_redir->fd);
	subshell_args[1] = cmd->token->str;
	subshell_args[0] = proc->full_program_name;
	if (execve(proc->full_program_name, subshell_args, env) == -1)
	{
		free_2d_char_array(env);
		free(subshell_args);
		free_all_and_exit2(errno, "execve error");
	}
	return (errno);
}

int32_t	exec_subshell(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	proc->errnum = build_cmd(cmd);
	if (has_error())
		return (proc->errnum);
	proc->errnum = 0;
	pid = ft_fork();
	if (pid == 0)
	{
		file_redirection(cmd, true);
		close_files_redirections(cmd);
		proc->errnum = execve_subshell(cmd);
		exit(proc->errnum);
	}
	proc->errnum = ft_waitpid(pid);
	return (proc->errnum);
}
