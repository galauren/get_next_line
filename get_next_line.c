#include "get_next_line.h"

static char *read_to_buffer(char *buffer, int *read_ret, int fd, char *tmp)
{
    int index;
    char *joined;

    *read_ret = read(fd, tmp, BUFFER_SIZE);
    if (*read_ret <= 0)
        return (buffer); // Rien lu, on ne touche pas

    tmp[*read_ret] = '\0';
    index = 0;
    while (tmp[index] && tmp[index] != '\n')
        index++;

    joined = ft_strjoin(buffer, tmp);
    if (!joined)
        return (NULL); // ATTENTION : pas de free(buffer) ici !
    
    free(buffer);
    if (tmp[index] == '\n')
        *read_ret = 0; // Trouvé un \n : pas besoin de relire
    return (joined);
}

static char *clean_buffer(char *buffer, int *read_ret, int fd)
{
    char tmp[BUFFER_SIZE + 1];

    *read_ret = 1;
    while (*read_ret > 0)
    {
        buffer = read_to_buffer(buffer, read_ret, fd, tmp);
        if (!buffer || *read_ret == 0)
            break;
    }
    return (buffer);
}

static char *extract_line(char *buffer, char **rest)
{
    int len;
    char *line;

    len = 0;
    while (buffer[len] && buffer[len] != '\n')
        len++;
    if (buffer[len] == '\n')
        len++; // Inclure le \n dans la line
    line = ft_substr(buffer, 0, len);
    if (!line)
        return (NULL);
    *rest = ft_strdup(buffer + len);
    free(buffer);
    return (line);
}

char *get_next_line(int fd)
{
    static char *stock;
    char *line;
    int ret;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    if (!stock)
    {
        stock = malloc(1);
        if (!stock)
            return (NULL);
        stock[0] = '\0';
    }
    stock = clean_buffer(stock, &ret, fd);
    if (!stock || ret < 0 || !*stock)
    {
        free(stock);
        stock = NULL;
        return (NULL);
    }
    line = extract_line(stock, &stock);
    return (line);
}
