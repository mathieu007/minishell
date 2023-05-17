/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/17 08:57:53 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_ptr(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

void	free_all()
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
	t_token_group	*token_group;
	t_data 			*data;
	t_cmd 			*cmd; 
	
	data = get_data();
	token_group = data->token_groups;
	cmd = data->cmds;	
	free_all();
	exit(status);
}
