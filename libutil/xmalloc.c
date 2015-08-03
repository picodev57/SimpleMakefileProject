#include "xmalloc.h"
#include "xlog.h"

// #include <stddef.h>
#include <stdlib.h>

void *xmalloc(size_t size) {
  void *mem;
  if (XGNU_UNLIKELY(size==0)) {
    xlog_debug("zero size alloc");
  }
  if (XGNU_UNLIKELY((mem=malloc(size))==NULL)) {
    xlog_error("unable to allocate memory - size = %zd", size);
  }
  xlog_debug("malloc mem at %p size %zd", mem, size);
  return mem;
}

void *xrealloc(void *mem, size_t size) {
  if (XGNU_UNLIKELY(size==0)) {
    xlog_debug("realloc to size 0 -> free");
  }
  if (XGNU_UNLIKELY(mem==NULL)) {
    xlog_warning("realloc NULL pointer -> malloc");
  }
  void *ptr=realloc(mem,size);
  if (XGNU_UNLIKELY(ptr==NULL)) {
    xlog_error("unable to realloc memory %p to size %zd", mem, size);
  } else {
    xlog_debug("realloc'd mem %p to %p (%zd)", mem, ptr, size);
  }
  return ptr;
}

void xfree(void *ptr) {
  if (XGNU_UNLIKELY(ptr==NULL)) {
    xlog_debug("freeing NULL ptr");
  }
  xlog_debug("freeing %p", ptr);
  free(ptr);
}