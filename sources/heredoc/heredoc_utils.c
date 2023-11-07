/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 17:04:24 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 09:02:28 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*return_eof(t_str *current, int heredoc_nb, bool *expand, int *count)
{
	while (current)
	{
		if (current->type == HEREDOC)
		{
			if (*count == heredoc_nb)
			{
				*expand = false;
				if (current->data_type == STR)
					*expand = true;
				return (current->data);
			}
			++(*count);
		}
		current = current->next;
	}
	return (NULL);
}

char	*find_eof(t_list *first, int heredoc_nb, bool *expand)
{
	char	*eof;
	int		count;
	t_list	*list;
	t_str	*current;

	list = first;
	count = 0;
	while (list)
	{
		current = (t_str *)(((t_cmd *)list->content)->redirs);
		eof = return_eof(current, heredoc_nb, expand, &count);
		if (eof)
			return (eof);
		list = list->next;
	}
	return (NULL);
}

void	fill_heredocs(t_msh *msh)
{
	int	i;

	i = 0;
	while (i < msh->heredoc_nb)
		msh->heredocs[i++] = -1;
	signal(SIGINT, handle_sig_in_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	free_heredoc(t_heredoc *heredoc)
{
	t_heredoc_line	*current;
	t_heredoc_line	*tmp;

	current = heredoc->first;
	while (current)
	{
		tmp = current;
		current = current->next;
		free(tmp->line);
		free(tmp);
	}
}

int	read_heredoc(t_msh *msh, t_heredoc *heredoc, char *eof, int pipe[2])
{
	char	*line;
	int		insave;

	heredoc->first = NULL;
	heredoc->last = NULL;
	insave = dup(0);
	while (true)
	{
		line = readline("> ");
		++heredoc->lines_counter;
		if (g_status == 257)
		{
			g_status = 130;
			free(line);
			ft_close_pipe(pipe);
			dup2(insave, 0);
			ft_close(&insave);
			return (130);
		}
		if (end_heredoc(msh, line, eof))
			return (ft_close(&insave), free(line), 0);
		if (!push_back_heredoc(msh, heredoc, line))
			return (ft_close(&insave), free(line), -1);
	}
}
