/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 14:39:24 by math              #+#    #+#             */
/*   Updated: 2023/05/03 08:13:15 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect	*new_redirect(t_cmd *cmd)
{
	t_redirect	*new;

	new = malloc(sizeof(t_redirect));
	if (new == NULL)
		return (NULL);
	new->fd_in = -1;
	new->fd_out = -1;
	cmd->redirect = new;
	return (new);
}

void	*free_redirect(t_cmd *cmd)
{
	if (cmd->redirect != NULL)
		free(cmd->redirect);
	cmd->redirect = NULL;
	cmd = cmd->next;
	return (NULL);
}
