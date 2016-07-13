#include "platform.h"
#include "logger.h"
#include "time_utils_pl.h"


DECL_STATIC BOOL_T g_first_time = BOOL_TRUE;
DECL_STATIC LARGE_INTEGER g_counts_per_sec;

API_RESULT get_current_time(TIME_SPAN *ct)
{
    LARGE_INTEGER count;

    if (g_first_time)
    {
        g_first_time = BOOL_FALSE;

        if (0 == QueryPerformanceFrequency(&g_counts_per_sec))
        {
            g_counts_per_sec.QuadPart = 0;
        }
    }

    if (NULL == ct)
    {
        log_info(("Input validation in get_current_time() failed."));
        return API_FAILURE;
    }

    if (g_counts_per_sec.QuadPart <= 0)
    {
        log_info(("QueryPerformanceFrequency() in get_current_time() failed."));
        return API_FAILURE;
    }

    if (0 == QueryPerformanceCounter(&count))
    {
        log_info(("QueryPerformanceCounter() in get_current_time() failed."));
        return API_FAILURE;
    }

    ct->sec = (UINT32)(count.QuadPart / g_counts_per_sec.QuadPart);
    ct->usec = (UINT32)(((count.QuadPart % g_counts_per_sec.QuadPart) * MILLION) / g_counts_per_sec.QuadPart);

    return API_SUCCESS;
}
