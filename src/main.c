/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 18:36:40 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/time.h>

int32_t	subshell(char **inputs)
{
	int32_t		ret;
	t_process	*proc;
	char		*str;

	str = *inputs;
	proc = get_process();
	proc->parent_in_fd = -1;
	proc->parent_out_fd = -1;
	proc->is_subshell = true;
	ret = exec_cmds(str);
	free_all_and_exit(proc->errnum);
	return (ret);
}

char	*get_prompt_input(void)
{
	char	*input;

	input = readline("MiniShell> ");
	if (!ft_strisempty(input))
		add_history(input);
	if (input == NULL)
	{
		ft_printf("\x1B[u\x1B[Aexit\n");
		free_all_and_exit(0);
	}
	return (input);
}

int32_t	main(int32_t argc, char **argv, char **env)
{
	char		*input;
	t_process	*proc;

	proc = get_process();
	init_data(argc, argv, env);
	get_process()->is_subshell = false;
	if (argc >= 2)
		return (subshell(&argv[1]));
	while (proc->signal == 0)
	{
		disable_ctrl_c_output();
		setup_signal_handlers();
		input = get_prompt_input();
		exec_cmds(input);
		free(input);
		if (proc->signal == SIGQUIT)
			free_all_and_exit(0);
	}
	free_all_and_exit(0);
	return (0);
}
