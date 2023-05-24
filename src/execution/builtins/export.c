#include "minishell.h"

void	export_no_variable(void)
{
	
	// t_env_cpy	*head;
	// t_env_cpy	*current;
	// bool		swap;

	// head = init_env(data);
	// current = head;
	// while (current)
	// {
	// 	if (strcmp(current->variable, current->next->variable) > 0)
	// 	{
	// 		swap_node_value(current, current->next);
	// 		swap = 1;
	// 	}
	// 	if (swap == true)
	// 		current = head;
	// 	else
	// 		current = current->next;
	// }
	// current = head;
	// while (current)
	// {
	// 	printf("declare -x %s=\"%s\"\n", current->variable, current->value);
	// 	current = current->next;
	// }
	// //////FREE chain list
}

int		count_env_vars()
{
	int			count;
	char		**env;

	env = environ;
	count = 0;
	while (*env)
	{
		count++;
		env++;
	}
	return (count);
}

char	*add_env_value(char *variable, char *value)
{
	char		*var;
	char		**env;
	int			i;
	char		**new_env;
	int			count;

	env = environ;
	count = count_env_vars() + 1;
	new_env = malloc(sizeof(char **) * count);
	var = ft_strjoin(variable, "=");
	if (!var)
		return (NULL);
	i = 0;		
	while (*env)
	{
		new_env[i] = *env;
		env++;
		i++;
	}
	new_env[i] = ft_strjoin(var, value);
	return (free(var), new_env[i]);
}

char	*replace_env_value(char *variable, char *value)
{
	size_t		len;
	char		*var;
	char		**env;

	env = environ;
	len = ft_strlen(variable) + 1;
	var = ft_strjoin(variable, "=");
	while (*env)
	{
		if (ft_strnstr(*env, var, len) == *env)
			return (*env = ft_strjoin(var, value), free(var), *env);
		env++;
	}
	return (free(var), NULL);
}

int	export_cmd(t_cmd *cmd)
{
	(void)cmd;
	char			**split_on_equal;
	int			i;
	size_t		len;
	t_process	*data;

	data = get_process();	
	if (cmd->args[0] == NULL)
		export_no_variable();
	if (cmd->options && cmd->options[0] != NULL)
	{
		printf("Export option \"%s\" not handle \n", cmd->options[0]);
		return (1);
	}
	i = 1;
	while (cmd->args[i])
	{
		split_on_equal = ft_split(cmd->args[i], '=');
		if (!split_on_equal)
			return(1) ; ////// ad protection freeee
		len = ft_strlen(split_on_equal[0]);	
		if (!replace_env_value(split_on_equal[0], cmd->args[i]))
			add_env_value(split_on_equal[0], cmd->args[i]);
		i++;
	}
	return(0);
}
