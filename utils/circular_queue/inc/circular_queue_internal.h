#ifndef _H_CIRCULAR_QUEUE_INTERNAL_
#define _H_CIRCULAR_QUEUE_INTERNAL_

#include "circular_queue_export.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * CQUE Logging APIs
 */
#ifdef CQUE_LOG_ENABLE
    #define CQUE_HEX(buf, buf_len)          log_queue(buf, buf_len)
    #define CQUE_TRC(msg)                   _log_trace("[CQUE]", msg)
    #define CQUE_INF(msg)                   _log_info("[CQUE]", msg)
    #define CQUE_WRN(msg)                   _log_warning("[CQUE]", msg)
    #define CQUE_ERR(msg)                   _log_error("[CQUE]", msg)
    #define CQUE_CTCL(msg)                  _log_critical("[CQUE]", msg)
#else /* CQUE_LOG_ENABLE */
    #define CQUE_HEX(buf, buf_len)
    #define CQUE_TRC(msg)
    #define CQUE_INF(msg)
    #define CQUE_WRN(msg)
    #define CQUE_ERR(msg)
    #define CQUE_CTCL(msg)
#endif /* CQUE_LOG_ENABLE */

#define CQUE_NUM_QUEUES                    (2u)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CIRCULAR_QUEUE_INTERNAL_ */
