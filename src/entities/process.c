/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/21 15:53:38 by bmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline t_process	*get_process(void)
{
	static t_process	proc[1];

	return (&proc[0]);
}

void	init_data(int32_t argc, char **argv, char **env)
{
	t_process		*proc;

	proc = get_process();
	proc->argc = argc;
	proc->argv = argv;
	proc->env = env;
	proc->env_cpy = init_env(proc);
}

