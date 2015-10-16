/*
 * Abstractions for common variable types and also fixed-width variable types.
 */

#ifndef _H_GENERIC_TYPE_ABSTRACTION_
#define _H_GENERIC_TYPE_ABSTRACTION_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* 
 * Numeric data types.
 */
#include <limits.h>

/* Signed and Unsigned 16 and 32 bit integer types.*/
#if (!defined(INT16) || !defined(INT32) || \
        !defined(UINT16) || !defined(UINT32))

/* Checking if implementation limits are defined. */
#if (!defined(SHRT_MAX) || !defined(INT_MAX) || !defined(LONG_MAX) || \
        !defined(USHRT_MAX) || !defined(UINT_MAX) || !defined(ULONG_MAX))
    #error "Key implementation limits (int) not defined."
#endif /* Checking if implementation limits are defined. */

#if INT_MAX == 32767
    typedef int                             INT16;
    #if LONG_MAX == 2147483647L
        typedef long                        INT32;
    #else /* LONG_MAX */
        #error "LONG_MAX does not perfectly define a 32 bit type."
    #endif /* LONG_MAX */
#elif INT_MAX == 2147483647L
    typedef int                             INT32;
    #if SHRT_MAX == 32767
        typedef short                       INT16;
    #else /* SHRT_MAX */
        #error "SHRT_MAX does not perfectly define a 16 bit type."
    #endif /* SHRT_MAX */
#else
    #error "INT_MAX does not perfectly define a 16 or 32 bit type."
#endif /* INT_MAX */

#if UINT_MAX == 65535
    typedef unsigned int                    UINT16;
    #if ULONG_MAX == 4294967295L
        typedef unsigned long               UINT32;
    #else /* ULONG_MAX */
        #error "ULONG_MAX does not perfectly define a 32 bit type."
    #endif /* ULONG_MAX */
#elif UINT_MAX == 4294967295L
    typedef unsigned int                    UINT32;
    #if USHRT_MAX == 65535
        typedef unsigned short              UINT16;
    #else /* USHRT_MAX */
        #error "USHRT_MAX does not perfectly define a 16 bit type."
    #endif /* USHRT_MAX */
#else
    #error "UINT_MAX does not perfectly define a 16 or 32 bit type."
#endif /* UINT_MAX */

#endif /* INT16, UINT16, INT32, UINT32 */

/* Signed and Unsigned 8 bit integer types.*/
#if (!defined(INT8) || !defined(UINT8))

#if (!defined(SCHAR_MAX) || !defined(UCHAR_MAX))
    #error "Key implementation limits (char) not defined."
#endif /* Checking if implementation limits are defined. */

#if SCHAR_MAX == 127
    typedef signed char                     INT8;
#else
    #error "SCHAR_MAX does not perfectly define a 8 bit type."
#endif /* SCHAR_MAX */

#if UCHAR_MAX == 255
    typedef unsigned char                   UINT8;
#else
    #error "UCHAR_MAX does not perfectly define a 8 bit type."
#endif /* UCHAR_MAX */

#endif /* INT8, UINT8 */

/*
 * Fast integer types.
 */
typedef unsigned int                        UINT16_F;
typedef int                                 INT16_F;


/* 
 * Non-numeric data types 
 */
#ifndef CHAR
/* Definition of CHAR */
typedef char                                CHAR;
#endif /* CHAR */

#ifndef UCHAR
/* Definition of UCHAR */
typedef unsigned char                       UCHAR;
#endif /* UCHAR */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_GENERIC_TYPE_ABSTRACTION_ */
