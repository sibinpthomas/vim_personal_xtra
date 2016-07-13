#ifndef _H_CIRCULAR_BUFFER_INTERNAL_
#define _H_CIRCULAR_BUFFER_INTERNAL_

#include "circular_buffer_export.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * CBUF Logging APIs
 */
#ifdef CBUF_LOG_ENABLE
    #define CBUF_HEX(buf, buf_len)          log_buffer(buf, buf_len)
    #define CBUF_TRC(msg)                   _log_trace("[CBUF]", msg)
    #define CBUF_INF(msg)                   _log_info("[CBUF]", msg)
    #define CBUF_WRN(msg)                   _log_warning("[CBUF]", msg)
    #define CBUF_ERR(msg)                   _log_error("[CBUF]", msg)
    #define CBUF_CTCL(msg)                  _log_critical("[CBUF]", msg)
#else /* CBUF_LOG_ENABLE */
    #define CBUF_HEX(buf, buf_len)
    #define CBUF_TRC(msg)
    #define CBUF_INF(msg)
    #define CBUF_WRN(msg)
    #define CBUF_ERR(msg)
    #define CBUF_CTCL(msg)
#endif /* CBUF_LOG_ENABLE */

#define CBUF_NUM_BUFFERS                    (2u)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CIRCULAR_BUFFER_INTERNAL_ */
