/*
 * Simple logging module
 *
 * No caveat here, code is self explanatory.
 *
 */

#ifdef XLOG

#include "xgnu_extensions.h"
#include "xlog.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#ifdef PTHREAD
#  include <pthread.h>
#  define xlog_get_thread_id pthread_self()
#else
#  define xlog_get_thread_id 0
#endif

static const int xlog_fd=STDERR_FILENO;        // All output to stderr
static const char xlog_lvl_char[]="DIWE";      // look up table
static struct timeval xlog_start_tv;           // keep the start time (set by xlog_init)
static volatile unsigned int xlog_record_id=0; // accessed atomically through builtins
                                               // to avoid mutex use
static pid_t xlog_starter_process;             // starter process id needed if the main
                                               // program forks

/*
 * XLOG_HEADER, XLOG_SEP and XLOG_FORMAT must be kept in sync
 * HEADER describes the record
 * SEP displays a separation
 * FORMAT is a printf format string to create the record log header, the last %s will be used
 *        to insert the log message printf format 
 */
static const char XLOG_HEADER[] = "Rec Id|L|       domain       |             file             |line|           function           |   cpu   | elapsed |   PID    |  threadid  | message  -->\n";
static const char XLOG_SEP[]    = "------|-|--------------------|------------------------------|----|------------------------------|---------|---------|----------|------------|------------>\n";
static const char XLOG_FORMAT[] = "%6u|%c|%-20s|%-30s|%4d|%30s|%9.3f|%9.3f|%10d|%12lX|%s\n";

/*
 * xlog_init is the module constructor = is called before main starts
 * xlog_fini is the module destructor  = is called after main returns
 * GNU attribute extension are used here
 */
static void xlog_init(void) XGNU_CONSTRUCTOR_HIGH;
static void xlog_fini(void) XGNU_DESTRUCTOR_HIGH;
/*
 * display XLOG_HEADER and XLOG_SEP
 */
static inline void xlog_write_headers(void);

/*
 * xlog_write_start_info (resp. xlog_write_stop_info) displays the date and
 * time during xlog_init (resp. xlog_fini) if DEBUG is defined
 */
static inline void xlog_write_start_info(void);
static inline void xlog_write_stop_info(void);

/*
 * returns the printf format like string containing the record log header 
 * described by XLOG_HEADER with the final %s replaced by the format
 * given to the calling macro
 */
static inline char* xlog_create_log_record_header( xlog_level_t level,
						   const char* domain,
						   const char* file, int line, const char* func,
						   float cpu_clock, float elapsed_time,
						   pid_t PID,unsigned long threadid,
						   const char* message_format) XGNU_MALLOC XGNU_FORMAT(10) XGNU_WARN_UNUSED_RES XGNU_NONNULL(2,3,5,10);
/*
 * outputs to stderr the record log
 */
static inline void xlog_write_log_record(const char* log_record_header, va_list arg_list) XGNU_NONNULL(1);

/*
 * 
 *
 */

static void xlog_init(void)
{
  xlog_write_headers();
  xlog_starter_process=getpid();
  gettimeofday(&xlog_start_tv,NULL);
#if DEBUG
  xlog_write_start_info();
#endif
}

static void xlog_fini(void)
{
  if (xlog_starter_process==getpid()) {
#if DEBUG
    xlog_write_stop_info();
#endif
  }
}

static inline void xlog_write_headers(void)
{
  write(xlog_fd,XLOG_HEADER, sizeof(XLOG_HEADER)/sizeof(XLOG_HEADER[0])-1);
  write(xlog_fd,XLOG_SEP, sizeof(XLOG_SEP)/sizeof(XLOG_SEP[0])-1);  
}

XGNU_TODO(merge xlog_write_start_info and xlog_write_stop_info)
static inline void xlog_write_start_info(void)
{
  /*
   * time format "%F %T" expands to "YYYY-MM-DD hh:mm:ss" : always 20 chars
   * including the terminal \0 
   */
  const char start_time_format[]="%F %T";
  char start_time[20]={0};
  time_t rawtime;
  struct tm* timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(start_time,sizeof(start_time),start_time_format,timeinfo);
  xlog_debug("stopwatch started at %s", start_time);
}

static inline void xlog_write_stop_info(void)
{
  /*
   * time format "%F %T" expands to "YYYY-MM-DD hh:mm:ss" : always 20 chars
   * including the terminal \0 
   */
  const char start_time_format[]="%F %T";
  char start_time[20]={0};
  time_t rawtime;
  struct tm* timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(start_time,sizeof(start_time),start_time_format,timeinfo);
  xlog_debug("stopwatch stoped at %s", start_time);
}

static inline char* xlog_create_log_record_header( xlog_level_t level,
						   const char* domain,
						   const char* file, int line, const char* func,
						   float cpu_clock, float elapsed_time,
						   pid_t PID, unsigned long threadid,
						   const char* message_format)
{
  char* record_header;
  int len;
  unsigned int record_id;

  record_id=__sync_fetch_and_add(&xlog_record_id,1);
  len=asprintf(&record_header, XLOG_FORMAT,
	       record_id,
	       xlog_lvl_char[level],
	       domain,
	       file, line, func,
	       cpu_clock, elapsed_time,
	       PID, threadid,
	       message_format);
  if (len==-1) {
    perror("unable to create log record header");
    exit(EXIT_FAILURE);
  }
  return record_header;
}

static inline void xlog_write_log_record(const char* log_record_header, va_list arg_list)
{
  int len,written,offset;
  char* log_record;

  len=vasprintf(&log_record,log_record_header,arg_list);
  if (len>0) {
    offset=0;
    /* long messages can be truncated and not written with one call to write
     * 
     */
    while ( (written=write(xlog_fd,log_record+offset,len))<len) {
      if (written<1) {
	perror("unable to write to stderr");
	exit(EXIT_FAILURE);
      } else {
	offset=offset+written+1;
	len=len-written;
      }
    }
    free(log_record);
  } else {
    perror("Unable to create log record");
    exit(EXIT_FAILURE);
  }
}

void xlog_log(const char* domain, const char* file, int line , const char* func, xlog_level_t level, const char* format, ...)
{
  va_list arg_list;
  va_start(arg_list,format);
  xlog_logv(domain, file, line, func, level, format, arg_list);
  va_end(arg_list);
}

void xlog_logv(const char* domain, const char* file, int line , const char* func, xlog_level_t level, const char* format, va_list arg_list)
{
  char *log_record_header;
  struct timeval now;
  float cpu_clock;
  float elapsed_time;

  // Create the record log header
  cpu_clock = ((float)clock())/(float)CLOCKS_PER_SEC;
  gettimeofday(&now,NULL);
  elapsed_time = (float)(now.tv_sec-xlog_start_tv.tv_sec) + (float)(now.tv_usec-xlog_start_tv.tv_usec)/1000000.0;

  log_record_header=xlog_create_log_record_header(level, 
						  domain, 
						  file, line, func, 
						  cpu_clock, elapsed_time,
						  getpid(),xlog_get_thread_id,
						  format);

  // Append the logging message 
  xlog_write_log_record(log_record_header, arg_list);
  free(log_record_header);

  // Abort if it's an error logging message
  if (level==XLOG_LEVEL_ERROR) {
    exit(EXIT_FAILURE);
  }
  
}

#endif /* defined(XLOG) */
