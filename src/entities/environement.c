#include "minishell.h"

// t_env_cpy	*new_env(char *variable, char *value)
// {
// 	t_env_cpy	*node;

// 	node = ft_calloc(1, sizeof(t_env_cpy));
// 	if (node == NULL)
// 		return (NULL);
// 	node->value = value;
// 	node->variable = variable;
// 	node->next = NULL;
// 	node->prev = NULL;
// 	return (node);
// }

/// @brief The environ variable is not malloc, it's a variable that is part of the process's memory space
/// if we try to free a non malloc env, unexpected behaviours will occur.
/// @return 
void	dup_env(void)
{
	while (*environ)
	{		
		*environ = ft_strdup(*environ);
		environ++;
	}
	*environ = NULL;
}

//take a variable and return the value
char	*get_env_value(char *variable)
{
	size_t		len;
	char		*var;

	len = ft_strlen(variable);
	while (*environ)
	{
		var = ft_strjoin(variable, "=");
		if (ft_strnstr(*environ, var, len) == *environ)
			return (free(var), &(*environ)[len + 1]);
		free(var);
		environ++;
	}
	return (NULL);
}

// //make a copy of the environement variable
// t_env_cpy	*copy_env(void)
// {
// 	t_env_cpy	*head;
// 	t_env_cpy	*current;
// 	t_env_cpy	*new_head;
// 	t_env_cpy	*new_current;

// 	head = get_data()->env_cpy;
// 	current = head;
// 	if (current)
// 	{
// 		new_current = new_env(ft_strdup(current->variable),
// 				ft_strdup(current->value));
// 		new_head = new_current;
// 		while (current)
// 		{
// 			new_current->next = new_env(ft_strdup(current->variable),
// 					ft_strdup(current->value));
// 			new_current->next->prev = new_current;
// 			new_current = new_current->next;
// 			current = current->next;
// 		}
// 		return (new_head);
// 	}
// 	return (NULL);
// }
