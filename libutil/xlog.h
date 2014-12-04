/*
 * Simple log module
 *
 * stderr is used as output
 * 4 log levels : debug, info, warning and error
 * 4 macros defined (one per log level), usage à la printf
 *   e.g.: xlog_debug("created %d objects", object_cout);
 *         xlog_info("Download speed %4.1lf %s", transfert_speed, transfert_units);
 *         xlog_warning("Hash table 80% full !");
 *         xlog_error("Unable to allocate memory\n->%s", streeror(errno));
 * Debug, info and warning macros are no-op if XLOG is not defined
 * Error macro always aborts
 *
 * log record format :
 *
 * Rec ID|L|domain|file|line|function|cpu|elapsed|PID|threadid|message
 *
 * Rec ID   : unsigned int starting at 0, incremented each time a record is
 *            created. Incrementation is atomic (no need to mutex the acces).
 *            If the program is forked, each copy of the process has its own
 *            record id incremented independently.
 * L        : char (log level)
 *            one of D (debug), I (info), W (warning), E (error)
 *            Debug level message are emitted only if DEBUG macro is defined
 *            Error level message causes program abort
 * domain   : value of the __DOMAIN__ macro at compilation time
 * file     : file name where the call occured
 * line     : position of the call
 * function : caller function
 * cpu      : cpu time used in seconds
 * elapsed  : time elapsed from the start of the program in seconds
 * PID      : PID from the calling process
 * threadid : Thread ID from the calling thread
 *            always 0 if THREAD macro is undefined
 * message  : Free message
 *
 **************************
 * NEVER directly use xlog_log or xlog_level_t in your code as they are not defined
 * if XLOG is not.
 */


#ifndef __xlog_h__
#define __xlog_h__

#ifdef XLOG

#include "xgnu_extensions.h"
#include <stdarg.h>

typedef enum { XLOG_LEVEL_DEBUG, XLOG_LEVEL_INFO, XLOG_LEVEL_WARNING, XLOG_LEVEL_ERROR } xlog_level_t;

void xlog_log (const char*, const char*, int, const char*, xlog_level_t, const char*, ...)     XGNU_PRINTF(6,7) XGNU_NONNULL(1,2,4,6);
void xlog_logv(const char*, const char*, int, const char*, xlog_level_t, const char*, va_list)                  XGNU_NONNULL(1,2,4,6);

#ifndef __DOMAIN__
#  define __DOMAIN__ ""
#endif

#ifndef __FILE_BASENAME__
#  define __FILE_BASENAME__ __FILE__
#endif

#ifdef DEBUG
#define xlog_debug(format, ...)   xlog_log( __DOMAIN__, __FILE_BASENAME__, __LINE__, __func__, XLOG_LEVEL_DEBUG,   format, ##__VA_ARGS__ )
#else /* !defined(DEBUG) */
#define xlog_debug(format, ...)
#endif /* defined(DEBUG) */

#define xlog_info(format, ...)    xlog_log( __DOMAIN__, __FILE_BASENAME__, __LINE__, __func__, XLOG_LEVEL_INFO,    format, ##__VA_ARGS__ )
#define xlog_warning(format, ...) xlog_log( __DOMAIN__, __FILE_BASENAME__, __LINE__, __func__, XLOG_LEVEL_WARNING, format, ##__VA_ARGS__ )
#define xlog_error(format, ...)   xlog_log( __DOMAIN__, __FILE_BASENAME__, __LINE__, __func__, XLOG_LEVEL_ERROR,   format, ##__VA_ARGS__ )

#else /* !defined(XLOG) */

#include <stdio.h>
#include <stdlib.h>

#define xlog_debug(format, ...)
#define xlog_info(format, ...)
#define xlog_warning(format, ...)
#define xlog_error(format, ...)   do {                                                             \
                                    fprintf(stderr,"%s(%s):%d -> ", __FILE__, __func__, __LINE__); \
                                    fprintf(stderr,format, ##__VA_ARGS__);                         \
				    fprintf(stderr,"\naborting\n");                                \
				    exit(EXIT_FAILURE);                                            \
                                  } while(0)

#endif /* XLOG */

#endif /* __xlog_h__ */
