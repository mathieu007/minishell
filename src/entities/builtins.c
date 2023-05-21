
#include "minishell.h"

char	**get_builtins_cmd(void)
{
	static char	*builtins[8];

	if (*builtins == NULL)
	{
		builtins[0] = BUILTINS_EXPORT;
		builtins[1] = BUILTINS_UNSET;
		builtins[2] = BUILTINS_ENV;
		builtins[3] = BUILTINS_EXIT;
		builtins[4] = BUILTINS_PWD;
		builtins[5] = BUILTINS_CD;
		builtins[6] = BUILTINS_ECHO;
		builtins[7] = NULL;
	}
	return (&builtins[0]);
}
