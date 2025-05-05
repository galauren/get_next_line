#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	int		fd;
//	int		fd2;
	char	*line;
	int i = 0;

	if (argc < 2)
	{
		printf("Usage: %s <file>\n", argv[0]);
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
//	fd2 = open(argv[2], O_RDONLY);
	//if (fd2 < 0)
//	{
//		perror("Error opening file");
//		return (1);
//	}
	if (fd < 0)
	{
		perror("Error opening file");
		return (1);
	}
	line = get_next_line(fd);
//	printf("%d : | %s\n", i, line);
//	free(line);
//	line = get_next_line(fd2);
	while (line)
	{
		printf("%d : | %s\n", i, line);
		free(line);
		line = get_next_line(fd);
		++i;
	}
	close(fd);
	free(line);
	return (0);
}
