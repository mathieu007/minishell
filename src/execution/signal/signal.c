#include "minishell.h"
#include <termios.h>

void disable_ctrl_c_output()
{
    struct termios term;
    if (tcgetattr(STDIN_FILENO, &term) != 0) {
        perror("Error termios getting terminal attributes");
        exit(1);
    }
//In the line term.c_lflag &= ~ECHOCTL, the ~ operator is used to invert 
//the bits of the ECHOCTL flag.
    term.c_lflag &= ~ECHOCTL; // Disable echoing of control characters

    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0) {
        perror("Error termios setting terminal attributes");
        exit(1);
    }
}

void sig_handler(int sig, siginfo_t *siginfo, void *context)
{
    (void)context;
	(void)sig;
    if (siginfo->si_signo == SIGINT)
    {
        write(1, "\n",1);
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
    signal(SIGQUIT, SIG_IGN);
	
}
