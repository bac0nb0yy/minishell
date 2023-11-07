/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 01:50:57 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 21:49:47 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork)
{
	char	pwd[PATH_MAX];
	int		save[2];

	if (!fork)
	{
		save[0] = dup(0);
		save[1] = dup(1);
	}
	if (!init_builtins(msh, cmd, curr, fork))
		return (end_builtins(msh, fork, &save));
	if (!getcwd(pwd, PATH_MAX))
	{	
		ft_dprintf(STDERR_FILENO, "minishell: pwd: %s\n", strerror(errno));
		g_status = 1;
	}
	else
		check_print(ft_printf("%s\n", pwd), "pwd");
	end_builtins(msh, fork, &save);
}
