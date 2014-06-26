
/**
 *  @file BT_os.c
 *
 *  This source file is part of EtherMind OS Abstraction module.
 *  In this file the platform dependent library calls are abstracted
 *  and mapped to calls used within the EtherMind Stack.
 *
 *  Version: Windows User Mode
 */

/*
 *  Copyright (C) 2000-2008. MindTree Limited.
 *  All rights reserved.
 */

/* -------------------------------------------- Header File Inclusion */
#include "BT_os.h"


/* -------------------------------------------- External Global Variables */


/* -------------------------------------------- Exported Global Variables */


/* -------------------------------------------- Static Global Variables */


/* -------------------------------------------- Functions */
/**
 *  \fn BT_thread_create
 *
 *  \brief To create a task/thread
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Create call.
 *  It creates a new thread of control  that  executes concurrently  with
 *  the  calling  thread.
 *
 *  \param thread (OUT)
 *         Caller allocated location to hold the Thread ID
 *         for the newly Created Thread
 *  \param attr (IN)
 *         The Thread Attributes, pre-initialized using BT_thread_attr_init()
 *  \param start_routine (IN)
 *         The Thread Start Routine for the newly created task/thread.
 *         Upon creation, the new task/thread calls this function,
 *         passing it 'thread_args' as the argument
 *  \param thread_args (IN)
 *         This parameter points to a caller allocated "resident" memory
 *         location, containing the arguments to be passed to
 *         the newly created task's/thread's start routine
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 BT_thread_create
      (
          OUT BT_thread_type *         thread,
          IN  BT_thread_attr_type *    attr,
          IN  void * (* start_routine)(void *),
          IN  void *                   thread_args
      )
{
    HANDLE handle;
    DWORD thread_id;

    handle = CreateThread
             (
                 attr,
                 0,
                 (LPTHREAD_START_ROUTINE) start_routine,
                 (LPVOID) thread_args,
                 0,
                 &thread_id
             );

    if (handle == NULL)
    {
        return -1;
    }
    SetThreadPriority
    (
        handle,
        THREAD_PRIORITY_TIME_CRITICAL
    );

    *thread = handle;

    return 0;
}


/**
 *  \fn BT_thread_attr_init
 *
 *  \brief To initialize a task's/thread's attributes to default settings
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread attribute init
 *  call. It initializes the given thread attribute object with default
 *  values.
 *
 *  \param attr (OUT)
 *         Thread attribute object to be initialized
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 BT_thread_attr_init
      (
          OUT BT_thread_attr_type *    attr
      )
{
    (*attr).nLength              = sizeof(SECURITY_ATTRIBUTES);
    (*attr).lpSecurityDescriptor = NULL;
    (*attr).bInheritHandle       = FALSE;

    return 0;
}


/**
 *  \fn BT_thread_mutex_init
 *
 *  \brief To initialize a Mutex object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Mutex Init Call.
 *  It creates or initializes the mutex object and fills it with default
 *  values for the attributes.
 *
 *  \param mutex (OUT)
 *         The Mutex variable to be Initialized
 *  \param mutex_attr (IN)
 *         Attribute of the mutex variable on initialization
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 BT_thread_mutex_init
      (
          OUT BT_thread_mutex_type *         mutex/* ,
          IN  BT_thread_mutex_attr_type *    mutex_attr */
      )
{
    *mutex = NULL;
    *mutex = CreateMutex (NULL, FALSE, NULL);

    if (NULL == *mutex)
    {
        return -1;
    }

    return 0;
}


