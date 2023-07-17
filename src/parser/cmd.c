#include "minishell.h"

t_cmd	*parse_cmd(t_cmd *cmd)
{
	if (cmd->type == CMD_PARENTHESES)
		return (cmd->name = ft_strdup(cmd->token->str), cmd);
	cmd->args = free_2d_char_array(cmd->args);
	cmd->name = free_ptr(cmd->name);
	cmd->args = parse_args(cmd->token);
	if (cmd->args == NULL || ((ft_strisempty(cmd->args[0])
		|| ft_striswhitespace(cmd->args[0])) && !cmd->has_redirection))
		return (NULL);
	cmd->name = ft_strdup(cmd->args[0]);
	if (!cmd->name)
		free_all_and_exit2(errno, "malloc error");
	cmd->is_builtin = is_builtins(cmd->name);
	if (!cmd->is_builtin)
	{
		cmd->full_path_name = free_ptr(cmd->full_path_name);
		cmd->full_path_name = get_full_path(cmd);
	}
	cmd->options = get_options(cmd->token);
	return (cmd);
}

int32_t	count_arr(char **arr)
{
	int32_t	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

char	**resize_array(char **arr, int32_t add_count)
{
	int32_t	count;
	char	**new;
	int32_t	i;

	count = count_arr(arr);
	new = malloc(sizeof(char *) * (add_count + count + 1));
	if (!new)
		free_all_and_exit2(errno, "malloc error");
	new[add_count + count] = NULL;
	i = 0;
	while (i < count)
	{
		new[i] = ft_strdup(arr[i]);
		i++;
	}
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
	dest[i] = NULL;
	return (dest);
}

void	*add_cmd_arg_to_main(t_cmd *main, t_cmd *redir)
{
	int32_t	args_count;
	int32_t	main_args_count;
	int32_t	index;
	char	**new_args;
	char	**args;

	args = redir->args;
	args_count = count_arr(args);
	if (args_count > 1 && main->type == CMD_PARENTHESES)
		return (write_err2(2, "syntax error near unexpected token: ", redir->args[1]), NULL);
	main_args_count = count_arr(main->args);
	if (args_count <= 1)
		return (NULL);
	index = main_args_count;
	new_args = resize_array(main->args, args_count - 1);
	main->args = free_2d_char_array(main->args);
	main->args = copy_args(new_args, index, &args[1]);
	if (main_args_count == 0)
		main->name = ft_strtrim(main->args[0], " ");
	if (!main->name)
		free_all_and_exit2(errno, "malloc error");
	main->is_builtin = is_builtins(main->name);
	if (!main->is_builtin)
	{
		main->full_path_name = free_ptr(main->full_path_name);
		main->full_path_name = get_full_path(main);
	}
	return (main);
}

t_cmd	*parse_redirect_out(t_cmd *main, t_cmd *redir)
{
	redir->args = free_2d_char_array(redir->args);
	redir->name = free_ptr(redir->name);
	redir->args = parse_args(redir->token);
	if (!redir->args)
		return (NULL);
	redir->name = ft_strdup(redir->args[0]);
	if (redir->name == NULL)
		free_all_and_exit2(errno, "malloc error");
	add_cmd_arg_to_main(main, redir);
	return (redir);
}

t_cmd	*parse_redirect_in(t_cmd *main, t_cmd *redir)
{
	redir->args = free_2d_char_array(redir->args);
	redir->name = free_ptr(redir->name);
	redir->args = parse_args(redir->token);
	if (!redir->args)
		return (NULL);
	redir->name = ft_strdup(redir->args[0]);
	if (redir->name == NULL)
		return (NULL);
	//main->in_redir = free_t_redirect(main->in_redir);
	add_cmd_arg_to_main(main, redir);
	return (redir);
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
	{
		cmd->full_path_name = free_ptr(cmd->full_path_name);
		cmd->full_path_name = get_full_path(cmd);
	}
	cmd->options = get_options(cmd->token);
	return (cmd);
}
