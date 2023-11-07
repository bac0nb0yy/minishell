/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 01:45:18 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/03 04:14:45 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	size_tokens(t_tokens *tokens)
{
	int		size;
	t_token	*tmp;

	size = 0;
	tmp = tokens->first;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return (size);
}

void	clear_tokens(t_tokens *tokens)
{
	t_token	*tmp;

	if (!tokens)
		return ;
	tmp = tokens->first;
	while (tmp)
	{
		tokens->first = tmp->next;
		if (tmp && tmp->content)
			free(tmp->content);
		if (tmp)
			free(tmp);
		tmp = tokens->first;
	}
	tokens->first = NULL;
	tokens->last = NULL;
}

void	print_tokens(t_tokens *tokens)
{
	t_token	*tmp;

	tmp = tokens->first;
	while (tmp)
	{
		if (tmp->content)
			printf("content: %s\n", tmp->content);
		if (tmp->type)
			printf("type: %d\n", tmp->type);
		if (is_str(tmp->type))
			printf("join : %d\n", tmp->join);
		if (tmp->expand)
			printf("expand: %d\n", tmp->expand);
		tmp = tmp->next;
	}
}

bool	push_back_token(t_tokens *tokens)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (perror("malloc"), false);
	new_token->content = NULL;
	new_token->type = UNKNOWN;
	new_token->expand = false;
	new_token->join = false;
	new_token->next = NULL;
	if (tokens->last)
	{
		tokens->last->next = new_token;
		new_token->prev = tokens->last;
		tokens->last = new_token;
	}
	else
	{
		tokens->first = new_token;
		tokens->last = new_token;
		tokens->last->prev = NULL;
	}
	return (true);
}

void	skip_spaces(char *buffer, int *i)
{
	while (buffer[*i] && ft_isspace(buffer[*i]))
		(*i)++;
}
