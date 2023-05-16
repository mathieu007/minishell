/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/16 19:23:40 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_ptr(void **ref_ptr)
{
	if (*ref_ptr)
	{
		free(*ref_ptr);
		*ref_ptr = NULL;
	}
	return (NULL);
}

void	free_all(void)
{
	t_token_group	*token_group;
	t_data 			*data;
	t_cmd 			*cmd;

	data = get_data();
	token_group = data->token_groups;
	cmd = data->cmds;
	if (token_group != NULL)
		free_t_token_group(token_group);
	if (cmd != NULL)
		free_t_cmd(cmd);
}

void	free_all_and_exit(int32_t status)
{
	free_all();
	exit(status);
}
