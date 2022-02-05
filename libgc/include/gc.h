/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 00:29:24 by asabani           #+#    #+#             */
/*   Updated: 2022/02/06 00:29:37 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H
# define GC_H
# include <stdlib.h>
# define GC_ALL	1U
# define GC_TMP	2U

typedef struct s_dustbin
{
	void				*garbage;
	struct s_dustbin	*next;
}	t_dustbin;

typedef unsigned short	t_gc_flag;
typedef struct s_gc
{
	t_dustbin	*dustbin;
	t_dustbin	*tmp_dustbin;
	struct s_gc	*(*append)(struct s_gc *, void *, t_gc_flag);
	void		(*clean)(struct s_gc **, t_gc_flag);
}	t_gc;

t_gc	*gc_init(void);
t_gc	*gc_append(t_gc *self, void *garbage, t_gc_flag flag);
void	gc_clean(t_gc **self, t_gc_flag flag);

#endif
