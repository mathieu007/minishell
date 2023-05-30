#include "minishell.h"

void	close_pipe_fds(t_cmd *cmd)
{
	close(cmd->pipe->fd_in);
	close(cmd->pipe->fd_out);
}
