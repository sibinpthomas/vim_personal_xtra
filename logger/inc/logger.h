#ifndef _H_LOGGER_
#define _H_LOGGER_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define LOGGER
#ifndef LOGGER_LEVEL
    #define LOGGER_LEVEL                        0    
#endif /* LOGGER_LEVEL */

#include "logger_internal.h"


#ifdef LOGGER
    #define HEX_DUMP_PREFIX "\n[xx HEX xx]: "

    API_RESULT init_logger(void);
    void shutdown_logger(void);
    
    #if defined(LOGGER_LEVEL) && (LOGGER_LEVEL >= 0) && (LOGGER_LEVEL <= 5)

        #if LOGGER_LEVEL <= 5
            #define log_critical(msg) fprintf (log_f_hdl, "\n[!! CTCL !!]:[%s]:[%d]: ", __FILE__, __LINE__); \
                                     _log msg;
            #define _log_critical(stamp, msg) fprintf (log_f_hdl, "\n[!! CTCL !!]:[%s]:[%d]:" stamp " ", __FILE__, __LINE__); \
                                             _log msg;
        #endif /* LOGGER_LEVEL > 4 */

        #if LOGGER_LEVEL <= 4
            #define log_error(msg) fprintf (log_f_hdl, "\n[** ERR **]:[%s]:[%d]: ", __FILE__, __LINE__); \
                                     _log msg;
            #define _log_error(stamp, msg) fprintf (log_f_hdl, "\n[** ERR **]:[%s]:[%d]:" stamp " ", __FILE__, __LINE__); \
                                             _log msg;
        #endif /* LOGGER_LEVEL > 3 */

        #if LOGGER_LEVEL <= 3
            #define log_warning(msg) fprintf (log_f_hdl, "\n[~~ WARN ~~]:[%s]:[%d]: ", __FILE__, __LINE__); \
                                     _log msg;
            #define _log_warning(stamp, msg) fprintf (log_f_hdl, "\n[~~ WARN ~~]:[%s]:[%d]:" stamp " ", __FILE__, __LINE__); \
                                             _log msg;
        #endif /* LOGGER_LEVEL > 2 */

        #if LOGGER_LEVEL <= 2
            #define log_info(msg)  fprintf (log_f_hdl, "\n[== INF ==]:[%s]:[%d]: ", __FILE__, __LINE__); \
                                     _log msg;
            #define _log_info(stamp, msg)  fprintf (log_f_hdl, "\n[== INF ==]:[%s]:[%d]:" stamp " ", __FILE__, __LINE__); \
                                             _log msg;
        #endif /* LOGGER_LEVEL > 1 */

        #if LOGGER_LEVEL <= 1
            #define log_trace(msg) fprintf (log_f_hdl, "\n[-- TRC --]:[%s]:[%d]: ", __FILE__, __LINE__); \
                                     _log msg;
            #define _log_trace(stamp, msg) fprintf (log_f_hdl, "\n[-- TRC --]:[%s]:[%d]:" stamp " ", __FILE__, __LINE__); \
                                             _log msg;

            #define log_simple(msg) _log msg;
            #define log_buffer(buf, buf_len) _hex_dump((buf), (buf_len));
        #endif /* LOGGER_LEVEL > 0 */

    #else /* defined(LOGGER_LEVEL) && (LOGGER_LEVEL >= 0) && (LOGGER_LEVEL <= 5) */
        #if defined(LOGGER_LEVEL) && ((LOGGER_LEVEL < 0) || (LOGGER_LEVEL > 5))
            #error "LOGGER_LEVEL should be a value in the range [0, 5]."
        #endif /* defined(LOGGER_LEVEL) && ((LOGGER_LEVEL < 0) || (LOGGER_LEVEL > 5)) */
    #endif /* defined(LOGGER_LEVEL) && (LOGGER_LEVEL >= 0) && (LOGGER_LEVEL <= 5) */
    
#else /* LOGGER */
    /* Always call init_logger() as part of a conditional statement otherwise
     * you will get a "statement with no effect" warning/error. */
    #define init_logger()                       API_SUCCESS
    #define shutdown_logger()

#endif /* LOGGER */

#ifndef log_simple
    #define log_simple(msg)
#endif /* log_buffer */

#ifndef log_buffer
    #define log_buffer(buf, buf_len)
#endif /* log_buffer */

#ifndef log_trace
    #define log_trace(msg)
    #define _log_trace(stamp, msg)
#endif /* log_trace */

#ifndef log_info
    #define log_info(msg)
    #define _log_info(stamp, msg)
#endif /* log_info */

#ifndef log_warning
    #define log_warning(msg)
    #define _log_warning(stamp, msg)
#endif /* log_warning */

#ifndef log_error
    #define log_error(msg)
    #define _log_error(stamp, msg)
#endif /* log_error */

#ifndef log_critical
    #define log_critical(msg)
    #define _log_critical(stamp, msg)
#endif /* log_critical */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_LOGGER_ */
