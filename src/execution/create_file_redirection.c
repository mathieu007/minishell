#include "minishell.h"

t_cmd	*create_redir_out(t_cmd *cmd)
{
	build_token_environement(cmd->token);
	cmd = parse_redirect(cmd);
	open_out_redir_fd(cmd);
	return (cmd->next);
}

t_cmd	*create_redir_append_out(t_cmd *cmd)
{
	build_token_environement(cmd->token);
	cmd = parse_redirect(cmd);
	open_out_append_redir_fd(cmd);
	return (cmd->next);
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
		close(STDOUT_FILENO);

		close(3);
	}
}
