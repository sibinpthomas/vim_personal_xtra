/*
 * Abstractions for shell (empty) Thread handling primitives.
 */

#ifndef _H_NULL_OS_
#define _H_NULL_OS_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

INT32 null_thread_create(INT32* , const INT32* ,
                         void *(*sr) (void *), void * );
INT32 null_thread_cancel(INT32 );
INT32 null_thread_join(INT32 , void ** );
INT32 null_thread_attr_init(INT32 t_at);
INT32 null_thread_attr_destroy(INT32 t_at);
INT32 null_thread_yield(void);
INT32 null_thread_mutex_init(INT32* , const INT32* );
INT32 null_thread_mutex_destroy(INT32* );
INT32 null_thread_mutex_lock(INT32* );
INT32 null_thread_mutex_unlock(INT32* );
INT32 null_thread_cond_init(INT32* , const INT32* );
INT32 null_thread_cond_destroy(INT32* );
INT32 null_thread_cond_wait(INT32* , INT32* );
INT32 null_thread_cond_signal(INT32* );

void null_sleep (UINT32 );
void null_msleep(UINT32 );
void null_usleep(UINT32 );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_NULL_OS_ */
