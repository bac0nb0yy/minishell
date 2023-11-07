/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 02:55:10 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 21:48:59 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	flags_echo(char *flag)
{
	int	i;

	i = 0;
	if (!flag)
		return (false);
	if (!flag[i] || flag[i] != '-')
		return (false);
	i++;
	while (flag[i])
	{
		if (flag[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static void	case_no_arg(t_msh *msh, t_cmd **cmd, bool fork, int *save)
{
	int	ret;

	ret = 0;
	if (!((*cmd)->command[1]))
		ret = ft_printf("\n");
	if (!fork)
	{
		dup2(save[0], 0);
		dup2(save[1], 1);
		ft_close(&save[0]);
		ft_close(&save[1]);
	}
	if (ret == -1)
	{
		g_status = 1;
		ft_dprintf(STDERR_FILENO, "minishell: echo: write error: %s\n",
			strerror(errno));
		clear_msh(msh, fork);
		return ;
	}
	if (fork)
		(clear_msh(msh, true), exit(0));
	clear_msh(msh, false);
	g_status = 0;
}

void	normal_case(t_cmd **cmd, int i)
{
	int	k;

	k = i;
	while ((*cmd)->command[k] && (*cmd)->command[k + 1])
	{
		if (!check_print(ft_printf("%s ", (*cmd)->command[k]), "echo"))
			return ;
		++k;
	}
	if (i == 1)
	{
		if (!check_print(ft_printf("%s\n", (*cmd)->command[k]), "echo"))
			return ;
	}
	else
	{
		if (!(*cmd)->command[k])
			return ;
		if (!check_print(ft_printf("%s", (*cmd)->command[k]), "echo"))
			return ;
	}
}

void	ft_echo(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork)
{
	int		i;
	int		save[2];

	if (!fork)
	{
		save[0] = dup(0);
		save[1] = dup(1);
	}
	if (!init_builtins(msh, cmd, curr, fork))
		return (end_builtins(msh, fork, &save));
	i = 1;
	if (!(*cmd)->command[i])
		return (case_no_arg(msh, cmd, fork, save));
	while (flags_echo((*cmd)->command[i]))
		i++;
	normal_case(cmd, i);
	end_builtins(msh, fork, &save);
	return ;
}
