/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lltoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:57:02 by tlivroze          #+#    #+#             */
/*   Updated: 2023/10/07 02:14:59 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long long	lllen(long long n)
{
	long long	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n = n / 10;
		len ++;
	}
	return (len);
}

static long long	get_sign(long long *nombre)
{
	long long		sign;

	sign = 0;
	if (*nombre < 0)
	{
		sign = 1;
		*nombre = -(*nombre);
	}
	return (sign);
}

static char	*zero(void)
{
	char	*height;

	height = malloc(2);
	if (!height)
		return (NULL);
	height[0] = '0';
	height[1] = 0;
	return (height);
}

char	*ft_lltoa(long long n)
{
	char			*str;
	long long		len;
	long long		sign;
	long long		i;

	i = 0;
	sign = get_sign(&n);
	if (n == 0)
		return (zero());
	len = lllen(n);
	str = malloc (sizeof(char) * (len + sign + 1));
	if (!str)
		return (NULL);
	if (sign == 1)
		str[0] = '-';
	while (n > 0)
	{
		str[len - 1 - i + sign] = n % 10 + '0';
		i++;
		n = n / 10;
	}
	return (str[i + sign] = '\0', str);
}
