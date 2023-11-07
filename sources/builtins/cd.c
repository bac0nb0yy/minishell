/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 03:39:55 by tlivroze          #+#    #+#             */
/*   Updated: 2023/11/05 21:49:30 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	replace_pwd(t_env *env)
{
	char	pwd[4096];
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = ft_strdup(get_env_value(env, "PWD"));
	if (!old_pwd)
		return (false);
	env_replace_value(env, "OLDPWD", old_pwd);
	getcwd(pwd, 4096);
	new_pwd = ft_strdup(pwd);
	if (!new_pwd)
		return (false);
	env_replace_value(env, "PWD", new_pwd);
	return (true);
}

static int	cd_env(t_msh *msh, char *var)
{
	const char	*dest = get_env_value(msh->env, var);

	if (!dest)
		return (ft_dprintf(STDERR_FILENO,
				"minishell: cd: %s not set\n", var), 1);
	if (chdir(dest) != 0)
		return (ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", dest,
				strerror(errno)), 1);
	if (ft_compstr(var, "OLDPWD") && !check_print(ft_printf("%s\n",
				dest), "cd"))
		return (1);
	if (!replace_pwd(msh->env))
		return (2);
	return (0);
}

static int	chdir_cdpath(t_msh *msh, t_cmd **cmd, char *arg)
{
	int		i;
	char	*tmp;

	if (!parse_path(cmd, msh->env, "CDPATH"))
		return (2);
	if (!(*cmd)->paths)
		return (1);
	i = 0;
	while ((*cmd)->paths[i])
	{
		tmp = join_three((*cmd)->paths[i], "/", arg);
		if (!tmp)
			return (2);
		if (chdir(tmp) == 0)
		{
			free(tmp);
			tmp = getcwd(NULL, 0);
			if (tmp && !check_print(ft_printf("%s\n", tmp), "cd"))
				return (free(tmp), 1);
			return (free(tmp), 0);
		}
		free(tmp);
		++i;
	}
	return (1);
}

static int	cd_normal(t_msh *msh, t_cmd **cmd, char *arg)
{
	int		ret;

	ret = 0;
	if (chdir(arg) == 0)
	{
		if (!replace_pwd(msh->env))
			return (2);
		return (0);
	}
	ret = chdir_cdpath(msh, cmd, arg);
	if (ret == 2)
		return (2);
	if (ret == 0)
	{
		if (!replace_pwd(msh->env))
			return (2);
		return (0);
	}
	ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", arg, strerror(errno));
	return (1);
}

void	cd(t_msh *msh, t_cmd **cmd, t_list *curr, bool fork)
{
	int		save[2];

	if (!fork)
	{
		save[0] = dup(0);
		save[1] = dup(1);
	}
	if (!init_builtins(msh, cmd, curr, fork))
		return (end_builtins(msh, fork, &save));
	if ((*cmd)->command[1] && (*cmd)->command[2])
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: cd: too many arguments\n");
		g_status = 1;
	}
	if (!((*cmd)->command[1]))
		g_status = cd_env(msh, "HOME");
	else if (ft_compstr((*cmd)->command[1], "-"))
		g_status = cd_env(msh, "OLDPWD");
	else
		g_status = cd_normal(msh, cmd, (*cmd)->command[1]);
	end_builtins(msh, fork, &save);
}
