/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:40:36 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 21:33:01 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void	__exit_error_redir__(t_msh *msh, char *error_message)
{
	ft_dprintf(STDERR_FILENO, "minishell: %s\n", error_message);
	free(error_message);
	close_pipes(&msh->exec, true, true);
	clear_msh(msh, true);
	exit(1);
}

void	do_redir(t_msh *msh, t_str *tmp, int *fd, int in_out)
{
	ft_close(fd);
	if (tmp->data_type == AMBIGUOUS)
		__exit_error_redir__(msh, ft_sprintf("%s: ambiguous redirect",
				tmp->data));
	if (in_out == 0)
	{
		*fd = open(tmp->data, O_RDONLY);
		if (*fd == -1)
			__exit_error_redir__(msh,
				ft_sprintf("%s: %s", tmp->data, strerror(errno)));
	}
	else if (in_out == 1)
	{
		*fd = open(tmp->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*fd == -1)
			__exit_error_redir__(msh,
				ft_sprintf("%s: %s", tmp->data, strerror(errno)));
	}
	else
	{
		*fd = open(tmp->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (*fd == -1)
			__exit_error_redir__(msh,
				ft_sprintf("%s: %s", tmp->data, strerror(errno)));
	}
}

void	close_fd_valid(t_msh *msh, int *heredocs, int heredoc_nb)
{
	int	i;

	if (!heredocs)
		return ;
	i = 0;
	while (i < heredoc_nb)
	{
		ft_close(&heredocs[i]);
		i++;
	}
	ft_close(&msh->exec.prev_pipe);
	ft_close(&msh->exec.pipe[0]);
	ft_close(&msh->exec.pipe[1]);
}

int	nb_cmd(t_list *pipeline)
{
	int		nb;
	t_list	*curr;

	nb = 0;
	curr = pipeline;
	while (curr)
	{
		nb++;
		curr = curr->next;
	}
	return (nb);
}
