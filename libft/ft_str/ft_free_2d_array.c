/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_2d_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 05:06:23 by dtelnov           #+#    #+#             */
/*   Updated: 2023/08/14 09:17:07 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_2d_array(void ***ptr)
{
	size_t	i;
	void	**arr;

	if (*ptr == NULL)
		return ;
	arr = *ptr;
	i = 0;
	while (arr[i])
	{
		if (*(arr + i) != NULL)
		{
			free(*(arr + i));
			*(arr + i) = NULL;
		}
		++i;
	}
	if (*((void **)ptr) != NULL)
	{
		free(*((void **)ptr));
		*((void **)ptr) = NULL;
	}
}
