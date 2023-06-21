
#include "minishell.h"

/// @brief if commands are executed sequentialy then we will need to parse
/// again the next command, ex: export FLAGS=-la && ls $FLAGS
/// if we parse the whole command once we will not be able to
/// get updated $FLAGS so we excute first command then parse the next and exec.
/// @param group
/// @return
t_cmd	*parse_cmd(t_cmd *cmd)
{
	cmd->args = parse_args(cmd->token);
	cmd->name = ft_strdup(cmd->args[0]);
	if (!cmd->name)
		free_all_and_exit2(errno, "malloc error");
	if (cmd->name == NULL)
		return (NULL);
	cmd->is_builtin = is_builtins(cmd->name);
	if (!cmd->is_builtin)
		cmd->full_path_name = get_full_path(cmd);
	cmd->options = get_options(cmd->token);
	return (cmd);
}

int32_t	count_arr(char **arr)
{
	int32_t	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr && arr[i] && *arr[i] != '\0')
		i++;
	return (i);
}

char	**resize_array(char **arr, int32_t add_count)
{
	int32_t	count;
	char	**new;
	int32_t	i;

	count = count_arr(arr);
	new = malloc(add_count + count + 1);
	if (!new)
		return (new);
	i = 0;
	while (i < count)
	{
		new[i] = ft_strdup(arr[i]);
		i++;
	}
	new[add_count + count] = NULL;
	return (new);
}

char	**copy_args(char **dest, int32_t i, char **src)
{
	if (!src)
		return (NULL);
	while (*src)
	{
		dest[i] = ft_strdup(*src);
		if (!dest[i])
			free_all_and_exit2(errno, "malloc error");
		src++;
		i++;
	}
	return (dest);
}

void	*add_cmd_arg_to_main(t_cmd *main, t_cmd *cmd)
{
	int32_t	args_count;
	int32_t	main_args_count;
	int32_t	index;
	char	**new_args;
	char	**args;

	args = cmd->args;
	args_count = count_arr(args);
	main_args_count = count_arr(main->args);
	if (args_count <= 1)
		return (NULL);
	index = main_args_count;
	new_args = resize_array(main->args, args_count - 1);
	if (!new_args)
		free_all_and_exit2(errno, "malloc error");
	main->args = free_2d_char_array(main->args);
	main->args = copy_args(new_args, index, &args[1]);
	if (main_args_count == 0)
		main->name = ft_strtrim(ft_strdup(main->args[0]), " ");
	if (!main->name)
		free_all_and_exit2(errno, "malloc error");
	main->is_builtin = is_builtins(main->name);
	if (!main->is_builtin)
		main->full_path_name = get_full_path(main);
	return (main);
}

t_cmd	*parse_redirect(t_cmd *main, t_cmd *cmd)
{
	cmd->args = parse_args(cmd->token);
	cmd->name = ft_strdup(cmd->args[0]);
	if (cmd->name == NULL)
		return (NULL);
	add_cmd_arg_to_main(main, cmd);
	return (cmd);
}

t_cmd	*parse_cmd2(t_cmd *cmd)
{
	cmd->args = parse_args(cmd->token);
	cmd->name = ft_strdup(cmd->args[0]);
	cmd->has_redirection = cmd->token->is_redirection;
	if (cmd->name == NULL)
		return (NULL);
	cmd->is_builtin = is_builtins(cmd->name);
	if (!cmd->is_builtin)
		cmd->full_path_name = get_full_path(cmd);
	cmd->options = get_options(cmd->token);
	return (cmd);
}
