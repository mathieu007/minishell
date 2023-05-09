/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 14:49:58 by bmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	echo_cmd(t_cmd *cmd)
{
	int i;
	
	i=0;
	if(cmd->options[0])
	i++;
	while (cmd->args)
	{
		printf("%s",cmd->args[i]);
		i++;
	}
	if ((!ft_strnstr(cmd->options, "-n", ft_strlen(cmd->options))) != 0)
	printf("\n");	
}
