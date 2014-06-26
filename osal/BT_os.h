
/**
 *  @file BT_os.h
 *
 *  This header file is part of EtherMind OS Abstraction module.
 *  In this file the platform specific data types are abstracted and
 *  mapped to data types used within the EtherMind Stack.
 *
 *  Version: Windows User Mode
 */

/*
 *  Copyright (C) 2000-2008. MindTree Limited.
 *  All rights reserved.
 */

#ifndef _H_BT_OS_
#define _H_BT_OS_

/* -------------------------------------------- Header File Inclusion */
/* Standard Headers */
#include <windows.h>
#include <winerror.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <io.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <assert.h>


/* -------------------------------------------- Global Definitions */
/* Definition for IN, OUT & INOUT Function/API Parameters */
#define IN
#define OUT
#define INOUT

/* -------------------------------------------- Structures/Data Types */
/* 'signed' data type of size '1 octet' */
#ifdef CHAR
#undef CHAR
#endif /* #ifndef CHAR */
typedef char CHAR;

/* 'signed' data type of size '1 octet' */
#ifndef WIN32
typedef char INT8;
#endif /* WIN32 */
/* 'unsigned' data type of size '1 octet' */
#ifdef UCHAR
#undef UCHAR
#endif /* #ifndef UCHAR */
typedef unsigned char UCHAR;

/* 'unsigned' data type of size '1 octet' */
#ifdef UINT8
#undef UINT8
#endif /* #ifndef UINT8 */
typedef unsigned char UINT8;

/* 'signed' data type of size '2 octet' */
#ifdef INT16
#undef INT16
#endif /* #ifndef INT16 */
typedef short int INT16;

/* 'unsigned' data type of size '2 octet' */
#ifdef UINT16
#undef UINT16
#endif /* #ifndef UINT16 */
typedef unsigned short int UINT16;

/* 'unsigned' data type of size '1 octet' */
#ifdef BOOLEAN
#undef BOOLEAN
#endif /* #ifndef BOOLEAN */
typedef unsigned char BOOLEAN;

/* Data type to represent Thread or Task Identifier */
typedef HANDLE BT_thread_type;

/* Data type to represent Thread or Task Attribute */
typedef SECURITY_ATTRIBUTES BT_thread_attr_type;

/* Data type to represent Mutex object */
typedef HANDLE BT_thread_mutex_type;

/* Data type to represent Attributes of a Mutex object */
typedef SECURITY_ATTRIBUTES BT_thread_mutex_attr_type;

/* Data type to represent Conditional Variable object */
typedef HANDLE BT_thread_cond_type;

/* Data type to represent Attributes of a Conditional Variable object */
typedef SECURITY_ATTRIBUTES BT_thread_cond_attr_type;

/* Definition of API_RESULT */
typedef UINT16    API_RESULT;

/* Definitions of API_SUCCESS & API_FAILURE */
#define API_SUCCESS             0x0000
#define API_FAILURE             0xFFFF

/* --------------------------------------------------- Macros */
/* Declaration of 'static' variable/function */
#ifdef BT_HAVE_STATIC_DECL
#define DECL_STATIC                     static
#else  /* BT_HAVE_STATIC_DECL */
#define DECL_STATIC
#endif /* BT_HAVE_STATIC_DECL */

/* Declaration of 'const' variable */
#ifdef BT_HAVE_CONST_DECL
#define DECL_CONST                      const
#else  /* BT_HAVE_CONST_DECL */
#define DECL_CONST
#endif /* BT_HAVE_CONST_DECL */

/* Abstractions for String library functions */
#define BT_str_len(s)                   strlen ((char *)(s))
#define BT_str_copy(d, s)               strcpy ((char *)(d), (char *)(s))
#define BT_str_ncopy(d, s, n)           strncpy ((char *)(d), (char *)(s), n)
#define BT_str_cmp(s1, s2)              strcmp ((char *)(s1), (char *)(s2))
#define BT_str_ncmp(s1, s2, n)          strncmp((char *)(s1), (char *)(s2), n)
#define BT_str_cat(d, s)                strcat ((char *)(d), (char *)(s))
#define BT_str_ncat(d, s, num)          strncat ((char *)(d), (char *)(s), (size_t)(num))
#define BT_str_chr(s, c)                strchr ((char *)(s), (int)(c))
#define BT_str_rchr(s, c)               strrchr ((char *)(s), (int)(c))
#define BT_str_str(s, ss)               strstr((char *)(s), (char *)(ss))

