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
/* Datatype to represent Mutex object */
typedef pthread_mutex_t                     pltfm_mutex_t;
/* Datatype to represent Conditional Variable object */
typedef pthread_cond_t                      pltfm_cond_t;

/* > Internal - 
 *          Datatype to represent the return value of a thread start routine */
typedef void*                               __thread_return_t;
/* > Internal - 
 *          Datatype to represent the argument to a thread start routine */
typedef void*                               __thread_arg_t;

/* > Internal - 
 *          Datatype to represent thread start routines */
typedef __thread_return_t (*__THREAD_START_ROUTINE)(__thread_arg_t);

/*
 * Macro to declare a thread start routine.
 * As far the programmer is concerned the thread start routine has the
 * following signature - 
 *      void start_routine(void);
 * i.e. the start routine accepts no parameters and returns no value.
 */
#define DECL_THREAD_ROUTINE(sr)             __thread_return_t (sr)(__thread_arg_t __dummy_arg)

/* Macro to be used to exit a thread start routine. */
#define EXIT_THREAD_ROUTINE                 return (__thread_return_t)NULL


/* --------------------------------------------------- Function Declarations */

/* 
 * Task/Thread Creation Primitives
 */

/*
int pltfm_thread_create(OUT pltfm_thread_t*,
                        IN PLTFM_THREAD_START_ROUTINE);
*/
#define pltfm_thread_create(t, sr)          pthread_create((pthread_t*)(t),\
                                                           NULL,\
                                                           (__THREAD_START_ROUTINE)(sr),\
                                                           NULL)

/*
int pltfm_thread_join(IN pltfm_thread_t);
*/
#define pltfm_thread_join(t)                pthread_join((pthread_t)(t), NULL)

/*
int pltfm_thread_yield(void);
*/
#define pltfm_thread_yield()                pthread_yield()


/* 
 * Task/Thread Synchronization Primitives
 */
/*
int pltfm_mutex_init(OUT pltfm_mutex_t* );
*/
#define pltfm_mutex_init(m)                 pthread_mutex_init((pthread_mutex_t*)(m), NULL)

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
int pltfm_cond_init(OUT pltfm_cond_t* );
*/
#define pltfm_cond_init(c)                  pthread_cond_init((pthread_cond_t*)(c), NULL)

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_OS_ABSTRACTION_ */
