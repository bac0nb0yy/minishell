/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 07:49:02 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 21:12:47 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_delete_element(t_env *env, char *key)
{
	int	i;

	i = 0;
	while (env[i].key)
	{
		if (ft_compstr(env[i].key, key))
		{
			free(env[i].key);
			free(env[i].value);
			while (env[i].key)
			{
				env[i] = env[i + 1];
				++i;
			}
			return ;
		}
		++i;
	}
}

int	size(t_env *env)
{
	int	i;

	i = 0;
	if (!env[i].key)
		return (0);
	while (env[i].key)
		i++;
	return (i);
}

int	verif_overflow(char *argv)
{
	char	*str;

	if (argv[0] == '+')
	{
		str = ft_lltoa(ft_atoll(&argv[1]));
		if (ft_strncmp(&argv[1], str, ft_strlen(str)))
			return (free(str), -1);
		free(str);
	}
	str = ft_lltoa(ft_atoll(argv));
	if (ft_strncmp(argv, str, ft_strlen(str)))
		return (free(str), -1);
	free(str);
	return (1);
}

void	set_shlvl(t_msh *msh)
{
	char		*var;
	int			shlvl;

	var = get_env_value(msh->env, "SHLVL");
	if (!var || !is_numeric(var, &shlvl))
		set_env_helper(msh, "SHLVL", "1");
	else if (shlvl < 0)
		set_env_helper(msh, "SHLVL", "0");
	else if (shlvl > 998)
	{
		ft_dprintf(STDERR_FILENO, "minishell: warning: shell level (%d) too \
high, resetting to 1\n", shlvl);
		set_env_helper(msh, "SHLVL", "1");
	}
	else
	{
		msh->env = ft_rm_elem_env(msh->env, "SHLVL");
		var = ft_itoa(shlvl + 1);
		msh->env = ft_add_elem_env(msh->env, "SHLVL", var);
		free(var);
	}
}

void	set_pwd(t_msh *msh)
{
	char	pwd[PATH_MAX];

	msh->env = ft_rm_elem_env(msh->env, "PWD");
	if (!getcwd(pwd, PATH_MAX))
	{
		ft_dprintf(STDERR_FILENO, "minishell-init: error retrieving current \
directory: getcwd: cannot access parent directories: %s\n", strerror(errno));
	}
	else
		msh->env = ft_add_elem_env(msh->env, "PWD", pwd);
	if (!get_env_value(msh->env, "OLDPWD"))
		msh->env = ft_add_elem_env(msh->env, "OLDPWD", NULL);
}
