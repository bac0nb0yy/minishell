/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 07:21:37 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 21:49:44 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	in(t_env *env, char	*s)
{
	int	i;

	i = 0;
	while (env[i].key)
	{
		if (ft_compstr(env[i].key, s))
			return (true);
		i++;
	}
	return (false);
}

void	alloc_copy(t_env *copy, t_env *env, int i, int j)
{
	while (i < j)
	{
		copy[i].key = malloc(sizeof(char) * ft_strlen(env[i].key) + 1);
		ft_strlcpy(copy[i].key, env[i].key, ft_strlen(env[i].key) + 1);
		if (!env[i].value)
			copy[i].value = NULL;
		else
		{
			copy[i].value = malloc(sizeof(char) * ft_strlen(env[i].value) + 1);
			ft_strlcpy(copy[i].value, env[i].value,
				ft_strlen(env[i].value) + 1);
		}
		i++;
	}
	copy[i].key = NULL;
	copy[i].key = NULL;
}

void	print_export_elem(t_env *copy, char *smallest)
{
	char	*value;

	if (g_status == 1)
		return ;
	value = get_env_value(copy, smallest);
	if (value)
		check_print(ft_printf("declare -x %s=\"%s\"\n", smallest, value),
			"export");
	else
		check_print(ft_printf("declare -x %s\n", smallest), "export");
}

void	export_without_args(t_env *env)
{
	int		i;
	int		j;
	t_env	*copy;
	char	*smallest;

	j = size(env);
	i = 0;
	copy = malloc(sizeof(t_env) * (j + 1));
	alloc_copy(copy, env, i, j);
	while (j > 0)
	{
		i = 0;
		smallest = copy[i].key;
		while (copy[i].key)
		{
			if (ft_strncmp(copy[i].key, smallest, ft_strlen(copy[i].key)) < 0)
				smallest = copy[i].key;
			i++;
		}
		print_export_elem(copy, smallest);
		env_delete_element(copy, smallest);
		j--;
	}
	free(copy);
}

void	export(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork)
{
	int	save[2];

	if (!fork)
	{
		save[0] = dup(0);
		save[1] = dup(1);
	}
	if (!init_builtins(msh, cmd, curr, fork))
		return (end_builtins(msh, fork, &save));
	g_status = 0;
	if (!(*cmd)->command[1])
		export_without_args(msh->env);
	else
		export_bis(msh, (*cmd)->command);
	return (end_builtins(msh, fork, &save));
}
