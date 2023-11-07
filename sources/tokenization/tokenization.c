/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 08:18:09 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 08:14:00 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_join(t_tokens *tokens, char *buffer, int *i)
{
	if (buffer[*i]
		&& tokens->last->type == STR
		&& (buffer[*i] == '\'' || buffer[*i] == '"'))
		return (true);
	if (buffer[*i] && tokens->last->type == STR_QUOTED
		&& (!is_metachar(buffer[*i]) && !ft_isspace(buffer[*i])))
		return (true);
	if (buffer[*i] == '$' && (tokens->last->type == STR || tokens->last->type
			== STR_QUOTED))
		return (true);
	return (false);
}

static void	print_token(t_token *token)
{
	if (token == NULL)
		ft_dprintf(STDERR_FILENO, "newline");
	else if (token->type == STR || token->type == STR_QUOTED)
		ft_dprintf(STDERR_FILENO, "%s", token->content);
	else if (token->type == HEREDOC)
		ft_dprintf(STDERR_FILENO, "<<");
	else if (token->type == I_FILE)
		ft_dprintf(STDERR_FILENO, "<");
	else if (token->type == O_FILE_TRUNC)
		ft_dprintf(STDERR_FILENO, ">");
	else if (token->type == O_FILE_APPEND)
		ft_dprintf(STDERR_FILENO, ">>");
	else if (token->type == PIPE)
		ft_dprintf(STDERR_FILENO, "|");
	else
		ft_dprintf(STDERR_FILENO, "unknown");
}

static void	print_syntax_error(t_token *token, bool next)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: syntax error near unexpected token `");
	if (next)
		print_token(token->next);
	else
		print_token(token);
	ft_dprintf(STDERR_FILENO, "'\n");
}

static bool	check_syntax_errors(t_tokens *tokens, int *heredoc_nb)
{
	t_token	*curr;
	bool	next;

	curr = tokens->first;
	next = true;
	while (curr)
	{
		if (is_redir(curr->type) && (!curr->next || !is_str(curr->next->type)))
			break ;
		if (curr->type == PIPE && !curr->prev)
		{
			next = false;
			break ;
		}
		if (curr->type == UNKNOWN || (curr->type == PIPE && (!curr->next
					|| (curr->next && curr->next->type == PIPE))))
			break ;
		if (curr->type == HEREDOC)
			(*heredoc_nb)++;
		curr = curr->next;
	}
	if (curr)
		return (print_syntax_error(curr, next), false);
	return (true);
}

int	tokenizer(t_msh *msh, t_tokens *tokens, char *buffer)
{
	int		i;
	int		ret;

	i = 0;
	while (buffer[i])
	{
		skip_spaces(buffer, &i);
		if (buffer[i] && !is_metachar(buffer[i]))
		{
			ret = push_token_str(buffer, &i, tokens);
			if (ret == -1 || ret == 2)
				return (clear_tokens(tokens), ret);
		}
		else if (buffer[i])
			if (!push_metachar_token(buffer, &i, tokens))
				return (clear_tokens(tokens), -1);
	}
	if (!check_syntax_errors(tokens, &msh->heredoc_nb))
	{
		msh->syntax_error = true;
		if (msh->heredoc_nb > 0)
			return (2 + msh->heredoc_nb);
		return (clear_tokens(tokens), 0);
	}
	return (msh->heredoc_nb);
}
