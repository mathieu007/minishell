/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/27 08:30:39 by mroy             ###   ########.fr       */
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
	proc->program = &ft_strchrlast(argv[0], '/')[1];
	proc->full_program_name = argv[0];
	if (!env || !*env)
		free_all_and_exit2(1, "error no environement path.");
	proc->env_cpy = free_t_env_cpy(proc->env_cpy);
	proc->env_cpy = init_env(proc);
}
