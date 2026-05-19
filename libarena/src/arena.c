/* ************************************************************************** */
/*                                                                            */
/*                       libarena — chunked bump allocator                    */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void	arena_init(t_arena *a)
{
	a->head = NULL;
}

void	arena_reset(t_arena *a)
{
	t_arena_chunk	*chunk;
	t_arena_chunk	*next;

	chunk = a->head;
	while (chunk)
	{
		next = chunk->next;
		free(chunk);
		chunk = next;
	}
	a->head = NULL;
}

void	arena_destroy(t_arena *a)
{
	arena_reset(a);
}

static size_t	align_up(size_t n)
{
	return ((n + (ARENA_ALIGN - 1)) & ~((size_t)ARENA_ALIGN - 1));
}

/*
 * Each chunk is laid out as
 *     [ t_arena_chunk header ][ padding to ARENA_ALIGN ][ payload `capacity` ]
 * so the user area starts at an ARENA_ALIGN-aligned offset *past* the
 * header. Earlier this returned `c + ARENA_ALIGN`, which was correct
 * only if the header happened to fit in ARENA_ALIGN bytes — on 64-bit
 * the header is 24 bytes and the first user write trampled the next-
 * pointer at offset 16.
 */
static size_t	header_size(void)
{
	return (align_up(sizeof(t_arena_chunk)));
}

static char	*chunk_data(t_arena_chunk *c)
{
	return ((char *)c + header_size());
}

static t_arena_chunk	*new_chunk(size_t want)
{
	t_arena_chunk	*c;
	size_t			cap;

	cap = ARENA_DEFAULT_CHUNK;
	if (want > cap)
		cap = want;
	c = malloc(header_size() + cap);
	if (!c)
		return (NULL);
	c->capacity = cap;
	c->used = 0;
	c->next = NULL;
	return (c);
}

void	*arena_alloc(t_arena *a, size_t n)
{
	t_arena_chunk	*c;
	size_t			need;
	char			*p;

	if (n == 0)
		n = 1;
	need = align_up(n);
	c = a->head;
	if (!c || c->used + need > c->capacity)
	{
		c = new_chunk(need);
		if (!c)
			return (NULL);
		c->next = a->head;
		a->head = c;
	}
	p = chunk_data(c) + c->used;
	c->used += need;
	return (p);
}

void	*arena_calloc(t_arena *a, size_t n)
{
	void	*p;

	p = arena_alloc(a, n);
	if (p)
		memset(p, 0, n);
	return (p);
}

char	*arena_strdup(t_arena *a, const char *s)
{
	size_t	len;
	char	*p;

	if (!s)
		return (NULL);
	len = strlen(s);
	p = arena_alloc(a, len + 1);
	if (!p)
		return (NULL);
	memcpy(p, s, len + 1);
	return (p);
}

char	*arena_strndup(t_arena *a, const char *s, size_t n)
{
	size_t	len;
	char	*p;

	if (!s)
		return (NULL);
	len = 0;
	while (len < n && s[len])
		len++;
	p = arena_alloc(a, len + 1);
	if (!p)
		return (NULL);
	memcpy(p, s, len);
	p[len] = '\0';
	return (p);
}

char	*arena_strjoin(t_arena *a, const char *s1, const char *s2)
{
	size_t	l1;
	size_t	l2;
	char	*p;

	if (!s1)
		s1 = "";
	if (!s2)
		s2 = "";
	l1 = strlen(s1);
	l2 = strlen(s2);
	p = arena_alloc(a, l1 + l2 + 1);
	if (!p)
		return (NULL);
	memcpy(p, s1, l1);
	memcpy(p + l1, s2, l2);
	p[l1 + l2] = '\0';
	return (p);
}

char	*arena_chardup(t_arena *a, char c)
{
	char	*p;

	p = arena_alloc(a, 2);
	if (!p)
		return (NULL);
	p[0] = c;
	p[1] = '\0';
	return (p);
}

char	*arena_itoa(t_arena *a, int n)
{
	char	buf[16];
	int		len;

	len = snprintf(buf, sizeof(buf), "%d", n);
	if (len < 0)
		return (arena_strdup(a, "0"));
	return (arena_strndup(a, buf, (size_t)len));
}

size_t	arena_used(const t_arena *a)
{
	const t_arena_chunk	*c;
	size_t				total;

	total = 0;
	c = a->head;
	while (c)
	{
		total += c->used;
		c = c->next;
	}
	return (total);
}
