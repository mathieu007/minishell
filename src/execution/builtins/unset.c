#include "minishell.h"


/// @brief Error must change: (data->env_cpy by cmd->env_cpy), each cmd may have it's own environement
/// sometime cmd->env_cpy is the exact ptr than data->env_cpy, ex: when command are sequential, but sometime,
/// when command are piped they have their own environement variable that are not modified by other commands
/// before the execution of the command i update the cmd environement variable accordingly,
/// not a single command should access data->env_cpy instead cmd->env_cpy.
/// @param cmd 
/// @return 
int	unset_cmd(t_cmd *cmd)
{
	(void)cmd;
	// t_env_cpy	*current;
	// int			i;
	// size_t		len;
	// t_data		*data;

	// data = get_data();
	// i = 0;
	// current = data->env_cpy;
	// if (cmd->options[0] != NULL)
	// {
	// 	printf("Unset option \"%s\" not handle \n", cmd->options[0]);
	// 	return(1);
	// }
	// while (cmd->args[i])
	// {
	// 	while (*environ)
	// 	{
	// 		len = ft_strlen(cmd->args[i]);
	// 		if (ft_strnstr(cmd->args[i], current->variable, len) != 0)
	// 		{
	// 			if (!current->prev)
	// 				data->env_cpy = data->env_cpy->next;
	// 			if (current->prev)
	// 				current->prev->next = current->next;
	// 			if (!current->next)
	// 				current->prev->next = NULL;
	// 		}
	// 		current = current->next;
	// 	}
	// 	i++;
	// }
	return(0);
}
