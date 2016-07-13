#include "platform.h"
#include "logger.h"
#include "circular_buffer_mt_internal.h"
#include "circular_buffer_mt.h"

#define CBUF(hdl)                           (cbuf_list[(hdl)-1])
#define is_invalid_cbuf_hdl(hdl)            (((hdl) == CBUF_NULL_HDL) || \
                                                ((hdl) > CBUF_MT_NUM_BUFFERS) || \
                                                (NULL == CBUF(hdl).buf))
#define is_cbuf_empty(hdl)                  (CBUF(hdl).wr == CBUF(hdl).rd)
#define is_cbuf_full(hdl)                   (modulo_add_wr((hdl), 1) == CBUF(hdl).rd)
#define get_cbuf_size(hdl)                  (((CBUF(hdl).size + CBUF(hdl).wr) - CBUF(hdl).rd) % CBUF(hdl).size)
#define get_cbuf_freespace(hdl)             (CBUF(hdl).size - 1 - get_cbuf_size(hdl))
#define modulo_add_wr(hdl, data_len)        ((CBUF(hdl).wr + (data_len)) % CBUF(hdl).size)
#define modulo_add_rd(hdl, data_len)        ((CBUF(hdl).rd + (data_len)) % CBUF(hdl).size)



/* Pool of Circular Buffers. */
DECL_STATIC CBUF cbuf_list[CBUF_MT_NUM_BUFFERS];
DECL_STATIC pltfm_mutex_t cbuf_mutex;


/* Private Functions. */
DECL_STATIC CBUF_HDL get_free_cbuf(void)
{
    UINT16_F i;

    for (i = 0; i < CBUF_MT_NUM_BUFFERS; i++)
    {
        if (NULL == cbuf_list[i].buf)
        {
            return ((CBUF_HDL)(i + 1));
        }
    }

    /* CBUF_HDL == CBUF_NULL_HDL means failure to allocate. */
    return CBUF_NULL_HDL; 
}

/* Public Functions. */

/* It is assumed that there is no race condition between the module's init
 * function and any other function of the module.
 *
 * A module's init function is expected to be called in the main thread before 
 * the creation of any other thread that might call the module's other 
 * functions.
 */
API_RESULT cbuf_init(void)
{
    UINT16_F i;

    CBUF_LIST_MUTEX_INIT();

    for (i = 0; i < CBUF_MT_NUM_BUFFERS; i++)
    {
        cbuf_list[i].buf = NULL;
        CBUF_RD_COND_INIT(i + 1);
        CBUF_WR_COND_INIT(i + 1);
    }

    return API_SUCCESS;
}

/* Can it be assumed that there is no race condition between the module's deinit
 * function and any other function of the module?
 *
 * Ideally a module's deinit function is expected to be called in the main
 * thread after all the threads that might call the module's other functions 
 * have joined the main thread.
 *
 * -- Keeping the MUTEX Lock until this question is answered.
 */
API_RESULT cbuf_deinit(void)
{
    UINT16_F i;

    CBUF_LIST_MUTEX_LOCK();
    for (i = 0; i < CBUF_MT_NUM_BUFFERS; i++)
    {
        CBUF_RD_COND_DESTROY(i + 1);
        CBUF_WR_COND_DESTROY(i + 1);
    }
    CBUF_LIST_MUTEX_UNLOCK();

    CBUF_LIST_MUTEX_DESTROY();

    return API_SUCCESS;
}

API_RESULT cbuf_alloc(UINT8* buf, UINT16 buf_size, CBUF_HDL* cb_hdl)
{
    if ((NULL == buf) || (buf_size < 2u) || (NULL == cb_hdl))
    {
        /* Buffer size has to be at least 2 since the buffer has to have space 
         * for at least one element *and* one slot in the buffer has to
         * be left free to differentiate between buffer full and buffer empty. 
         *   r == w                      -  Implies Q empty
         *   w + 1 (mod buf_size) == r   -  Implies Q full */
        CBUF_MT_INF(("Invalid parameters for circular buffer.\n"));
        return CBUF_MT_INVALID_BUF_PARAMS;
    }

    CBUF_LIST_MUTEX_LOCK();

    *cb_hdl = get_free_cbuf();
    if (CBUF_NULL_HDL == *cb_hdl)
    {
        CBUF_LIST_MUTEX_UNLOCK();

        CBUF_MT_INF(("No free CBUF elements in the list.\n"));
        return CBUF_MT_NO_FREE_BUFFERS;
    }

    CBUF(*cb_hdl).rd = 0;
    CBUF(*cb_hdl).wr = 0;
    CBUF(*cb_hdl).size = buf_size;
    CBUF(*cb_hdl).suspend_wr = BOOL_FALSE;
    CBUF(*cb_hdl).suspend_rd = BOOL_FALSE;
    /* Assigning 'buf' last since 'buf' value is used to check if CBUF is free. */
    CBUF(*cb_hdl).buf = buf;

    CBUF_LIST_MUTEX_UNLOCK();

    return API_SUCCESS;
}

