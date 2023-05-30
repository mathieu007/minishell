
#include "minishell.h"

void sig_handler(int sig, siginfo_t *siginfo, void *context)
{
    (void)context;
	(void)sig;
    if (siginfo->si_signo == SIGINT)
    {
        printf("\nInterrupted with Ctrl + C\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else if (siginfo->si_signo == SIGTERM)
    {
        printf("Exit\n");
        exit(EXIT_SUCCESS);
    }

}

void setup_signal_handlers()
{
    struct sigaction sa;
    sa.sa_sigaction = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    // Set up signal handlers for SIGINT and SIGTERM
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
   sigaction(SIGQUIT, &sa, NULL);
	
}
