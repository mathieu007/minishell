/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sibling_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/16 08:15:12 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

inline bool	is_sibling_quote(t_token *token)
{
	if ((token->type == TK_DOUBLEQUOTE || token->type == TK_SINGLEQUOTE)
		&& token->prev && (token->prev->type != TK_CLOSINGDOUBLEQUOTE
			&& token->prev->type != TK_CLOSINGSINGLEQUOTE))
		return (false);
	return (true);
}

inline t_token	*get_quotes_str(t_token *token, char *str, char **ouput)
{
	char	*quote_str;
	char	*temp;

	while (token)
	{
		if (token->type == TK_DOUBLEQUOTE)
		{
			quote_str = get_double_quote_str(token, str);
			token = advance_to(token, TK_CLOSINGDOUBLEQUOTE)->next;
			if (token && is_sibling_quote(token))
			{
				temp = *ouput;
				*ouput = ft_strjoin(*ouput, quote_str);
				if (temp)
					free(temp);
				free(quote_str);
			}
			else
				*ouput = quote_str;
		}
		else if (token->type == TK_SINGLEQUOTE)
		{
			quote_str = get_double_quote_str(token, str);
			token = advance_to(token, TK_CLOSINGSINGLEQUOTE)->next;
			if (token && is_sibling_quote(token))
			{
				temp = *ouput;
				*ouput = ft_strjoin(*ouput, quote_str);
				if (temp)
					free(temp);
				free(quote_str);
			}
			else
				*ouput = quote_str;
		}
		else
			break ;
	}
	return (token);
}
