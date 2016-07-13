#ifndef _H_SOCKETS_PL_
#define _H_SOCKETS_PL_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
int socket_send(IN SOCKET_HDL,
                IN DECL_CONST UINT8*,
                IN size_t,
                IN int);
*/
#define socket_send(socket, buf, len, flags)     send((SOCKET)(socket), (const char *)(buf), (int)(len), (int)(flags))

/*
int socket_recv(IN SOCKET_HDL,
                OUT UINT8*,
                IN size_t,
                IN int);
*/
#define socket_recv(socket, buf, len, flags)     recv((SOCKET)(socket), (char *)(buf), (int)(len), (int)(flags))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_SOCKETS_PL_ */
