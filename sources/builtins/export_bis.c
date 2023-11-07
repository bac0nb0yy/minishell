/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_bis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:36:10 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 21:00:06 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_valid(char *args)
{
	if (!is_valid_identifier(args))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", args);
		g_status = 1;
		return (false);
	}
	return (true);
}

void	export_bis(t_msh *msh, char **args)
{
	int		i;
	char	*tmp;
	char	*key;

	i = 1;
	while (args[i])
	{
		if (!check_valid(args[i]))
		{
			++i;
			continue ;
		}
		tmp = ft_strchr(args[i], '=');
		if (tmp)
		{
			key = ft_substr(args[i], 0, tmp - args[i]);
			set_env_helper(msh, key, tmp + 1);
			free(key);
		}
		else
			set_env_helper(msh, args[i], NULL);
		++i;
	}
}
