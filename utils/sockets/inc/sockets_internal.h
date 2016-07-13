#ifndef _H_SOCKETS_INTERNAL_
#define _H_SOCKETS_INTERNAL_

#include "sockets_pl_internal.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * SOCKETS Logging APIs
 */
#ifdef SOCKETS_LOG_ENABLE
    #define SOCK_HEX(buf, buf_len)          log_buffer(buf, buf_len)
    #define SOCK_TRC(msg)                   _log_trace("[SOCK]", msg)
    #define SOCK_INF(msg)                   _log_info("[SOCK]", msg)
    #define SOCK_WRN(msg)                   _log_warning("[SOCK]", msg)
    #define SOCK_ERR(msg)                   _log_error("[SOCK]", msg)
    #define SOCK_CTCL(msg)                  _log_critical("[SOCK]", msg)
#else /* SOCKETS_LOG_ENABLE */
    #define SOCK_HEX(buf, buf_len)
    #define SOCK_TRC(msg)
    #define SOCK_INF(msg)
    #define SOCK_WRN(msg)
    #define SOCK_ERR(msg)
    #define SOCK_CTCL(msg)
#endif /* SOCKETS_LOG_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_SOCKETS_INTERNAL_ */
