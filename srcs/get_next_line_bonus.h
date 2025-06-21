/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohrahma <mohrahma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 23:26:35 by mohrahma          #+#    #+#             */
/*   Updated: 2024/04/04 04:18:36 by mohrahma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_n
{
	char			value;
	struct s_n		*next;
}	t_n;

typedef struct s_queue
{
	t_n	*head;
	t_n	*tail;
	int	size;
}	t_queue;

int		ft_strchr(const char *s, char c);
char	*get_l(t_queue **queue);
int		create_queue(int nbytes, char *buffer, t_queue **queue);
int		read_file(int fd, t_queue **queue);
char	*get_next_line(int fd);
int		mem_size(t_queue **queue);
char	*end_line(t_queue **queue, char *line, int i);
void	free_all(t_queue **queue);
int		init_queue(t_queue **queue);

#endif