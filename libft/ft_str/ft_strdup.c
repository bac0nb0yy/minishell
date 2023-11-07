/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 19:31:12 by dtelnov           #+#    #+#             */
/*   Updated: 2023/10/06 21:22:06 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*tmp;
	size_t	i;

	if (!s)
	{
		tmp = malloc(1);
		if (!tmp)
			return (NULL);
		tmp[0] = 0;
		return (tmp);
	}
	tmp = malloc(ft_strlen(s) + 1);
	i = 0;
	if (!tmp)
		return (NULL);
	while (s[i])
	{
		tmp[i] = s[i];
		++i;
	}
	tmp[i] = 0;
	return (tmp);
}
