/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 16:43:49 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/04 16:47:24 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	fill_tokens_info(t_tokens *tokens, char *data, int type, bool expand)
{
	if (!push_back_token(tokens))
		return (false);
	tokens->last->content = data;
	tokens->last->type = type;
	tokens->last->expand = expand;
	return (true);
}
