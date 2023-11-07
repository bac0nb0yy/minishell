/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 16:14:36 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/04 16:14:42 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_commands(t_list *pipeline)
{
	t_str	*curr_redir;
	t_str	*curr_arg;
	t_cmd	*command;

	while (pipeline)
	{
		command = (t_cmd *)pipeline->content;
		curr_redir = (t_str *)command->redirs;
		printf("redirs:\n");
		while (curr_redir)
		{
			printf("'%s' and type and data type: %d %d\n",
				curr_redir->data, curr_redir->type, curr_redir->data_type);
			curr_redir = curr_redir->next;
		}
		curr_arg = (t_str *)command->args;
		printf("args:\n");
		while (curr_arg)
		{
			printf("'%s'\n", (char *)curr_arg->data);
			curr_arg = curr_arg->next;
		}
		pipeline = pipeline->next;
		printf("----------------\n");
	}
}
