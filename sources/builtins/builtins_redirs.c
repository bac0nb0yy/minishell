/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 04:32:58 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 21:45:52 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline bool	__exit_redir__(t_msh *msh, char *data, bool ambigous)
{
	char	*error_message;

	if (ambigous)
		error_message = ft_sprintf("%s: ambiguous redirect", data);
	else
		error_message = ft_sprintf("%s: %s\n", data, strerror(errno));
	ft_dprintf(STDERR_FILENO, "minishell: %s\n", error_message);
	free(error_message);
	clear_msh(msh, false);
	g_status = 1;
	return (false);
}

static bool	do_redir_no_fork(t_msh *msh, t_str *tmp, int *fd, int in_out)
{
	ft_close(fd);
	if (tmp->data_type == AMBIGUOUS)
		return (__exit_redir__(msh, tmp->data, true));
	if (in_out == I_FILE)
	{
		*fd = open(tmp->data, O_RDONLY);
		if (*fd == -1)
			return (__exit_redir__(msh, tmp->data, false));
	}
	else if (in_out == O_FILE_TRUNC)
	{
		*fd = open(tmp->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*fd == -1)
			return (__exit_redir__(msh, tmp->data, false));
	}
	else
	{
		*fd = open(tmp->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (*fd == -1)
			return (__exit_redir__(msh, tmp->data, false));
	}
	return (true);
}

static bool	init_fds_no_fork(t_msh *msh, t_cmd *cmd, int *fd_in, int *fd_out)
{
	t_str	*tmp;

	tmp = cmd->redirs;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			ft_close(fd_in);
			*fd_in = msh->heredocs[msh->exec.curr_heredoc];
			msh->exec.curr_heredoc++;
		}
		else if (tmp->type == I_FILE)
		{
			if (!do_redir_no_fork(msh, tmp, fd_in, tmp->type))
				return (false);
		}
		else if (tmp->type == O_FILE_TRUNC || tmp->type == O_FILE_APPEND)
		{
			if (!do_redir_no_fork(msh, tmp, fd_out, tmp->type))
				return (false);
		}
		tmp = tmp->next;
	}
	return (true);
}

bool	pipes_no_fork(t_msh *msh, t_list *curr)
{
	int	fd_in;
	int	fd_out;

	fd_in = 0;
	fd_out = 1;
	if (!init_fds_no_fork(msh, (t_cmd *)curr->content, &fd_in, &fd_out))
		return (ft_close(&fd_out), ft_close(&fd_in), false);
	if (fd_in != 0)
		dup2(fd_in, STDIN_FILENO);
	if (fd_out != 1)
		dup2(fd_out, STDOUT_FILENO);
	return (ft_close(&fd_out), ft_close(&fd_in), true);
}
