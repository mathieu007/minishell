#include "minishell.h"

t_subshell	*get_subshell(void)
{
	static t_subshell	*process_shell;

	if (!process_shell)
	{
		process_shell = malloc(sizeof(t_subshell));
		if (!process_shell)
			return (NULL);
		process_shell->cwd = get_cwd()
	}
	return (process_shell);
}

void	init_subshell(void)
{
	
}