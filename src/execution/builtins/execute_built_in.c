#include "minishell.h"

void	execute_built_in(t_cmd *cmd , t_data *data)
{
	size_t	len;

	
	//printf("you are using a built in command \n");
	
	len = ft_strlen(cmd->name);
	if (ft_strnstr(cmd->name, "echo", len) != 0)
		echo_cmd (cmd);
	if (ft_strnstr(cmd->name, "env", len) != 0)
		env_cmd(data);
	if (ft_strnstr(cmd->name, "pwd", len) != 0)
		pwd_cmd();
	if (ft_strnstr(cmd->name, "cd", len) != 0)
		cd_cmd(cmd);
	if (ft_strnstr(cmd->name, "export", len) != 0)
		export(cmd);
	if (ft_strnstr(cmd->name, "unset", len) != 0)
		unset_cmd(data,cmd);
	if (ft_strnstr(cmd->name, "exit", len) != 0)
		exit_cmd (0);
}
