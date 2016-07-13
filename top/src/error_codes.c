#include "platform.h"
#include "error_codes.h"

CHAR default_err[] = "Couldn't find the error code in provided dictionary.";

DECL_CONST CHAR* error_string(DECL_CONST ERR_DIC_ELEM* err_dic,
                              UINT32 err_dic_len,
                              API_RESULT err_code)
{
    UINT32 i;

    /* Using Linear search because there is no guarantee that the error codes
     * dictionary will be sorted. */
    for (i = 0; i < err_dic_len; i++)
    {
        if (err_dic[i].err_code == err_code)
        {
            return err_dic[i].str;
        }
    }

    return default_err;
}
