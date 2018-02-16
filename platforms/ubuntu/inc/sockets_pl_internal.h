#ifndef _H_SOCKETS_PL_INTERNAL_
#define _H_SOCKETS_PL_INTERNAL_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define pltfm_socket_startup(ERR_LOG, errval)
#define pltfm_socket_cleanup(ERR_LOG, errval)

/* int pltfm_closesocket(IN SOCKET_HDL) */
#define pltfm_closesocket(hdl)                   close((hdl))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_SOCKETS_PL_INTERNAL_ */
