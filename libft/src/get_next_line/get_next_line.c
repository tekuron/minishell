/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danz <danz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:26:21 by danz              #+#    #+#             */
/*   Updated: 2025/11/14 01:44:49 by danz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	*freenil(void *a)
{
	free(a);
	return (NULL);
}

static char	*extract_line(char *buffer)
{
	int		i;
	char	*line;

	i = 0;
	if (!buffer)
		return (NULL);
	if (!buffer[0])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = malloc(i + (buffer[i] == '\n') + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*update_buffer(char *buffer)
{
	int		i;
	int		j;
	char	*new_buf;

	i = 0;
	j = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
		return (freenil(buffer));
	new_buf = malloc(gnl_strlen(buffer + i + 1) + 1);
	if (!new_buf)
		return (freenil(buffer));
	i++;
	while (buffer[i])
		new_buf[j++] = buffer[i++];
	new_buf[j] = '\0';
	free(buffer);
	return (new_buf);
}

static char	*read_and_join(int fd, char *buffer)
{
	char	*tmp;
	int		nbytes;

	tmp = malloc(BUFFER_SIZE + 1);
	if (!tmp)
		return (NULL);
	while (!buffer || !gnl_strchr(buffer, '\n'))
	{
		nbytes = read(fd, tmp, BUFFER_SIZE);
		if (nbytes == 0)
			break ;
		else if (nbytes < 0)
		{
			free(tmp);
			return (freenil(buffer));
		}
		tmp[nbytes] = 0;
		buffer = gnl_strjoin2(buffer, tmp);
		if (!buffer)
			return (freenil(tmp));
	}
	free(tmp);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffers[OPEN_MAX];
	char		*buffer;
	char		*line;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = buffers[fd];
	buffer = read_and_join(fd, buffer);
	if (!buffer)
	{
		buffers[fd] = NULL;
		return (NULL);
	}
	line = extract_line(buffer);
	buffer = update_buffer(buffer);
	buffers[fd] = buffer;
	return (line);
}
