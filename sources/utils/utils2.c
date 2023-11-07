/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 04:29:43 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 20:58:35 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_env_helper(t_msh *msh, char *key, char *value)
{
	msh->env = ft_rm_elem_env(msh->env, key);
	msh->env = ft_add_elem_env(msh->env, key, value);
}

void	free_old_env(t_env *env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i].key)
	{
		if (env[i].key)
		{
			free(env[i].key);
			env[i].key = NULL;
		}
		if (env[i].value)
		{
			free(env[i].value);
			env[i].value = NULL;
		}
		i++;
	}
	free(env);
}

void	ft_add_elem_env_sub(t_env *env, t_env **tmp, char *key, char *value)
{
	int	i;

	i = 0;
	while (env[i].key)
	{
		if (env[i].key)
			(*tmp)[i].key = ft_strdup(env[i].key);
		if (env[i].value)
			(*tmp)[i].value = ft_strdup(env[i].value);
		else
			(*tmp)[i].value = NULL;
		i++;
	}
	(*tmp)[i].key = ft_strdup(key);
	if (value)
		(*tmp)[i].value = ft_strdup(value);
	else
		(*tmp)[i].value = NULL;
	++i;
	(*tmp)[i].key = NULL;
	(*tmp)[i].value = NULL;
}

t_env	*ft_add_elem_env(t_env *env, char *key, char *value)
{
	t_env	*tmp;

	if (!env)
		tmp = malloc(sizeof(t_env) * 2);
	else
		tmp = malloc(sizeof(t_env) * (size(env) + 2));
	if (!tmp)
	{
		if (!key)
			free(key);
		if (!value)
			free(value);
	}
	ft_add_elem_env_sub(env, &tmp, key, value);
	return (free_old_env(env), tmp);
}
