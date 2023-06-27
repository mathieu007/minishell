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
	if (pipe->fd_in != -1)
	{
		close(pipe->fd_in);
	}
	if (pipe->fd_out != -1)
	{
		close(pipe->fd_out);
	}
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
	pid_t	exited_pid;

	i = 0;
	while (cmd && cmd->pid)
	{
		exited_pid = waitpid(cmd->pid, &status, 0);
		if (exited_pid == -1)
			free_all_and_exit2(errno, "waitpid error");
		cmd = cmd->next;
		i++;
	}
}

void	file_redirection(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->has_redirection && cmd->in_redir && cmd->in_redir->fd > 0)
	{
		redirect_input(cmd);
		if (cmd->out_redir && cmd->out_redir->fd > 0)
			redirect_output(cmd);
	}
	else if (cmd->has_redirection && cmd->out_redir && cmd->out_redir->fd > 0)
		redirect_output(cmd);
}

t_cmd	*fork_first_child(t_cmd *pipe)
{
	pid_t		pid;
	t_process	*proc;
	t_cmd		*cmd;
	t_cmd		*redir;

	cmd = pipe->child;
	pipe_cmd(pipe);
	redir = cmd->next;
	proc = get_process();
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (!cmd)
		return (pipe->next);
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		if (cmd->has_redirection)
			create_fd_redir(cmd, redir->child);
		dup2(pipe->pipe->fd_out, STDOUT_FILENO);
		close(pipe->pipe->fd_out);
		close(pipe->pipe->fd_in);
		file_redirection(cmd);
		close_files_redirections(cmd);
		proc->errnum = exec_commands(cmd, false);
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
	t_cmd		*redir;

	cmd = pipe->child;
	redir = cmd->next;
	proc = get_process();
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (!cmd)
		return (pipe->next);
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		if (cmd->has_redirection)
			create_fd_redir(cmd, redir->child);
		dup2(pipe->prev->pipe->fd_in, STDIN_FILENO);
		close_prev_pipes(pipe);
		file_redirection(cmd);
		close_files_redirections(cmd);
		proc->errnum = exec_commands(cmd, false);
		exit(proc->errnum);
	}
	pipe->pid = pid;
	close_prev_pipes(pipe);
	return (pipe->next);
}

void	dup_close_middle_pipes(t_cmd *cmd, t_cmd *pipe, t_cmd *redir)
{
	if (cmd->has_redirection)
		create_fd_redir(cmd, redir->child);
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
	t_cmd		*redir;

	cmd = pipe->child;
	pipe_cmd(pipe);
	redir = cmd->next;
	proc = get_process();
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (!cmd)
		return (pipe->next);
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		dup_close_middle_pipes(cmd, pipe, redir);
		file_redirection(cmd);
		close_files_redirections(cmd);
		proc->errnum = exec_commands(cmd, false);
		exit(proc->errnum);
	}
	pipe->pid = pid;
	close(pipe->pipe->fd_out);
	close_prev_pipes(pipe);
	return (pipe->next);
}

int32_t	exec_pipes_cmds(t_cmd *pipe_group)
{
	t_cmd		*start;
	t_process	*proc;
	t_cmd		*pipe;

	pipe = pipe_group->child;
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
