#include "minishell.h"

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
	while (cmd && cmd->pipe)
	{
		waitpid(cmd->pid, &status, 0);
		cmd = cmd->next;
		i++;
	}
	waitpid(cmd->pid, &status, 0);
}

void	fork_first_child(t_cmd *cmd)
{
	pid_t		pid;
	t_process 	*proc;

	proc = get_process();

	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		get_process()->env_cpy = proc->env_cpy;
		build_token_environement(cmd->token);
		if (contains_parentheses(cmd->token))
			proc->errnum = exec_sequence(cmd->child);
		cmd = parse_at_execution(cmd);
		dup2(cmd->pipe->fd_out, STDOUT_FILENO);
		close(cmd->pipe->fd_in);
		close(cmd->pipe->fd_out);
		proc->errnum = cmd->func(cmd);
		exit(proc->errnum);
	}	
	cmd->pid = pid;
}

void	fork_last_child(t_cmd *cmd)
{
	pid_t		pid;
	t_process 	*proc;

	proc = get_process();
	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		get_process()->env_cpy = proc->env_cpy;
		build_token_environement(cmd->token);
		if (contains_parentheses(cmd->token))
			proc->errnum = exec_sequence(cmd->child);
		cmd = parse_at_execution(cmd);
		dup2(cmd->prev->pipe->fd_in, STDIN_FILENO);
		close(cmd->prev->pipe->fd_in);
		close(cmd->prev->pipe->fd_out);
		proc->errnum = cmd->func(cmd);
		exit(proc->errnum);
	}
	close(cmd->prev->pipe->fd_in);
	close(cmd->prev->pipe->fd_out);
	cmd->pid = pid;
}

void	fork_middle_child(t_cmd *cmd)
{
	pid_t		pid;
	t_process 	*proc;

	proc = get_process();
	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		get_process()->env_cpy = proc->env_cpy;
		build_token_environement(cmd->token);
		if (contains_parentheses(cmd->token))
			proc->errnum = exec_sequence(cmd->child);
		cmd = parse_at_execution(cmd);
		dup2(cmd->prev->pipe->fd_in, STDIN_FILENO);
		dup2(cmd->pipe->fd_out, STDOUT_FILENO);
		close(cmd->pipe->fd_out);
		close(cmd->prev->pipe->fd_in);
		close(cmd->prev->pipe->fd_out);
		proc->errnum = cmd->func(cmd);
		exit(proc->errnum);
	}
	close(cmd->prev->pipe->fd_in);
	close(cmd->prev->pipe->fd_out);
	close(cmd->pipe->fd_out);
	cmd->pid = pid;
}

void	*fork_pipes(t_cmd *cmd)
{
	t_cmd	*start;

	start = cmd;
	if (cmd)
		fork_first_child(cmd);
	cmd = cmd->next;
	while (cmd && cmd->next && cmd->next->cmd_seq_type == CMD_PIPE)
	{
		fork_middle_child(cmd);
		cmd = cmd->next;
	}
	if (cmd && cmd->cmd_seq_type == CMD_PIPE)
		fork_last_child(cmd);
	wait_childs(start);
	return (NULL);
}
