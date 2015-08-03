#ifndef xmalloc_h__
#define xmalloc_h__

#include <stddef.h>

void *xmalloc(size_t);
void *xrealloc(void *, size_t);
void xfree(void *);

#endif /* xmalloc_h__ */