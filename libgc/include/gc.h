/**
 * @file gc.h
 * @author BlaanTeam (blaanteam@gmail.com)
 * @brief The garbage collector header file
 * @version 0.1
 * @date 2022-01-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef GC_H
#define GC_H
#include <stdlib.h>
#define GC_ALL	1U
#define GC_TMP	2U

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
