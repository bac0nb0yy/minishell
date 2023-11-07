/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 05:04:43 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 08:51:33 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(char *str)
{
	if (!*str || *str == '=' || ft_isdigit(*str))
		return (0);
	while (*str && *str != '=')
	{
		if (*str != '_' && !ft_isalnum(*str))
			return (0);
		str++;
	}
	return (1);
}

t_env	*ft_rm_elem_env_bis(t_env *env, int size1, int pos)
{
	t_env	*part1;

	part1 = malloc(sizeof(t_env) * (size1 + 1));
	pos = 0;
	while (pos < size1)
	{
		part1[pos].key = ft_strdup(env[pos].key);
		part1[pos].value = ft_strdup(env[pos].value);
		pos++;
	}
	part1[pos].key = NULL;
	part1[pos].value = NULL;
	return (part1);
}

t_env	*ft_rm_elem_env_ter(t_env *env, t_env *part1, int size1, int pos)
{
	t_env	*part2;

	part2 = malloc(sizeof(t_env) * (size(env)));
	pos = 0;
	while (pos < size(env) - 1)
	{
		if (pos < size1)
		{
			part2[pos].key = ft_strdup(part1[pos].key);
			part2[pos].value = ft_strdup(part1[pos].value);
		}
		else
		{
			part2[pos].key = ft_strdup(env[pos + 1].key);
			part2[pos].value = ft_strdup(env[pos + 1].value);
		}
		pos++;
	}
	part2[pos].key = NULL;
	part2[pos].value = NULL;
	return (part2);
}

t_env	*ft_rm_elem_env(t_env *env, char *key)
{
	t_env	*part1;
	t_env	*part2;
	int		size1;
	int		pos;

	if (!env)
		return (NULL);
	pos = 0;
	while (env[pos].key)
	{
		if (ft_compstr(env[pos].key, key))
			break ;
		pos++;
	}
	if (!env[pos].key)
		return (env);
	if (env[pos].key && !ft_compstr(env[pos].key, key))
		return (env);
	size1 = pos;
	part1 = ft_rm_elem_env_bis(env, size1, pos);
	pos = size1;
	part2 = ft_rm_elem_env_ter(env, part1, size1, pos);
	free_old_env(part1);
	free_old_env(env);
	return (part2);
}

bool	is_builtin(t_msh *msh, t_list *curr, t_cmd **cmd, bool fork)
{
	if (!(*cmd)->args)
		return (false);
	if (ft_compstr((*cmd)->args->data, ECHOO))
		return (ft_echo(msh, cmd, curr, fork), true);
	else if (ft_compstr((*cmd)->args->data, CD))
		return (cd(msh, cmd, curr, fork), true);
	else if (ft_compstr((*cmd)->args->data, PWD))
		return (pwd(msh, cmd, curr, fork), true);
	else if (ft_compstr((*cmd)->args->data, EXPORT))
		return (export(msh, cmd, curr, fork), true);
	else if (ft_compstr((*cmd)->args->data, UNSET))
		return (unset(msh, cmd, curr, fork), true);
	else if (ft_compstr((*cmd)->args->data, ENV))
		return (ft_env(msh, cmd, curr, fork), true);
	else if (ft_compstr((*cmd)->args->data, EXIT))
		return (ft_exit(msh, cmd, curr, fork), true);
	return (false);
}
