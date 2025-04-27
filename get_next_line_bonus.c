#include "get_next_line_bonus.h"
#include <limits.h> // pour OPEN_MAX

static char *read_to_buffer(char *buffer, int *read_ret, int fd, char *tmp)
{
    int     index;
    char    *joined;

    *read_ret = read(fd, tmp, BUFFER_SIZE);
    if (*read_ret <= 0)
        return (buffer);
    tmp[*read_ret] = '\0';
    index = 0;
    while (tmp[index] && tmp[index] != '\n')
        index++;
    joined = ft_strjoin(buffer, tmp);
    free(buffer);
    if (!joined)
        return (NULL);
    if (tmp[index] == '\n')
        *read_ret = 0;
    return (joined);
}

static char *clean_buffer(char *buffer, int *read_ret, int fd)
{
    char    tmp[BUFFER_SIZE + 1];

    *read_ret = 1;
    while (*read_ret > 0)
    {
        buffer = read_to_buffer(buffer, read_ret, fd, tmp);
        if (!buffer)
            break ;
    }
    return (buffer);
}

static char *extract_line(char *buffer, char **rest)
{
    int     len;
    char    *line;

    len = 0;
    while (buffer[len] && buffer[len] != '\n')
        len++;
    line = ft_substr(buffer, 0, len + (buffer[len] == '\n'));
    if (!line)
        return (NULL);
    if (buffer[len] == '\n')
        *rest = ft_strdup(buffer + len + 1);
    else
        *rest = NULL;
    free(buffer);
    return (line);
}

char *get_next_line(int fd)
{
    static char *stock[1024];
    char        *line;
    int         ret;

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
    if (!stock[fd] || ret < 0 || stock[fd][0] == '\0')
    {
        free(stock[fd]);
        stock[fd] = NULL;
        return (NULL);
    }
    line = extract_line(stock[fd], &stock[fd]);
    return (line);
}
