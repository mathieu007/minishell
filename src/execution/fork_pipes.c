#include "minishell.h"

void	close_prev_pipes(t_cmd *cmd)
{
	if (!cmd)
		return ;
	cmd = cmd->prev;
	if (cmd->pipe)
	{
		close(cmd->pipe->fd_in);
		close(cmd->pipe->fd_out);
	}
}

t_pipe	*prev_pipe(t_cmd *cmd)
{
	if (!cmd)
		return (NULL);
	cmd = cmd->prev;
	if (cmd)
		return (cmd->pipe);
	return (NULL);
}

void	close_pipes(t_pipe *pipe)
{
	close(pipe->fd_in);
	close(pipe->fd_out);
}

t_pipe	*init_pipes(int32_t *fds, t_cmd *cmd)
{
	cmd->pipe = malloc(sizeof(t_cmd));
	if (!cmd->pipe)
		return (NULL);
	cmd->pipe->fd_in = fds[0];
	cmd->pipe->fd_out = fds[1];
	return (cmd->pipe);
}

void	pipe_cmd(t_cmd *cmd)
{
	int32_t	fds[2];

	if (pipe(fds) == -1)
		free_all_and_exit2(errno, "Pipe error");
	init_pipes(fds, cmd);
}

static void	wait_childs(t_cmd *cmd)
{
	int32_t	i;
	int32_t	status;

	i = 0;
	while (cmd && cmd->pid)
	{
		waitpid(cmd->pid, &status, 0);
		cmd = cmd->next;
		i++;
	}
}

void	file_redirection(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->in_redir && cmd->in_redir->fd > 0)
	{
		redirect_input(cmd);
		if (cmd->out_redir && cmd->out_redir->fd > 0)
			redirect_output(cmd);
	}
	else if (cmd->out_redir && cmd->out_redir->fd > 0)
		redirect_output(cmd);
}

t_cmd	*fork_first_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;
	t_cmd		*redir;

	cmd = pipe->child;
	redir = cmd->next;
	proc = get_process();
	if (cmd->has_redirection)
		create_fd_redir(cmd, redir->child);
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{		
		get_process()->env_cpy = proc->env_cpy;
		dup2(pipe->pipe->fd_out, STDOUT_FILENO);
		file_redirection(cmd);
		close(pipe->pipe->fd_out);
		proc->errnum = exec_commands(cmd, false);
		close_files_redirections(cmd);
		exit(proc->errnum);
	}
	close_files_redirections(cmd);
	pipe->pid = pid;
	return (pipe->next);
}


t_cmd	*fork_last_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;
	t_cmd		*redir;

	cmd = pipe->child;
	redir = cmd->next;
	proc = get_process();
	if (cmd->has_redirection)
		create_fd_redir(cmd, redir->child);
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		get_process()->env_cpy = proc->env_cpy;
		dup2(pipe->prev->pipe->fd_in, STDIN_FILENO);
		file_redirection(cmd);
		close_prev_pipes(pipe);
		proc->errnum = exec_commands(cmd, false);
		close_files_redirections(cmd);
		exit(proc->errnum);
	}
	close_files_redirections(cmd);
	pipe->pid = pid;
	close_prev_pipes(pipe);
	return (pipe->next);
}

t_cmd	*fork_middle_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;
	t_cmd		*redir;

	cmd = pipe->child;
	redir = cmd->next;
	proc = get_process();
	if (cmd->has_redirection)
		create_fd_redir(cmd, redir->child);
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		get_process()->env_cpy = proc->env_cpy;
		dup2(pipe->prev->pipe->fd_in, STDIN_FILENO);
		if (cmd->has_redirection)
		{
			close(prev_pipe(pipe)->fd_in);
			file_redirection(cmd);
		}
		else
		{
			dup2(pipe->pipe->fd_out, STDOUT_FILENO);
			close_prev_pipes(pipe);
			close(pipe->pipe->fd_out);
		}
		proc->errnum = exec_commands(cmd, false);
		close_files_redirections(cmd);
		exit(proc->errnum);
	}
	pipe->pid = pid;
	close_files_redirections(cmd);
	close(pipe->pipe->fd_out);
	close_prev_pipes(pipe);
	return (pipe->next);
}

int32_t	fork_pipes(t_cmd *cmd)
{
	t_cmd		*start;
	t_process	*proc;
	t_cmd		*pipe;

	pipe = cmd;
	proc = get_process();
	start = pipe;
	pipe = fork_first_child(pipe);
	while (pipe && pipe->next)
		pipe = fork_middle_child(pipe);
	if (pipe)
		pipe = fork_last_child(pipe);
	wait_childs(start);
	return (proc->errnum);
}
