#ifndef _H_CIRCULAR_BUFFER_MT_INTERNAL_
#define _H_CIRCULAR_BUFFER_MT_INTERNAL_

#include "circular_buffer_mt_export.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct {
    UINT8* buf;
    UINT16 size;
    UINT16 rd;
    UINT16 wr;
    BOOL_T suspend_wr;
    BOOL_T suspend_rd;
    /* Necessary only when there are more than one threads 
     * reading (or writing) from different Queues. */
    /* pltfm_mutex_t mutex; */
    pltfm_cond_t wr_condv;
    pltfm_cond_t rd_condv;
} CBUF;

/*
 * Total number of Multi-thread compatible circular buffers.
 */
#define CBUF_MT_NUM_BUFFERS                 (5u)

/*
 * Thread synchronization macro functions.
 */
#define CBUF_LIST_MUTEX_INIT()              PLTFM_MUTEX_INIT(cbuf_mutex, CBUF_MT,\
                                                CBUF_MT_MUTEX_INIT_FAILED)
#define CBUF_LIST_MUTEX_DESTROY()           PLTFM_MUTEX_DESTROY(cbuf_mutex, CBUF_MT,\
                                                CBUF_MT_MUTEX_DESTROY_FAILED)
#define CBUF_LIST_MUTEX_LOCK()              PLTFM_MUTEX_LOCK(cbuf_mutex, CBUF_MT,\
                                                CBUF_MT_MUTEX_LOCK_FAILED)
#define CBUF_LIST_MUTEX_UNLOCK()            PLTFM_MUTEX_UNLOCK(cbuf_mutex, CBUF_MT,\
                                                CBUF_MT_MUTEX_UNLOCK_FAILED)
#define CBUF_LIST_MUTEX_LOCK_VOID()         PLTFM_MUTEX_LOCK(cbuf_mutex, CBUF_MT,\
                                                RETURN_VOID)
#define CBUF_LIST_MUTEX_UNLOCK_VOID()       PLTFM_MUTEX_UNLOCK(cbuf_mutex, CBUF_MT,\
                                                RETURN_VOID)
#define CBUF_RD_COND_INIT(hdl)              PLTFM_COND_INIT((CBUF(hdl).rd_condv), CBUF_MT,\
                                                CBUF_MT_CONDV_INIT_FAILED)
#define CBUF_RD_COND_DESTROY(hdl)           PLTFM_COND_INIT((CBUF(hdl).rd_condv), CBUF_MT,\
                                                CBUF_MT_CONDV_DESTROY_FAILED)
#define CBUF_RD_SIGNAL(hdl)                 PLTFM_COND_SIGNAL((CBUF(hdl).rd_condv), CBUF_MT,\
                                                CBUF_MT_CONDV_SIGNAL_FAILED)
#define CBUF_RD_WAIT(hdl)                   PLTFM_COND_WAIT((CBUF(hdl).rd_condv),\
                                                cbuf_mutex, CBUF_MT,\
                                                CBUF_MT_CONDV_WAIT_FAILED)
#define CBUF_WR_COND_INIT(hdl)              PLTFM_COND_INIT((CBUF(hdl).wr_condv), CBUF_MT,\
                                                CBUF_MT_CONDV_INIT_FAILED)
#define CBUF_WR_COND_DESTROY(hdl)           PLTFM_COND_INIT((CBUF(hdl).wr_condv), CBUF_MT,\
                                                CBUF_MT_CONDV_DESTROY_FAILED)
#define CBUF_WR_SIGNAL(hdl)                 PLTFM_COND_SIGNAL((CBUF(hdl).wr_condv), CBUF_MT,\
                                                CBUF_MT_CONDV_SIGNAL_FAILED)
#define CBUF_WR_WAIT(hdl)                   PLTFM_COND_WAIT((CBUF(hdl).wr_condv),\
                                                cbuf_mutex, CBUF_MT,\
                                                CBUF_MT_CONDV_WAIT_FAILED)

/*
 * CBUF Logging APIs
 */
#ifdef CBUF_MT_LOG_ENABLE
    #define CBUF_MT_HEX(buf, buf_len)       log_buffer(buf, buf_len)
    #define CBUF_MT_TRC(msg)                _log_trace("[CBUF_MT]", msg)
    #define CBUF_MT_INF(msg)                _log_info("[CBUF_MT]", msg)
    #define CBUF_MT_WRN(msg)                _log_warning("[CBUF_MT]", msg)
    #define CBUF_MT_ERR(msg)                _log_error("[CBUF_MT]", msg)
    #define CBUF_MT_CTCL(msg)               _log_critical("[CBUF_MT]", msg)
#else /* CBUF_MT_LOG_ENABLE */
    #define CBUF_MT_HEX(buf, buf_len)
    #define CBUF_MT_TRC(msg)
    #define CBUF_MT_INF(msg)
    #define CBUF_MT_WRN(msg)
    #define CBUF_MT_ERR(msg)
    #define CBUF_MT_CTCL(msg)
#endif /* CBUF_MT_LOG_ENABLE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CIRCULAR_BUFFER_MT_INTERNAL_ */
