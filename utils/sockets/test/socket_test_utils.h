#ifndef _H_SOCKET_TEST_UTILS_
#define _H_SOCKET_TEST_UTILS_

#include "sockets.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifdef SOCKETS_LOG_ENABLE
    void log_socket_details(SOCKET_HDL hdl, BOOL_T is_server);

    void log_socket_error(void);
#else
    #define log_socket_details(hdl, is_server)

    #define log_socket_error()
#endif /* SOCKETS_LOG_ENABLE*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_SOCKET_TEST_UTILS_ */
