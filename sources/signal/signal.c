/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 20:21:39 by dtelnov           #+#    #+#             */
/*   Updated: 2023/09/02 20:21:39 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sig_in_fork(int sig)
{
	if (sig == SIGINT)
		g_status = 130;
	else if (sig == SIGQUIT)
		g_status = 131;
}

void	handle_sigint(int sig)
{
	(void)sig;
	g_status = 130;
	ft_dprintf(STDERR_FILENO, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sig_in_heredoc(int sig)
{
	int	fd;

	if (sig == SIGINT)
	{
		fd = open("/dev/null", O_RDONLY);
		dup2(fd, STDIN_FILENO);
		ft_close(&fd);
		ft_dprintf(STDERR_FILENO, "\n");
		g_status = 257;
	}
}
