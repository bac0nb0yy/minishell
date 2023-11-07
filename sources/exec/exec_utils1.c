/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:41:38 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/04 17:25:40 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_directory(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

bool	get_cmd_path(t_cmd **cmd)
{
	int		i;
	char	*tmp;

	if (!(*cmd)->command[0])
		return (true);
	if (ft_strchr((*cmd)->command[0], '/'))
		return ((*cmd)->cmd_path = ft_strdup((*cmd)->command[0]),
			(*cmd)->cmd_path != NULL);
	if (!(*cmd)->command[0][0] || (*cmd)->command[0][0] == '.'
		|| !(*cmd)->paths)
		return (true);
	i = 0;
	while ((*cmd)->paths[i])
	{
		tmp = join_three((*cmd)->paths[i], "/", (*cmd)->command[0]);
		if (!tmp)
			return (false);
		if (access(tmp, F_OK) == 0)
			return ((*cmd)->cmd_path = tmp, true);
		free(tmp);
		++i;
	}
	return (true);
}

static void	assign_errors(t_cmd *cmd, char *error_message,
							char *error_path, int ret)
{
	cmd->error_message = error_message;
	cmd->error_path = error_path;
	cmd->ret_value = ret;
}

int	execute_errors(t_msh *msh, t_cmd *cmd, int ret_value, bool invalid)
{
	if (ret_value == RET_COMMAND_NOT_FOUND)
	{
		assign_errors(cmd, strerror(errno), cmd->command[0], ret_value);
		if (!invalid)
		{
			assign_errors(cmd, COMMAND_NOT_FOUND, cmd->command[0], ret_value);
			if (!cmd->command || !cmd->command[0])
				assign_errors(cmd, COMMAND_NOT_FOUND, "", ret_value);
		}
	}
	else
	{
		assign_errors(cmd, strerror(errno), cmd->command[0], ret_value);
		if (is_directory(cmd->cmd_path))
			assign_errors(cmd, IS_A_DIRECTORY, cmd->command[0], ret_value);
	}
	if (!cmd->command[0] && cmd->args && cmd->args->data_type == AMBIGUOUS)
		return (cmd->ret_value = 0, cmd->ret_value);
	if (!cmd->command[0] && msh->heredocs)
		return (cmd->ret_value = 0, cmd->ret_value);
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
		cmd->error_path, cmd->error_message);
	return (cmd->ret_value);
}

bool	parse_path(t_cmd **cmd, t_env *env, char *path)
{
	char	*paths;

	paths = get_env_value(env, path);
	if (!paths)
		return (true);
	(*cmd)->paths = ft_split(paths, ':');
	if (!(*cmd)->paths)
		return (false);
	return (true);
}
