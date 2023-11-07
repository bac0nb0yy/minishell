/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 09:08:33 by dtelnov           #+#    #+#             */
/*   Updated: 2023/10/07 02:48:59 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_stradd_back(t_str **lst, t_str *new)
{
	t_str	*tmp;

	if (lst && *lst)
	{
		tmp = *lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else if (lst)
		*lst = new;
}

void	ft_strclear(t_str **lst)
{
	t_str	*tmp;

	while (lst && *lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->data)
			free(((*lst)->data));
		free(*lst);
		*lst = tmp;
	}
	if (lst)
		*lst = NULL;
}

int	ft_strsize(t_str *lst)
{
	int		size;
	t_str	*tmp;

	tmp = lst;
	size = 0;
	while (tmp)
	{
		if (tmp->data_type != AMBIGUOUS)
			size++;
		tmp = tmp->next;
	}
	return (size);
}

bool	add_args(t_str *curr_data, t_cmd **command)
{
	int		k;
	char	**arr;

	arr = ft_split(curr_data->data, 1);
	if (!arr)
		return (ft_strclear(&curr_data), false);
	k = -1;
	while (arr[++k])
		ft_stradd_back(&(*command)->args, ft_strnew(ft_strdup(arr[k]),
				curr_data->data_type, curr_data->type));
	ft_strclear(&curr_data);
	ft_free_2d_array((void ***)&arr);
	return (true);
}

bool	add_redirs(t_str *curr_data, t_cmd **command, char **arr)
{
	int		k;

	k = -1;
	while (arr[++k])
		ft_stradd_back(&(*command)->redirs, ft_strnew(ft_strdup(arr[k]),
				curr_data->data_type, curr_data->type));
	ft_strclear(&curr_data);
	ft_free_2d_array((void ***)&arr);
	return (true);
}
