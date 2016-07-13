#include "platform.h"
#include "logger.h"
#include "time_utils.h"

API_RESULT get_time_difference(DECL_CONST TIME_SPAN *initial, DECL_CONST TIME_SPAN *later, TIME_SPAN *diff)
{
    if ((NULL == initial) || (NULL == later) || (NULL == diff) ||
            (initial->usec >= (UINT32)MILLION) || (later->usec >= (UINT32)MILLION) ||
            (initial->sec > later->sec) ||
            ((initial->sec == later->sec) && (initial->usec > later->usec)))
    {
        log_info(("Input validation in get_time_difference() failed."));
        return API_FAILURE;
    }

    if (later->usec < initial->usec)
    {
        /* Carrying over 1 second to subtract micro-seconds. */
        diff->sec = later->sec - 1 - initial->sec;
        diff->usec = later->usec + (UINT32)MILLION - initial->usec;
    }
    else
    {
        diff->sec = later->sec - initial->sec;
        diff->usec = later->usec - initial->usec;
    }

    return API_SUCCESS;
}

API_RESULT get_elapsed_time(DECL_CONST TIME_SPAN *reference, TIME_SPAN *diff)
{
    TIME_SPAN ct;

    if ((NULL == reference) || (NULL == diff) || (reference->usec >= (UINT32)MILLION))
    {
        log_info(("Input validation in get_elapsed_time() failed."));
        return API_FAILURE;
    }

    if (API_SUCCESS != get_current_time(&ct))
    {
        log_info(("get_current_time() in get_elapsed_time() failed."));
        return API_FAILURE;
    }

    /* Only way the call to get_time_difference() below can fail is if
     * 'reference' time is detected to be later that 'ct'. */
    return get_time_difference(reference, &ct, diff);
}
