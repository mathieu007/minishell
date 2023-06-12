#include "minishell.h"

bool	is_redirection(t_cmd_seq seq)
{
	return (seq && (seq == CMD_FILEIN
			|| seq == CMD_FILEOUT
			|| seq == CMD_FILEOUT_APPPEND
			|| seq == CMD_HEREDOC));
}

// int32_t count_args2(char **args)
// {
// 	int32_t count;

// 	count = 0;
// 	if (args)
// 	{
// 		while (args[count])
// 			count++;
// 	}
// 	return (count);
// }

t_cmd	*create_redir_out(t_cmd *main, t_cmd *cmd)
{
	build_redir_token_environement(cmd->token, cmd->cmd_seq_type);
	if (get_process()->errnum > 0)
	{
		while (cmd->next
			&& (cmd->next->cmd_seq_type == CMD_FILEIN
				|| cmd->next->cmd_seq_type == CMD_FILEOUT
				|| cmd->next->cmd_seq_type == CMD_FILEOUT_APPPEND))
			cmd = cmd->next;
		return (cmd->next);
	}
	cmd = parse_redirect(main, cmd);
	open_out_redir_fd(cmd);
	return (cmd->next);
}

t_cmd	*create_redir_append_out(t_cmd *main, t_cmd *cmd)
{
	build_redir_token_environement(cmd->token, cmd->cmd_seq_type);
	if (get_process()->errnum > 0)
	{
		while (cmd->next
			&& (cmd->next->cmd_seq_type == CMD_FILEIN
				|| cmd->next->cmd_seq_type == CMD_FILEOUT
				|| cmd->next->cmd_seq_type == CMD_FILEOUT_APPPEND))
			cmd = cmd->next;
		return (cmd->next);
	}
	cmd = parse_redirect(main, cmd);
	open_out_append_redir_fd(cmd);
	return (cmd->next);
}

t_cmd	*create_redir_in(t_cmd *main, t_cmd *cmd)
{
	build_redir_token_environement(cmd->token, cmd->cmd_seq_type);
	if (get_process()->errnum > 0)
	{
		while (cmd->next
			&& (cmd->next->cmd_seq_type == CMD_FILEIN
				|| cmd->next->cmd_seq_type == CMD_FILEOUT
				|| cmd->next->cmd_seq_type == CMD_FILEOUT_APPPEND))
			cmd = cmd->next;
		return (cmd->next);
	}
	cmd = parse_redirect(main, cmd);
	open_in_redir_fd(cmd);
	return (cmd->next);
}

t_cmd	*create_redir_heredoc(t_cmd *main, t_cmd *cmd)
{
	build_redir_token_environement(cmd->token, cmd->cmd_seq_type);
	cmd = parse_redirect(main, cmd);
	open_in_redir_fd(cmd);
	return (cmd->next);
}

/// @brief at first main == cmd
/// @param main 
/// @param cmd 
/// @return
t_cmd	*create_redir(t_cmd *main, t_cmd *cmd)
{
	if (cmd->next && cmd->next->cmd_seq_type == CMD_FILEOUT)
		cmd = create_redir_out(main, cmd->next);
	else if (cmd->next && cmd->next->cmd_seq_type == CMD_FILEOUT_APPPEND)
		cmd = create_redir_append_out(main, cmd->next);
	else if (cmd->next && cmd->next->cmd_seq_type == CMD_FILEIN)
		cmd = create_redir_in(main, cmd->next);
	else if (cmd->next && cmd->next->cmd_seq_type == CMD_HEREDOC)
		cmd = create_redir_heredoc(main, cmd->next);
	else
		cmd = cmd;
	if (cmd->next && is_redirection(cmd->next->cmd_seq_type))
		cmd = create_redir(main, cmd);
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
