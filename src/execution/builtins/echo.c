#include "minishell.h"

int	count_args(t_cmd *cmd)
{
	int	i;
	char *arg;

	i = 0;
	while (cmd->args && cmd->args[i] && cmd->args[i][0] != '\0')
	{
		arg = cmd->args[i];
		i++;
	}
	return (i);
}

static int32_t	skip_options(t_cmd *cmd)
{
	int	i;
	int	arg_i;

	arg_i = 1;
	if (!cmd->args[arg_i] || !cmd->args[arg_i][0] || cmd->args[arg_i][0] != '-')
		return (1);
	while (cmd->args[arg_i] && cmd->args[arg_i][0] == '-')
	{
		i = 1;
		while (cmd->args[arg_i][i])
		{
			if (cmd->args[arg_i][i] != 'n')
				return (arg_i);
			i++;
		}
		arg_i++;
	}
	return (arg_i);
}

int	echo_cmd(t_cmd *cmd)
{
	int j;
	int count;
	bool has_option_n;

	has_option_n = false;
	count = count_args(cmd);
	if (count == 1)
	{
		printf("\n");
		return (0);
	}
	j = skip_options(cmd);
	if (j > 1)
	{
		has_option_n = true;
	}
	while (j < count)
	{
		printf("%s", cmd->args[j]);
		if (cmd->args[j + 1])
			printf(" ");
		j++;
	}
	if (!has_option_n)
		printf("\n");
	return (0);
}
