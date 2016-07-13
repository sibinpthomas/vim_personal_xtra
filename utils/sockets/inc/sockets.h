#ifndef _H_SOCKETS_
#define _H_SOCKETS_

#include "sockets_pl.h"
#include "sockets_export.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/* Initializes the pool of circular buffer data-structures. */
API_RESULT sockets_init(BOOL_T , UINT16 , UINT16 , DECL_CONST CHAR* );
/* Called when circular buffers are no longer needed. */
API_RESULT sockets_deinit(void);

/* Provide pointer to raw buffer and buffer capacity to obtain the handle to a
 * circular buffer which will utilize this raw buffer. */
API_RESULT socket_alloc(/* OUT */SOCKET_HDL* );
/* Frees a previously allocated circular buffer. */
API_RESULT socket_free(SOCKET_HDL );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_SOCKETS_ */
