#include "minishell.h"

void	copy_redirection(t_redirect *dest, t_redirect *src)
{
	dest->fd = src->fd;
	dest->fd_is_temp = src->fd_is_temp;
	dest->file = free_ptr(dest->file);
	dest->input_file = free_ptr(dest->input_file);
	dest->file = ft_strdup(src->file);
	dest->input_file = ft_strdup(src->input_file);
	dest->flags = src->flags;
}

bool	is_redirection(t_cmd_seq seq)
{
	return (seq && (seq == CMD_FILEIN || seq == CMD_FILEOUT
			|| seq == CMD_FILEOUT_APPPEND || seq == CMD_HEREDOC));
}

inline bool	is_token_redirection(t_token_type type)
{
	return (type && (type == TK_GREAT || type == TK_GREATGREAT
			|| type == TK_LESS || type == TK_LESSLESS));
}
