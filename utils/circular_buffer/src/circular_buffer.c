#include "platform.h"
#include "logger.h"
#include "circular_buffer_internal.h"
#include "circular_buffer.h"

#define CBUF(hdl)                           (cbuf_list[(hdl)-1])
#define is_invalid_cbuf_hdl(hdl)            (((hdl) == CBUF_NULL_HDL) || \
                                                ((hdl) > CBUF_NUM_BUFFERS) || \
                                                (NULL == CBUF(hdl).buf))
#define is_cbuf_empty(hdl)                  (CBUF(hdl).wr == CBUF(hdl).rd)
#define is_cbuf_full(hdl)                   (modulo_add_wr((hdl), 1) == CBUF(hdl).rd)
#define get_cbuf_size(hdl)                  (((CBUF(hdl).size + CBUF(hdl).wr) - CBUF(hdl).rd) % CBUF(hdl).size)
#define get_cbuf_free(hdl)                  (CBUF(hdl).size - 1 - get_cbuf_size(hdl))
#define modulo_add_wr(hdl, data_len)        ((CBUF(hdl).wr + (data_len)) % CBUF(hdl).size)
#define modulo_add_rd(hdl, data_len)        ((CBUF(hdl).rd + (data_len)) % CBUF(hdl).size)
#define can_cbuf_take_data(hdl, data_len)   ((get_cbuf_free(hdl) < (data_len)) ? BOOL_FALSE : BOOL_TRUE)


/* Pool of Circular Buffers. */
DECL_STATIC CBUF cbuf_list[CBUF_NUM_BUFFERS];


/* Private Functions. */
DECL_STATIC CBUF_HDL get_free_cbuf(void)
{
    UINT16_F i;

    for (i = 0; i < CBUF_NUM_BUFFERS; i++)
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
API_RESULT cbuf_init(void)
{
    UINT16_F i;

    for (i = 0; i < CBUF_NUM_BUFFERS; i++)
    {
        cbuf_list[i].buf = NULL;
    }

    return API_SUCCESS;
}

API_RESULT cbuf_deinit(void)
{
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
        CBUF_INF(("Invalid parameters for circular buffer.\n"));
        return CBUF_INVALID_BUF_PARAMS;
    }

    *cb_hdl = get_free_cbuf();

    if (CBUF_NULL_HDL == *cb_hdl)
    {
        CBUF_INF(("No free CBUF elements in the list.\n"));
        return CBUF_NO_FREE_BUFFERS;
    }

    CBUF(*cb_hdl).rd = 0;
    CBUF(*cb_hdl).wr = 0;
    CBUF(*cb_hdl).size = buf_size;
    /* Assigning 'buf' last since 'buf' value is used to check if CBUF is free. */
    CBUF(*cb_hdl).buf = buf;

    return API_SUCCESS;
}

API_RESULT cbuf_free(CBUF_HDL hdl)
{
    if (is_invalid_cbuf_hdl(hdl))
    {
        return CBUF_INVALID_CBUF_HDL;
    }

    CBUF(hdl).buf = NULL;

    return API_SUCCESS;
}

API_RESULT cbuf_reset(CBUF_HDL hdl)
{
    if (is_invalid_cbuf_hdl(hdl))
    {
        return CBUF_INVALID_CBUF_HDL;
    }

    CBUF(hdl).rd = 0;
    CBUF(hdl).wr = 0;

    return API_SUCCESS;
}

API_RESULT cbuf_give(CBUF_HDL hdl, DECL_CONST UINT8* data, UINT16 data_len)
{
    UINT16 w2e; /* Num spaces from Write Pointer to end of buffer. */

    if (is_invalid_cbuf_hdl(hdl) || (NULL == data) ||
            (0 == data_len) || (data_len > (CBUF(hdl).size - 1)))
    {
        CBUF_INF(("Invalid parameters passed to cbuf_give().\n"));
        return CBUF_INVALID_DATA_PARAMS;
    }

    if (BOOL_FALSE == can_cbuf_take_data(hdl, data_len))
    {
        CBUF_INF(("Buffer can not accept such large amount of data (%u).\n", data_len));
        return CBUF_NOT_ENUF_SPACE;
    }

    CBUF_TRC(("Free Space = %u, Data Length = %u\n", get_cbuf_free(hdl), data_len));
    /* Need to find contiguous memory length before doing 1 or 2 memcpy(). */
    w2e = CBUF(hdl).size - CBUF(hdl).wr;
    if (data_len <= w2e)
    {
        pltfm_memcpy(&(CBUF(hdl).buf[CBUF(hdl).wr]), data, data_len);
    }
    else
    {
        pltfm_memcpy(&(CBUF(hdl).buf[CBUF(hdl).wr]), data, w2e);
        pltfm_memcpy(CBUF(hdl).buf, &data[w2e], (data_len - w2e));
    }
    
    CBUF(hdl).wr = modulo_add_wr(hdl, data_len);

    return API_SUCCESS;
}

UINT16 cbuf_peek(CBUF_HDL hdl, UINT8* buf, UINT16 buf_size)
{
    UINT16 r2e; /* Num spaces from Read Pointer to end of buffer. */
    UINT16 data_len;

    if (is_invalid_cbuf_hdl(hdl) || (NULL == buf) ||
            (0 == buf_size) || (is_cbuf_empty(hdl)))
    {
        CBUF_INF(("Either the Queue is empty or the input parameters are invalid.\n"));
        return 0;
    }

    CBUF_TRC(("CBUF Size = %u, Output Buffer Size = %u\n", get_cbuf_size(hdl), buf_size));
    data_len = get_cbuf_size(hdl);
    if (buf_size < data_len)
    {
        data_len = buf_size;
    }

    /* Need to find contiguous memory length before doing 1 or 2 memcpy(). */
    r2e = CBUF(hdl).size - CBUF(hdl).rd;
    if (data_len <= r2e)
    {
        pltfm_memcpy(buf, &(CBUF(hdl).buf[CBUF(hdl).rd]), data_len);
    }
    else
    {
        pltfm_memcpy(buf, &(CBUF(hdl).buf[CBUF(hdl).rd]), r2e);
        pltfm_memcpy(&buf[r2e], CBUF(hdl).buf, (data_len - r2e));
    }

    return data_len;
}
    
UINT16 cbuf_get(CBUF_HDL hdl, UINT8* buf, UINT16 buf_size)
{
    UINT16 data_len;

    data_len = cbuf_peek(hdl, buf, buf_size);
    CBUF(hdl).rd = modulo_add_rd(hdl, data_len);

    return data_len;
}

#ifdef CBUF_LOG_ENABLE
void print_cbuf_data(CBUF_HDL hdl)
{
    UINT16 data_len;
    UINT16 r2e;

    log_simple(("\nBuffer Base pointer  - %u", CBUF(hdl).buf));
    log_simple(("\nBuffer Size          - %u", CBUF(hdl).size));
    log_simple(("\nBuffer Free          - %u", get_cbuf_free(hdl)));
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
}
#endif /* CBUF_LOG_ENABLE */
