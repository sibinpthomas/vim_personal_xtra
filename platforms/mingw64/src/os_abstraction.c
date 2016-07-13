#include "platform.h"
#include "logger.h"

int uSleep(UINT32 usec) 
{ 
    HANDLE hTimer = NULL;
    LARGE_INTEGER liDueTime;
    int retval = 0;

    /* Create an unnamed waitable timer. */
    hTimer = CreateWaitableTimer(NULL, TRUE, NULL); 
    if (NULL == hTimer)
    {
        log_info(("CreateWaitableTimer failed (%d)\n", GetLastError()));
        return 1;
    }

    /* Convert to 100 nanosecond interval,
    * negative value indicates relative time */
    liDueTime.QuadPart = -1 * (__int64)usec / 10;

    do
    {
        /* Set a timer to wait for 'usec' microseconds. */
        if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
        {
            log_info(("SetWaitableTimer failed (%d)\n", GetLastError()));
            retval = 2;
            break;
        }

        /* Wait for the timer. */
        if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
        {
            log_info(("WaitForSingleObject failed (%d)\n", GetLastError()));
            retval = 3;
            break;
        }
    } while (0);

    if (!CloseHandle(hTimer))
    {
        log_info(("CloseHandle failed (%d)\n", GetLastError()));
        retval = 4;
    }

    return retval;
}
