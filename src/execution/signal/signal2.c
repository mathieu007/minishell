/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/21 08:35:05 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

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

void	sig_child_readline_handler(int sig, siginfo_t *siginfo, void *context)
{
	t_process	*proc;
	t_cmd		*cmd;

	(void)context;
	(void)sig;
	proc = get_process();
	cmd = proc->cmds;
	if (siginfo->si_signo == SIGINT && proc->in_here_doc)
	{
		write(1, "\n", 1);
		close_all_fds(cmd);
		free_all_and_exit(1);
	}
	else if (siginfo->si_signo == SIGTERM)
	{
		close_all_fds(cmd);
		free_all_and_exit(0);
	}
}
