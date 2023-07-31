/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/26 14:52:59 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*handle_parentheses_syntax_error(t_cmd *redir)
{
	write_err2(258, "syntax error near unexpected token: ", redir->args[1]);
	return (NULL);
}

void	*set_main_command_properties(t_cmd *main, t_cmd *redir	
, int32_t args_count)
{
	int32_t	index;
	char	**new_args;
	char	**args;
	int32_t	main_args_count;

	args = redir->args;
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
	main->full_path_name = free_ptr(main->full_path_name);
	if (!main->is_builtin)
		main->full_path_name = get_full_path(main->name);
	return (main);
}

void	*add_redir_arg_to_main(t_cmd *main, t_cmd *redir)
{
	int32_t	args_count;

	args_count = count_arr(redir->args);
	if (args_count > 1 && main->type == CMD_PARENTHESES)
		return (handle_parentheses_syntax_error(redir));
	if (args_count <= 1)
		return (NULL);
	main = set_main_command_properties(main, redir, args_count);
	return (main);
}

// void	*add_redir_arg_to_main(t_cmd *main, t_cmd *redir)
// {
// 	int32_t	args_count;
// 	int32_t	main_args_count;
// 	int32_t	index;
// 	char	**new_args;
// 	char	**args;

// 	args = redir->args;
// 	args_count = count_arr(args);
// 	if (args_count > 1 && main->type == CMD_PARENTHESES)
// 	{
// 		return (write_err2(258, "syntax error near unexpected token: ",
// 				redir->args[1]), NULL);
// 	}
// 	main_args_count = count_arr(main->args);
// 	if (args_count <= 1)
// 		return (NULL);
// 	index = main_args_count;
// 	new_args = resize_array(main->args, args_count - 1);
// 	main->args = free_2d_char_array(main->args);
// 	main->args = copy_args(new_args, index, &args[1]);
// 	if (main_args_count == 0)
// 		main->name = ft_strtrim(main->args[0], " ");
// 	if (!main->name)
// 		free_all_and_exit2(errno, "malloc error");
// 	main->is_builtin = is_builtins(main->name);
// 	main->full_path_name = free_ptr(main->full_path_name);
// 	if (!main->is_builtin)
// 		main->full_path_name = get_full_path(main->name);
// 	return (main);
// }
