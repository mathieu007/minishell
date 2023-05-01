/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/04/30 17:45:06 by math             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief All command executed inside parentheses (echo '123' && echo '456')
/// @return 
int32_t sub_process()
{
	char *env[] = { "MYVAR=hello", NULL };
	char *argv[] = { "/bin/bash", "-c", "echo $MYVAR", NULL };
	pid_t pid = fork();

	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	} 
	else if (pid == 0) {
		// Set the environment variables for the subshell
        if (environ)
		{
            for (int i = 0; environ[i]; ++i)
			{
                putenv(environ[i]);
            }
        }
        if (env) {
            for (int i = 0; env[i]; ++i) {
                putenv(env[i]);
            }
        }
        // Execute the command inside the subshell
        if (execve("/bin/bash", argv, environ) == -1)
		{
            perror("execve");
            exit(EXIT_FAILURE);
        }
    } else {
        if (waitpid(pid, NULL, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        printf("Child process finished\n");
    }

    return 0;
}
