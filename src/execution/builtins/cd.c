/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/09 15:31:04 by bmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	cd_cmd(t_cmd *cmd)
{
	int		result;
	char	path[1024];
	char	*path_to_call;

	result = 0;
	if (getcwd(path, sizeof(path)) != NULL)
	{
		if (cmd->options[0])
			{
				printf("Error option \"%s\" not handle \n", cmd->options);
				return(0);
			}
		path_to_call = ft_strjoin(path, "/");
		path_to_call = ft_strjoin(path_to_call, cmd->args);
		printf("cd function path =%s \n", path_to_call);
		result = chdir(path_to_call);
	}
	if (result != 0)
		perror("cd_cmd FAilED");
	return (result);
}
