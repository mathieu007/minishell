/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/02 13:32:11 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	fork_exec(t_cmd *cmd, void *(*child)(t_cmd *cmd),
	void *(*parent)(t_cmd *cmd))
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork error:");
		free_all_and_exit();
		return (1);
	}
	if (pid == 0)
		child(cmd);
	else
		parent(cmd);
	return (0);	
}
