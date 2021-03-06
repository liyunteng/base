/*
 * log.h - log
 *
 * Date   : 2021/01/14
 */
#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    LOG_EMERG,              /* 0 */
    LOG_PANIC = LOG_EMERG,  /* 0 */
    LOG_ALERT,              /* 1 */
    LOG_CRIT,               /* 2 */
    LOG_FATAL = LOG_CRIT,   /* 2 */
    LOG_ERR,                /* 3 */
    LOG_ERROR = LOG_ERR,    /* 3 */
    LOG_WARN,               /* 4 */
    LOG_WARNING = LOG_WARN, /* 4 */
    LOG_NOTICE,             /* 5 */
    LOG_INFO,               /* 6 */
    LOG_DEBUG,              /* 7 */
    LOG_VERBOSE,            /* 8 */
} LOG_LEVEL_E;

enum LOG_OUTTYPE {
    LOG_OUTTYPE_STDOUT,
    LOG_OUTTYPE_STDERR,
    LOG_OUTTYPE_FILE,
    LOG_OUTTYPE_MMAP,
    LOG_OUTTYPE_UDP,
    LOG_OUTTYPE_TCP,
    LOG_OUTTYPE_LOGCAT,
    LOG_OUTTYPE_SYSLOG,
    LOG_OUTTYPE_NONE,
};


typedef struct log_handler log_handler_t;
typedef struct log_format log_format_t;
typedef struct log_output log_output_t;


log_handler_t *log_handler_create(const char *ident);
void log_handler_destroy(log_handler_t *handler);
log_handler_t *log_handler_get(const char *ident);
int log_handler_set_default(log_handler_t *handler);


//%d  YYYY-MM-DD HH:MM:SS
//%d(%Y/%m/%d %H:%M:%S)  YYYY/MM/DD HH:MM:SS
//%E(LOGNAME)  environment $LOGNAME
//%ms ms
//%us us
//%H hostname
//%c ident
//%V LEVEL
//%v level
//%F __FILE__
//%U __FUNC__
//%L __LINE__
//%p pid
//%t tid
//%T tid hex
//%C color
//%R color_reset
//%n '\n'
//%r '\r'
//%% '%'
//%m user message
log_format_t *log_format_create(const char *format);
void log_format_destroy(log_format_t *format);

// LOG_OUTTYPE_STDERR
// LOG_OUTTYPE_STDOUT
// LOG_OUTTYPE_LOGCAT  need no arg
//
// LOG_OUTTYPE_SYSLOG  char *ident
//                     int options
//                     int facility
//
// LOG_OUTTYPE_FILE    char *file_path
//                     char *log_name
//                     size_t file_size
//                     int bakup_num
//
// LOG_OUTTYPE_MMAP    char *file_path
//                     char *log_name
//                     size_t file_size
//                     int bakup_num
//                     size_t map_size
//                     size_t msync_interval
//
// LOG_OUTTYPE_UDP
// LOG_OUTTYPE_TCP     char *addr
//                     int port
//
log_output_t *log_output_create(enum LOG_OUTTYPE type, ...);
void log_output_destroy(log_output_t *output);


// level_begin  -1 == LOG_VERBOSE
// level_en     -1 == LOG_EMERG
// This will print handler's log to output, use format, when loglevel between
// level_begin and level_end
int log_bind(log_handler_t *handler, LOG_LEVEL_E level_beign,
             LOG_LEVEL_E level_end, log_format_t *format, log_output_t *output);
int log_unbind(log_handler_t *handler, log_format_t *format,
               log_output_t *output);


void mlog_printf(log_handler_t *handler, LOG_LEVEL_E level, const char *file,
                 const char *function, long line, const char *format, ...);

void log_printf(LOG_LEVEL_E level, const char *file, const char *function,
                long line, const char *format, ...);

void log_cleanup(void);
void log_dump(void);


#define MLOG_PRINTF(handler, level, fmt...)                                    \
    do {                                                                       \
        mlog_printf(handler, level, __FILE__, __FUNCTION__, __LINE__, fmt);    \
    } while (0)
#define MLOGV(handler, fmt...) MLOG_PRINTF(handler, LOG_VERBOSE, fmt)
#define MLOGD(handler, fmt...) MLOG_PRINTF(handler, LOG_DEBUG, fmt)
#define MLOGI(handler, fmt...) MLOG_PRINTF(handler, LOG_INFO, fmt)
#define MLOGN(handler, fmt...) MLOG_PRINTF(handler, LOG_NOTICE, fmt)
#define MLOGW(handler, fmt...) MLOG_PRINTF(handler, LOG_WARNING, fmt)
#define MLOGE(handler, fmt...) MLOG_PRINTF(handler, LOG_ERROR, fmt)
#define MLOGF(handler, fmt...) MLOG_PRINTF(handler, LOG_FATAL, fmt)
#define MLOGA(handler, fmt...) MLOG_PRINTF(handler, LOG_ALERT, fmt)
#define MLOGP(handler, fmt...) MLOG_PRINTF(handler, LOG_PANIC, fmt)


#define LOG_PRINTF(level, fmt...)                                              \
    do {                                                                       \
        log_printf(level, __FILE__, __FUNCTION__, __LINE__, fmt);              \
    } while (0)
#define LOGV(fmt...) LOG_PRINTF(LOG_VERBOSE, fmt)
#define LOGD(fmt...) LOG_PRINTF(LOG_DEBUG, fmt)
#define LOGI(fmt...) LOG_PRINTF(LOG_INFO, fmt)
#define LOGN(fmt...) LOG_PRINTF(LOG_NOTICE, fmt)
#define LOGW(fmt...) LOG_PRINTF(LOG_WARNING, fmt)
#define LOGE(fmt...) LOG_PRINTF(LOG_ERROR, fmt)
#define LOGF(fmt...) LOG_PRINTF(LOG_FATAL, fmt)
#define LOGA(fmt...) LOG_PRINTF(LOG_ALERT, fmt)
#define LOGP(fmt...) LOG_PRINTF(LOG_PANIC, fmt)

#define LOG_INIT(ident, level)                                                 \
    do {                                                                       \
        log_format_t *__format = log_format_create("%d.%ms [%5.5V] %m%n");     \
        log_output_t *__output = log_output_create(                            \
            LOG_OUTTYPE_FILE, ".", (ident), 4 * 1024 * 1024, 4);               \
        log_handler_t *__handler = log_handler_create(ident);                  \
        if (__format && __output && __handler) {                               \
            log_bind(__handler, level, -1, __format, __output);                \
            log_handler_set_default(__handler);                                \
        }                                                                      \
    } while (0)


#ifdef __cplusplus
}
#endif

#endif
