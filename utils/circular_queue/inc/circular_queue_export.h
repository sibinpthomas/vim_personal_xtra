#ifndef _H_CIRCULAR_QUEUE_EXPORT_
#define _H_CIRCULAR_QUEUE_EXPORT_

#include "error_codes.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define CQUE_INVALID_QUEUE_PARAMS           ((API_RESULT)(0x0001|CQUE_ID))
#define CQUE_INVALID_DATA_PARAMS            ((API_RESULT)(0x0003|CQUE_ID))
#define CQUE_INVALID_CQUE_HDL               ((API_RESULT)(0x0004|CQUE_ID))
#define CQUE_NO_FREE_QUEUES                 ((API_RESULT)(0x0005|CQUE_ID))
#define CQUE_NOT_ENUF_SPACE                 ((API_RESULT)(0x0006|CQUE_ID))
#define CQUE_INVALID_QUEUE_ADDR             ((API_RESULT)(0x0007|CQUE_ID))
#define CQUE_INVALID_FUNC_PARAMS            ((API_RESULT)(0x0008|CQUE_ID))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CIRCULAR_QUEUE_EXPORT_ */
