#include "ft_printf.h"

void	ft_putchar(char c, int *write_len)
{
	int	written;

	if (*write_len == -1)
		return ;
	written = 0;
	written = write(2, &c, 1);
	if (written == -1)
		*write_len = -1;
	else
		*write_len += written;
}
