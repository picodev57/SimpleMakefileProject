#include "xstring_builder.h"
#include <stddef.h>
#include <string.h>
#include "xlog.h"
#include "xmalloc.h"

static xstring_builder_t *xstring_builder_expand(xstring_builder_t *xsb);

static xstring_builder_t *xstring_builder_expand(xstring_builder_t *xsb) {
  size_t new_capacity = ((double) xsb->capacity)*XSTR_EXP_FACTOR;
  xlog_debug("string builder expand from %zd to %zd", xsb->capacity, new_capacity);
  xsb->cstr=xrealloc(xsb->cstr, new_capacity+1);
  xsb->capacity=new_capacity;
  return xsb;
}

xstring_builder_t *xstring_builder_new() {
  char *tmp_cstr=xmalloc(XSTR_INITIAL_CAPACITY+1);
  *tmp_cstr=0;
  xstring_builder_t *tmp=xmalloc(sizeof *tmp);
  tmp->cstr=tmp_cstr;
  tmp->capacity=XSTR_INITIAL_CAPACITY;
  tmp->length=0;
  return tmp;
}

void xstring_builder_free(xstring_builder_t *xsb) {
  if (xsb) xfree(xsb->cstr);
  xfree(xsb);
}

xstring_builder_t *xstring_builder_append(xstring_builder_t *xsb, const char *str, size_t length) {
  if (length==0)
    length=strlen(str);
  while (xsb->length+length>xsb->capacity+1) {
    xsb=xstring_builder_expand(xsb);
  }
  strncat(xsb->cstr,str,length);
  xsb->length+=length;
  return xsb;
}

char *xstring_builder_detach_cstring(xstring_builder_t *xsb) {
  char *old_cstr=xrealloc(xsb->cstr,xsb->length+1);
  char *new_cstr=xmalloc(XSTR_INITIAL_CAPACITY+1);
  *new_cstr=0;
  xsb->cstr=new_cstr;
  xsb->capacity=XSTR_INITIAL_CAPACITY;
  xsb->length=0;
  return old_cstr;
}