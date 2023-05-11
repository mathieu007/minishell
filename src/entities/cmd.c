/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/11 11:37:10 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd()
{
	t_cmd	*new;

	new = ft_calloc(1, sizeof(t_cmd));
	if (new == NULL)
		return (NULL);
	return (new);
}

inline t_cmd	*get_first_cmd(void)
{
	static t_cmd	*cmd;

	if (cmd == NULL)
	{
		cmd = new_cmd();
		get_data()->last_cmd = cmd;
		return (cmd);
	}		
	return (&cmd[0]);
}

inline t_cmd	*get_last_cmd(void)
{
	if (!get_data()->last_cmd)
		return (get_first_cmd());
	return (get_data()->last_cmd);
}

inline t_cmd	*add_cmd(void)
{
	t_cmd		*last;
	t_cmd		*new;

	last = get_last_cmd();
	if (!last->prev)
		new = last;
	else
	{
		new = new_cmd();
		if (new == NULL)
			return (NULL);
		last->next = new;
		new->prev = last;
	}
	get_data()->tokens_count++;
	get_data()->last_cmd = new;
	return (new);
}

void	*free_cmd(t_cmd *cmd)
{
	free_pipe(cmd);
	free_redirect(cmd);
	return (NULL);
}
