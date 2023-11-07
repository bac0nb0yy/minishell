/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 02:06:20 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/06 15:26:29 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_content_token(char *buffer, int *i, char **data)
{
	int		ret;

	if (buffer[*i] == '"' || buffer[*i] == '\'')
		ret = get_quoted_data(buffer + *i, data);
	else
		ret = get_unquoted_data(buffer + *i, data);
	if (ret == -1)
		return (-1);
	else if (ret == -2)
		return (ft_dprintf(STDERR_FILENO,
				"minishell: syntax error: unmatch quotation\n"), -2);
	*i += ret;
	return (ret);
}

t_tokentype	get_tokentype(char *buffer, int *i)
{
	if (buffer[*i] == '|')
		return (++(*i), PIPE);
	else if (buffer[*i] == '>' && buffer[*i + 1] == '|')
		return (((*i) += 2), UNKNOWN);
	else if (buffer[*i] == '<' && buffer[*i + 1] != '<')
		return (++(*i), I_FILE);
	else if (buffer[*i] == '>' && buffer[*i + 1] != '>')
		return (++(*i), O_FILE_TRUNC);
	else if (buffer[*i] == '<' && buffer[*i + 1] == '<')
		return (((*i) += 2), HEREDOC);
	else if (buffer[*i] == '>' && buffer[*i + 1] == '>')
		return (((*i) += 2), O_FILE_APPEND);
	else
		return (UNKNOWN);
}

bool	push_metachar_token(char *buffer, int *i, t_tokens *tokens)
{
	if (!push_back_token(tokens))
		return (perror("malloc"), false);
	tokens->last->type = get_tokentype(buffer, i);
	tokens->last->content = NULL;
	return (true);
}

static int	void_string(char **data, int *i, int *type)
{
	*data = malloc(1);
	if (!(*data))
		return (-1);
	(*data)[0] = '\0';
	*i += 3;
	*type = STR_QUOTED;
	return (0);
}

int	push_token_str(char *buffer, int *i, t_tokens *tokens)
{
	char	*data;
	int		type;
	bool	expand;
	int		ret;

	data = NULL;
	type = STR;
	expand = true;
	if (buffer[*i] == '\'' || (tokens->last && tokens->last->type == HEREDOC))
		expand = false;
	if (buffer[*i] == '\'' || buffer[*i] == '"')
		type = STR_QUOTED;
	if (buffer[*i] == '$' && ((buffer[*i + 1] == '"' && buffer[*i + 2] == '"')
			|| (buffer[*i + 1] == '\'' && buffer[*i + 2] == '\'')))
		ret = void_string(&data, i, &type);
	else
		ret = get_content_token(buffer, i, &data);
	if (ret == -1)
		return (ret);
	if (ret == -2)
		return (2);
	if (!fill_tokens_info(tokens, data, type, expand))
		return (-1);
	tokens->last->join = is_join(tokens, buffer, i);
	return (0);
}
