

#include "minishell.h"

void	sig_handler(int sig, siginfo_t *siginfo, void *context)
{
	(void)sig;
	(void)context;
	if (siginfo->si_signo == SIGINT)
	{
		printf("\nInterrupted with Ctrl + C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (siginfo->si_signo == SIGQUIT)
	{
		printf("\nInterrupted with \"Ctrl+backsalsh\"\n");
		signal(SIGQUIT,SIG_IGN);
	}
}
