#ifndef _H_SOCKETS_PL_
#define _H_SOCKETS_PL_

#include "sockets_pl_export.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int socket_send(SOCKET_HDL,
                DECL_CONST UINT8*,
                size_t,
                int);

int socket_recv(SOCKET_HDL,
                UINT8*,
                size_t,
                int);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_SOCKETS_PL_ */
