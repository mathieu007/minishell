/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 13:17:35 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	update_variable(t_env_cpy *current, char *variable, char *value,
		size_t len)
{
	bool	swap;

	swap = false;
	while (current)
	{
		if (ft_strncmp(variable, current->variable, len) == 0)
		{
			current->value = free_ptr(current->value);
			current->value = ft_strdup(value);
			swap = true;
			break ;
		}
		current = current->next;
	}
	return (swap);
}

int32_t	add_or_update_env(char *name, char *value)
{
	size_t		len;
	bool		swap;
	t_process	*proc;

	proc = get_process();
	if (!value)
		return (0);
	len = ft_strlen(name);
	swap = update_variable(proc->env_cpy, name, value, len);
	if (!swap)
		add_env_node(proc, name, value);
	return (1);
}

bool	set_cwd(char *cwd)
{
	t_process	*proc;
	char		*oldpwd;

	if (!cwd)
		return (false);
	proc = get_process();
	oldpwd = get_env_value("PWD");
	add_or_update_env("OLDPWD", oldpwd);
	proc->cwd = free_ptr(proc->cwd);
	proc->cwd = cwd;
	add_or_update_env("PWD", cwd);
	free(oldpwd);
	return (true);
}

/// @brief this function automatically get the updated current working directory
/// even if an other process modify the current working directory location
/// ex: rename, move, or delete the current workin dir.
/// There is multiple fall back layer, don't know if bash do it that way.
/// @param cmd
/// @return
char	*get_cwd(void)
{
	static char	buffer[PATH_MAX + 1];
	t_process	*proc;
	char		*cur_dir;

	proc = get_process();
	cur_dir = getcwd(&buffer[0], PATH_MAX + 1);
	if (!cur_dir)
		free_all_and_exit2(1,
			"An error occur while trying to get the current working dir.");
	set_cwd(ft_strdup(cur_dir));
	return (ft_strdup(proc->cwd));
}

char	*get_cwd_with_backslash(void)
{
	char	*cwd;

	cwd = get_cwd();
	if (cwd && !ft_strisempty(cwd) && cwd[ft_strlen(cwd) - 1] != '/')
		cwd = ft_strjoinfree(cwd, "/");
	return (cwd);
}
