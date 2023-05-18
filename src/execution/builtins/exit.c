/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/18 14:40:53 by bmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_cmd(t_cmd *cmd)
{
	if (cmd->args[0] == NULL)
		exit(0);
	else if (cmd->args[0] != NULL)
		exit(ft_atoi(cmd->args[0]));
	else
	{
		printf("exit: too many arguments");
		return(1);
	}
}


//////// signal =  128 +signnal_number

///convertir le status de waitpid en error

//(convertir le exit stauts en error)