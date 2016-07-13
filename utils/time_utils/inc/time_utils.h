#ifndef _H_TIME_UTILS_
#define _H_TIME_UTILS_

#include "time_utils_export.h"
#include "time_utils_pl.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

API_RESULT get_time_difference(DECL_CONST TIME_SPAN * , DECL_CONST TIME_SPAN * , TIME_SPAN * );
API_RESULT get_elapsed_time(DECL_CONST TIME_SPAN * , TIME_SPAN * );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_TIME_UTILS_ */
