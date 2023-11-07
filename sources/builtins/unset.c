/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 06:52:35 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 21:49:49 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork)
{
	int	i;
	int	save[2];

	if (!fork)
	{
		save[0] = dup(0);
		save[1] = dup(1);
	}
	if (!init_builtins(msh, cmd, curr, fork))
		return (end_builtins(msh, fork, &save));
	i = 1;
	while ((*cmd)->command[i])
	{
		if (is_valid_identifier((*cmd)->command[i])
			&& !ft_strchr((*cmd)->command[i], '='))
			msh->env = ft_rm_elem_env(msh->env, (*cmd)->command[i]);
		i++;
	}
	return (g_status = 0, end_builtins(msh, fork, &save));
}
