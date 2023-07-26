/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/26 12:12:54 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sig, siginfo_t *siginfo, void *context)
{
	t_process	*proc;

	(void)context, (void)sig;
	proc = get_process();
	if (siginfo->si_signo == SIGINT && proc->execution == EXEC_CONTINUATION)
		return ;
	else if (siginfo->si_signo == SIGINT && proc->execution == EXEC_HEREDOC)
		return ;
	else if (siginfo->si_signo == SIGINT && (proc->execution == EXEC_CAT
			|| proc->execution == EXEC_SLEEP))
	{
		write(1, "\n", 1);
		kill(proc->pid, SIGTERM);
		proc->errnum = 130;
	}
	else if (siginfo->si_signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (siginfo->si_signo == SIGTERM)
		proc->signal = SIGTERM;
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	signal(SIGQUIT, sigquit_handler);
}

void	reset_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	signal(SIGQUIT, sigquit_handler);
}

void	sigquit_handler(int val)
{
	t_process	*proc;

	proc = get_process();
	(void)val;
	if (proc->execution == EXEC_CAT
		|| proc->execution == EXEC_SLEEP)
	{
		write(1, "QUIT : 3\n", 10);
		rl_on_new_line();
		rl_replace_line("", 0);
		proc->errnum = 131;
		kill(proc->pid, SIGTERM);
	}
}
