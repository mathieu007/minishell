/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 14:39:24 by math              #+#    #+#             */
/*   Updated: 2023/06/08 07:15:17 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect	*new_redirect(t_cmd *cmd)
{
	t_redirect	*new;

	new = malloc(sizeof(t_redirect));
	if (new == NULL)
		return (NULL);
	new->fd = -1;
	new->fd = -1;
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
