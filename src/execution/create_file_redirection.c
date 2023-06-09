#include "minishell.h"

bool	is_redirection(t_cmd *cmd)
{
	return (cmd && (cmd->cmd_seq_type == CMD_FILEIN
		|| cmd->cmd_seq_type == CMD_FILEOUT
		|| cmd->cmd_seq_type == CMD_FILEOUT_APPPEND
		|| cmd->cmd_seq_type == CMD_HEREDOC));
}

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

t_cmd	*create_redir_in(t_cmd *cmd)
{
	build_token_environement(cmd->token);
	cmd = parse_redirect(cmd);
	open_in_redir_fd(cmd);
	return (cmd->next);
}

t_cmd	*create_redir_heredoc(t_cmd *cmd)
{
	build_token_environement(cmd->token);
	cmd = parse_redirect(cmd);
	open_in_redir_fd(cmd);
	return (cmd->next);
}

t_cmd	*create_redir(t_cmd *cmd)
{
	if (cmd->next && cmd->next->cmd_seq_type == CMD_FILEOUT)
		cmd = create_redir_out(cmd->next);
	else if (cmd->next && cmd->next->cmd_seq_type == CMD_FILEOUT_APPPEND)
		cmd = create_redir_append_out(cmd->next);	
	else if (cmd->next && cmd->next->cmd_seq_type == CMD_FILEIN)
		cmd = create_redir_in(cmd->next);
	else if (cmd->next && cmd->next->cmd_seq_type == CMD_HEREDOC)
		cmd = create_redir_heredoc(cmd->next);
	return (cmd);
}

void	redirect_output(t_cmd *cmd)
{
	if (!cmd->out_redir)
		return ;
	if (dup2(cmd->out_redir->fd, STDOUT_FILENO) == -1)
		free_all_and_exit2(errno, "Could not redirect output");
	if (close(cmd->out_redir->fd) == -1)
		free_all_and_exit2(errno, "Could not close the fd");
}
