/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/10 12:00:57 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_cmd(t_cmd *cmd)
{
	int i;
	
	i=0;
	if(cmd->options[0])
	i++;
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		i++;
	}
	if ((!ft_strnstr(cmd->options[0], "-n", ft_strlen(cmd->options[0]))) != 0)
	printf("\n");
}
