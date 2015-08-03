#ifndef xstring_builder_h__
#define xstring_builder_h__

#include <stddef.h>

#define XSTR_INITIAL_CAPACITY 4096
#define XSTR_EXP_FACTOR ((double) 1.618034)

struct xstring_builder {
  size_t capacity;
  size_t length;
  char *cstr;
};

typedef struct xstring_builder xstring_builder_t;

xstring_builder_t *xstring_builder_new();
void xstring_builder_free(xstring_builder_t *xsb);

xstring_builder_t *xstring_builder_append(xstring_builder_t *xsb,
                                          const char *str, size_t length);
char *xstring_builder_detach_cstring(xstring_builder_t *xsb);

#endif /* xstring_builder_h__ */