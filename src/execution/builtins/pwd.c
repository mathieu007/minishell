/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/23 16:36:59 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_cmd(t_cmd *cmd)
{
	char	*cwd;

	(void)cmd;
	cwd = get_cwd();
	if (cwd)
	{
		ft_printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	else
	{
		ft_printf("No such file or directory");
		return (1);
	}
}
