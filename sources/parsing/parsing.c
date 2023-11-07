/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 02:15:56 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/03 05:12:23 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	push_back_command(t_list **pipeline, t_cmd **command)
{
	t_list	*new;

	new = ft_lstnew((void *)(*command));
	if (!new)
		return (false);
	ft_lstadd_back(pipeline, new);
	return (true);
}

bool	push_redirs_command(t_token **curr, t_cmd **command)
{
	t_str	*curr_data;
	char	**arr;

	*curr = (*curr)->next;
	curr_data = ft_strnew(ft_strdup((*curr)->content),
			(*curr)->type, (*curr)->prev->type);
	if (!curr_data || !curr_data->data)
		return (false);
	if (!curr_data->data[0])
		curr_data->data_type = STR_QUOTED;
	if ((*curr)->expand)
		curr_data->data = expander(curr_data->data, (*command)->env);
	if ((*curr)->type == STR_QUOTED)
		replace_spaces_back(curr_data->data);
	if (!get_args_content(&curr_data, curr, command))
		return (false);
	arr = ft_split(curr_data->data, 1);
	if (!arr)
		return (false);
	if (!curr_data->data || !curr_data->data[0] || (arr[0] && arr[1]))
		return (case_null_redir(curr_data, curr, command, arr), true);
	if (!add_redirs(curr_data, command, arr))
		return (false);
	*curr = (*curr)->next;
	return (true);
}

bool	push_args_command(t_token **curr, t_cmd **command)
{
	t_str	*curr_data;

	curr_data = ft_strnew(ft_strdup((*curr)->content),
			(*curr)->type, (*curr)->type);
	if (!curr_data || !curr_data->data)
		return (false);
	if ((*curr)->expand)
		curr_data->data = expander(curr_data->data, (*command)->env);
	if ((*curr)->type == STR_QUOTED)
		replace_spaces_back(curr_data->data);
	if (!get_args_content(&curr_data, curr, command))
		return (false);
	if ((*command)->args && ft_compstr("export", (*command)->args->data))
		replace_spaces_back(curr_data->data);
	if (!curr_data->data || !curr_data->data[0])
		return (case_null_args(curr_data, curr, command), true);
	if (!add_args(curr_data, command))
		return (false);
	*curr = (*curr)->next;
	return (true);
}

bool	parse_command(t_list **pipeline, t_token **curr, t_cmd **command)
{
	while (*curr && (*curr)->type != PIPE)
	{
		if (is_str((*curr)->type))
			push_args_command(curr, command);
		else
			push_redirs_command(curr, command);
	}
	if (!push_back_command(pipeline, command))
		return (false);
	return (true);
}

bool	parser(t_tokens *tokens, t_list **pipeline, t_env *env)
{
	t_cmd		*command;
	t_token		*curr;

	curr = tokens->first;
	while (curr)
	{
		command = ft_calloc(1, sizeof(t_cmd));
		if (!command)
			return (clear_commands(pipeline), false);
		command->env = env;
		if (!parse_command(pipeline, &curr, &command))
			return (clear_commands(pipeline), false);
		if (curr)
			curr = curr->next;
	}
	return (true);
}
