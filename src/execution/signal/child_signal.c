/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_signal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/08/02 10:00:10 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

void	sig_child_readline_handler(int sig, siginfo_t *siginfo, void *context)
{
	t_process	*proc;

	(void)context;
	(void)sig;
	proc = get_process();
	proc->signal = siginfo->si_signo;
	if (siginfo->si_signo == SIGINT && (proc->execution == EXEC_HEREDOC
			|| proc->execution == EXEC_CONTINUATION))
	{
		proc->errnum = 1;
		close_all_pipes();
		close_all_fds();
		free_all_and_exit(proc->errnum);
	}
	else if (siginfo->si_signo == SIGTERM)
		proc->signal = SIGTERM;
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

void	sig_child_handler(int sig, siginfo_t *siginfo, void *context)
{
	t_process	*proc;

	(void)context;
	(void)sig;
	proc = get_process();
	proc->signal = siginfo->si_signo;
	if (siginfo->si_signo == SIGINT && (proc->execution == EXEC_CAT
			|| proc->execution == EXEC_SLEEP))
	{
		close_all_pipes();
		proc->errnum = 130;
	}
	else if (siginfo->si_signo == SIGTERM)
		proc->signal = SIGTERM;
}

void	setup_child_signal_handlers(t_cmd *cmd)
{
	struct sigaction	sa;
	t_process			*proc;

	proc = get_process();
	if (ft_strequal(cmd->name, "cat") || ft_strendwith(cmd->name, "/cat")
		|| ft_strequal(cmd->name, "wc") || ft_strendwith(cmd->name, "/wc")
		|| ft_strequal(cmd->name, "grep") || ft_strendwith(cmd->name, "/grep"))
		proc->execution = EXEC_CAT;
	else if (ft_strequal(cmd->name, "sleep") 
		|| ft_strendwith(cmd->name, "/sleep"))
		proc->execution = EXEC_SLEEP;
	sa.sa_sigaction = sig_child_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	signal(SIGQUIT, sigquit_handler);
}
