#include "minishell.h"

void	close_prev_pipes(t_cmd *cmd)
{
	if (!cmd)
		return ;
	cmd = cmd->prev;
	while (cmd && cmd->cmd_seq_type != CMD_PIPE)
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
	while (cmd && cmd->cmd_seq_type != CMD_PIPE)
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
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
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

void	exec_first_pipe_redirection(t_cmd *main, t_cmd *cmd)
{
	t_process	*proc;
	t_cmd		*last_in;
	t_cmd		*last_out;

	proc = get_process();
	last_in = last_in_redir(cmd);
	last_out = last_out_redir(cmd);
	if (last_in)
	{
		redirect_input(last_in);
		if (last_out)
			redirect_output(last_out);
	}
	else if (last_out)
		redirect_output(last_out);
	close_pipes(main->pipe);
	proc->errnum = main->func(main);
}

void	file_redirection(t_cmd *cmd)
{
	t_cmd		*last_in;
	t_cmd		*last_out;

	if (!cmd)
		return ;
	last_in = last_in_redir(cmd);
	last_out = last_out_redir(cmd);
	if (last_in)
	{
		redirect_input(last_in);
		if (last_out)
			redirect_output(last_out);
	}
	else if (last_out)
		redirect_output(last_out);
}

t_cmd	*fork_first_child(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	pid = fork();
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		if (cmd->next && cmd->next->is_redirection)
			create_fd_redir(cmd, cmd->next);
		get_process()->env_cpy = proc->env_cpy;
		dup2(cmd->pipe->fd_out, STDOUT_FILENO);
		if (cmd->next && cmd->next->is_redirection)
			file_redirection(cmd->next);
		close_pipes(cmd->pipe);
		proc->errnum = cmd->func(cmd);
		exit(proc->errnum);
	}
	cmd->pid = pid;
	cmd = cmd->next;
	while (cmd && cmd->is_redirection)
		cmd = cmd->next;
	return (cmd);
}

t_cmd	*fork_last_child(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;

	proc = get_process();
	pid = fork();
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{	
		if (cmd->next && cmd->next->is_redirection)
			create_fd_redir(cmd, cmd->next);
		get_process()->env_cpy = proc->env_cpy;
		dup2(prev_pipe(cmd)->fd_in, STDIN_FILENO);
		if (cmd->next && cmd->next->is_redirection)
			file_redirection(cmd->next);
		close_prev_pipes(cmd);		
		proc->errnum = cmd->func(cmd);
		exit(proc->errnum);
	}	
	cmd->pid = pid;
	close_prev_pipes(cmd);
	cmd = cmd->next;
	while (cmd && cmd->is_redirection)
		cmd = cmd->next;
	return (cmd);
}

t_cmd	*fork_middle_child(t_cmd *cmd)
{
	pid_t		pid;
	t_process 	*proc;

	proc = get_process();
	pid = fork();
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		if (cmd->next && cmd->next->is_redirection)
			create_fd_redir(cmd, cmd->next);
		get_process()->env_cpy = proc->env_cpy;
		dup2(prev_pipe(cmd)->fd_in, STDIN_FILENO);
		if (cmd->next && cmd->next->is_redirection)
		{
			close(prev_pipe(cmd)->fd_in);
			file_redirection(cmd->next);
		}			
		else
		{
			dup2(cmd->pipe->fd_out, STDOUT_FILENO);
			close_prev_pipes(cmd);
			close(cmd->pipe->fd_out);
		}		
		proc->errnum = cmd->func(cmd);
		exit(proc->errnum);
	}
	close(cmd->pipe->fd_out);
	close_prev_pipes(cmd);
	cmd->pid = pid;
	cmd = cmd->next;
	while (cmd && cmd->is_redirection)
		cmd = cmd->next;
	return (cmd);
}

void	*fork_pipes(t_cmd *cmd)
{
	t_cmd	*start;

	start = cmd;
	cmd = fork_first_child(cmd);
	while (cmd && !cmd->token->is_last_pipe)
		cmd = fork_middle_child(cmd);
	if (cmd && cmd->token->is_last_pipe)
		cmd = fork_last_child(cmd);
	wait_childs(start);
	return (NULL);
}
