/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/24 07:05:36 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline t_data	*get_data(void)
{
	static t_data	data[1];

	return (&data[0]);
}

void	init_data(int32_t argc, char **argv)
{
	t_data		*data;

	data = get_data();
	data->argc = argc;
	data->argv = argv;
}