/**
 *  \fn BT_thread_mutex_lock
 *
 *  \brief To lock a Mutex object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Mutex Lock call.
 *  It locks the given mutex.
 *  If the given mutex is currently unlocked, it becomes locked and owned by
 *  the calling thread, and this routine returns immediately.
 *  If the mutex is already locked by another thread, this routine suspends
 *  the calling thread until the mutex is unlocked.
 *
 *  \param mutex (INOUT)
 *         The Mutex variable to be locked
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 BT_thread_mutex_lock
      (
          INOUT BT_thread_mutex_type *    mutex
      )
{
    DWORD retval;

    retval = WaitForSingleObject (*mutex, INFINITE);

    if (retval == WAIT_FAILED)
    {
        /* Wait */
        Sleep (1000*1000);
        return -1;
    }

    return 0;
}


/**
 *  \fn BT_thread_mutex_unlock
 *
 *  \brief To unlock a Mutex object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Mutex Unlock call.
 *  It unlocks the given mutex. The mutex is assumed to be locked and
 *  owned by the calling thread.
 *
 *  \param mutex (INOUT)
 *         The Mutex variable to be unlocked
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 BT_thread_mutex_unlock
      (
          INOUT BT_thread_mutex_type *    mutex
      )
{
    INT32 retval;

    retval = ReleaseMutex (*mutex);

    if (retval == 0)
    {
        return -1;
    }

    return 0;
}


/**
 *  \fn BT_thread_cond_init
 *
 *  \brief To initialize a Conditional Variable object
 *
 *  \par Description:
 *  This function implements the Conditional Variable Initialisation call.
 *  It initializes the given conditional variable.
 *
 *  \param cond (OUT)
 *         The Conditional Variable
 *  \param cond_attr (IN)
 *         Attributes of the conditional variable on initialization
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 *
 *  \note
 *  A conditional variable is a synchronization primitive that allows
 *  threads to suspend execution and relinquish the processors
 *  until some predicate on shared resources is satisfied.
 */
INT32 BT_thread_cond_init
      (
          OUT BT_thread_cond_type *         cond,
          IN  BT_thread_cond_attr_type *    cond_attr
      )
{
    *cond = NULL;

    *cond = CreateEvent (cond_attr, TRUE, FALSE, NULL);

    if (*cond == NULL)
    {
        return -1;
    }

    return 0;
}


/**
 *  \fn BT_thread_cond_wait
 *
 *  \brief To wait on a Conditional Variable object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Wait on
 *  Conditional Variable call.
 *  It waits for the conditional variable to be signaled.
 *  This routine releases the  mutex and waits for the condition variable
 *  to  be signaled. The execution of the calling thread is suspended
 *  until the conditional variable is signaled.
 *  The mutex must be locked by the calling thread before
 *  calling this function. Before returning to the calling thread,
 *  it will re-acquire the mutex variable.
 *
 *  \param cond (INOUT)
 *         The Conditional Variable
 *  \param cond_mutex (INOUT)
 *         The Conditional Mutex Variable
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 BT_thread_cond_wait
      (
          INOUT BT_thread_cond_type *     cond,
          INOUT BT_thread_mutex_type *    cond_mutex
      )
{
    DWORD retval;

    /*
     *  Mutex needs to be released initially then followup wait
     *  for Cond to happen
     */
    ReleaseMutex (*cond_mutex);

    retval = WaitForSingleObject (*cond, INFINITE);

    if (retval == WAIT_FAILED)
    {
        WaitForSingleObject (*cond_mutex, INFINITE);

        return -1;
    }

    ResetEvent (*cond);
    WaitForSingleObject (*cond_mutex, INFINITE);

    return 0;
}


/**
 *  \fn BT_thread_cond_signal
 *
 *  \brief To signal a Conditional Variable object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Signalling on
 *  Conditional Variable Call.
 *  It will signal and restart one of the threads that are waiting on
 *  the given conditional variable.
 *
 *  \param cond (INOUT)
 *         The Conditional Variable to signal
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 BT_thread_cond_signal
      (
          INOUT BT_thread_cond_type *    cond
      )
{
    INT32 retval;

    retval = SetEvent (*cond);

    if (retval == 0)
    {
        return -1;
    }

    return 0;
}

#ifndef MEMWATCH
/**
 *  \fn BT_alloc_mem
 *
 *  \brief To allocate memory dynamically
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Memory Allocation call.
 *  It allocates memory of 'size' bytes and returns a pointer to
 *  the allocated memory.
 *
 *  \param size (IN)
 *         The number of bytes to allocate
 *
 *  \return void * :  Pointer to the newly Allocated Memory segment.
 *                    NULL, in case of failure
 */
