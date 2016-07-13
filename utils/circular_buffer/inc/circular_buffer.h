#ifndef _H_CIRCULAR_BUFFER_
#define _H_CIRCULAR_BUFFER_

#include "circular_buffer_export.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef UINT16                              CBUF_HDL;
#define CBUF_NULL_HDL                       ((CBUF_HDL)0u)

typedef struct {
    UINT8* buf;
    UINT16 size;
    UINT16 rd;
    UINT16 wr;
} CBUF;

/* Initializes the pool of circular buffer data-structures. */
API_RESULT cbuf_init(void);
/* Called when circular buffers are no longer needed. */
API_RESULT cbuf_deinit(void);

/* Provide pointer to raw buffer and buffer capacity to obtain the handle to a
 * circular buffer which will utilize this raw buffer. */
API_RESULT cbuf_alloc(UINT8* , UINT16 , /* OUT */CBUF_HDL* );
/* Frees a previously allocated circular buffer. */
API_RESULT cbuf_free(CBUF_HDL );

/* Writes are never partial, either it is all or nothing. */
API_RESULT cbuf_give(CBUF_HDL , DECL_CONST UINT8* , UINT16 );
/* Reads up to requested length or buffer capacity - whichever is lower. */
UINT16 cbuf_get(CBUF_HDL , UINT8* , UINT16 );

/* The read and write pointer is reset to 0. */
API_RESULT cbuf_reset(CBUF_HDL );
/* Reads up to requested length or buffer capacity - whichever is lower without
 * affecting the read pointer. */
UINT16 cbuf_peek(CBUF_HDL , UINT8* , UINT16 );

#ifdef CBUF_LOG_ENABLE
    void print_cbuf_data(CBUF_HDL );
#else /* CBUF_LOG_ENABLE */
    #define print_cbuf_data(hdl)
#endif /* CBUF_LOG_ENABLE */

/* TODO - maybe */
/* * Return CBUF Size, Free, Max
 * * Search for an element.
 * * Remove an element, given an index.
 * * Purge all similar elements of a certain kind.
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CIRCULAR_BUFFER_ */
