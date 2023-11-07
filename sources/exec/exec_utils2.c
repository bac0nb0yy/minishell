/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:42:37 by dtelnov           #+#    #+#             */
/*   Updated: 2023/10/07 02:14:21 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**join_env_to_arr(t_env *env)
{
	char	**arr;
	int		i;

	arr = malloc(sizeof(char *) * (size(env) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env[i].key)
	{
		arr[i] = join_three(env[i].key, "=", env[i].value);
		if (!arr[i])
		{
			ft_free_2d_array((void ***)&arr);
			return (NULL);
		}
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

void	close_pipes(t_exec *exec, bool fd1, bool fd2)
{
	if (fd1 && exec->pipe[0] != -1)
		ft_close(&exec->pipe[0]);
	if (fd2 && exec->pipe[1] != -1)
		ft_close(&exec->pipe[1]);
}

void	ft_close(int *fd)
{
	if (*fd > 2 && *fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

void	ft_close_pipe(int fd[2])
{
	ft_close(&fd[0]);
	ft_close(&fd[1]);
}

void	exit_error_command(t_msh *msh, bool fork, char *msg)
{
	close_pipes(&msh->exec, true, true);
	clear_msh(msh, true);
	ft_dprintf(STDERR_FILENO, "minishell: %s\n", msg);
	if (fork)
		exit(EXIT_FAILURE);
}
