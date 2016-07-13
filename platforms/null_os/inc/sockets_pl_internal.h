#ifndef _H_SOCKETS_PL_INTERNAL_
#define _H_SOCKETS_PL_INTERNAL_

#include "sockets_pl_export.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define SHUT_RD                             1
#define SHUT_WR                             2
#define SHUT_RDWR                           3

#define pltfm_socket_startup(ERR_LOG, errval)
#define pltfm_socket_cleanup(ERR_LOG, errval)

int pltfm_closesocket(SOCKET_HDL);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_SOCKETS_PL_INTERNAL_ */
