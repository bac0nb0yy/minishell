/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 20:21:01 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 20:50:05 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_commands(t_exec *exec)
{
	while (1)
	{
		exec->wpid = wait(&exec->status);
		if (exec->wpid == -1)
			break ;
		if (exec->wpid != exec->pid || exec->ret_value != EXIT_SUCCESS)
			continue ;
		if (WTERMSIG(exec->status) == 2)
			ft_dprintf(STDERR_FILENO, "\n");
		else if (WTERMSIG(exec->status) == 3)
			ft_dprintf(STDERR_FILENO, "Quit (core dumped)\n");
		if (WIFEXITED(exec->status))
			exec->ret_value = WEXITSTATUS(exec->status);
		else
			exec->ret_value = 128 + WTERMSIG(exec->status);
	}
	return (exec->ret_value);
}

static inline void	__pipe_and_fork_and_sig__(t_msh *msh, t_list *curr)
{
	signal(SIGINT, handle_sig_in_fork);
	signal(SIGQUIT, handle_sig_in_fork);
	if (curr->next && pipe(msh->exec.pipe) == -1)
		exit_error_command(msh, false, "pipe() failed");
	msh->exec.pid = fork();
	if (msh->exec.pid == -1)
		exit_error_command(msh, false, "fork() failed");
}

int	number_of_heredocs(t_list *list)
{
	int		count;
	t_str	*tmp;

	count = 0;
	tmp = ((t_cmd *)list->content)->redirs;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
			++count;
		tmp = tmp->next;
	}
	return (count);
}

void	do_exec(t_msh *msh, t_list *curr, t_cmd *curr_cmd)
{
	while (curr)
	{
		curr_cmd = curr->content;
		__pipe_and_fork_and_sig__(msh, curr);
		if (msh->exec.pid == 0)
			child(msh, &curr_cmd, curr);
		msh->exec.curr_heredoc += number_of_heredocs(curr);
		ft_close(&msh->exec.prev_pipe);
		if (curr->next)
		{
			msh->exec.prev_pipe = msh->exec.pipe[0];
			ft_close(&msh->exec.pipe[1]);
		}
		curr = curr->next;
	}
}

int	exec(t_msh *msh)
{
	t_list	*curr;
	t_cmd	*curr_cmd;

	msh->exec = (t_exec){{-1, -1}, -1, 0, 0, 0, 0, 0};
	curr = (*msh).pipeline;
	if (!curr)
		return (g_status);
	curr_cmd = curr->content;
	if (!curr->next && is_builtin(msh, curr, &curr_cmd, false))
		return (g_status);
	do_exec(msh, curr, curr_cmd);
	(ft_close(&msh->exec.prev_pipe), close_pipes(&msh->exec, true, true));
	return (wait_commands(&msh->exec));
}
