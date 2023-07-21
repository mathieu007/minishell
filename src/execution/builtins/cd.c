/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 07:26:48 by math              #+#    #+#             */
/*   Updated: 2023/07/21 15:12:34 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>

char	*ft_strcat(char *dest, const char *src)
{
	size_t	dest_len;
	size_t	i;

	dest_len = 0;
	while (dest[dest_len] != '\0')
		dest_len++;
	i = 0;
	while (src[i] != '\0')
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

int	cd_cmd(t_cmd *cmd)
{
	char		*path;
	char		*path_to_change;
	t_process	*proc;
	char		path_buff[1024];
	int			result;

	proc = get_process();
	path = NULL;
	path_to_change = cmd->args[1];
	if (cmd->options != NULL)
		return (ft_printf("Error: Option \"%s\" not supported.\n",
				cmd->options[0]), 1);
	if (cmd->args[1] == NULL)
		return (ft_printf("Error: No directory specified.\n"), 1);
	if (path_to_change[0] != '/')
	{
		path = get_cwd();
		if (!path)
		{
			ft_printf("minishell: cd: %s: No such file or directory\n",
				cmd->args[1]);
			return (1);
		}
		path = ft_strjoinfree(path, "/");
		path = ft_strjoinfree(path, path_to_change);
		path_to_change = path;
	}
	result = chdir(path_to_change);
	proc->cwd = free_ptr(proc->cwd);
	proc->cwd = ft_strdup(getcwd(path_buff, 1024));
	if (path)
		free(path);
	if (result != 0)
		return (ft_printf("minishell: cd: %s: No such file or directory\n",
				cmd->args[1]), 1);
	return (0);
}
