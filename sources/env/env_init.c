/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 21:42:12 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/04 15:52:10 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	env_replace_value(t_env *env, char *key, char *new_value)
{
	int	i;

	i = 0;
	while (env[i].key)
	{
		if (ft_compstr(env[i].key, key))
		{
			free(env[i].value);
			env[i].value = new_value;
			return (true);
		}
		++i;
	}
	return (false);
}

char	*get_env_value(t_env *env, char *key)
{
	int	i;

	i = 0;
	while (env[i].key)
	{
		if (ft_compstr(env[i].key, key))
			return (env[i].value);
		++i;
	}
	return (NULL);
}

bool	print_env(t_env *env, char *builtin)
{
	int	i;

	i = 0;
	while (env[i].key)
	{
		if (env[i].value)
		{
			if (!check_print(ft_printf("%s=%s\n", env[i].key, env[i].value)
					, builtin))
				return (false);
		}
		++i;
	}
	return (true);
}

static int	get_env_size(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		++i;
	return (i);
}

bool	parse_env(t_env **env, char **envp)
{
	char		*key;
	char		*value;
	int			i;
	const int	size = get_env_size(envp);

	*env = malloc(sizeof(t_env) * (size + 1));
	if (!env)
		return (false);
	i = 0;
	while (envp[i])
	{
		key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		value = ft_substr(envp[i], ft_strchr(envp[i], '=') - envp[i] + 1,
				ft_strlen(envp[i]));
		(*env)[i] = (t_env){key, value};
		++i;
	}
	(*env)[i] = (t_env){};
	return (true);
}
