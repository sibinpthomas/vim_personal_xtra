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

#include <Windows.h>
#include "generic_type_abstraction.h"

/* Datatype to represent Thread or Task Identifier */
typedef HANDLE                              pltfm_thread_t;
/* Datatype to represent Mutex object */
typedef SRWLOCK                             pltfm_mutex_t;
/* Datatype to represent Conditional Variable object */
typedef CONDITION_VARIABLE                  pltfm_cond_t;

/* > Internal - 
 *          Datatype to represent the return value of a thread start routine */
typedef DWORD                               __thread_return_t;
/* > Internal - 
 *          Datatype to represent the argument to a thread start routine */
typedef LPVOID                              __thread_arg_t;

/* > Internal - 
 *          Datatype to represent thread start routines */
typedef __thread_return_t(__stdcall *__THREAD_START_ROUTINE)(__thread_arg_t);

/*
 * Macro to declare a thread start routine.
 * As far the programmer is concerned the thread start routine has the
 * following signature - 
 *      void start_routine(void);
 * i.e. the start routine accepts no parameters and returns no value.
 */
#define DECL_THREAD_ROUTINE(sr)             __thread_return_t (sr)(__thread_arg_t __dummy_arg)

/* Macro to be used to exit a thread start routine. */
#define EXIT_THREAD_ROUTINE                 return (__thread_return_t)0


/* --------------------------------------------------- Function Declarations */

/* 
 * Task/Thread Creation Primitives
 */

/*
int pltfm_thread_create(OUT pltfm_thread_t*,
                        IN __THREAD_START_ROUTINE );
*/
#define pltfm_thread_create(t, sr)  \
    ((((*((pltfm_thread_t*)(t))) = CreateThread(NULL,\
                                                0,\
                                                (__THREAD_START_ROUTINE)(sr),\
                                                NULL,\
                                                0,\
                                                NULL)) \
                                                    != NULL) ? 0 : -1)

/*
int pltfm_thread_join(IN pltfm_thread_t);
*/
#define pltfm_thread_join(t)  \
    ((WaitForSingleObject((t), INFINITE) == WAIT_OBJECT_0) ? 0 : -1)

/*
int pltfm_thread_yield(void);
*/
#define pltfm_thread_yield()                ((0 != SwitchToThread()) ? 0 : -1)


/* 
 * Task/Thread Synchronization Primitives
 */
/*
int pltfm_mutex_init(OUT pltfm_mutex_t* );
*/
#define pltfm_mutex_init(m)                 (InitializeSRWLock((pltfm_mutex_t*)(m)), 0)

/*
int pltfm_mutex_destroy(IN pltfm_mutex_t* );
*/
#define pltfm_mutex_destroy(m)              (0)

/*
int pltfm_mutex_lock(INOUT pltfm_mutex_t* );
*/
#define pltfm_mutex_lock(m)                 (AcquireSRWLockExclusive((pltfm_mutex_t*)(m)), 0)

/*
int pltfm_mutex_unlock(INOUT pltfm_mutex_t* );
*/
#define pltfm_mutex_unlock(m)               (ReleaseSRWLockExclusive((pltfm_mutex_t*)(m)), 0)

/*
int pltfm_cond_init(OUT pltfm_cond_t* );
*/
#define pltfm_cond_init(c)                  (InitializeConditionVariable((pltfm_cond_t*)(c)), 0)

/*
int pltfm_cond_destroy(OUT pltfm_cond_t* );
*/
#define pltfm_cond_destroy(c)               (0)

/*
Any value that is not CONDITION_VARIABLE_LOCKMODE_SHARED is supposed to imply
that the SRW lock is in exclusive mode.
Defining CONDITION_VARIABLE_LOCKMODE_EXCLUSIVE so as to avert accidentally
providing the same value to the 'Flags' parameter as CONDITION_VARIABLE_LOCKMODE_SHARED
when 'Exclusive' was intended.

Refer - https://msdn.microsoft.com/en-us/library/windows/desktop/ms686304.aspx
*/
#define CONDITION_VARIABLE_LOCKMODE_EXCLUSIVE  (0)

/*
int pltfm_cond_wait(INOUT pltfm_cond_t* ,
                    INOUT pltfm_mutex_t* );
*/
#define pltfm_cond_wait(c, c_m)  \
    ((0 != SleepConditionVariableSRW((pltfm_cond_t*)(c),\
                                     (pltfm_mutex_t*)(c_m),\
                                     INFINITE,\
                                     CONDITION_VARIABLE_LOCKMODE_EXCLUSIVE)) ?\
                                         0 : -1)

/*
int pltfm_cond_signal(INOUT pltfm_cond_t* );
*/
#define pltfm_cond_signal(c)                (WakeConditionVariable((pltfm_cond_t*)(c)), 0)


/* 
 * Task/Thread Delay Primitives
 */

/* 
 * Time in seconds.

void pltfm_sleep (IN UINT32 );
*/
#define pltfm_sleep(sec)                    (void)Sleep((DWORD)((1000) * (sec)))

/*
 * Time in milli-seconds.

void pltfm_msleep(IN UINT32 );
*/
#define pltfm_msleep(msec)                  (void)Sleep((DWORD)(msec))

/* Declaration for uSleep() function. */
int uSleep(UINT32 );

/*
 * Time in micro-seconds.

void pltfm_usleep(IN UINT32 );
*/
#define pltfm_usleep(usec)                  (void)uSleep((UINT32)(usec))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_OS_ABSTRACTION_ */
