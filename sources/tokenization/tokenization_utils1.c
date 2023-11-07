/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 05:16:47 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 21:17:03 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_str(int c)
{
	return (c == STR || c == STR_QUOTED);
}

bool	is_metachar(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

bool	is_redir(int c)
{
	return (c >= HEREDOC && c <= O_FILE_APPEND);
}

int	get_unquoted_data(char *buffer, char **data)
{
	int	len;

	len = 0;
	if (buffer[len] == '$' && (buffer[len + 1] == '\''
			|| buffer[len + 1] == '"'))
	{
		*data = ft_strdup("");
		if (!*data)
			return (-1);
		return (1);
	}
	while (buffer[len] && !ft_isspace(buffer[len])
		&& !is_metachar(buffer[len])
		&& buffer[len] != '\'' && buffer[len] != '"'
		&& (len == 0 || buffer[len] != '$'))
		++len;
	*data = malloc(sizeof(char) * (len + 1));
	if (!*data)
		return (-1);
	ft_strlcpy(*data, buffer, len + 1);
	return (len);
}

int	get_quoted_data(char *buffer, char **data)
{
	int			len;
	const char	quote = buffer[0];

	len = 1;
	while (buffer[len] && buffer[len] != quote)
		++len;
	if (buffer[len] != quote)
		return (-2);
	*data = ft_substr(buffer, 1, len - 1);
	if (!*data)
		return (-1);
	return (len + 1);
}
