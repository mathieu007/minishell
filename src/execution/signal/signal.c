/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:02:29 by mroy              #+#    #+#             */
/*   Updated: 2023/08/02 09:15:31 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_sigint(void)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_handler(int sig, siginfo_t *siginfo, void *context)
{
	t_process	*proc;

	(void)context, (void)sig;
	proc = get_process();
	proc->signal = siginfo->si_signo;
	if (siginfo->si_signo == SIGINT && proc->execution == EXEC_CONTINUATION)
		return ;
	else if (siginfo->si_signo == SIGINT && proc->execution == EXEC_HEREDOC)
		return (close_all_pipes());
	else if (siginfo->si_signo == SIGINT
		&& (proc->execution == EXEC_CAT || proc->execution == EXEC_SLEEP))
	{
		write(1, "\n", 1);
		proc->signal = 0;
		kill(proc->pid, SIGTERM);
		proc->errnum = 130;
	}
	else if (siginfo->si_signo == SIGINT)
		print_sigint();
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
	signal(SIGQUIT, SIG_IGN);
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
	if (proc->execution == EXEC_CAT || proc->execution == EXEC_SLEEP)
	{
		write(1, "QUIT : 3\n", 10);
		rl_on_new_line();
		rl_replace_line("", 0);
		proc->errnum = 131;
		kill(proc->pid, SIGTERM);
	}
}