API_RESULT cbuf_free(CBUF_HDL hdl)
{
    CBUF_LIST_MUTEX_LOCK();

    if (is_invalid_cbuf_hdl(hdl))
    {
        CBUF_LIST_MUTEX_UNLOCK();

        return CBUF_MT_INVALID_HDL;
    }

    CBUF(hdl).buf = NULL;

    CBUF_LIST_MUTEX_UNLOCK();

    return API_SUCCESS;
}

API_RESULT cbuf_reset(CBUF_HDL hdl)
{
    CBUF_LIST_MUTEX_LOCK();

    if (is_invalid_cbuf_hdl(hdl))
    {
        CBUF_LIST_MUTEX_UNLOCK();
        return CBUF_MT_INVALID_HDL;
    }

    CBUF(hdl).rd = 0;
    CBUF(hdl).wr = 0;

    CBUF_LIST_MUTEX_UNLOCK();

    return API_SUCCESS;
}

API_RESULT cbuf_suspend_write(CBUF_HDL hdl, BOOL_T suspend_wr)
{
    CBUF_LIST_MUTEX_LOCK();

    if (is_invalid_cbuf_hdl(hdl))
    {
        CBUF_LIST_MUTEX_UNLOCK();
        return CBUF_MT_INVALID_HDL;
    }

    if (suspend_wr != CBUF(hdl).suspend_wr)
    {
        CBUF(hdl).suspend_wr = suspend_wr;
        if (BOOL_TRUE == suspend_wr)
        {
            /* Signal waiting Read thread so that the read thread can stop
             * waiting for more data. */
            CBUF_WR_SIGNAL(hdl);
        }
    }

    CBUF_LIST_MUTEX_UNLOCK();

    return API_SUCCESS;
}

API_RESULT cbuf_suspend_read(CBUF_HDL hdl, BOOL_T suspend_rd)
{
    CBUF_LIST_MUTEX_LOCK();

    if (is_invalid_cbuf_hdl(hdl))
    {
        CBUF_LIST_MUTEX_UNLOCK();
        return CBUF_MT_INVALID_HDL;
    }

    if (suspend_rd != CBUF(hdl).suspend_rd)
    {
        CBUF(hdl).suspend_rd = suspend_rd;
        if (BOOL_TRUE == suspend_rd)
        {
            /* Signal waiting Write thread so that the write thread can stop
             * waiting for the buffer to get cleared up. */
            CBUF_RD_SIGNAL(hdl);
        }
    }

    CBUF_LIST_MUTEX_UNLOCK();

    return API_SUCCESS;
}

