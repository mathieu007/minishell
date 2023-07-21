/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/21 08:35:05 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

void	sig_handler(int sig, siginfo_t *siginfo, void *context)
{
	t_process	*proc;
	t_cmd		*cmd;

	(void)context;
	(void)sig;
	proc = get_process();
	cmd = proc->cmds;
	if (siginfo->si_signo == SIGINT && (proc->in_here_doc
			|| proc->in_continuation))
		return ;
	else if (proc->in_cat && siginfo->si_signo == SIGINT)
		write(1, "\n", 2);
	else if (siginfo->si_signo == SIGINT)
	{
		write(1, "\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (siginfo->si_signo == SIGTERM)
	{
		close_all_fds(cmd);
		free_all_and_exit(0);
	}
}

void	close_all_fds(t_cmd *cmd)
{
	while (cmd)
	{
		close_files_redirections(cmd);
		if (cmd->child)
			close_all_fds(cmd->child);
		cmd = cmd->next;
	}
}

void	setup_child_realine_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = sig_child_readline_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	sigquit_handler(int val)
{
	t_process	*proc;
	t_cmd		*cmd;

	(void)val;
	proc = get_process();
	cmd = proc->cmds;
	write(1, "QUIT : 3\n", 9);
	close_all_fds(cmd);
	reset_cmd();
	rl_on_new_line();
	rl_replace_line("", 0);
}
