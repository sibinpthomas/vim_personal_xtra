/*
 * Definition of special types and constants of those types.
 * Generic utility macro functions.
 * Abstraction for certain C storage classes and type qualifiers.
 */

#ifndef _H_PLTFM_AUXILIARY_
#define _H_PLTFM_AUXILIARY_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/* 
 * Special data types.
 */
#ifndef API_RESULT
/* Definition of API_RESULT */
typedef UINT16                              API_RESULT;
#endif /* API_RESULT */

#ifndef BOOLEAN
/* Definition of BOOLEAN */
typedef UINT8                               BOOLEAN;
#endif /* BOOLEAN */


/* 
 * Return type constants.
 */
#ifndef API_SUCCESS
/* Definition of API_SUCCESS */
#define API_SUCCESS                         ((API_RESULT)0u)
#endif /* API_SUCCESS */

#ifndef API_FAILURE
/* Definition of API_FAILURE */
#define API_FAILURE                         ((API_RESULT)~(0u))
#endif /* API_FAILURE */


/* 
 * BOOLEAN type constants.
 */
#ifndef BOOL_TRUE
/* Definition of BOOL_TRUE */
#define BOOL_TRUE                           ((BOOLEAN)1u)
#endif /* BOOL_TRUE */

#ifndef BOOL_FALSE
/* Definition of BOOL_FALSE */
#define BOOL_FALSE                          ((BOOLEAN)0u)
#endif /* BOOL_FALSE */


/* Bit manipulation primitives. */
#define BIT(n)                              ((unsigned)(n))
#define EXTRACT_BITNUM(val, bitnum)         (((val) >> (bitnum)) & 1)
#define SET_BITNUM(val, bitnum)             ((val) |= (1 << (bitnum)))
#define CLR_BITNUM(val, bitnum)             ((val) &= (~(1 << (bitnum))))


/* Host Unsigned integer type to UINT8 BE/LE buffer. */
UINT16 _htole_bufn(UINTMAX hnum, UINT8 *buf, UINT16 nbytes);
UINT16 _htobe_bufn(UINTMAX hnum, UINT8 *buf, UINT16 nbytes);

#define H2LE_BUFN(hnum, buf, nbytes)        _htole_bufn((hnum), (buf), (UINT16)(nbytes))
#define H2BE_BUFN(hnum, buf, nbytes)        _htobe_bufn((hnum), (buf), (UINT16)(nbytes))

#define H2LE_BUF(hnum, buf)                 _htole_bufn((hnum), (buf), (UINT16)sizeof(hnum))
#define H2BE_BUF(hnum, buf)                 _htobe_bufn((hnum), (buf), (UINT16)sizeof(hnum))

/* UINT8 BE/LE buffer to Host Unsigned integer type. */
UINT16 _letoh_bufn(UINT8 *buf, void *hnum, UINT16 nbytes);
UINT16 _betoh_bufn(UINT8 *buf, void *hnum, UINT16 nbytes);

#define LE2H_BUFN(buf, hnum, nbytes)        _letoh_bufn((buf), &(hnum), (UINT16)(nbytes))
#define BE2H_BUFN(buf, hnum, nbytes)        _betoh_bufn((buf), &(hnum), (UINT16)(nbytes))

#define LE2H_BUF(buf, hnum)                 _letoh_bufn((buf), &(hnum), (UINT16)sizeof(hnum))
#define BE2H_BUF(buf, hnum)                 _betoh_bufn((buf), &(hnum), (UINT16)sizeof(hnum))


/* Macro to calculate the length of arrays irrespective of their types. */
#define LEN_OF_ARRAY(array)                 (sizeof((array))/sizeof((array)[0]))


/* Compile time assert declaration. */
#define CT_ASSERT_ON(condition)             extern void ct_assert(char [sizeof(char[1 - 2*!!(condition)])])
/* Compile time assert statement. */
#define CT_ASSERT_STMT_ON(condition)        ((void)sizeof(char[1 - 2*!!(condition)]))


/* Definition for IN, OUT & INOUT Function/API parameter qualifiers. */
#define IN
#define OUT
#define INOUT


/*
 * Abstraction for certain C storage classes and type qualifiers to enable the
 * optional turning off/on of these qualifiers/storage-class-definitions.
 */
/* Declaration of 'static' variable/function */
#ifdef PLTFM_NO_STATIC_DECL
#define DECL_STATIC
#else  /* PLTFM_NO_STATIC_DECL */
#define DECL_STATIC                         static
#endif /* PLTFM_NO_STATIC_DECL */

/* Declaration of 'const' variable */
#ifdef PLTFM_NO_CONST_DECL
#define DECL_CONST
#else  /* PLTFM_NO_CONST_DECL */
#define DECL_CONST                          const
#endif /* PLTFM_NO_CONST_DECL */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_PLTFM_AUXILIARY_ */
