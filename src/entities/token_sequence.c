/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/06/05 15:01:46 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_sequence	*new_token_sequence()
{
	t_token_sequence	*new;

	new = ft_calloc(1, sizeof(t_token_sequence));
	if (!new)
		return (NULL);
	return (new);
}

// t_token_sequence	*add_token_sequence(char *start, int32_t len)
// {
// 	t_token_sequence	*last;
// 	t_token_sequence	*new;
// 	t_process		*data;

// 	data = get_process();
// 	last = data->last_token_sequence;
// 	new = new_token_sequence();
// 	if (!new)
// 		return (NULL);
// 	if (last)
// 	{
// 		last->next = new;
// 		new->prev = last;
// 	}
// 	else
// 		data->token_sequence = new;
// 	new->str = ft_strtrim(ft_strncpy(start, len), " ");
// 	new->len = len;

// 	data->last_token_sequence = new;
// 	return (new);
// }

void	reset_token_sequence(t_token_sequence *seq)
{
	if (!seq)
		return ;
	free_t_tokens(seq->token);
	seq->token = NULL;
	seq->last_token = NULL;
	free(seq->str);
}
