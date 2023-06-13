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
	return (cmd);
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
	return (cmd);
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
	return (cmd);
}

t_cmd	*create_redir_heredoc(t_cmd *main, t_cmd *cmd)
{
	build_redir_token_environement(cmd->token, cmd->cmd_seq_type);
	cmd = parse_redirect(main, cmd);
	open_in_redir_fd(cmd);
	return (cmd);
}

/// @brief at first main == cmd
/// @param main 
/// @param cmd 
/// @return
t_cmd	*create_redir(t_cmd *main, t_cmd *cmd)
{
	if (cmd && cmd->cmd_seq_type == CMD_FILEOUT)
		create_redir_out(main, cmd);
	else if (cmd && cmd->cmd_seq_type == CMD_FILEOUT_APPPEND)
		create_redir_append_out(main, cmd);
	else if (cmd && cmd->cmd_seq_type == CMD_FILEIN)
		create_redir_in(main, cmd);
	else if (cmd && cmd->cmd_seq_type == CMD_HEREDOC)
		create_redir_heredoc(main, cmd);
	if (cmd && cmd->next && is_redirection(cmd->next->cmd_seq_type))
		cmd = create_redir(main, cmd->next);
	return (cmd);
}

void	close_redirections(t_cmd *cmd)
{
	if (!cmd->next || (!cmd->next->out_redir && !cmd->next->in_redir))
		return ;
	// if (is_redirection(cmd->next->cmd_seq_type))
	// {
	// 	while (cmd && (cmd->out_redir || cmd->in_redir))
	// 	{
	// 		if (cmd->out_redir)
	// 		{
	// 			if (close(cmd->out_redir->fd) == -1)
	// 				free_all_and_exit2(errno, "Could not close the fd");
	// 		}
	// 		else if (cmd->in_redir)
	// 		{
	// 			if (close(cmd->in_redir->fd) == -1)
	// 				free_all_and_exit2(errno, "Could not close the fd");
	// 		}
	// 		cmd = cmd->next;
	// 	}
	// }
}

void	redirect_output(t_cmd *cmd)
{
	if (!cmd || !cmd->out_redir)
		return ;

	if (dup2(cmd->out_redir->fd, STDOUT_FILENO) == -1)
		free_all_and_exit2(errno, "Could not redirect output");
	if (close(cmd->out_redir->fd) == -1)
		free_all_and_exit2(errno, "Could not close the fd");
}

void	redirect_input(t_cmd *cmd)
{
	if (!cmd || !cmd->in_redir)
		return ;
	
	if (dup2(cmd->in_redir->fd, STDIN_FILENO) == -1)
		free_all_and_exit2(errno, "Could not redirect input");
	if (close(cmd->in_redir->fd) == -1)
		free_all_and_exit2(errno, "Could not close the fd");
}
