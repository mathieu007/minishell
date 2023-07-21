/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
	add_redir_arg_to_main(main, redir);
	return (redir);
}

t_cmd	*parse_redirect_in(t_cmd *main, t_cmd *redir)
{
	redir->args = free_2d_char_array(redir->args);
	redir->args = parse_args(redir->token);
	if (!redir->args)
		return (NULL);
	redir->name = free_ptr(redir->name);
	redir->name = ft_strdup(redir->args[0]);
	if (redir->name == NULL)
		return (NULL);
	add_redir_arg_to_main(main, redir);
	return (redir);
}
