/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 01:01:44 by dtelnov           #+#    #+#             */
/*   Updated: 2023/09/02 01:01:44 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	end_heredoc(t_msh *msh, char *line, char *eof)
{
	const size_t	length = ft_strlen(eof);

	if (!line)
		return (ft_dprintf(STDERR_FILENO, "minishell: warning: here-document at line %d delimited by end-of-file \
(wanted `%s')\n", msh->lines_counter, eof), true);
	return (ft_strncmp(line, eof, length) == 0 && line[length] == 0);
}

static inline void	__set_heredoc__(t_heredoc *heredoc, t_heredoc_line *node)
{
	if (!heredoc->first)
	{
		heredoc->first = node;
		heredoc->last = node;
	}
	else
	{
		heredoc->last->next = node;
		heredoc->last = node;
	}
}

bool	push_back_heredoc(t_msh *msh, t_heredoc *heredoc, char *line)
{
	t_heredoc_line	*node;
	char			*tmp;

	node = malloc(sizeof(t_heredoc_line));
	if (!node)
		return (false);
	node->line = line;
	if (heredoc->expand)
	{
		node->line = expander(line, msh->env);
		replace_spaces_back(node->line);
	}
	tmp = ft_strjoin(node->line, "\n");
	if (!tmp)
		return (free(node->line), free(node), false);
	free(node->line);
	node->line = tmp;
	node->next = NULL;
	__set_heredoc__(heredoc, node);
	return (true);
}

void	write_heredoc(t_msh *msh, t_heredoc *heredoc, int pipe[2])
{
	t_heredoc_line	*current;
	long			line_len;

	current = heredoc->first;
	while (current)
	{
		line_len = ft_strlen(current->line);
		if (write(pipe[1], current->line, line_len) != line_len)
			(free_heredoc(heredoc), ft_close_pipe(pipe),
				exit_error_command(msh, true, "write() failed\n"));
		current = current->next;
	}
	free_heredoc(heredoc);
	ft_close_pipe(pipe);
	(clear_msh(msh, true), exit(EXIT_SUCCESS));
}

int	create_heredocs(t_msh *msh)
{
	int		i;
	int		ret;
	char	*eof;
	bool	expand;
	t_list	*cur;

	if (msh->heredoc_nb == 0)
		return (0);
	msh->heredocs = malloc(sizeof(int) * msh->heredoc_nb);
	if (!msh->heredocs)
		return (exit_error_command(msh, false, "malloc() failed\n"), -1);
	fill_heredocs(msh);
	i = 0;
	cur = msh->pipeline;
	while (i < msh->heredoc_nb)
	{
		eof = find_eof(msh->pipeline, i, &expand);
		ret = init_heredoc(msh, &i, eof, expand);
		if (ret != 0)
			return (ret);
		i++;
	}
	return (0);
}
