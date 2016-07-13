#ifndef _H_CIRCULAR_BUFFER_EXPORT_
#define _H_CIRCULAR_BUFFER_EXPORT_

#include "error_codes.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define CBUF_INVALID_BUF_PARAMS             ((API_RESULT)(0x0001|CBUF_ID))
#define CBUF_INVALID_DATA_PARAMS            ((API_RESULT)(0x0003|CBUF_ID))
#define CBUF_INVALID_CBUF_HDL               ((API_RESULT)(0x0004|CBUF_ID))
#define CBUF_NO_FREE_BUFFERS                ((API_RESULT)(0x0005|CBUF_ID))
#define CBUF_NOT_ENUF_SPACE                 ((API_RESULT)(0x0006|CBUF_ID))
#define CBUF_INVALID_BUFFER_ADDR            ((API_RESULT)(0x0007|CBUF_ID))
#define CBUF_INVALID_FUNC_PARAMS            ((API_RESULT)(0x0008|CBUF_ID))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CIRCULAR_BUFFER_EXPORT_ */
