#ifndef _H_ERROR_CODES_
#define _H_ERROR_CODES_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define CBUF_ID                             0xF100
#define CQUE_ID                             0xF200
#define CBUF_MT_ID                          0xF300
#define SOCKETS_ID                          0xF400

#define ERROR_STRING(err)                   #err

typedef struct
{
    DECL_CONST API_RESULT err_code;
    DECL_CONST CHAR* str;
}ERR_DIC_ELEM;

DECL_CONST CHAR* error_string(DECL_CONST ERR_DIC_ELEM* , UINT32 , API_RESULT );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_ERROR_CODES_ */
