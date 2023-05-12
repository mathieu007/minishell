
#include "minishell.h"

int	cd_cmd(t_cmd *cmd)
{
	int		result;
	char	path[PATH_MAX];
	char	*path_to_call;

	result = 0;
	if (getcwd(path, sizeof(path)) != NULL)
	{
		if (cmd->options[0] != NULL)
			{
				printf("Error option \"%s\" not handle \n", cmd->options[0]);
				return(1);
			}
		path_to_call = ft_strjoin(path, "/");
		path_to_call = ft_strjoin(path_to_call, cmd->args[0]);
		
		#ifdef _DEBUG
		printf("cd function path =%s \n", path_to_call);
		#endif
		
		result = chdir(path_to_call);
	}
	if (result != 0)
		perror("cd_cmd FAilED");
	return (result);
}
