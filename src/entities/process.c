/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/01 18:47:25 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline t_process	*get_process(void)
{
	static t_process	proc[1];

	return (&proc[0]);
}

char	*copy_path(char *program_path, char *buffer, char *prog_name)
{
	char	*path;

	path = NULL;
	if (!prog_name || ft_strisempty(prog_name))
		path = get_shell_path("./bin/minishell");
	if (!path)
		path = get_shell_path("./minishell");
	if (!path)
		path = get_shell_path(prog_name);
	if (path)
		ft_strcpy(buffer, path);
	else
		free_all_and_exit2(1, "error could not get shell path.");
	add_env_node(get_process(), "full_program_name", buffer);
	program_path = get_env_value("full_program_name");
	free_ptr(path);
	return (program_path);
}

void	add_program_path(char *progname)
{
	static char		buffer[1024];
	char			*program_path;
	t_process		*proc;

	proc = get_process();
	program_path = get_env_value("full_program_name");
	if (!program_path)
		program_path = copy_path(program_path, buffer, progname);
	else
		ft_strcpy(buffer, program_path);
	if (ft_strlen(program_path) > 1024)
	{
		free_ptr(program_path);
		free_all_and_exit2(1, "full_program_name path too long.");
	}
	proc->full_program_name = buffer;
	free_ptr(program_path);
	if (!check_path_is_execve(proc->full_program_name))
		return (free_all_and_exit2(1, "minishell not executable"));
}

void	init_data(int32_t argc, char **argv, char **env)
{
	t_process		*proc;

	proc = get_process();
	proc->argc = argc;
	proc->argv = argv;
	proc->env = env;
	proc->program = &ft_strchrlast(argv[0], '/')[1];
	if (!env || !*env)
		free_all_and_exit2(1, "error no environement path.");
	proc->env_cpy = free_t_env_cpy(proc->env_cpy);
	proc->env_cpy = init_env(proc);
	add_program_path(argv[0]);
}