API_RESULT cbuf_give(CBUF_HDL hdl, DECL_CONST UINT8* data, UINT16* data_len, CBUF_FLAGS flags)
{
    UINT16 w2e; /* Num spaces from Write Pointer to end of buffer. */
    UINT16 free_space;

    CBUF_LIST_MUTEX_LOCK();

    if (is_invalid_cbuf_hdl(hdl) || (NULL == data) || (NULL == data_len) ||
            (0 == *data_len) ||
            ((IS_BITNUM_SET(flags, CBUF_XACT_BIT)) && (*data_len > (CBUF(hdl).size - 1))))
    {
        CBUF_LIST_MUTEX_UNLOCK();

        CBUF_MT_INF(("Invalid parameters passed to cbuf_give().\n"));
        return CBUF_MT_INVALID_INPUT_BUF_PARAMS;
    }

    free_space = get_cbuf_freespace(hdl);
    if (IS_BITNUM_SET(flags, CBUF_WAIT_BIT))
    {
        if (IS_BITNUM_SET(flags, CBUF_XACT_BIT))
        {
            while ((free_space < *data_len) && (BOOL_FALSE == CBUF(hdl).suspend_rd))
            {
                CBUF_MT_TRC(("cbuf_give(): Waiting - Flag 'CBUF_XACT_BIT' set, waiting for %u bytes of free space.\n", *data_len));
                CBUF_RD_WAIT(hdl);
                free_space = get_cbuf_freespace(hdl);
            }
            if (free_space < *data_len)
            {
                CBUF_LIST_MUTEX_UNLOCK();

                CBUF_MT_INF(("The buffer has suspended read operations, while in cbuf_give()."
                             " Input Buffer Size = %u.\n", *data_len));
                return CBUF_MT_SUSPENDED_READ;
            }
        }
        else /* Wait for next read (ANY). */
        {
            while ((0 == free_space) && (BOOL_FALSE == CBUF(hdl).suspend_rd))
            {
                CBUF_MT_TRC(("cbuf_give(): Waiting - Flag 'CBUF_XACT_BIT' is clear, waiting for next read.\n"));
                CBUF_RD_WAIT(hdl);
                free_space = get_cbuf_freespace(hdl);
            }
            if (0 == free_space)
            {
                CBUF_LIST_MUTEX_UNLOCK();

                CBUF_MT_INF(("The buffer has suspended read operations, while in cbuf_give()."
                             " Input Buffer Size = %u.\n", *data_len));
                return CBUF_MT_SUSPENDED_READ;
            }
        }
    }
    else if ((IS_BITNUM_CLR(flags, CBUF_XACT_BIT)) && (0 == free_space))
    {
        CBUF_LIST_MUTEX_UNLOCK();

        CBUF_MT_INF(("The buffer is full in cbuf_give(). Input Buffer Size = %u.\n", *data_len));
        *data_len = 0;
        return API_SUCCESS;
    }
    else if ((IS_BITNUM_SET(flags, CBUF_XACT_BIT)) && (free_space < *data_len))
    {
        CBUF_LIST_MUTEX_UNLOCK();

        CBUF_MT_INF(("The buffer doesn't have enough free space in cbuf_give(). "
                     "Input Buffer Size = %u.\n", *data_len));
        return CBUF_MT_NOT_ENUF_SPACE;
    }


    CBUF_MT_TRC(("Free Space = %u, Data Length = %u\n", free_space, *data_len));
    if (free_space < *data_len)
    {
        *data_len = free_space;
    }

    /* Need to find contiguous memory length before doing 1 or 2 memcpy(). */
    w2e = CBUF(hdl).size - CBUF(hdl).wr;
    if (*data_len <= w2e)
    {
        pltfm_memcpy(&(CBUF(hdl).buf[CBUF(hdl).wr]), data, *data_len);
    }
    else
    {
        pltfm_memcpy(&(CBUF(hdl).buf[CBUF(hdl).wr]), data, w2e);
        pltfm_memcpy(CBUF(hdl).buf, &data[w2e], (*data_len - w2e));
    }
    
    CBUF(hdl).wr = modulo_add_wr(hdl, *data_len);

    /* Signal write completed to thread waiting to read the buffer. */
    CBUF_WR_SIGNAL(hdl);

    CBUF_LIST_MUTEX_UNLOCK();

    return API_SUCCESS;
}

DECL_STATIC API_RESULT cbuf_peek(CBUF_HDL hdl, UINT8* buf, UINT16* buf_size, CBUF_FLAGS flags)
{
    UINT16 r2e; /* Num spaces from Read Pointer to end of buffer. */
    UINT16 data_len;

    CBUF_LIST_MUTEX_LOCK();

    if (is_invalid_cbuf_hdl(hdl) || (NULL == buf) || (NULL == buf_size) ||
            (0 == *buf_size) || 
            ((IS_BITNUM_SET(flags, CBUF_XACT_BIT)) && (*buf_size > (CBUF(hdl).size - 1))))
    {
        CBUF_LIST_MUTEX_UNLOCK();

        CBUF_MT_INF(("The input parameters are invalid in cbuf_peek().\n"));
        return CBUF_MT_INVALID_OUTPUT_BUF_PARAMS;
    }

    data_len = get_cbuf_size(hdl);
    if (IS_BITNUM_SET(flags, CBUF_WAIT_BIT))
    {
        if (IS_BITNUM_SET(flags, CBUF_XACT_BIT))
        {
            while ((data_len < *buf_size) && (BOOL_FALSE == CBUF(hdl).suspend_wr))
            {
                CBUF_MT_TRC(("Waiting in cbuf_peek() - Flag 'CBUF_XACT_BIT' set, waiting for %u bytes of data.\n", *buf_size));
                CBUF_WR_WAIT(hdl);
                data_len = get_cbuf_size(hdl);
            }
            if (data_len < *buf_size)
            {
                CBUF_LIST_MUTEX_UNLOCK();

                CBUF_MT_INF(("The buffer has suspended write operations, while in cbuf_peek()."
                             " Output Buffer Size = %u.\n", *buf_size));
                return CBUF_MT_SUSPENDED_WRITE;
            }
        }
        else /* Wait for next write (ANY). */
        {
            while ((0 == data_len) && (BOOL_FALSE == CBUF(hdl).suspend_wr))
            {
                CBUF_MT_TRC(("Waiting in cbuf_peek() - Flag 'CBUF_XACT_BIT' is clear, waiting for next write.\n"));
                CBUF_WR_WAIT(hdl);
                data_len = get_cbuf_size(hdl);
            }
            if (0 == data_len)
            {
                CBUF_LIST_MUTEX_UNLOCK();

                CBUF_MT_INF(("The buffer has suspended write operations, while in cbuf_peek()."
                             " Output Buffer Size = %u.\n", *buf_size));
                return CBUF_MT_SUSPENDED_WRITE;
            }
        }
    }
    else if ((IS_BITNUM_CLR(flags, CBUF_XACT_BIT)) && (0 == data_len))
    {
        CBUF_LIST_MUTEX_UNLOCK();

        CBUF_MT_INF(("The buffer is empty in cbuf_peek(). Output Buffer Size = %u.\n", *buf_size));
        *buf_size = 0;
        return API_SUCCESS;
    }
    else if ((IS_BITNUM_SET(flags, CBUF_XACT_BIT)) && (data_len < *buf_size))
    {
        CBUF_LIST_MUTEX_UNLOCK();

        CBUF_MT_INF(("The buffer doesn't have enough data in cbuf_peek().\n"
                     "Requested = %u bytes, Available = %u bytes.\n",
                     *buf_size, data_len));
        return CBUF_MT_NOT_ENUF_DATA;
    }


    CBUF_MT_TRC(("cbuf_peek(): CBUF data len = %u, Output Buffer Size = %u\n", data_len, *buf_size));
    if (*buf_size < data_len)
    {
        data_len = *buf_size;
    }

    /* Need to find contiguous memory length before doing 1 or 2 memcpy(). */
    r2e = CBUF(hdl).size - CBUF(hdl).rd;
    if ((data_len > 0) && (data_len <= r2e))
    {
        pltfm_memcpy(buf, &(CBUF(hdl).buf[CBUF(hdl).rd]), data_len);
    }
    else
    {
        pltfm_memcpy(buf, &(CBUF(hdl).buf[CBUF(hdl).rd]), r2e);
        pltfm_memcpy(&buf[r2e], CBUF(hdl).buf, (data_len - r2e));
    }

    CBUF_LIST_MUTEX_UNLOCK();

    *buf_size = data_len;
    return API_SUCCESS;
}
    
