#ifndef _H_CIRCULAR_BUFFER_MT_
#define _H_CIRCULAR_BUFFER_MT_

#include "circular_buffer_mt_export.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Initializes the pool of circular buffer data-structures. */
API_RESULT cbuf_init(void);
/* Called when circular buffers are no longer needed. */
API_RESULT cbuf_deinit(void);

/* Provide pointer to raw buffer and buffer capacity to obtain the handle to a
 * circular buffer which will utilize this raw buffer. */
API_RESULT cbuf_alloc(UINT8* , UINT16 , /* OUT */CBUF_HDL* );
/* Frees a previously allocated circular buffer. */
API_RESULT cbuf_free(CBUF_HDL );

/* Writes to CBUF.
 * Params:
 *      buf_size - (Input) Length of the provided buffer.
 *                 (Output) Number of bytes that *were* copied from the provided buffer.
 * Flags:
 *      Default (0) - Non-Blocking write, full buffer or nothing.
 *                    (could be 0)
 *      Bit 0       - Non-Blocking (CLR)/ Wait (SET)
 *                    CBUF_NBLK (0) / CBUF_WAIT (1)
 *
 *      Bit 1       - Any (CLR)/ Exact (SET)
 *                    CBUF_ANY (0) / CBUF_XACT (2)
 *                    Any: 
 *                      * Writes at least 1 byte (up to provided buffer 
 *                        size) in blocking mode.
 *                      * In non-blocking mode writes as much data as 
 *                        possible - even if it's zero bytes.
 *                    Exact: 
 *                      * Writes exactly the provided buffer size number 
 *                        of bytes in blocking mode.
 *                      * In non-blocking mode returns error if 
 *                        provided buffer size number of bytes can not
 *                        be accomodated in the buffer.
 *      Bit 2:7     - xxxxxx (Don't care)
 */
API_RESULT cbuf_give(CBUF_HDL , DECL_CONST UINT8* , UINT16* , CBUF_FLAGS );

/* Reads from CBUF.
 * Params:
 *      buf_size - (Input) Number of bytes that can be copied in to the provided buffer.
 *                 (Output) Number of bytes that *were* copied in to the provided buffer.
 * Flags:
 *      Default (0) - Non-Blocking read, up to requested length or CBUF data length
 *                    (could be 0)
 *      Bit 0       - Non-Blocking (CLR)/ Wait (SET)
 *                    CBUF_NBLK (0) / CBUF_WAIT (1)
 *
 *      Bit 1       - Any (CLR)/ Exact (SET)
 *                    CBUF_ANY (0) / CBUF_XACT (2)
 *                    Any: 
 *                      * Fetches at least 1 byte (up to requested 
 *                        length) in blocking mode.
 *                      * In non-blocking mode fetches as much data as 
 *                        available - even if it's zero bytes.
 *                    Exact: 
 *                      * Fetches exactly the requested number of bytes 
 *                        in blocking mode.
 *                      * In non-blocking mode returns error if 
 *                        requested number of byte is not available.
 *
 *      Bit 2       - Read (CLR)/ Peek (SET)
 *                    CBUF_READ (0) / CBUF_PEEK (4)
 *                    Peek: 
 *                      * Gets data without removing that data from the 
 *                        buffer.
 *      Bit 3:7     - xxxxx (Don't care)
 */
API_RESULT cbuf_get(CBUF_HDL , UINT8* , /*INOUT*/UINT16* , CBUF_FLAGS );

/* Informs if writing to the buffer has suspended.
 * When suspend state is set a read thread waiting on cbuf_get() is signaled. */
API_RESULT cbuf_suspend_write(CBUF_HDL , BOOL_T );

/* Informs if reading from the buffer has suspended.
 * When suspend state is set a write thread waiting on cbuf_give() is signaled. */
API_RESULT cbuf_suspend_read(CBUF_HDL , BOOL_T );

/* The read and write pointer is reset to 0. */
API_RESULT cbuf_reset(CBUF_HDL );

#ifdef CBUF_MT_LOG_ENABLE
    void print_cbuf_data(CBUF_HDL );
#else /* CBUF_MT_LOG_ENABLE */
    #define print_cbuf_data(hdl)
#endif /* CBUF_MT_LOG_ENABLE */

/* TODO - maybe */
/* * Return CBUF Size, Free, Max
 * * Search for an element.
 * * Remove an element, given an index.
 * * Purge all similar elements of a certain kind.
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CIRCULAR_BUFFER_MT_ */
