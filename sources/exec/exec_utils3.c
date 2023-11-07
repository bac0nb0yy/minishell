/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:38:44 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 21:31:04 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_fds(t_msh *msh, t_cmd *cmd, int *fd_in, int *fd_out)
{
	t_str	*tmp;

	tmp = cmd->redirs;
	while (tmp)
	{
		if (tmp->type == I_FILE)
			do_redir(msh, tmp, fd_in, 0);
		else if (tmp->type == HEREDOC)
		{
			ft_close(fd_in);
			*fd_in = msh->heredocs[msh->exec.curr_heredoc];
			msh->exec.curr_heredoc++;
		}
		else if (tmp->type == O_FILE_TRUNC || tmp->type == AMBIGUOUS)
			do_redir(msh, tmp, fd_out, 1);
		else if (tmp->type == O_FILE_APPEND || tmp->type == AMBIGUOUS)
			do_redir(msh, tmp, fd_out, 2);
		tmp = tmp->next;
	}
}

void	pipes(t_msh *msh, t_list *curr)
{
	int	fd_in;
	int	fd_out;

	fd_in = 0;
	fd_out = 1;
	init_fds(msh, (t_cmd *)curr->content, &fd_in, &fd_out);
	if (fd_in != 0)
		dup2(fd_in, STDIN_FILENO);
	if (fd_out != 1)
		dup2(fd_out, STDOUT_FILENO);
	if (fd_in == 0)
		dup2(msh->exec.prev_pipe, STDIN_FILENO);
	(ft_close(&fd_out), ft_close(&fd_in));
	ft_close(&msh->exec.prev_pipe);
	if (curr->next)
	{
		if (fd_out == 1)
			dup2(msh->exec.pipe[1], STDOUT_FILENO);
	}
	close_pipes(&msh->exec, true, true);
}

bool	init_cmds(t_cmd **cmd)
{
	int		i;
	char	**arr;
	t_str	*tmp;

	i = 0;
	arr = malloc(sizeof(char *) * (ft_strsize((*cmd)->args) + 1));
	if (!arr)
		return (false);
	tmp = (*cmd)->args;
	while (tmp)
	{
		if (tmp->data_type != AMBIGUOUS)
		{
			arr[i] = tmp->data;
			i++;
		}
		tmp = tmp->next;
	}
	arr[i] = NULL;
	(*cmd)->command = arr;
	return (true);
}

static void	set_ret_value(t_msh *msh, t_cmd **cmd,
							int *ret_value, bool *invalid)
{
	char	**tmp;

	if (!(*cmd)->cmd_path)
		*ret_value = RET_COMMAND_NOT_FOUND;
	else if (access((*cmd)->cmd_path, F_OK) != 0)
	{
		*ret_value = RET_COMMAND_NOT_FOUND;
		if (errno == ENOTDIR || errno == EISDIR)
			*ret_value = RET_CANNOT_EXECUTE;
		*invalid = true;
	}
	else
	{
		tmp = join_env_to_arr((*cmd)->env);
		close_fd_valid(msh, msh->heredocs, msh->heredoc_nb);
		execve((*cmd)->cmd_path, (*cmd)->command, tmp);
		ft_free_2d_array((void ***)&tmp);
		*ret_value = RET_CANNOT_EXECUTE;
	}
}

void	child(t_msh *msh, t_cmd **cmd, t_list *curr)
{
	int		ret_value;
	bool	invalid;
	int		ret;

	pipes(msh, curr);
	if (is_builtin(msh, curr, cmd, true))
		exit(g_status);
	if (!parse_path(cmd, msh->env, "PATH") || !init_cmds(cmd))
		exit_error_command(msh, true, "malloc() failed");
	invalid = false;
	if (!get_cmd_path(cmd))
		exit_error_command(msh, true, "malloc() failed");
	set_ret_value(msh, cmd, &ret_value, &invalid);
	execute_errors(msh, *cmd, ret_value, invalid);
	ret = (*cmd)->ret_value;
	clear_msh(msh, true);
	exit(ret);
}