API_RESULT cbuf_get(CBUF_HDL hdl, UINT8* buf, UINT16* buf_size, CBUF_FLAGS flags)
{
    API_RESULT retval;

    retval = cbuf_peek(hdl, buf, buf_size, flags);

    /* Check if CBUF_PEEK flag is clear. */
    if ((API_SUCCESS == retval) && (IS_BITNUM_CLR(flags, CBUF_PEEK_BIT)) &&
                                   (*buf_size > 0))
    {
        CBUF_LIST_MUTEX_LOCK();

        CBUF(hdl).rd = modulo_add_rd(hdl, *buf_size);

        /* Signal read completed to thread waiting to write to the buffer. */
        CBUF_RD_SIGNAL(hdl);

        CBUF_LIST_MUTEX_UNLOCK();
        CBUF_MT_TRC(("cbuf_get(): Read data length = %u\n", *buf_size));
    }

    return retval;
}

#ifdef CBUF_MT_LOG_ENABLE
void print_cbuf_data(CBUF_HDL hdl)
{
    UINT16 data_len;
    UINT16 r2e;

    CBUF_LIST_MUTEX_LOCK_VOID();

    if (is_invalid_cbuf_hdl(hdl))
    {
        CBUF_LIST_MUTEX_UNLOCK_VOID();
        CBUF_MT_INF(("The input parameters are invalid in print_cbuf_data().\n"));
        return;
    }

    log_simple(("\nBuffer Base pointer  - %u", CBUF(hdl).buf));
    log_simple(("\nBuffer Size          - %u", CBUF(hdl).size));
    log_simple(("\nBuffer Free          - %u", get_cbuf_freespace(hdl)));
    log_simple(("\nBuffer Read Pointer  - %u", CBUF(hdl).rd));
    log_simple(("\nBuffer Write Pointer - %u", CBUF(hdl).wr));
    if ((NULL != CBUF(hdl).buf) && (CBUF(hdl).size > 0))
    {
        log_simple(("\n** Buffer raw ** - "));
        log_buffer(CBUF(hdl).buf, CBUF(hdl).size);
    }
    if ((NULL != CBUF(hdl).buf) && (get_cbuf_size(hdl) > 0))
    {
        log_simple(("\n@@ Buffer Contents @@ - "));
        r2e = CBUF(hdl).size - CBUF(hdl).rd;
        data_len = get_cbuf_size(hdl);
        if (data_len <= r2e)
        {
            log_buffer(&(CBUF(hdl).buf[CBUF(hdl).rd]), data_len);
        }
        else
        {
            log_buffer(&(CBUF(hdl).buf[CBUF(hdl).rd]), r2e);
            log_buffer(CBUF(hdl).buf, (data_len - r2e));
        }
    }

    CBUF_LIST_MUTEX_UNLOCK_VOID();
}
#endif /* CBUF_MT_LOG_ENABLE */
