#include "minishell.h"

void	*free_2d_array(void **tab)
{
	int	i;

	if (!tab)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	return (NULL);
}

void	*free_2d_char_array(char **tab)
{
	int	i;

	if (!tab)
		return (NULL);
	i = 0;
	while (tab[i] != NULL)
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	return (NULL);
}

void	print_reverse_path(t_token *current)
{
	while (current)
	{
		printf("rev path: %s\n", current->str);
		current = current->parent;
	}
}

int	check_for_double_free(t_token *start)
{
	t_token	*current;

	if (!start->next)
		current = start->child;
	else
		current = start->next;
	while (current)
	{
		if (current == start)
		{
			print_reverse_path(current);
			return (1);
		}
		if (!current->next)
			current = current->child;
		else
			current = current->next;
	}
	return (0);
}

void	*find_double_free(t_token *token)
{
	t_token	*start;

	start = token;
	if (!token)
		return (NULL);
	while (start)
	{
		if (check_for_double_free(start))
			return (NULL);
		if (!start->next)
			start = start->child;
		else
			start = start->next;
	}
	return (NULL);
}

// void	*find_double_free(t_token *token)
// {
// 	t_token	*start;
// 	t_token	*current;

// 	if (!token)
// 		return (NULL);
// 	start = token;
// 	while (start != NULL)
// 	{
// 		if (!start->next)
// 			current = start->child;
// 		else
// 			current = start->next;
// 		while (current != NULL)
// 		{
// 			if (current == start)
// 			{
// 				while (current)
// 				{
// 					printf("rev path: %s\n", current->str);
// 					current = current->parent;
// 				}
// 				return (NULL);
// 			}
// 			if (!current->next)
// 				current = current->child;
// 			else
// 				current = current->next;
// 		}
// 		if (!start->next)
// 			start = start->child;
// 		else
// 			start = start->next;
// 	}
// 	return (NULL);
// }
