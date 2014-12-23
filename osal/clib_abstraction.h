/*
 * Abstractions for functions declared in C standard header files.
 */

#ifndef _H_CLIB_ABSTRACTION_
#define _H_CLIB_ABSTRACTION_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <assert.h>

/* Abstractions for functions testing and mapping characters. */
#define pltfm_isalnum(c)                    ((INT32)isalnum((int)(c)))
#define pltfm_isalpha(c)                    ((INT32)isalpha((int)(c)))
#define pltfm_isdigit(c)                    ((INT32)isdigit((int)(c)))
#define pltfm_isxdigit(c)                   ((INT32)isxdigit((int)(c)))
#define pltfm_islower(c)                    ((INT32)islower((int)(c)))
#define pltfm_isupper(c)                    ((INT32)isupper((int)(c)))
#define pltfm_isprint(c)                    ((INT32)isprint((int)(c)))
#define pltfm_tolower(c)                    ((INT32)tolower((int)(c)))
#define pltfm_toupper(c)                    ((INT32)toupper((int)(c)))

/* Abstractions for String library functions. */
#define pltfm_strlen(s)                     ((UINT32)strlen((char *)(s)))
#define pltfm_strcpy(d, s)                  ((CHAR *)strcpy((char *)(d), (char *)(s)))
#define pltfm_strncpy(d, s, n)              ((CHAR *)strncpy((char *)(d), (char *)(s), (size_t)(n)))
#define pltfm_strcmp(s1, s2)                ((INT32)strcmp((char *)(s1), (char *)(s2)))
#define pltfm_strncmp(s1, s2, n)            ((INT32)strncmp((char *)(s1), (char *)(s2), (size_t)(n)))
#define pltfm_strcat(d, s)                  ((CHAR *)strcat((char *)(d), (char *)(s)))
#define pltfm_strncat(d, s, n)              ((CHAR *)strncat((char *)(d), (char *)(s), (size_t)(n)))
#define pltfm_strchr(s, c)                  ((CHAR *)strchr((char *)(s), (int)(c)))
#define pltfm_strrchr(s, c)                 ((CHAR *)strrchr((char *)(s), (int)(c)))
#define pltfm_strstr(s, ss)                 ((CHAR *)strstr((char *)(s), (char *)(ss)))

/* Abstractions for Memory manipulation functions. */
#define pltfm_memchr(b, c, n)               memchr((b), (int)(c), (size_t)(n))
#define pltfm_memset(b, c, n)               memset((b), (int)(c), (size_t)(n))
#define pltfm_memcmp(b1, b2, n)             ((INT32)memcmp((b1), (b2), (size_t)(n)))
#define pltfm_memcpy(b1, b2, n)             memcpy((b1), (b2), (size_t)(n))
#define pltfm_memmove(b1, b2, n)            memmove((b1), (b2), (size_t)(n))
#define pltfm_malloc(size)                  malloc((size_t)(size))
#define pltfm_calloc(n, size)               calloc((size_t)(n), (size_t)(size))
#define pltfm_free(b)                       free((b))

/* Abstractions for Random Number Generator functions. */
#define pltfm_srand(seed)                   srand((unsigned int)(seed))
#define pltfm_rand()                        ((INT32)rand())

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CLIB_ABSTRACTION_ */
