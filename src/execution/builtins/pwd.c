/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/21 15:13:09 by mroy             ###   ########.fr       */
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
		return (0);
	}
	else
	{
		ft_printf("No such file or directory");
		return (1);
	}
}
