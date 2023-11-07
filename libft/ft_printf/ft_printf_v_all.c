/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_v_all.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 03:14:50 by dtelnov           #+#    #+#             */
/*   Updated: 2023/07/30 03:38:19 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_vsprintf(const char *s, int *size, va_list args, va_list args_bis)
{
	char	*to_print;

	*size = ft_arglen(s, args) + 1;
	to_print = ft_argjoin(s, args_bis, *size);
	return (to_print);
}

int	ft_vdprintf(int fd, const char *s, va_list args, va_list args_bis)
{
	char	*display;
	int		ret;

	display = ft_vsprintf(s, &ret, args, args_bis);
	if (!display)
		return (va_end(args), va_end(args_bis), -1);
	write(fd, display, ret);
	free(display);
	return (ret);
}

int	ft_vprintf(const char *s, va_list args, va_list args_bis)
{
	char	*display;
	int		ret;

	display = ft_vsprintf(s, &ret, args, args_bis);
	if (!display)
		return (va_end(args), va_end(args_bis), -1);
	write(1, display, ret);
	free(display);
	return (ret);
}
