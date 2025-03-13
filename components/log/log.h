/*
 * Copyright (c) 2006-2024 LGT Development Team
 *
 * Change Logs:
 * Date         Author      Notes
 * 2025-01-06   Evlers      first implementation
 */

#ifndef _LOG_H_
#define _LOG_H_

#include "compiler.h"
#include "kservice.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_NONE,       /*!< No log output */
    LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO,       /*!< Information messages which describe normal flow of events */
    LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} log_level_t;

#ifndef CONFIG_LOG_COLORS
#define CONFIG_LOG_COLORS 0
#endif
#ifndef PRE_FORMAT_NEWLINE_CHAR
#define PRE_FORMAT_NEWLINE_CHAR ""
#endif
#ifndef POST_FORMAT_NEWLINE_CHAR
#define POST_FORMAT_NEWLINE_CHAR "\n"
#endif
#ifndef CONFIG_LOG_MAXIMUM_LEVEL
#define CONFIG_LOG_MAXIMUM_LEVEL LOG_INFO
#endif

#if CONFIG_LOG_COLORS
#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D
#define LOG_COLOR_V
#else
#define LOG_COLOR_E
#define LOG_COLOR_W
#define LOG_COLOR_I
#define LOG_COLOR_D
#define LOG_COLOR_V
#define LOG_RESET_COLOR
#endif /* CONFIG_LOG_COLORS */

#define LOG_LOCAL_LEVEL  CONFIG_LOG_MAXIMUM_LEVEL

#define DEFINE_LOG_TAG(sTr) static const char TAG[] = #sTr

void log_write(int level, const char *tag, const char *format, ...);
void log_buffer_hexdump_internal(const char *tag, const void *buffer, uint16_t buff_len, log_level_t log_level);

#if CONFIG_LOG_COLORS
#define LOG_FORMAT(letter, format)   LOG_COLOR_ ## letter  PRE_FORMAT_NEWLINE_CHAR #letter " %s: " format LOG_RESET_COLOR POST_FORMAT_NEWLINE_CHAR
#else
#define LOG_FORMAT(letter, format) PRE_FORMAT_NEWLINE_CHAR #letter " %s: " format POST_FORMAT_NEWLINE_CHAR
#endif

#if defined(__cplusplus) && (__cplusplus >  201703L)
#define LOG_LEVEL(level, tag, format, ...) do {                     \
        if (level==LOG_ERROR )          { log_write(LOG_ERROR,      tag, LOG_FORMAT(E, format), tag __VA_OPT__(,) __VA_ARGS__); } \
        else if (level==LOG_WARN )      { log_write(LOG_WARN,       tag, LOG_FORMAT(W, format), tag __VA_OPT__(,) __VA_ARGS__); } \
        else if (level==LOG_DEBUG )     { log_write(LOG_DEBUG,      tag, LOG_FORMAT(D, format), tag __VA_OPT__(,) __VA_ARGS__); } \
        else if (level==LOG_VERBOSE )   { log_write(LOG_VERBOSE,    tag, LOG_FORMAT(V, format), tag __VA_OPT__(,) __VA_ARGS__); } \
        else                            { log_write(LOG_INFO,       tag, LOG_FORMAT(I, format), tag __VA_OPT__(,) __VA_ARGS__); } \
    } while(0)
#else
#define LOG_LEVEL(level, tag, format, ...) do {                     \
        if (level==LOG_ERROR )          { log_write(LOG_ERROR,      tag, LOG_FORMAT(E, format), tag, ##__VA_ARGS__); } \
        else if (level==LOG_WARN )      { log_write(LOG_WARN,       tag, LOG_FORMAT(W, format), tag, ##__VA_ARGS__); } \
        else if (level==LOG_DEBUG )     { log_write(LOG_DEBUG,      tag, LOG_FORMAT(D, format), tag, ##__VA_ARGS__); } \
        else if (level==LOG_VERBOSE )   { log_write(LOG_VERBOSE,    tag, LOG_FORMAT(V, format), tag, ##__VA_ARGS__); } \
        else                            { log_write(LOG_INFO,       tag, LOG_FORMAT(I, format), tag, ##__VA_ARGS__); } \
    } while(0)
#endif

#if defined(__cplusplus) && (__cplusplus >  201703L)
#define LOG_LEVEL_LOCAL(level, tag, format, ...) do {               \
        if ( LOG_LOCAL_LEVEL >= level ) LOG_LEVEL(level, tag, format __VA_OPT__(,) __VA_ARGS__); \
    } while(0)
#else
#define LOG_LEVEL_LOCAL(level, tag, format, ...) do {               \
        if ( LOG_LOCAL_LEVEL >= level ) LOG_LEVEL(level, tag, format, ##__VA_ARGS__); \
    } while(0)
#endif

#if defined(__cplusplus) && (__cplusplus >  201703L)
#define LOGE( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_ERROR,   tag, format __VA_OPT__(,) __VA_ARGS__)
#define LOGW( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_WARN,    tag, format __VA_OPT__(,) __VA_ARGS__)
#define LOGI( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_INFO,    tag, format __VA_OPT__(,) __VA_ARGS__)
#define LOGD( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_DEBUG,   tag, format __VA_OPT__(,) __VA_ARGS__)
#define LOGV( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_VERBOSE, tag, format __VA_OPT__(,) __VA_ARGS__)
#else // !(defined(__cplusplus) && (__cplusplus >  201703L))
#define LOGE( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_ERROR,   tag, format, ##__VA_ARGS__)
#define LOGW( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_WARN,    tag, format, ##__VA_ARGS__)
#define LOGI( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_INFO,    tag, format, ##__VA_ARGS__)
#define LOGD( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_DEBUG,   tag, format, ##__VA_ARGS__)
#define LOGV( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_VERBOSE, tag, format, ##__VA_ARGS__)
#endif // !(defined(__cplusplus) && (__cplusplus >  201703L))

#define LOG_BUFFER_HEXDUMP( tag, buffer, buff_len, level ) \
    do {\
        if ( LOG_LOCAL_LEVEL >= (level) ) { \
            log_buffer_hexdump_internal( tag, buffer, buff_len, level ); \
        } \
    } while(0)

#define HEXLOGE(tag, buff, len) LOG_BUFFER_HEXDUMP(tag, buff, len, LOG_ERROR)
#define HEXLOGW(tag, buff, len) LOG_BUFFER_HEXDUMP(tag, buff, len, LOG_WARN)
#define HEXLOGI(tag, buff, len) LOG_BUFFER_HEXDUMP(tag, buff, len, LOG_INFO)
#define HEXLOGD(tag, buff, len) LOG_BUFFER_HEXDUMP(tag, buff, len, LOG_DEBUG)
#define HEXLOGV(tag, buff, len) LOG_BUFFER_HEXDUMP(tag, buff, len, LOG_VERBOSE)

#ifdef __cplusplus
}
#endif

#endif /* _LOG_H_ */
