#include "platform.h"
#include "logger.h"
#include "circular_queue_internal.h"
#include "circular_queue.h"

#define CQUE(hdl)                           (cque_list[(hdl)-1])
#define is_invalid_cque_hdl(hdl)            (((hdl) == CQUE_NULL_HDL) || \
                                                ((hdl) > CQUE_NUM_QUEUES))
#define is_cque_empty(hdl)                  (CQUE(hdl).wr == CQUE(hdl).rd)
#define is_cque_full(hdl)                   (modulo_add_wr((hdl), 1) == CQUE(hdl).rd)
#define get_cque_size(hdl)                  (((CQUE(hdl).n_elem_max + CQUE(hdl).wr) - CQUE(hdl).rd) % CQUE(hdl).n_elem_max)
#define get_cque_free(hdl)                  (CQUE(hdl).n_elem_max - 1 - get_cque_size(hdl))
#define modulo_add_wr(hdl, n_elem)          ((CQUE(hdl).wr + (n_elem)) % CQUE(hdl).n_elem_max)
#define modulo_add_rd(hdl, n_elem)          ((CQUE(hdl).rd + (n_elem)) % CQUE(hdl).n_elem_max)
#define can_cque_take_data(hdl, n_elem)     ((get_cque_free(hdl) < (n_elem)) ? BOOL_FALSE : BOOL_TRUE)


/* Pool of Circular Queues. */
DECL_STATIC CQUE cque_list[CQUE_NUM_QUEUES];


/* Private Functions. */
DECL_STATIC CQUE_HDL get_free_cque(void)
{
    UINT16_F i;

    for (i = 0; i < CQUE_NUM_QUEUES; i++)
    {
        if (NULL == cque_list[i].buf)
        {
            return ((CQUE_HDL)(i + 1));
        }
    }

    /* CQUE_HDL == CQUE_NULL_HDL means failure to allocate. */
    return CQUE_NULL_HDL; 
}

/* Public Functions. */
API_RESULT cque_init(void)
{
    UINT16_F i;

    for (i = 0; i < CQUE_NUM_QUEUES; i++)
    {
        cque_list[i].buf = NULL;
    }

    return API_SUCCESS;
}

API_RESULT cque_deinit(void)
{
    return API_SUCCESS;
}

API_RESULT cque_alloc(void* buf, UINT16 elem_size, UINT16 n_elem_max, CQUE_HDL* cq_hdl)
{
    if ((NULL == buf) || (0 == elem_size) || (n_elem_max < 2u) || (NULL == cq_hdl))
    {
        /* Queue size ('n_elem_max') has to be at least 2 since the queue has to
         * have space for at least one element *and* one slot in the queue has
         * to be left free to differentiate between queue full and queue empty. 
         *   r == w                      -  Implies Q empty
         *   w + 1 (mod n_elem_max) == r   -  Implies Q full */
        CQUE_INF(("Invalid parameters for circular queue.\n"));
        return CQUE_INVALID_QUEUE_PARAMS;
    }

    *cq_hdl = get_free_cque();

    if (CQUE_NULL_HDL == *cq_hdl)
    {
        CQUE_INF(("No free CQUE elements in the list.\n"));
        return CQUE_NO_FREE_QUEUES;
    }

    CQUE(*cq_hdl).buf = (UINT8 *)buf;
    CQUE(*cq_hdl).elem_size = elem_size;
    CQUE(*cq_hdl).n_elem_max = n_elem_max;
    CQUE(*cq_hdl).rd = 0;
    CQUE(*cq_hdl).wr = 0;

    return API_SUCCESS;
}

API_RESULT cque_free(CQUE_HDL hdl)
{
    if (is_invalid_cque_hdl(hdl))
    {
        return CQUE_INVALID_CQUE_HDL;
    }

    CQUE(hdl).buf = NULL;

    return API_SUCCESS;
}

API_RESULT cque_reset(CQUE_HDL hdl)
{
    if (is_invalid_cque_hdl(hdl))
    {
        return CQUE_INVALID_CQUE_HDL;
    }

    CQUE(hdl).rd = 0;
    CQUE(hdl).wr = 0;

    return API_SUCCESS;
}

