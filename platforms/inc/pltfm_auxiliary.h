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

/* Definition of API_RESULT */
typedef UINT16                              API_RESULT;

/* Definition of BOOL_T */
typedef int                                 BOOL_T;


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
 * BOOL_T type constants.
 */
#ifndef BOOL_TRUE
/* Definition of BOOL_TRUE */
#define BOOL_TRUE                           ((BOOL_T)(1==1))
#endif /* BOOL_TRUE */

#ifndef BOOL_FALSE
/* Definition of BOOL_FALSE */
#define BOOL_FALSE                          ((BOOL_T)(1==0))
#endif /* BOOL_FALSE */


/* Bit manipulation primitives. */
#define BIT(n)                              ((unsigned)(n))
#define EXTRACT_BITNUM(val, bitnum)         (((val) >> (bitnum)) & 1)
#define SET_BITNUM(val, bitnum)             ((val) |= (1 << (bitnum)))
#define CLR_BITNUM(val, bitnum)             ((val) &= (~(1 << (bitnum))))
#define IS_BITNUM_SET(val, bitnum)          (EXTRACT_BITNUM((val), (bitnum)) == 1)
#define IS_BITNUM_CLR(val, bitnum)          (EXTRACT_BITNUM((val), (bitnum)) == 0)
#define EXTRACT_BITFIELD(val, lsb, len)     (((val) & (((1u << (len)) - 1u) <<\
                                              (lsb))) >> (lsb))


/* Strips the characters Space, Horizontal Tab, CR, LF from both ends of
 * the input string. */
CHAR* pltfm_strstrip(CHAR* );

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

/* Mutex/Cond Var operations */
#define RETURN_VOID

#define PLTFM_MUTEX_INIT(mutex, MODULE, errval)\
    if (pltfm_mutex_init(&(mutex)) != 0)\
    {\
        MODULE##_ERR(("Failed to initialize " #MODULE " module level mutex.\n"));\
        return errval;\
    }
#define PLTFM_MUTEX_DESTROY(mutex, MODULE, errval)\
    if (pltfm_mutex_destroy(&(mutex)) != 0)\
    {\
        MODULE##_ERR(("Failed to destroy " #MODULE " module level mutex.\n"));\
        MODULE##_ERR((strerror(pltfm_mutex_destroy(&(mutex)))));\
        return errval;\
    }
#define PLTFM_MUTEX_LOCK(mutex, MODULE, errval)\
    if (pltfm_mutex_lock(&(mutex)) != 0)\
    {\
        MODULE##_ERR(("Failed to lock " #MODULE " module level mutex.\n"));\
        MODULE##_ERR(("%s\n", strerror(pltfm_mutex_lock(&(mutex)))));\
        return errval;\
    }
#define PLTFM_MUTEX_UNLOCK(mutex, MODULE, errval)\
    if (pltfm_mutex_unlock(&(mutex)) != 0)\
    {\
        MODULE##_ERR(("Failed to unlock " #MODULE " module level mutex.\n"));\
        MODULE##_ERR((strerror(pltfm_mutex_unlock(&(mutex)))));\
        return errval;\
    }
#define PLTFM_COND_INIT(condv, MODULE, errval)\
    if (pltfm_cond_init(&(condv)) != 0)\
    {\
        MODULE##_ERR(("Failed to initialize " #MODULE " conditional variable.\n"));\
        return errval;\
    }
#define PLTFM_COND_DESTROY(condv, MODULE, errval)\
    if (pltfm_cond_destroy(&(condv)) != 0)\
    {\
        MODULE##_ERR(("Failed to destroy " #MODULE " conditional variable.\n"));\
        return errval;\
    }
#define PLTFM_COND_SIGNAL(condv, MODULE, errval)\
    if (pltfm_cond_signal(&(condv)) != 0)\
    {\
        MODULE##_ERR(("Failed to signal " #MODULE " conditional variable.\n"));\
        return errval;\
    }
#define PLTFM_COND_WAIT(condv, cond_mutex, MODULE, errval)\
    if (pltfm_cond_wait(&(condv), &(cond_mutex)) != 0)\
    {\
        MODULE##_ERR(("Failed to wait on " #MODULE " conditional variable.\n"));\
        return errval;\
    }

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_PLTFM_AUXILIARY_ */
