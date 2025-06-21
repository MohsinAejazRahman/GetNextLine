/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohrahma <mohrahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 19:06:52 by mohrahma          #+#    #+#             */
/*   Updated: 2024/04/04 04:19:27 by mohrahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_l(t_queue **queue)
{
	char	*line;
	int		i;
	t_n		*temp;

	i = 0;
	line = malloc((*queue)->size + 1);
	if (line == NULL)
		return (free(line), NULL);
	while ((*queue)->size > 1)
	{
		temp = (*queue)->head;
		line[i] = temp->value;
		(*queue)->head = (*queue)->head->next;
		free(temp);
		(*queue)->size--;
		i++;
	}
	line = end_line(queue, line, i);
	return (line);
}

int	create_queue(int nbytes, char *buffer, t_queue **queue)
{
	int		i;
	t_n		*newnode;

	i = 0;
	if (!(*queue))
		if (init_queue(&(*queue)) == 0)
			return (0);
	while (i < nbytes)
	{
		newnode = malloc(sizeof(t_n));
		if (!newnode)
			return (0);
		newnode->value = buffer[i];
		newnode->next = NULL;
		if ((*queue)->tail != NULL)
			(*queue)->tail->next = newnode;
		(*queue)->tail = newnode;
		if ((*queue)->head == NULL)
			(*queue)->head = newnode;
		i++;
	}
	return (1);
}

int	read_file(int fd, t_queue **queue)
{
	char	*buffer;
	int		nbytes;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (0);
	buffer[BUFFER_SIZE] = '\0';
	nbytes = 1;
	while (nbytes)
	{
		nbytes = read(fd, buffer, BUFFER_SIZE);
		if (nbytes < 0)
			return (free(buffer), 0);
		if (create_queue(nbytes, buffer, &(*queue)) == 0)
			return (free(buffer), 0);
		if (ft_strchr(buffer, '\n') == 1)
			break ;
	}
	free(buffer);
	return (1);
}

char	*get_next_line(int fd)
{
	static t_queue	*queue;
	char			*line;

	if (fd < 0 || BUFFER_SIZE < 1 || read(fd, NULL, 0) < 0)
	{
		if (queue)
			free_all(&queue);
		return (NULL);
	}
	if (read_file(fd, &queue) == 0)
		return (free_all(&queue), NULL);
	if (mem_size(&queue) == 0)
		return (free_all(&queue), NULL);
	line = get_l(&queue);
	return (line);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char	*line;
// 	int		fd;
// 	int		flag;

// 	flag = 1;
// 	fd = open("file", O_RDONLY);
// 	while (flag++ < 3)
// 	{
// 		line = get_next_line(fd);
// 		printf("line = |%s|\n", line);
// 		free (line);
// 		if (!line)
// 			break ;
// 	}
// 	flag = 1;
// 	printf("fd: %d\n", fd);
// 	close(fd);
// 	line = get_next_line(fd);
// 	printf("line asdf= |%s|\n", line);
// 	free(line);
// 	fd = open("file", O_RDONLY);
// 	printf("-----------------\n");
// 	while (flag == 1)
// 	{
// 		line = get_next_line(fd);
// 		printf("line = |%s|\n", line);
// 		free (line);
// 		if (!line)
// 			break ;
// 	}
// 	close(fd);
// 	return (1);
// }
