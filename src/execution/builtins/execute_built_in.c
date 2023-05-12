#include "minishell.h"

int	execute_built_in(t_cmd *cmd)
{
	size_t	len;

	len = 0;
#ifdef _DEBUG
	printf("you are using a built in command \n");
#endif
	len = ft_strlen(cmd->name);
	if (ft_strnstr(cmd->name, "echo", len) != 0)
	{
		cmd->func = &echo_cmd;
		return (0);
	}
	if (ft_strnstr(cmd->name, "env", len) != 0)
	{
		cmd->func = &env_cmd;
		return (0);
	}
	if (ft_strnstr(cmd->name, "pwd", len) != 0)
	{
		cmd->func = &pwd_cmd;
		return (0);
	}
	if (ft_strnstr(cmd->name, "cd", len) != 0)
	{
		cmd->func = &cd_cmd;
		return (0);
	}
	if (ft_strnstr(cmd->name, "export", len) != 0)
	{
		cmd->func = &export_cmd;
		return (0);
	}
	if (ft_strnstr(cmd->name, "unset", len) != 0)
	{
		cmd->func = &unset_cmd;
		return (0);
	}
	if (ft_strnstr(cmd->name, "exit", len) != 0)
	{
		cmd->func = &exit_cmd;
		return (0);
	}
	else
		return (1);
	//????		cmd->func(cmd);
}
