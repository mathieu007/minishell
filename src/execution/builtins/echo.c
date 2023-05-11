/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/05/10 12:04:16 by bmartin          ###   ########.fr       */
=======
/*   Updated: 2023/05/10 12:00:57 by mroy             ###   ########.fr       */
>>>>>>> Math
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_cmd(t_cmd *cmd)
{
<<<<<<< HEAD
	int	i;

	i = 0;
	if ((ft_strnstr(cmd->options[i], "-n", ft_strlen(cmd->options[i]))) != 0)
	{
		while ((ft_strnstr(cmd->options[i], "-n",
					ft_strlen(cmd->options[i]))) != 0)
			i++;
	}
=======
	int i;
	
	i=0;
	if(cmd->options[0])
	i++;
>>>>>>> Math
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		i++;
	}
	if ((!ft_strnstr(cmd->options[0], "-n", ft_strlen(cmd->options[0]))) != 0)
<<<<<<< HEAD
		printf("\n");
=======
	printf("\n");
>>>>>>> Math
}
