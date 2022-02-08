/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 00:29:24 by asabani           #+#    #+#             */
/*   Updated: 2022/02/08 18:53:36 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H
# define GC_H

# include <stdlib.h>

# define GC_ALL				1U
# define GC_TMP				2U
# define GC_DESTROY_SELF	4U

typedef unsigned short	t_gc_flag;

typedef struct s_dustbin
{
	void				*garbage;
	struct s_dustbin	*next;
}	t_dustbin;

typedef struct s_gc
{
	t_dustbin	*dustbin;
	t_dustbin	*tmp_dustbin;
	struct s_gc	*(*append)(struct s_gc *, void *, t_gc_flag);
	void		(*clean)(struct s_gc **, t_gc_flag);
}				t_gc;

t_gc	*gc_init(void);
t_gc	*gc_append(t_gc *self, void *garbage, t_gc_flag flag);
void	gc_clean(t_gc **self, t_gc_flag flag);

#endif
