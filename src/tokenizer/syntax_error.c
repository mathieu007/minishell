/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/31 11:43:17 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_error(char *token_err)
{
	char		*illegal_token;
	char		*cmsg;

	illegal_token = "syntax error near unexpected token `";
	get_process()->syntax_error = true;
	cmsg = ft_strjoinfree(token_err, "'");
	write_err2(258, illegal_token, cmsg);
	free(cmsg);
}
