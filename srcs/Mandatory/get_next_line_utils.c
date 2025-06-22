/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohrahma <mohrahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 23:26:42 by mohrahma          #+#    #+#             */
/*   Updated: 2024/04/04 04:18:53 by mohrahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strchr(const char *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
		{
			j++;
			break ;
		}
		i++;
	}
	if ((j > 0) || (s[i] == c))
		return (1);
	else
		return (0);
}

int	mem_size(t_queue **queue)
{
	t_n	*temp;

	(*queue)->size = 0;
	temp = (*queue)->head;
	while (temp && temp->value != '\n' && temp->value != '\0')
	{
		(*queue)->size++;
		temp = temp->next;
	}
	if (!temp && (*queue)->size == 0)
		return (0);
	if (!temp && (*queue)->size > 0)
		return (1);
	if (temp->value == '\n')
		(*queue)->size++;
	return (1);
}

char	*end_line(t_queue **queue, char *line, int i)
{
	t_n	*temp;

	temp = (*queue)->head;
	line[i] = temp->value;
	(*queue)->head = (*queue)->head->next;
	if ((*queue)->head == NULL)
		(*queue)->tail = NULL;
	(*queue)->size--;
	free(temp);
	temp = NULL;
	line[i + 1] = '\0';
	return (line);
}

void	free_all(t_queue **queue)
{
	t_n	*current;
	t_n	*next;

	if ((*queue)->head == NULL && (*queue)->tail == NULL && (*queue)->size == 0)
	{
		free((*queue));
		*queue = NULL;
		return ;
	}
	current = (*queue)->head;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	(*queue)->head = NULL;
	(*queue)->tail = NULL;
	(*queue)->size = 0;
}

int	init_queue(t_queue **queue)
{
	(*queue) = malloc(sizeof(t_queue));
	if (!(*queue))
		return (0);
	(*queue)->head = NULL;
	(*queue)->tail = NULL;
	return (1);
}
