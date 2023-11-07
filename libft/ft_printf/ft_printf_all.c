/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_all.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 03:24:03 by dtelnov           #+#    #+#             */
/*   Updated: 2023/10/20 14:57:46 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_sprintf(const char *s, ...)
{
	va_list	args;
	va_list	args_bis;
	char	*to_print;
	int		size_malloc;

	va_start(args, s);
	va_start(args_bis, s);
	size_malloc = ft_arglen(s, args) + 1;
	to_print = ft_argjoin(s, args_bis, size_malloc);
	va_end(args);
	va_end(args_bis);
	return (to_print);
}

int	ft_dprintf(int fd, const char *s, ...)
{
	va_list	args;
	va_list	args_bis;
	va_list	args_third;
	char	*display;
	int		ret;

	va_start(args, s);
	va_start(args_bis, s);
	display = ft_vsprintf(s, &ret, args, args_bis);
	if (!display)
		return (va_end(args), va_end(args_bis), -1);
	va_end(args);
	va_end(args_bis);
	va_start(args_third, s);
	ret = ft_arglen(s, args_third);
	va_end(args_third);
	if (write(fd, display, ret) == -1)
		return (free(display), -1);
	free(display);
	return (ret);
}

int	ft_printf(const char *s, ...)
{
	va_list	args;
	va_list	args_bis;
	va_list	args_third;
	char	*display;
	int		ret;

	va_start(args, s);
	va_start(args_bis, s);
	display = ft_vsprintf(s, &ret, args, args_bis);
	if (!display)
		return (va_end(args), va_end(args_bis), -1);
	va_end(args);
	va_end(args_bis);
	va_start(args_third, s);
	ret = ft_arglen(s, args_third);
	va_end(args_third);
	if (write(1, display, ret) == -1)
		return (free(display), -1);
	free(display);
	return (ret);
}
