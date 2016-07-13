#ifndef _H_SOCKETS_PL_INTERNAL_
#define _H_SOCKETS_PL_INTERNAL_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define SHUT_RD                             SD_RECEIVE
#define SHUT_WR                             SD_SEND
#define SHUT_RDWR                           SD_BOTH

#define pltfm_socket_startup(ERR_LOG, errval)\
    {\
        WSADATA wsaData;\
        if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)\
        {\
            ERR_LOG(("WSAStartup() failed.\n"));\
            return errval;\
        }\
    }
#define pltfm_socket_cleanup(ERR_LOG, errval)\
    if (WSACleanup() != 0)\
    {\
        ERR_LOG(("WSACleanup() failed.\n"));\
        return errval;\
    }

/* int pltfm_closesocket(IN SOCKET_HDL) */
#define pltfm_closesocket(hdl)                   closesocket((hdl))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_SOCKETS_PL_INTERNAL_ */
