#ifndef xgnu_extensions_h__
#define xgnu_extensions_h__

#define XGNU_ALIAS(alias_name)   __attribute__((__alias__(#alias_name)))
#define XGNU_ALLOC_SIZE(x, ...)  __attribute__((__alloc_size__(x, ##__VA_ARGS__)))
#define XGNU_ALWAYS_INLINE       __attribute__((__always_inline__))
#define XGNU_FLATTEN             __attribute__((__flatten__))
#define XGNU_ERROR(message)      __attribute__((__error__(message)))
#define XGNU_WARNING(message)    __attribute__((__warning__(message)))
#define XGNU_CDECL               __attribute__((__cdecl__))
#define XGNU_CONST_FUNC          __attribute__((__const__))
#define XGNU_PURE_FUNC           __attribute__((__pure__))
#define XGNU_CONSTRUCTOR_HIGH    __attribute__((__constructor__(200)))
#define XGNU_DESTRUCTOR_HIGH     __attribute__((__destructor__(200)))
#define XGNU_CONSTRUCTOR         __attribute__((__constructor__(2000)))
#define XGNU_DESTRUCTOR          __attribute__((__destructor__(2000)))
#define XGNU_CONSTRUCTOR_LOW     __attribute__((__constructor__(20000)))
#define XGNU_DESTRUCTOR_LOW      __attribute__((__destructor__(20000)))
#define XGNU_DEPRECATED(message) __attribute__((__deprecated__(message)))
#define XGNU_DLLEXPORT           __attribute__((__dllexport__))
#define XGNU_DLLIMPORT           __attribute__((__dllimport__))
#define XGNU_RESOLVER_FUNC(func) __attribute__((__ifunc__(#func)))
#define XGNU_MALLOC              __attribute__((__malloc__))
#define XGNU_NOINLINE            __attribute__((__noinline__))
#define XGNU_NONNULL(x,...)      __attribute__((__nonnull__(x, ##__VA_ARGS__)))
#define XGNU_NORETURN            __attribute__((__noreturn__))
#define XGNU_HOT                 __attribute__((__hot__))
#define XGNU_COLD                __attribute__((__cold__))
#define XGNU_SENTINEL            __attribute__((__sentinel__))
#define XGNU_UNUSED              __attribute__((__unused__))
#define XGNU_USED                __attribute__((__used__))

#define XGNU_VIS_DEFAULT         __attribute__((__visibility__("default")))
#define XGNU_VIS_HIDDEN          __attribute__((__visibility__("hidden")))
#define XGNU_VIS_INTERNAL        __attribute__((__visibility__("internal")))
#define XGNU_VIS_PROTECTED       __attribute__((__visibility__("protected")))

#define XGNU_WARN_UNUSED_RES     __attribute__((__warn_unused_result__))

                 
#define XGNU_PRINTF( format_idx, arg_idx )    \
          __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#define XGNU_SCANF( format_idx, arg_idx )     \
          __attribute__((__format__ (__scanf__, format_idx, arg_idx)))
#define XGNU_FORMAT( arg_idx )                \
          __attribute__((__format_arg__ (arg_idx)))




#define XGNU_CLEANUP_FUNC(func)  __attribute__((__cleanup__(func)))
#define XGNU_VECTOR_SIZE(size)   __attribute__((__vector_size__(size)))

#define XGNU_TRANSPARENT_UNION   __attribute__((__transparent_union__))


#ifdef  __cplusplus
#  define BEGIN_C_DECLS  extern "C" {
#  define END_C_DECLS    }
#else
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif

#ifndef	FALSE
#  define FALSE (0)
#endif

#ifndef	TRUE
#  define TRUE  (!FALSE)
#endif

#undef	MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#undef	MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#undef	ABS
#define ABS(a)	   (((a) < 0) ? -(a) : (a))

#undef	CLAMP
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))


#define XGNU_LIKELY(expr)   (__builtin_expect( (expr), 1 ))
#define XGNU_UNLIKELY(expr) (__builtin_expect( (expr), 0 ))

#define XGNU_DO_PRAGMA(x) _Pragma (#x)
#define XGNU_TODO(x)       XGNU_DO_PRAGMA(message ("TODO - " #x))
#define XGNU_FIX(x)        XGNU_DO_PRAGMA(message ("FIX - " #x))

#endif /* xgnu_extensions_h__ */
