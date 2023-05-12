/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/12 15:33:45 by bmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_all(t_token_group *token_group, t_cmd *cmd, t_data *data)
{
	if (token_group != NULL)
		free_t_token_group(token_group);
	if (cmd != NULL)
		free_t_cmd(cmd);
	if (data != NULL)
		free_t_data(data);
}

void	free_all_and_exit(t_token_group *token_group, t_cmd *cmd, t_data *data,
		int32_t status)
{
	free_all(token_group, cmd, data);
	exit(status);
}
