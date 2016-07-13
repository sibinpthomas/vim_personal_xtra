#ifndef _H_TIME_UTILS_EXPORT_
#define _H_TIME_UTILS_EXPORT_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define BILLION                             (1E9)
#define MILLION                             (1E6)
#define THOUSAND                            (1E3)

/* Closely matches 'struct timeval' from <time.h> */
typedef struct
{
    /* Time span, in seconds. */
    UINT32 sec;
    /* Time span, in micro-seconds. */
    UINT32 usec;
}TIME_SPAN;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_TIME_UTILS_EXPORT_ */
