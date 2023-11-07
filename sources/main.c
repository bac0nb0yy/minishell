/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 02:42:20 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 21:17:13 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

int	lexer(char *buffer, t_msh *msh)
{
	int	ret;

	if (!buffer[0])
		return (g_status);
	msh->heredoc_nb = 0;
	ret = tokenizer(msh, &msh->tokens, buffer);
	if (ret == -1)
		return (clear_msh(msh, false), -1);
	if (msh->syntax_error == true && msh->heredoc_nb == 0)
		return (clear_msh(msh, false), 2);
	if (!parser(&msh->tokens, &msh->pipeline, msh->env))
		return (clear_msh(msh, false), -1);
	ret = create_heredocs(msh);
	if (ret == -1)
		return (clear_msh(msh, false), -1);
	if (ret == 130)
		return (clear_msh(msh, false), ret);
	if (msh->syntax_error == true)
		return (clear_msh(msh, false), 2);
	ret = exec(msh);
	clear_msh(msh, false);
	return (ret);
}

static int	routine(t_msh *msh)
{
	char	*buffer;

	while (true)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		buffer = readline(PROMPT);
		if (!buffer)
		{
			ft_dprintf(STDERR_FILENO, "exit\n");
			break ;
		}
		++msh->lines_counter;
		if (ft_strlen(buffer) > 0)
			add_history(buffer);
		g_status = lexer(buffer, msh);
		if (g_status == -1)
			ft_dprintf(STDERR_FILENO, "minishell: a function failed\n");
		free(buffer);
	}
	clear_msh(msh, true);
	return (g_status);
}

void	init_msh(t_msh *msh, char **envp)
{
	msh->env = NULL;
	if (!parse_env(&msh->env, envp))
		(perror("malloc() failed to create env"), exit(EXIT_FAILURE));
	set_shlvl(msh);
	set_pwd(msh);
	msh->pipeline = NULL;
	msh->tokens = (t_tokens){};
	msh->heredocs = NULL;
	msh->syntax_error = false;
	msh->heredoc_nb = 0;
	msh->lines_counter = 0;
	msh->exec = (t_exec){{-1, -1}, -1, 0, 0, 0, 0, 0};
}

void	clear_msh(t_msh *msh, bool env_clear)
{
	if (!msh)
		return ;
	if (env_clear)
	{
		free_old_env(msh->env);
		msh->env = NULL;
	}
	if (msh->pipeline)
	{
		clear_commands(&msh->pipeline);
		msh->pipeline = NULL;
	}
	clear_tokens(&msh->tokens);
	if (msh->heredocs)
	{
		close_fd_valid(msh, msh->heredocs, msh->heredoc_nb);
		free(msh->heredocs);
		msh->heredocs = NULL;
	}
	close_fd_valid(msh, msh->heredocs, msh->heredoc_nb);
	msh->syntax_error = false;
	msh = NULL;
}

int	main(int ac, char **av, char **envp)
{
	t_msh	msh;
	int		ret;

	init_msh(&msh, envp);
	(void)ac;
	(void)av;
	ret = routine(&msh);
	return (ret);
}