#define BT_mem_set                      memset
#define BT_memset                       memset
#define BT_memcmp                       BT_mem_cmp

#define EXTRACT_BITNUM(val, bitnum)                (((val) >> (bitnum)) & 1)
#define SET_BITNUM(val, bitnum)                    ((val) |= (1 << (bitnum)))
#define CLR_BITNUM(val, bitnum)                    ((val) &= (~(1 << (bitnum))))

#define BIT_0                           (0) 
#define BIT_1                           (1) 
#define BIT_2                           (2) 
#define BIT_3                           (3) 
#define BIT_4                           (4) 
#define BIT_5                           (5) 
#define BIT_6                           (6) 
#define BIT_7                           (7) 
#define BIT_8                           (8) 
#define BIT_9                           (9) 
#define BIT_10                          (10)
#define BIT_11                          (11)
#define BIT_12                          (12)
#define BIT_13                          (13)
#define BIT_14                          (14)
#define BIT_15                          (15)
#define BIT_16                          (16)
#define BIT_17                          (17)
#define BIT_18                          (18)
#define BIT_19                          (19)
#define BIT_20                          (20)
#define BIT_21                          (21)
#define BIT_22                          (22)
#define BIT_23                          (23)
#define BIT_24                          (24)
#define BIT_25                          (25)
#define BIT_26                          (26)
#define BIT_27                          (27)
#define BIT_28                          (28)
#define BIT_29                          (29)
#define BIT_30                          (30)
#define BIT_31                          (31)
/* -------------------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/* Task/Thread Creation Primitives */
INT32 BT_thread_create
      (
          OUT BT_thread_type *         thread,
          IN  BT_thread_attr_type *    thread_attr,
          IN  void * (* start_routine)(void *),
          IN  void *                   thread_args
      );

INT32 BT_thread_attr_init
      (
          OUT BT_thread_attr_type *    thread_attr
      );

/* Task/Thread Synchronization Primitives */
INT32 BT_thread_mutex_init
      (
          OUT BT_thread_mutex_type *         mutex/* ,
          IN  BT_thread_mutex_attr_type *    mutex_attr */
      );

INT32 BT_thread_mutex_lock
      (
          INOUT BT_thread_mutex_type *    mutex
      );

INT32 BT_thread_mutex_unlock
      (
          INOUT BT_thread_mutex_type *    mutex
      );

INT32 BT_thread_cond_init
      (
          OUT BT_thread_cond_type *         cond,
          IN  BT_thread_cond_attr_type *    cond_attr
      );

INT32 BT_thread_cond_wait
      (
          INOUT BT_thread_cond_type *     cond,
          INOUT BT_thread_mutex_type *    cond_mutex
      );

INT32 BT_thread_cond_signal
      (
          INOUT BT_thread_cond_type *    cond
      );

/* Memory Management Primitives */
#ifndef MEMWATCH
void * BT_alloc_mem ( IN UINT32 size );
void BT_free_mem ( IN void * ptr );
#else
#define BT_alloc_mem(size)    mwMalloc(size,__FILE__,__LINE__)
#define BT_free_mem(size)     mwFree(size,__FILE__,__LINE__)
#endif /* MEMWATCH */

void * BT_mem_copy ( IN void * dest, IN const void * src, IN UINT32 size );
void * BT_mem_move ( IN void * dest, IN const void * src, IN UINT32 size );
INT32 BT_mem_cmp ( IN const void *dest, IN const void *src, IN UINT32 size);

/* Task/Thread Delay Primitives */
void BT_sleep ( IN UINT32 tm );
void BT_usleep ( IN UINT32 tm );

#ifdef __cplusplus
};
#endif


#endif  /* _H_BT_OS_ */

