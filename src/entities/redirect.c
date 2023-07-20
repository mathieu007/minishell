/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 14:39:24 by math              #+#    #+#             */
/*   Updated: 2023/07/20 13:41:41 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect	*new_redirect()
{
	t_redirect	*new;

	new = ft_calloc(1, sizeof(t_redirect));
	if (new == NULL)
		free_all_and_exit2(errno, "malloc error");
	new->fd = -1;
	return (new);
}
