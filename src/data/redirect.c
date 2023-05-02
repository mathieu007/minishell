/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 14:39:24 by math              #+#    #+#             */
/*   Updated: 2023/05/02 09:07:10 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect	*new_redirect(t_cmd *cmd)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (redir == NULL)
		return (NULL);
	redir->fd_in = -1;
	redir->fd_out = -1;
	cmd->redirect = redir;
	return (redir);
}

void	*free_redirect(t_cmd *cmd)
{
	if (cmd->redirect != NULL)
		free(cmd->redirect);
	cmd->redirect = NULL;
	cmd = cmd->next;
	return (NULL);
}
