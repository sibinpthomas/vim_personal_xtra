#ifndef _H_CIRCULAR_BUFFER_MT_EXPORT_
#define _H_CIRCULAR_BUFFER_MT_EXPORT_

#include "error_codes.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef UINT16                              CBUF_HDL;
#define CBUF_NULL_HDL                       ((CBUF_HDL)0u)

typedef UINT8                               CBUF_FLAGS;
#define CBUF_WAIT_BIT                       BIT(0)
#define CBUF_NBLK                           ((CBUF_FLAGS)0x00u)
#define CBUF_WAIT                           ((CBUF_FLAGS)0x01u)

#define CBUF_XACT_BIT                       BIT(1)
#define CBUF_ANY                            ((CBUF_FLAGS)0x00u)
#define CBUF_XACT                           ((CBUF_FLAGS)0x02u)

#define CBUF_PEEK_BIT                       BIT(2)
#define CBUF_READ                           ((CBUF_FLAGS)0x00u)
#define CBUF_PEEK                           ((CBUF_FLAGS)0x04u)

/* Multi-Threaded Circular Buffer module error codes. */
#define CBUF_MT_INVALID_FUNC_PARAMS         ((API_RESULT)(0x0001|CBUF_MT_ID))
#define CBUF_MT_INVALID_BUF_PARAMS          ((API_RESULT)(0x0002|CBUF_MT_ID))
#define CBUF_MT_INVALID_INPUT_BUF_PARAMS    ((API_RESULT)(0x0003|CBUF_MT_ID))
#define CBUF_MT_INVALID_OUTPUT_BUF_PARAMS   ((API_RESULT)(0x0004|CBUF_MT_ID))
#define CBUF_MT_INVALID_HDL                 ((API_RESULT)(0x0005|CBUF_MT_ID))
#define CBUF_MT_INVALID_FLAGS               ((API_RESULT)(0x0006|CBUF_MT_ID))
#define CBUF_MT_NO_FREE_BUFFERS             ((API_RESULT)(0x0007|CBUF_MT_ID))
#define CBUF_MT_NOT_ENUF_SPACE              ((API_RESULT)(0x0008|CBUF_MT_ID))
#define CBUF_MT_NOT_ENUF_DATA               ((API_RESULT)(0x0009|CBUF_MT_ID))
#define CBUF_MT_SUSPENDED_WRITE             ((API_RESULT)(0x000A|CBUF_MT_ID))
#define CBUF_MT_SUSPENDED_READ              ((API_RESULT)(0x000B|CBUF_MT_ID))
#define CBUF_MT_MUTEX_INIT_FAILED           ((API_RESULT)(0x000C|CBUF_MT_ID))
#define CBUF_MT_CONDV_INIT_FAILED           ((API_RESULT)(0x000D|CBUF_MT_ID))
#define CBUF_MT_MUTEX_DESTROY_FAILED        ((API_RESULT)(0x000E|CBUF_MT_ID))
#define CBUF_MT_CONDV_DESTROY_FAILED        ((API_RESULT)(0x000F|CBUF_MT_ID))
#define CBUF_MT_MUTEX_LOCK_FAILED           ((API_RESULT)(0x0010|CBUF_MT_ID))
#define CBUF_MT_MUTEX_UNLOCK_FAILED         ((API_RESULT)(0x0011|CBUF_MT_ID))
#define CBUF_MT_CONDV_WAIT_FAILED           ((API_RESULT)(0x0012|CBUF_MT_ID))
#define CBUF_MT_CONDV_SIGNAL_FAILED         ((API_RESULT)(0x0013|CBUF_MT_ID))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CIRCULAR_BUFFER_MT_EXPORT_ */
