/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 17:42:19 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/04 17:42:40 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_heredoc(t_msh *msh, int *curr_heredoc, char *eof, bool expand)
{
	int			read_status;
	t_heredoc	heredoc;
	int			pipe_fd[2];

	heredoc = (t_heredoc){NULL, NULL, 0, expand};
	if (pipe(pipe_fd) == -1)
		return (exit_heredoc(msh, false, &heredoc, "pipe() failed\n"), -1);
	fcntl(pipe_fd[0], F_SETPIPE_SZ, BUF_SIZE);
	fcntl(pipe_fd[1], F_SETPIPE_SZ, BUF_SIZE);
	read_status = read_heredoc(msh, &heredoc, eof, pipe_fd);
	if (read_status == -1)
		return (ft_close_pipe(pipe_fd),
			exit_heredoc(msh, false, &heredoc, "malloc() failed\n"), -1);
	msh->lines_counter += heredoc.lines_counter;
	if (read_status == 130)
		return (free_heredoc(&heredoc), ft_close_pipe(pipe_fd), 130);
	read_status = fork();
	if (read_status == -1)
		return (ft_close_pipe(pipe_fd),
			exit_heredoc(msh, false, &heredoc, "fork() failed\n"), -1);
	if (read_status == 0)
		write_heredoc(msh, &heredoc, pipe_fd);
	(free_heredoc(&heredoc), ft_close(&pipe_fd[1]));
	msh->heredocs[*curr_heredoc] = pipe_fd[0];
	return (0);
}

void	exit_heredoc(t_msh *msh, bool fork, t_heredoc *heredoc, char *msg)
{
	free_heredoc(heredoc);
	exit_error_command(msh, fork, msg);
}
