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
int pltfm_thread_create(OUT pltfm_thread_t*,
                        IN pltfm_thread_attr_t*,
                        IN PLTFM_THREAD_START_ROUTINE,
                        IN pltfm_thread_arg_t);
*/
#define pltfm_thread_create(t, t_at, sr, t_ar)  pthread_create((pthread_t*)(t),\
                                                               (pthread_attr_t*)(t_at),\
                                                               (void *(*) (void *))(sr),\
                                                               (void *)(t_ar))

/*
int pltfm_thread_cancel(IN pltfm_thread_t);
*/
#define pltfm_thread_cancel(t)              pthread_cancel((pthread_t)(t))

/*
int pltfm_thread_join(IN pltfm_thread_t,
                      OUT void **retval);
*/
#define pltfm_thread_join(t, retval)        pthread_join((pthread_t)(t), (retval))

/*
int pltfm_thread_attr_init(OUT pltfm_thread_attr_t*);
*/
#define pltfm_thread_attr_init(t_at)        pthread_attr_init((pthread_attr_t)(t_at))

/*
int pltfm_thread_attr_destroy(IN pltfm_thread_attr_t*);
*/
#define pltfm_thread_attr_destroy(t_at)     pthread_attr_destroy((pthread_attr_t)(t_at))

/*
int pltfm_thread_yield(void);
*/
#define pltfm_thread_yield()                pthread_yield()


/* 
 * Task/Thread Synchronization Primitives
 */
/*
int pltfm_mutex_init(OUT pltfm_mutex_t* ,
                     IN pltfm_mutexattr_t* );
*/
#define pltfm_mutex_init(m, m_at)           pthread_mutex_init((pthread_mutex_t*)(m),\
                                                               (pthread_mutexattr_t*)(m_at))

/*
int pltfm_mutex_destroy(IN pltfm_mutex_t* );
*/
#define pltfm_mutex_destroy(m)              pthread_mutex_destroy((pthread_mutex_t*)(m))

/*
int pltfm_mutex_lock(INOUT pltfm_mutex_t* );
*/
#define pltfm_mutex_lock(m)                 pthread_mutex_lock((pthread_mutex_t*)(m))

/*
int pltfm_mutex_unlock(INOUT pltfm_mutex_t* );
*/
#define pltfm_mutex_unlock(m)               pthread_mutex_unlock((pthread_mutex_t*)(m))

/*
int pltfm_cond_init(OUT pltfm_cond_t* ,
                    IN pltfm_condattr_t* );
*/
#define pltfm_cond_init(c, c_at)            pthread_cond_init((pthread_cond_t*)(c),\
                                                              (pthread_condattr_t*)(c_at))

/*
int pltfm_cond_destroy(OUT pltfm_cond_t* );
*/
#define pltfm_cond_destroy(c)               pthread_cond_destroy((pthread_cond_t*)(c))

/*
int pltfm_cond_wait(INOUT pltfm_cond_t* ,
                    INOUT pltfm_mutex_t* );
*/
#define pltfm_cond_wait(c, c_m)             pthread_cond_wait((pthread_cond_t*)(c),\
                                                              (pthread_mutex_t*)(c_m))

/*
int pltfm_cond_signal(INOUT pltfm_cond_t* );
*/
#define pltfm_cond_signal(c)                pthread_cond_signal((pthread_cond_t*)(c))


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
