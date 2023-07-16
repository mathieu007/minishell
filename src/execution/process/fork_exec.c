/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/13 09:25:15 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*fork_exec(t_cmd *cmd, void *(*child)(t_cmd *cmd),
	void *(*parent)(t_cmd *cmd))
{
	pid_t	pid;

	pid = ft_fork();
	if (pid == 0)
		child(cmd);
	else
		parent(cmd);
	return (cmd);
}
