/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_len_nums.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 04:46:41 by dtelnov           #+#    #+#             */
/*   Updated: 2023/07/30 03:32:10 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_intlen(int number, int base)
{
	int	len;

	len = !number;
	if (number < 0)
		++len;
	while (number)
	{
		number /= base;
		++len;
	}
	return (len);
}

int	ft_uintlen(unsigned int number, int base)
{
	int				len;

	len = !number;
	while (number)
	{
		number /= base;
		++len;
	}
	return (len);
}

int	ft_ullen(unsigned long number, int base)
{
	int				len;

	if (!number)
		return (5);
	len = !number;
	while (number)
	{
		number /= base;
		++len;
	}
	return (len + 2);
}

int	calculate_len(va_list args, char format)
{
	int			ret;

	if (format == 'p')
		ret = ft_ullen(va_arg(args, unsigned long), 16);
	else if (format == 'd' || format == 'i')
		ret = ft_intlen(va_arg(args, int), 10);
	else if (format == 'u')
		ret = ft_uintlen(va_arg(args, unsigned int), 10);
	else if (format == 'x' || format == 'X')
		ret = ft_uintlen(va_arg(args, unsigned int), 16);
	else
		ret = -1;
	return (ret);
}

int	ft_strarglen(char *s)
{
	int		len;

	if (!s)
		return (6);
	len = 0;
	while (s[len])
		++len;
	return (len);
}
