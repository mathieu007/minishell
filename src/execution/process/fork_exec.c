/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/23 08:38:57 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*fork_exec(t_cmd *cmd, void *(*child)(t_cmd *cmd),
	void *(*parent)(t_cmd *cmd))
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	if (pid == 0)
		child(cmd);
	else
		parent(cmd);
	return (cmd);
}
