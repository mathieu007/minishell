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
