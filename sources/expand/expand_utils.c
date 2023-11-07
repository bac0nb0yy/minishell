/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:42:08 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 08:02:02 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_spaces_back(char *str)
{
	int		i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if (str[i] == 1)
			str[i] = ' ';
		++i;
	}
}

void	replace_spaces(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			str[i] = 1;
		++i;
	}
}

int	size_element(char *str, int *index)
{
	int		i;

	i = 0;
	if (str[i] != '$')
	{
		while (str[i] && str[i] != '$')
			++i;
		return (*index += i, i);
	}
	else
	{
		++i;
		if (str[i] && !ft_isdigit(str[i]) && str[i] != '?')
		{
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				++i;
		}
		else
			++i;
	}
	return (*index += i, i);
}

int	count_words(char *str)
{
	int		i;
	int		size;

	i = 0;
	size = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			++i;
			if (str[i] && !ft_isdigit(str[i]) && str[i] != '?')
			{
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
					++i;
			}
			++size;
		}
		else
		{
			while (str[i] && str[i] != '$')
				++i;
			++size;
		}
	}
	return (size);
}
