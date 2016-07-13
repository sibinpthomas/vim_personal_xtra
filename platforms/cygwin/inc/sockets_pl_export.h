#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

#ifndef _H_SOCKETS_PL_EXPORT_
#define _H_SOCKETS_PL_EXPORT_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef int                                 SOCKET_HDL;

#define INVALID_SOCKET                      ((SOCKET_HDL)(-1))
#define SOCKET_ERROR                        ((int)(-1))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_SOCKETS_PL_EXPORT_ */
