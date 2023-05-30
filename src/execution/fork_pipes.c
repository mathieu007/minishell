#include "minishell.h"

void	write_msg(int32_t stderror, char *msg)
{
	if (msg)
		write(stderror, msg, ft_strlen(msg));
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
	while (cmd && cmd->pipe)
	{
		waitpid(cmd->pipe->pid, &status, 0);
		cmd = cmd->next;
		i++;
	}
}

void	fork_first_child(t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(cmd->pipe->fd_in);
		dup2(cmd->pipe->fd_out, STDOUT_FILENO);
		//close_pipe_fds(cmd);
		close(cmd->pipe->fd_out);
		cmd->func(cmd);
	}
	close(cmd->pipe->fd_in);
	close(cmd->pipe->fd_out);
	cmd->pipe->pid = pid;
}

void	fork_last_child(t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		dup2(cmd->pipe->fd_in, STDIN_FILENO);
		close(cmd->pipe->fd_out);
		close(cmd->pipe->fd_in);
		//close_pipe_fds(cmd);
		cmd->func(cmd);
	}
	close(cmd->prev->pipe->fd_in);
	close(cmd->prev->pipe->fd_out);
	close(cmd->pipe->fd_in);
	close(cmd->pipe->fd_out);
	cmd->pipe->pid = pid;
}

void	fork_middle_child(t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		dup2(cmd->prev->pipe->fd_in, STDIN_FILENO);
		dup2(cmd->pipe->fd_out, STDOUT_FILENO);
		close_pipe_fds(cmd);
		cmd->func(cmd);
	}
	close(cmd->prev->pipe->fd_in);
	close(cmd->prev->pipe->fd_out);
	close(cmd->pipe->fd_out);
	cmd->pipe->pid = pid;
}

void	exec_pipes(t_cmd *cmd)
{
	int32_t	i;
	t_cmd	*start;

	i = 0;
	// dup2(1, STDOUT_FILENO);
	// dup2(0, STDIN_FILENO);
	start = cmd;
	while (cmd && cmd->pipe)
	{
		if (i == 0)
			fork_first_child(cmd);
		else if (!cmd->next || !cmd->next->pipe)
			fork_last_child(cmd);
		else
			fork_middle_child(cmd);
		cmd = cmd->next;
		i++;
	}
	wait_childs(start);
}
