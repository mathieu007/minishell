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

void	sig_handler(int sig, siginfo_t *siginfo, void *context)
{
	t_process	*proc;
	(void)context;
	(void)sig;

	proc = get_process();
	if (siginfo->si_signo == SIGINT && proc->is_here_doc)
		exit(0);
	else if (siginfo->si_signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (siginfo->si_signo == SIGTERM)
		free_all_and_exit(0);
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
