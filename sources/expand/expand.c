/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtelnov <dtelnov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 07:55:15 by dtelnov           #+#    #+#             */
/*   Updated: 2023/11/05 06:46:36 by dtelnov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	special_chars(char c)
{
	return (c == '!' || c == '@' || c == '#' || c == '$' || c == '*'
		|| c == '-' || c == '_' || c == '?');
}

static char	*expand_one(char *str, t_env *env)
{
	char	*key;
	char	*value;
	int		i;

	i = 0;
	if (!str[i])
		return (ft_strdup("$"));
	if (str[i] == '?')
		return (ft_sprintf("%d", g_status));
	if (ft_isdigit(str[i]))
		return (ft_strdup(""));
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		++i;
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, &str[0], i + 1);
	key[i] = 0;
	value = get_env_value(env, key);
	if (!value)
		value = ft_strdup("");
	else
		value = ft_strdup(value);
	free(key);
	return (value);
}

bool	fill_arr(char ***arr, char *str, t_env *env)
{
	int			k;
	int			i;
	const int	len_str = ft_strlen(str);

	k = 0;
	i = 0;
	while (i < len_str && str[i])
	{
		if (str[i] == '$' && !ft_isalnum(str[i + 1])
			&& !special_chars(str[i + 1]))
			(*arr)[k] = ft_substr(str, i, size_element(&str[i], &i));
		else if (str[i] == '$')
		{
			(*arr)[k] = expand_one(&str[i + 1], env);
			replace_spaces((*arr)[k]);
			(void)size_element(&str[i], &i);
		}
		else if (str[i])
			(*arr)[k] = ft_substr(str, i, size_element(&str[i], &i));
		++k;
	}
	(*arr)[k] = NULL;
	return (true);
}

static char	*expand(char **str, t_env *env)
{
	char		**arr;
	const int	arr_size = count_words(*str);
	char		*tmp;
	int			i;

	arr = malloc(sizeof(char *) * (arr_size + 1));
	if (!arr || !fill_arr(&arr, *str, env))
		return (NULL);
	free(*str);
	*str = arr[0];
	i = 1;
	while (i < arr_size)
	{
		tmp = ft_strjoin(*str, arr[i]);
		free(*str);
		free(arr[i]);
		*str = tmp;
		++i;
	}
	free(arr);
	return (*str);
}

char	*expander(char *str, t_env *env)
{
	if (!str[0])
		return (str);
	str = expand(&str, env);
	return (str);
}
