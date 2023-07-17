#include "minishell.h"

t_cmd	*fork_first_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;

	cmd = pipe->child;
	pipe_cmd(pipe);
	proc = get_process();
	proc->errnum = build_cmd(cmd);
	if (proc->errnum > 0 || proc->errnum == -1)
		return (pipe->next);
	pid = ft_fork();
	if (pid == 0)
	{
		dup2(pipe->pipe->fd_out, STDOUT_FILENO);
		close(pipe->pipe->fd_out);
		close(pipe->pipe->fd_in);
		proc->errnum = exec_commands(cmd, true);
		exit(proc->errnum);
	}
	pipe->pid = pid;
	return (pipe->next);
}

t_cmd	*fork_last_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;

	cmd = pipe->child;
	proc = get_process();
	proc->errnum = build_cmd(cmd);
	if (proc->errnum > 0 || proc->errnum == -1)
		return (pipe->next);
	pid = ft_fork();
	if (pid == 0)
	{
		dup2(pipe->prev->pipe->fd_in, STDIN_FILENO);
		close_prev_pipes(pipe);
		proc->errnum = exec_commands(cmd, true);
		exit(proc->errnum);
	}
	pipe->pid = pid;
	close_prev_pipes(pipe);
	return (pipe);
}

void	dup_close_middle_pipes(t_cmd *cmd, t_cmd *pipe)
{
	dup2(pipe->prev->pipe->fd_in, STDIN_FILENO);
	if (cmd->has_redirection)
	{
		close(pipe->prev->pipe->fd_in);
		close(pipe->prev->pipe->fd_out);
		close_pipes(pipe->pipe);
	}
	else
	{
		dup2(pipe->pipe->fd_out, STDOUT_FILENO);
		close_prev_pipes(pipe);
		close(pipe->pipe->fd_in);
		close(pipe->pipe->fd_out);
	}
}

t_cmd	*fork_middle_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;

	cmd = pipe->child;
	pipe_cmd(pipe);
	proc = get_process();
	proc->errnum = build_cmd(cmd);
	if (proc->errnum > 0 || proc->errnum == -1)
		return (pipe->next);
	pid = ft_fork();
	if (pid == 0)
	{
		dup_close_middle_pipes(cmd, pipe);
		proc->errnum = exec_commands(cmd, true);
		exit(proc->errnum);
	}
	pipe->pid = pid;
	close(pipe->pipe->fd_out);
	close_prev_pipes(pipe);
	return (pipe->next);
}

int32_t	exec_pipes_cmds(t_cmd *pipes)
{
	t_cmd		*start;
	t_process	*proc;

	proc = get_process();
	start = pipes;
	pipes = fork_first_child(pipes);
	while (pipes && pipes->next)
		pipes = fork_middle_child(pipes);
	if (pipes)
		pipes = fork_last_child(pipes);
	wait_childs(start);
	return (proc->errnum);
}
