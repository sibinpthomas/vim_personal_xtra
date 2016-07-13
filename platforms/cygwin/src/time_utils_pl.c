#include "platform.h"
#include "logger.h"
#include "time_utils_export.h"

API_RESULT get_current_time(TIME_SPAN *ct)
{
    struct timespec curr_timespec;

    if (NULL == ct)
    {
        log_info(("Input validation in get_current_time() failed."));
        return API_FAILURE;
    }

    if(clock_gettime(CLOCK_REALTIME, &curr_timespec) == -1)
    {
        log_info(("clock_gettime() in get_current_time() failed."));
        return API_FAILURE;
    }

    ct->sec = (UINT32)curr_timespec.tv_sec;
    ct->usec = (UINT32)curr_timespec.tv_nsec/THOUSAND;

    return API_SUCCESS;
}
