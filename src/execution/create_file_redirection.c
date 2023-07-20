/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_file_redirection.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_redir_out(t_cmd *main, t_cmd *redir)
{
	open_out_redir_fd(redir);
	if (!redir->out_redir)
		return (NULL);
	main->out_redir = free_t_redirect(main->out_redir);
	main->out_redir = new_redirect();
	copy_redirection(main->out_redir, redir->out_redir);
	return (redir);
}

t_cmd	*create_redir_append_out(t_cmd *main, t_cmd *redir)
{
	open_out_append_redir_fd(redir);
	if (!redir->out_redir)
		return (NULL);
	main->out_redir = free_t_redirect(main->out_redir);
	main->out_redir = new_redirect();
	copy_redirection(main->out_redir, redir->out_redir);
	return (redir);
}

t_cmd	*create_redir_in(t_cmd *main, t_cmd *redir)
{
	open_in_redir_fd(redir);
	if (!redir->in_redir)
		return (NULL);
	main->in_redir = free_t_redirect(main->in_redir);
	main->in_redir = new_redirect();
	copy_redirection(main->in_redir, redir->in_redir);
	return (redir);
}

t_cmd	*create_redir_heredoc(t_cmd *main, t_cmd *redir)
{
	open_redir_heredoc(redir);
	if (!redir->in_redir)
		return (NULL);
	main->in_redir = free_t_redirect(main->in_redir);
	main->in_redir = new_redirect();
	main->in_redir->is_here_doc = true;
	copy_redirection(main->in_redir, redir->in_redir);
	return (redir);
}