void * BT_alloc_mem ( IN UINT32 size )
{
    void * buf;

    buf = (void *) malloc ((size_t) size);

#ifdef BT_STATUS
    if (NULL == buf)
    {
        /* Memory Allocated Failed: Updated EtherMind Status Flag */
        BT_status_set_bit (STATUS_BIT_MEM_ALLOC_FAILED, STATUS_BIT_SET);
    }
#endif /* BT_STATUS */

    return buf;
}


/**
 *  \fn BT_free_mem
 *
 *  \brief To free dynamically allocated memory
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Memory Free call.
 *  It frees the memory space specified by the given pointer.
 *
 *  \param ptr (IN)
 *         Pointer to the Memory location to be freed
 *
 *  \return None
 */
void BT_free_mem ( IN void * ptr )
{
    free (ptr);
    return;
}
#endif /* MEMWATCH */

/**
 *  \fn BT_mem_copy
 *
 *  \brief To copy memory from one location to another
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Memory Copy call.
 *  It copies 'size' bytes of memory area from 'src' to 'dest'.
 *
 *  \param dest (IN)
 *         Pointer to the Destination Memory location
 *  \param src (IN)
 *         Pointer to the Source Memory Location
 *  \param size (IN)
 *         Number of Bytes to Copy
 *
 *  \return
 *
 *  \note
 *  This routine might not behave as expected if the source and
 *  destination memory area overlaps.
 */
void * BT_mem_copy ( IN void * dest, IN const void * src, IN UINT32 size )
{
    return ((void *) memcpy (dest, src, (size_t) size));
}

/**
 *  \fn BT_mem_move
 *
 *  \brief To copy memory from one location to another
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Memory Move call.
 *  It copies 'size' bytes of memory area from 'src' to 'dest'.
 *
 *  \param dest (IN)
 *         Pointer to the Destination Memory location
 *  \param src (IN)
 *         Pointer to the Source Memory Location
 *  \param size (IN)
 *         Number of Bytes to Copy
 *
 *  \return
 *
 *  \note
 *  This routine might not behave as expected if the source and
 *  destination memory area overlaps.
 */
void * BT_mem_move ( IN void * dest, IN const void * src, IN UINT32 size )
{
    return ((void *) memmove (dest, src, (size_t) size));
}

INT32 BT_mem_cmp ( IN const void *dest, IN const void *src, IN UINT32 size)
{
    return memcmp (dest, src, (size_t)size);
}


/**
 *  \fn BT_sleep
 *
 *  \brief To delay execution for a specified number of seconds
 *
 *  \par Description:
 *  This function implements the OS Wrapper for a Thread or task to sleep
 *  for specified number of seconds.
 *
 *  \param tm (IN)
 *         Number of seconds the calling task or thread wants to sleep
 *
 *  \return None
 *
 */
void BT_sleep ( IN UINT32 tm )
{
    Sleep (tm * 1000);
    return;
}


/**
 *  \fn BT_usleep
 *
 *  \brief To delay execution for a specified number of microseconds
 *
 *  \par Description:
 *  This function implements the OS Wrapper for a Thread or task to sleep
 *  for specified number of micro-seconds.
 *
 *  \param tm (IN)
 *         Number of micro-seconds the calling task or thread wants to sleep
 *
 *  \return None
 *
 */
void BT_usleep( IN UINT32 tm )
{
    Sleep ((tm + 999)/1000);
    return;
}


