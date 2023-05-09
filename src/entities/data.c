/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 13:57:27 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline t_data	*get_data(void)
{
	static t_data	data;	
	
	return (&data);
}

void init_data(int32_t argc, char **argv, char **env)
{
	t_data *data;

	data = get_data();
	data->argc = argc;
	data->argv = argv;
	data->env = env;
}

