#include "minishell.h"

void	*create_redir_out(t_cmd *cmd)
{
	t_redirect	*redir;

	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	open_out_redir_fd(cmd);
	redir = cmd->out_redir;
	if (cmd && cmd->prev)
		cmd->prev->out_redir = redir;
	return (NULL);
}

void	*create_redir_append_out(t_cmd *cmd)
{
	t_redirect	*redir;

	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	open_out_append_redir_fd(cmd);
	redir = cmd->out_redir;
	if (cmd && cmd->prev)
		cmd->prev->out_redir = redir;
	return (NULL);
}

void	redirect_output(t_cmd *cmd)
{
	if (cmd->out_redir)
	{
		if (dup2(cmd->out_redir->fd, STDOUT_FILENO) == -1)
		{
			perror("Failed to redirect output");
			free_all_and_exit(EXIT_FAILURE);
		}
		close(cmd->out_redir->fd);
	}
}
