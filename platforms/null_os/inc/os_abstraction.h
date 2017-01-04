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

#include "null_os.h"
#include "generic_type_abstraction.h"

/* Datatype to represent Thread or Task Identifier*/
typedef INT32                               pltfm_thread_t;
/* Datatype to represent Mutex object */
typedef INT32                               pltfm_mutex_t;
/* Datatype to represent Conditional Variableobject*/
typedef INT32                               pltfm_cond_t;
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
#define pltfm_thread_create(t, sr)          null_thread_create((pltfm_thread_t*)(t),\
                                                               (__THREAD_START_ROUTINE)(sr))

/*
int pltfm_thread_join(IN pltfm_thread_t,
                        OUT void **retval);
*/
#define pltfm_thread_join(t)                null_thread_join((pltfm_thread_t)(t))

/*
int pltfm_thread_yield(void);
*/
#define pltfm_thread_yield()                null_thread_yield()


/* 
 * Task/Thread Synchronization Primitives
 */
/*
int pltfm_mutex_init(OUT pltfm_mutex_t* );
*/
#define pltfm_mutex_init(m)                 null_thread_mutex_init((pltfm_mutex_t*)(m))

/*
int pltfm_mutex_destroy(IN pltfm_mutex_t* );
*/
#define pltfm_mutex_destroy(m)              null_thread_mutex_destroy((pltfm_mutex_t*)(m))

/*
int pltfm_mutex_lock(INOUT pltfm_mutex_t* );
*/
#define pltfm_mutex_lock(m)                 null_thread_mutex_lock((pltfm_mutex_t*)(m))

/*
int pltfm_mutex_unlock(INOUT pltfm_mutex_t* );
*/
#define pltfm_mutex_unlock(m)               null_thread_mutex_unlock((pltfm_mutex_t*)(m))

/*
int pltfm_cond_init(OUT pltfm_cond_t* );
*/
#define pltfm_cond_init(c)                  null_thread_cond_init((pltfm_cond_t*)(c))

/*
int pltfm_cond_destroy(OUT pltfm_cond_t* );
*/
#define pltfm_cond_destroy(c)               null_thread_cond_destroy((pltfm_cond_t*)(c))

/*
int pltfm_cond_wait(INOUT pltfm_cond_t* ,
                      INOUT pltfm_mutex_t* );
*/
#define pltfm_cond_wait(c, c_m)             null_thread_cond_wait((pltfm_cond_t*)(c),\
                                                                  (pltfm_mutex_t*)(c_m))

/*
int pltfm_cond_signal(INOUT pltfm_cond_t* );
*/
#define pltfm_cond_signal(c)                null_thread_cond_signal((pltfm_cond_t*)(c))


/* 
 * Task/Thread Delay Primitives
 */

/* 
 * Time in seconds.

void pltfm_sleep (IN UINT32 );
*/
#define pltfm_sleep(sec)                    null_sleep(sec)

/*
 * Time in milli-seconds.

void pltfm_msleep(IN UINT32 );
*/
#define pltfm_msleep(msec)                  null_msleep(msec)

/*
 * Time in micro-seconds.

void pltfm_usleep(IN UINT32 );
*/
#define pltfm_usleep(usec)                  null_usleep(usec)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_OS_ABSTRACTION_ */
