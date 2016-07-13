#ifndef _H_CIRCULAR_QUEUE_
#define _H_CIRCULAR_QUEUE_

#include "circular_queue_export.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef UINT16                              CQUE_HDL;
#define CQUE_NULL_HDL                       ((CQUE_HDL)0u)

typedef struct {
    UINT8* buf;
    UINT16 elem_size;  /* Size in bytes of each element of the Q. */
    UINT16 n_elem_max;  /* Maximum number of elements the Q can support. */
    UINT16 rd;  /* Read pointer. */
    UINT16 wr;  /* Write pointer. */
} CQUE;

/* Initializes the pool of circular queue data-structures. */
API_RESULT cque_init(void);
/* Called when circular queues are no longer needed. */
API_RESULT cque_deinit(void);

/* Provide pointer to raw queue and queue capacity to obtain the handle to a
 * circular queue which will utilize this raw queue. */
API_RESULT cque_alloc(void* , UINT16 , UINT16 , /* OUT */CQUE_HDL* );
/* Frees a previously allocated circular queue. */
API_RESULT cque_free(CQUE_HDL );

/* Writes are never partial, either it is all or nothing. */
API_RESULT cque_push(CQUE_HDL , DECL_CONST void* , UINT16 );
/* Reads up to requested length or queue capacity - whichever is lower. */
UINT16 cque_pop(CQUE_HDL , /* OUT */void* , UINT16 );

/* The read and write pointer is reset to 0. */
API_RESULT cque_reset(CQUE_HDL );
/* Reads up to requested length or queue capacity - whichever is lower without
 * affecting the read pointer. */
UINT16 cque_peek(CQUE_HDL , /* OUT */void* , UINT16 );

#ifdef CQUE_LOG_ENABLE
    void print_cque_data(CQUE_HDL );
#else /* CQUE_LOG_ENABLE */
    #define print_cque_data(hdl)
#endif /* CQUE_LOG_ENABLE */

/* TODO - maybe */
/* * Search for an element.
 * * Remove an element, given an index.
 * * Purge all similar elements of a certain kind.
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CIRCULAR_QUEUE_ */
