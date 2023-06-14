#include "minishell.h"

void	*free_t_redirect(t_redirect *redirect)
{
	if (!redirect)
		return (NULL);
	redirect->file = free_ptr(redirect->file);
	redirect->input_file = free_ptr(redirect->input_file);
	free(redirect);
	return (NULL);
}

void	*free_t_env_cpy(t_env_cpy *env_cpy)
{
	t_env_cpy	*current;
	t_env_cpy	*next;

	current = env_cpy;
	while (current != NULL)
	{
		next = current->next;
		current->variable = free_ptr(current->variable);
		current->value = free_ptr(current->value);
		free(current);
		current = next;
	}
	return (NULL);
}

