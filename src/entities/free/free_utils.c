#include "minishell.h"

void	free_2d_array(void **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}


void	free_2d_char_array(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
