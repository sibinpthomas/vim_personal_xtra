#ifndef _H_LOGGER_
#define _H_LOGGER_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "logger_internal.h"


#ifdef LOGGER
    #define HEX_DUMP_PREFIX "\n[xx HEX xx]: "

    API_RESULT init_logger(void);
    void shutdown_logger(void);
    
    #if defined(LOGGER_LEVEL) && (LOGGER_LEVEL >= 0) && (LOGGER_LEVEL <= 6)

        #if LOGGER_LEVEL <= 6
        /*
         * This level of logging is to be used for short-term testing only, 
         * usually the unit-testing phase.
         * Ideally these logging APIs should not be found in checked-in code.
         */
            #define log_temp(msg) fprintf (log_f_hdl, "\n[!! TEMP !!]:[%s]:[%d]: ", __FILE__, __LINE__); \
                                  _log msg;
            #define _log_temp(stamp, msg) fprintf (log_f_hdl, "\n[!! TEMP !!]:[%s]:[%d]:" stamp " ", __FILE__, __LINE__); \
                                          _log msg;

            #define log_simple_temp(msg) _log msg;
            #define log_buffer_temp(buf, buf_len) _hex_dump((buf), (buf_len));
        #endif /* LOGGER_LEVEL > 5 */

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

    #else /* defined(LOGGER_LEVEL) && (LOGGER_LEVEL >= 0) && (LOGGER_LEVEL <= 6) */
        #if defined(LOGGER_LEVEL) && ((LOGGER_LEVEL < 0) || (LOGGER_LEVEL > 6))
            #error "LOGGER_LEVEL should be a value in the range [0, 6]."
        #endif /* defined(LOGGER_LEVEL) && ((LOGGER_LEVEL < 0) || (LOGGER_LEVEL > 6)) */
    #endif /* defined(LOGGER_LEVEL) && (LOGGER_LEVEL >= 0) && (LOGGER_LEVEL <= 6) */
    
#else /* LOGGER */
    /* Always call init_logger() as part of a conditional statement otherwise
     * you will get a "statement with no effect" warning/error. */
    #define init_logger()                       API_SUCCESS
    #define shutdown_logger()

#endif /* LOGGER */

#ifndef log_simple
    #define log_simple(msg)
#endif /* log_simple */

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

#ifndef log_temp
    #define log_temp(msg)
    #define _log_temp(stamp, msg)
#endif /* log_temp */

#ifndef log_simple_temp
    #define log_simple_temp(msg)
#endif /* log_simple_temp */

#ifndef log_buffer_temp
    #define log_buffer_temp(buf, buf_len)
#endif /* log_buffer_temp */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_LOGGER_ */
