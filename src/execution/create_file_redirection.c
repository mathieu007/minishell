#include "minishell.h"

bool	is_redirection(t_cmd_seq seq)
{
	return (seq && (seq == CMD_FILEIN
		|| seq == CMD_FILEOUT
		|| seq== CMD_FILEOUT_APPPEND
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

t_cmd	*create_redir_out(t_cmd *cmd)
{
	// char	*cmd_str;
	// int32_t	i;

	build_token_environement(cmd->token);
	cmd = parse_redirect(cmd);
	// if (count_args2(cmd->args) > 1)
	// {
	// 	i = 1;
	// 	cmd_str = cmd->prev->token->str;
	// 	cmd_str = ft_strjoinfree(cmd_str, " ");
	// 	while (cmd->args[i])
	// 	{
	// 		cmd_str = ft_strjoinfree(cmd_str, cmd->args[i]);
	// 		cmd_str = ft_strjoinfree(cmd_str, " ");
	// 		i++;
	// 	}
	// }
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
