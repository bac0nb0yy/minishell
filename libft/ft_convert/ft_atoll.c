/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 23:55:09 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 21:23:21 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	conv(char a)
{
	long	i;

	i = a - 48;
	return (i);
}

long long	ft_atoll(const char *nptr)
{
	long long			result;
	long long			n;

	result = 0;
	n = 1;
	while ((*nptr >= 9 && *nptr <= 15) || *nptr == 32)
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			n = -1;
		nptr++;
	}
	while (*nptr > 47 && *nptr < 58)
	{
		if (*nptr > 47 && *nptr < 58)
			result = (result * 10) + conv(*nptr);
		nptr++;
	}
	result = result * n;
	return (result);
}
