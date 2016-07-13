#ifndef _H_SOCKETS_EXPORT_
#define _H_SOCKETS_EXPORT_

#include "error_codes.h"
#include "sockets_pl_export.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define SOCKETS_INVALID_FUNC_PARAMS         ((API_RESULT)(0x0001|SOCKETS_ID))
#define SOCKETS_INIT_FAILED                 ((API_RESULT)(0x0002|SOCKETS_ID))
#define SOCKETS_DEINIT_FAILED               ((API_RESULT)(0x0003|SOCKETS_ID))
#define SOCKETS_CREATE_FAILED               ((API_RESULT)(0x0004|SOCKETS_ID))
#define SOCKETS_LISTEN_FAILED               ((API_RESULT)(0x0005|SOCKETS_ID))
#define SOCKETS_ACCEPT_FAILED               ((API_RESULT)(0x0006|SOCKETS_ID))
#define SOCKETS_BIND_FAILED                 ((API_RESULT)(0x0007|SOCKETS_ID))
#define SOCKETS_CONNECT_FAILED              ((API_RESULT)(0x0008|SOCKETS_ID))
#define SOCKETS_INVALID_FLAGS               ((API_RESULT)(0x0009|SOCKETS_ID))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_SOCKETS_EXPORT_ */
