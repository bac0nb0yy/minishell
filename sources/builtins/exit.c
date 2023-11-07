/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 02:28:57 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 21:49:37 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_save(int (*save)[2], bool fork)
{
	if (fork)
	{
		(*save)[0] = -1;
		(*save)[1] = -1;
		return ;
	}
	(*save)[0] = dup(0);
	(*save)[1] = dup(1);
}

void	ft_exit(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork)
{
	int	ret;
	int	save[2];

	init_save(&save, fork);
	if (!init_builtins(msh, cmd, curr, fork))
		return (end_builtins(msh, fork, &save));
	if (isatty(STDIN_FILENO) && !fork)
		ft_dprintf(STDERR_FILENO, "exit\n");
	if (!(*cmd)->command[1])
		return (dupclose(&save), end_builtins(msh, true, &save));
	if (is_numeric((*cmd)->command[1], &ret) && !(*cmd)->command[2])
		return (g_status = ret & 0xFF, dupclose(&save),
			end_builtins(msh, true, &save));
	if (!is_numeric((*cmd)->command[1], &ret))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n",
			(*cmd)->command[1]);
		return (g_status = 2, dupclose(&save), end_builtins(msh, true, &save));
	}
	ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
	return (g_status = 1, end_builtins(msh, fork, &save));
}
