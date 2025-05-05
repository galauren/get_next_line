/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galauren <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:49:33 by galauren          #+#    #+#             */
/*   Updated: 2025/05/05 17:01:08 by galauren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*set_free(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	return (NULL);
}

static char	*read_to_buffer(char *buffer, int *read_ret, int fd, char *tmp)
{
	int		i;
	char	*joined;

	*read_ret = read(fd, tmp, BUFFER_SIZE);
	if (*read_ret < 0)
		return (set_free(&buffer));
	if (*read_ret == 0)
		return (buffer);
	tmp[*read_ret] = '\0';
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\n')
			*read_ret = 0;
		i++;
	}
	joined = ft_strjoin(buffer, tmp);
	if (!joined)
		return (set_free(&buffer));
	set_free(&buffer);
	return (joined);
}

static char	*clean_buffer(char *buffer, int *read_ret, int fd)
{
	char	tmp[BUFFER_SIZE + 1];

	*read_ret = 1;
	while (*read_ret > 0)
	{
		buffer = read_to_buffer(buffer, read_ret, fd, tmp);
		if (!buffer)
			return (NULL);
		if (*read_ret == 0)
			break ;
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
		return (set_free(&buffer));
	if (buffer[len] == '\n')
		*rest = ft_strdup(buffer + len + 1);
	else
		*rest = NULL;
	set_free(&buffer);
	return (line);
}

// Fonction principale bonus qui lit une ligne depuis un fichier
char	*get_next_line(int fd)
{
	static char	*stock[1024];
	char		*rest;
	char		*line;
	int			ret;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)
		return (NULL);
	if (!stock[fd])
	{
		stock[fd] = malloc(1);
		if (!stock[fd])
			return (NULL);
		stock[fd][0] = '\0';
	}
	ret = 0;
	stock[fd] = clean_buffer(stock[fd], &ret, fd);
	if (ret < 0 || !stock[fd] || stock[fd][0] == '\0')
		return (set_free(&stock[fd]));
	rest = NULL;
	line = extract_line(stock[fd], &rest);
	stock[fd] = rest;
	return (line);
}
