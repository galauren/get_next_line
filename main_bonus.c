#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

static int hash_fd(int fd)
{
    fd = (fd ^ 61);
    fd = fd * 0x9E3779B1; // constante de Knuth
    return (fd & 0xFFFFFF); // 24 bits pour RGB
}

void get_fg_color(int fd, char *buf, size_t size)
{
    int hash = hash_fd(fd);
    int r = (hash >> 16) & 0xFF;
    int g = (hash >> 8) & 0xFF;
    int b = hash & 0xFF;

    snprintf(buf, size, "\e[38;2;%d;%d;%dm", r, g, b);
}

void get_bg_color(int fd, char *buf, size_t size)
{
    int	hash = hash_fd(fd);
    int	r = (hash >> 16) & 0xFF;
    int	g = (hash >> 8) & 0xFF;
    int	b = hash & 0xFF;

    snprintf(buf, size, "\e[48;2;%d;%d;%dm", r, g, b);
}

int	main(int ac, char **argv)
{
	int		fd[1024] = {0};
	char	fd_eof[1024] = {0};
	char	line_num[1024] = {0};
	char	bg_color[32];
	char	fg_color[32];
	char	*line;
	int		i = 0;
	int		fds_to_open;
	int		count_closed = 0;

	if (ac-- < 2)
	{
		printf("Usage: %s <file> (<files>...)\n", argv[0]);
		return (1);
	}
	fds_to_open = ac;
	line = NULL;
	while (count_closed < fds_to_open)
	{
		if (i >= ac || i > 1023)
			i = 0;
		if (!fd[i])
		{
			fd[i] = open(argv[i + 1], O_RDONLY);
			if (fd[i] < 0)
			{
				printf("\e[31mFD[%4d]|????????|\e[0;41mError opening file..\e[0m\n", i);
				fd_eof[i] = 1;
				++count_closed;
			}
		}
		if (!fd_eof[i])
			line = get_next_line(fd[i]);
		if (!line && !(fd[i] < 0))
		{
			if (fd_eof[i] != 1)
			{
				close(fd[i]);
				fd_eof[i] = 1;
				++count_closed;
			}
			printf("\e[31mFD[%4d]|????????|\e[0;41mTERMINATED.\e[0m\n", i);
		}
		else
		{
			get_bg_color(fd[i], bg_color, sizeof(bg_color));
			get_fg_color(fd[i], fg_color, sizeof(fg_color));
			printf("%sFD[%4d]|%8d|\e[0m%s%s\e[0m", bg_color, i, ++(line_num[i]), fg_color, line);
			free(line);
			line = NULL;
		}
		++i;
	}
	return (0);
}
