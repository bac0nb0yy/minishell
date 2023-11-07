/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 06:07:08 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/04 16:14:13 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_commands(t_list **pipeline)
{
	t_cmd	*command;
	t_list	*tmp;

	while (*pipeline)
	{
		tmp = (*pipeline)->next;
		command = (t_cmd *)(*pipeline)->content;
		if (command)
		{
			ft_strclear(&command->redirs);
			ft_strclear(&command->args);
			ft_free_2d_array((void ***)&command->paths);
			free(command->cmd_path);
			free(command->redirs);
			free(command->args);
			free(command->command);
			free(command);
		}
		if (*pipeline)
			free(*pipeline);
		*pipeline = tmp;
	}
}

bool	get_args_content(t_str **curr_data, t_token **curr, t_cmd **command)
{
	char	*tmp;
	char	*tmp2;

	while (*curr && (*curr)->join)
	{
		tmp = (*curr_data)->data;
		if ((*curr)->next->type == STR_QUOTED)
			(*curr_data)->data_type = STR_QUOTED;
		if ((*curr)->next->expand)
		{
			tmp2 = expander(ft_strdup((*curr)->next->content), (*command)->env);
			if ((*curr)->next->type == STR_QUOTED)
				replace_spaces_back(tmp2);
			(*curr_data)->data = ft_strjoin((*curr_data)->data, tmp2);
			free(tmp2);
		}
		else
		(*curr_data)->data = ft_strjoin((*curr_data)->data,
					(*curr)->next->content);
		free(tmp);
		if (!(*curr_data)->data)
			return (false);
		*curr = (*curr)->next;
	}
	return (true);
}

void	case_null_redir(t_str *curr_data, t_token **curr,
						t_cmd **command, char **arr)
{
	if (arr[0] && arr[1])
		ft_stradd_back(&(*command)->redirs,
			ft_strnew(ft_strdup((*curr)->content), AMBIGUOUS, curr_data->type));
	else if (!curr_data->data || curr_data->data_type == STR_QUOTED)
		ft_stradd_back(&(*command)->redirs, ft_strnew(ft_strdup(""),
				curr_data->data_type, curr_data->type));
	else
		ft_stradd_back(&(*command)->redirs,
			ft_strnew(ft_strdup((*curr)->content), AMBIGUOUS, curr_data->type));
	ft_strclear(&curr_data);
	ft_free_2d_array((void ***)&arr);
	*curr = (*curr)->next;
}

void	case_null_args(t_str *curr_data, t_token **curr, t_cmd **command)
{
	if ((!curr_data->data || !curr_data->data[0])
		&& curr_data->data_type == STR_QUOTED)
		ft_stradd_back(&(*command)->args, ft_strnew(ft_strdup(""),
				curr_data->data_type, curr_data->type));
	else if ((!curr_data->data || !curr_data->data[0])
		&& curr_data->data_type != STR_QUOTED)
		ft_stradd_back(&(*command)->args,
			ft_strnew(ft_strdup(""), AMBIGUOUS, curr_data->type));
	ft_strclear(&curr_data);
	*curr = (*curr)->next;
}

t_str	*ft_strnew(char *data, int data_type, int type)
{
	t_str	*lst;

	lst = malloc(sizeof(t_str));
	if (!lst)
		return (NULL);
	lst->data = data;
	lst->data_type = data_type;
	lst->type = type;
	lst->next = NULL;
	return (lst);
}
