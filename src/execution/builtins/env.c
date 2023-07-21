/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/21 15:12:59 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	env_cmd(t_cmd *cmd)
{
	t_process	*data;
	t_env_cpy	*current;

	(void)cmd;
	data = get_process();
	if (!data->env_cpy)
		data->env_cpy = init_env(data);
	current = data->env_cpy;
	while (current && current->value != NULL)
	{
		ft_printf("%s=%s\n", current->variable, current->value);
		current = current->next;
	}
	return (1);
}
