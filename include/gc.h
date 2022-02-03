/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 18:11:31 by asabani           #+#    #+#             */
/*   Updated: 2022/02/03 18:12:09 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H
# define GC_H
# include <stdlib.h>

typedef struct s_dustbin
{
	void				*garbage;
	struct s_dustbin	*next;
}	t_dustbin;

typedef struct s_gc
{
	t_dustbin	*dustbin;
	struct s_gc	*(*append)(struct s_gc *, void *);
	void		(*clean)(struct s_gc *);
}	t_gc;

t_gc	*gc_init(void);
t_gc	*gc_append(t_gc *self, void *garbage);
void	gc_clean(t_gc *self);

#endif