API_RESULT cque_push(CQUE_HDL hdl, DECL_CONST void* data, UINT16 n_elem)
{
    UINT16 w2e; /* Num spaces from Write Pointer to end of queue. */

    if (is_invalid_cque_hdl(hdl) || (NULL == data) ||
            (0 == n_elem) || (n_elem > (CQUE(hdl).n_elem_max - 1)))
    {
        CQUE_INF(("Invalid parameters passed to cque_push().\n"));
        return CQUE_INVALID_DATA_PARAMS;
    }

    if (BOOL_FALSE == can_cque_take_data(hdl, n_elem))
    {
        CQUE_INF(("Queue can not accept such large amount of data (%u).\n", n_elem));
        return CQUE_NOT_ENUF_SPACE;
    }

    CQUE_TRC(("Free Space = %u, Data Length = %u\n", get_cque_free(hdl), n_elem));
    /* Need to find contiguous memory length before doing 1 or 2 memcpy(). */
    w2e = CQUE(hdl).n_elem_max - CQUE(hdl).wr;
    if (n_elem <= w2e)
    {
        pltfm_memcpy(&(CQUE(hdl).buf[CQUE(hdl).wr * CQUE(hdl).elem_size]), data, (n_elem * CQUE(hdl).elem_size));
    }
    else
    {
        pltfm_memcpy(&(CQUE(hdl).buf[CQUE(hdl).wr * CQUE(hdl).elem_size]), data, (w2e * CQUE(hdl).elem_size));
        pltfm_memcpy(CQUE(hdl).buf, &((UINT8 *)data)[w2e * CQUE(hdl).elem_size], ((n_elem - w2e) * CQUE(hdl).elem_size));
    }
    
    CQUE(hdl).wr = modulo_add_wr(hdl, n_elem);

    return API_SUCCESS;
}

UINT16 cque_peek(CQUE_HDL hdl, void* buf, UINT16 rd_n_elem)
{
    UINT16 r2e; /* Num spaces from Read Pointer to end of queue. */
    UINT16 n_elem;

    if (is_invalid_cque_hdl(hdl) || (NULL == buf) ||
            (0 == rd_n_elem) || (is_cque_empty(hdl)))
    {
        CQUE_INF(("Either the Queue is empty or the input parameters are invalid.\n"));
        return 0;
    }

    CQUE_TRC(("CQUE Size = %u, Output Queue Size = %u\n", get_cque_size(hdl), rd_n_elem));
    n_elem = get_cque_size(hdl);
    if (rd_n_elem < n_elem)
    {
        n_elem = rd_n_elem;
    }

    /* Need to find contiguous memory length before doing 1 or 2 memcpy(). */
    r2e = CQUE(hdl).n_elem_max - CQUE(hdl).rd;
    if (n_elem <= r2e)
    {
        pltfm_memcpy(buf, &(CQUE(hdl).buf[CQUE(hdl).rd * CQUE(hdl).elem_size]), (n_elem * CQUE(hdl).elem_size));
    }
    else
    {
        pltfm_memcpy(buf, &(CQUE(hdl).buf[CQUE(hdl).rd * CQUE(hdl).elem_size]), (r2e * CQUE(hdl).elem_size));
        pltfm_memcpy(&((UINT8 *)buf)[r2e * CQUE(hdl).elem_size], CQUE(hdl).buf, ((n_elem - r2e) * CQUE(hdl).elem_size));
    }

    return n_elem;
}
    
UINT16 cque_pop(CQUE_HDL hdl, void* buf, UINT16 rd_n_elem)
{
    UINT16 n_elem;

    n_elem = cque_peek(hdl, buf, rd_n_elem);
    CQUE(hdl).rd = modulo_add_rd(hdl, n_elem);

    return n_elem;
}

#ifdef CQUE_LOG_ENABLE
void print_cque_data(CQUE_HDL hdl)
{
    UINT16 n_elem;
    UINT16 r2e;

    log_simple(("\nQueue Base pointer  - %u", CQUE(hdl).buf));
    log_simple(("\nQueue Elem Size     - %u", CQUE(hdl).elem_size));
    log_simple(("\nQueue Max num Elements - %u", CQUE(hdl).n_elem_max));
    log_simple(("\nQueue Free          - %u", get_cque_free(hdl)));
    log_simple(("\nQueue Read Pointer  - %u", CQUE(hdl).rd));
    log_simple(("\nQueue Write Pointer - %u", CQUE(hdl).wr));

    if ((NULL != CQUE(hdl).buf) && (CQUE(hdl).n_elem_max > 0))
    {
        log_simple(("\n** Queue raw ** - "));
        log_buffer(CQUE(hdl).buf, CQUE(hdl).n_elem_max * CQUE(hdl).elem_size);
    }

    if ((NULL != CQUE(hdl).buf) && (get_cque_size(hdl) > 0))
    {
        log_simple(("\n@@ Queue Contents @@ - "));
        r2e = CQUE(hdl).n_elem_max - CQUE(hdl).rd;
        n_elem = get_cque_size(hdl);
        if (n_elem <= r2e)
        {
            log_buffer(&(CQUE(hdl).buf[CQUE(hdl).rd]), n_elem * CQUE(hdl).elem_size);
        }
        else
        {
            log_buffer(&(CQUE(hdl).buf[CQUE(hdl).rd]), r2e * CQUE(hdl).elem_size);
            log_buffer(CQUE(hdl).buf, (n_elem - r2e) * CQUE(hdl).elem_size);
        }
    }
}
#endif /* CQUE_LOG_ENABLE */
