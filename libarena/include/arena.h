/* ************************************************************************** */
/*                                                                            */
/*                       libarena — chunked bump allocator                    */
/*                                                                            */
/*  An arena is a list of chunks. arena_alloc bumps a pointer inside the      */
/*  current chunk; when full it allocates another (doubling) chunk and        */
/*  links it in. arena_reset / arena_destroy walks the chunks and frees       */
/*  the whole pile in one go.                                                 */
/*                                                                            */
/*  Individual allocations cannot be freed — that's the whole point. For a    */
/*  shell, the lifetime of one command line maps cleanly to one arena reset. */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARENA_H
# define ARENA_H

# include <stddef.h>

# define ARENA_DEFAULT_CHUNK	((size_t)(64 * 1024))
# define ARENA_ALIGN			16

typedef struct s_arena_chunk
{
	size_t					capacity;
	size_t					used;
	struct s_arena_chunk	*next;
}	t_arena_chunk;

typedef struct s_arena
{
	t_arena_chunk	*head;
}	t_arena;

/* Lifecycle. arena_init must be called on a fresh t_arena before use. */
void	arena_init(t_arena *a);
void	arena_reset(t_arena *a);
void	arena_destroy(t_arena *a);

/* Core allocator. Returns NULL on OOM. Memory is uninitialized. */
void	*arena_alloc(t_arena *a, size_t n);
/* Same but zero-initialized. */
void	*arena_calloc(t_arena *a, size_t n);

/* String helpers — all return arena-owned memory. */
char	*arena_strdup(t_arena *a, const char *s);
char	*arena_strndup(t_arena *a, const char *s, size_t n);
char	*arena_strjoin(t_arena *a, const char *s1, const char *s2);
char	*arena_chardup(t_arena *a, char c);
char	*arena_itoa(t_arena *a, int n);

/* Diagnostics — total bytes currently allocated across all chunks. */
size_t	arena_used(const t_arena *a);

#endif
