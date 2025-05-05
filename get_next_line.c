/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galauren <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:50:34 by galauren          #+#    #+#             */
/*   Updated: 2025/05/05 17:03:07 by galauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	there_is_a_newline(char *buffer)
{
	size_t	i;

	i = 0;
	if (!buffer)
		return (0);
	while (buffer[i])
		if (buffer[i++] == '\n')
			return (1);
	return (0);
}

static char	*append_to_buffer(char *buffer, char *tmp)
{
	char	*joined;

	joined = ft_strjoin(buffer, tmp);
	if (!joined)
		return (set_free(&buffer));
	set_free(&buffer);
	return (joined);
}

static char	*read_until_newline(char *buffer, int *read_ret, int fd)
{
	char	tmp[BUFFER_SIZE + 1];

	*read_ret = 1;
	while (*read_ret > 0 && !there_is_a_newline(buffer))
	{
		*read_ret = read(fd, tmp, BUFFER_SIZE);
		if (*read_ret < 0)
			return (set_free(&buffer));
		tmp[*read_ret] = '\0';
		buffer = append_to_buffer(buffer, tmp);
		if (!buffer)
			return (NULL);
	}
	return (buffer);
}

static char	*extract_line(char *buffer, char **rest)
{
	int		len;
	char	*line;

	len = 0;
	while (buffer[len] && buffer[len] != '\n')
		len++;
	line = ft_substr(buffer, 0, len + (buffer[len] == '\n'));
	if (!line)
		return (NULL);
	if (buffer[len] == '\n')
	{
		*rest = ft_strdup(buffer + len + 1);
		if (!*rest)
			return (set_free(&line));
	}
	else
		*rest = NULL;
	set_free(&buffer);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stock = NULL;
	char		*line;
	char		*rest;
	int			ret;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stock)
	{
		stock = malloc(1);
		if (!stock)
			return (NULL);
		stock[0] = '\0';
	}
	ret = 0;
	stock = read_until_newline(stock, &ret, fd);
	if (ret < 0 || !stock || stock[0] == '\0')
		return (set_free(&stock));
	rest = NULL;
	line = extract_line(stock, &rest);
	if (!line)
		return (set_free(&stock), set_free(&rest));
	stock = rest;
	return (line);
}
