/*
 * Abstractions for Thread handling primitives.
 * Abstractions for File handling primitives.
 * Abstractions for Timer related primitives.
 */

#ifndef _H_OS_ABSTRACTION_
#define _H_OS_ABSTRACTION_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <unistd.h>
#include <pthread.h>
#include "generic_type_abstraction.h"

/* Datatype to represent Thread or Task Identifier */
typedef pthread_t                           pltfm_thread_t;
/* Datatype to represent Thread or Task Attribute */
typedef pthread_attr_t                      pltfm_thread_attr_t;
/* Datatype to represent Mutex object */
typedef pthread_mutex_t                     pltfm_mutex_t;
/* Datatype to represent Attributes of a Mutex object */
typedef pthread_mutexattr_t                 pltfm_mutexattr_t;
/* Datatype to represent Conditional Variable object */
typedef pthread_cond_t                      pltfm_cond_t;
/* Datatype to represent Attributes of a Conditional Variable object */
typedef pthread_condattr_t                  pltfm_condattr_t;
/* Datatype to represent the return value of a thread start routine */
typedef void*                               pltfm_thread_return_t;
/* Datatype to represent the argument to a thread start routine */
typedef void*                               pltfm_thread_arg_t;

/* Datatype to represent thread start routines */
typedef pltfm_thread_return_t (*PLTFM_THREAD_START_ROUTINE)(pltfm_thread_arg_t);

/* Thread return type constants. */
#define PLTFM_THREAD_RETURN_VAL_SUCCESS     NULL
#define PLTFM_THREAD_RETURN_VAL_FAILURE     NULL


/* --------------------------------------------------- Function Declarations */

/* 
 * Task/Thread Creation Primitives
 */

/*
INT32 pltfm_thread_create(OUT pltfm_thread_t*,
                           IN pltfm_thread_attr_t*,
                           IN PLTFM_THREAD_START_ROUTINE,
                           IN pltfm_thread_arg_t);
*/
#define pltfm_thread_create(t, t_at, sr, t_ar)  ((INT32)pthread_create((pltfm_thread_t*)(t),\
                                                                       (pltfm_thread_attr_t*)(t_at),\
                                                                       (PLTFM_THREAD_START_ROUTINE)(sr),\
                                                                       (pltfm_thread_arg_t)(t_ar)))

/*
INT32 pltfm_thread_cancel(IN pltfm_thread_t);
*/
#define pltfm_thread_cancel(t)              ((INT32)pthread_cancel((pltfm_thread_t)(t)))

/*
INT32 pltfm_thread_join(IN pltfm_thread_t,
                        OUT void **retval);
*/
#define pltfm_thread_join(t, retval)        ((INT32)pthread_join((pltfm_thread_t)(t), (retval)))

/*
INT32 pltfm_thread_attr_init(OUT pltfm_thread_attr_t*);
*/
#define pltfm_thread_attr_init(t_at)        ((INT32)pthread_attr_init((pltfm_thread_attr_t)(t_at)))

/*
INT32 pltfm_thread_attr_destroy(IN pltfm_thread_attr_t*);
*/
#define pltfm_thread_attr_destroy(t_at)     ((INT32)pthread_attr_destroy((pltfm_thread_attr_t)(t_at)))


/* 
 * Task/Thread Synchronization Primitives
 */
/*
INT32 pltfm_mutex_init(OUT pltfm_mutex_t* ,
                       IN pltfm_mutexattr_t* );
*/
#define pltfm_mutex_init(m, m_at)           ((INT32)pthread_mutex_init((pltfm_mutex_t*)(m),\
                                                                       (pltfm_mutexattr_t*)(m_at)))

/*
INT32 pltfm_mutex_destroy(IN pltfm_mutex_t* );
*/
#define pltfm_mutex_destroy(m)              ((INT32)pthread_mutex_destroy((pltfm_mutex_t*)(m)))

/*
INT32 pltfm_mutex_lock(INOUT pltfm_mutex_t* );
*/
#define pltfm_mutex_lock(m)                 ((INT32)pthread_mutex_lock((pltfm_mutex_t*)(m)))

/*
INT32 pltfm_mutex_unlock(INOUT pltfm_mutex_t* );
*/
#define pltfm_mutex_unlock(m)               ((INT32)pthread_mutex_unlock((pltfm_mutex_t*)(m)))

/*
INT32 pltfm_cond_init(OUT pltfm_cond_t* ,
                      IN pltfm_condattr_t* );
*/
#define pltfm_cond_init(c, c_at)            ((INT32)pthread_cond_init((pltfm_cond_t*)(c),\
                                                                      (pltfm_condattr_t*)(c_at)))

/*
INT32 pltfm_cond_destroy(OUT pltfm_cond_t* );
*/
#define pltfm_cond_destroy(c)               ((INT32)pthread_cond_destroy((pltfm_cond_t*)(c)))

/*
INT32 pltfm_cond_wait(INOUT pltfm_cond_t* ,
                      INOUT pltfm_mutex_t* );
*/
#define pltfm_cond_wait(c, c_m)             ((INT32)pthread_cond_wait((pltfm_cond_t*)(c),\
                                                                      (pltfm_mutex_t*)(c_m)))

/*
INT32 pltfm_cond_signal(INOUT pltfm_cond_t* );
*/
#define pltfm_cond_signal(c)                ((INT32)pthread_cond_signal((pltfm_cond_t*)(c)))


/* 
 * Task/Thread Delay Primitives
 */

/* 
 * Time in seconds.

void pltfm_sleep (IN UINT32 );
*/
#define pltfm_sleep(sec)                    (void)sleep((unsigned int)(sec))

/*
 * Time in milli-seconds.

void pltfm_msleep(IN UINT32 );
*/
#define pltfm_msleep(msec)                  (void)usleep((useconds_t)(1000 * (msec)))

/*
 * Time in micro-seconds.

void pltfm_usleep(IN UINT32 );
*/
#define pltfm_usleep(usec)                  (void)usleep((useconds_t)(usec))


/* 
 * POSIX compliant. Not part of standard C. 
 */
int clock_getres(clockid_t clk_id, struct timespec *res);
int clock_gettime(clockid_t clk_id, struct timespec *tp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_OS_ABSTRACTION_ */
