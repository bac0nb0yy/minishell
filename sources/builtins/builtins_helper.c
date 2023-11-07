/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 04:19:01 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 07:55:37 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric(char *str, int *dst)
{
	unsigned long	num;
	int				sign;

	sign = 1;
	num = 0;
	while (*str && (((*str >= 9) && (*str <= 13)) || (*str == ' ')))
		++str;
	if (*str == '+' || *str == '-')
	{
		if (*str++ == '-')
			sign = -1;
	}
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + *str - '0';
		if ((num > LONG_MAX && sign == 1)
			|| (num - 1 > LONG_MAX && sign == -1))
			return (0);
		str++;
	}
	while (*str && (((*str >= 9) && (*str <= 13)) || (*str == ' ')))
		++str;
	if (*str != '\0')
		return (0);
	return (*dst = (sign * num), 1);
}

void	dupclose(int (*save)[2])
{
	if ((*save)[0] == -1 || (*save)[1] == -1)
		return ;
	dup2((*save)[0], 0);
	dup2((*save)[1], 1);
	ft_close(&(*save)[0]);
	ft_close(&(*save)[1]);
}

bool	init_builtins(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork)
{
	if (fork)
		pipes(msh, curr);
	else
	{
		if (!pipes_no_fork(msh, curr))
			return (clear_msh(msh, false), false);
	}
	if (!init_cmds(cmd))
		return (clear_msh(msh, false), false);
	return (true);
}

void	end_builtins(t_msh *msh, bool fork, int (*save)[2])
{
	if (fork)
	{
		clear_msh(msh, true);
		exit(g_status);
	}
	dupclose(save);
	clear_msh(msh, false);
}

bool	check_print(int ret, char *builtin)
{
	if (ret < 0)
	{
		if (ft_compstr(builtin, "echo") || ft_compstr(builtin, "cd")
			|| ft_compstr(builtin, "pwd") || ft_compstr(builtin, "export"))
			g_status = 1;
		if (ft_compstr(builtin, "env"))
			g_status = 125;
		ft_dprintf(STDERR_FILENO, "minishell: %s: write error: %s\n",
			builtin, strerror(errno));
		return (false);
	}
	g_status = 0;
	return (true);
}
