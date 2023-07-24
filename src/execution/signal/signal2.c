/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/24 12:47:15 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_child_fds(t_cmd *cmd)
{
	while (cmd)
	{
		close_files_redirections(cmd);
		if (cmd->child)
			close_all_child_fds(cmd->child);
		cmd = cmd->next;
	}
}

void	close_all_fds()
{
	t_process	*proc;
	t_cmd		*cmd;

	proc = get_process();
	cmd = proc->cmds;
	while (cmd)
	{
		close_files_redirections(cmd);
		if (cmd->child)
			close_all_child_fds(cmd->child);
		cmd = cmd->next;
	}
	if (proc->continuation && proc->continuation->fd > 0)
	{
		close(proc->continuation->fd);
		proc->continuation->fd = -1;
	}
}

void	enable_ctrl_c_output(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) != 0)
	{
		perror("Error termios getting terminal attributes");
		exit(EXIT_FAILURE);
	}
	term.c_lflag |= ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0)
	{
		perror("Error termios setting terminal attributes");
		exit(EXIT_FAILURE);
	}
}

void	disable_ctrl_c_output(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) != 0)
	{
		perror("Error termios getting terminal attributes");
		exit(1);
	}
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0)
	{
		perror("Error termios setting terminal attributes");
		exit(1);
	}
}
