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
#include <errno.h>

/* Abstractions for functions testing and mapping characters. */
#define pltfm_isalnum(c)                    isalnum((int)(c))
#define pltfm_isalpha(c)                    isalpha((int)(c))
#define pltfm_isdigit(c)                    isdigit((int)(c))
#define pltfm_isxdigit(c)                   isxdigit((int)(c))
#define pltfm_islower(c)                    islower((int)(c))
#define pltfm_isupper(c)                    isupper((int)(c))
#define pltfm_isprint(c)                    isprint((int)(c))
#define pltfm_tolower(c)                    tolower((int)(c))
#define pltfm_toupper(c)                    toupper((int)(c))

/* Abstractions for String library functions. */
#define pltfm_strlen(s)                     strlen((char *)(s))
#define pltfm_strcpy(d, s)                  strcpy((char *)(d), (char *)(s))
#define pltfm_strncpy(d, s, n)              strncpy((char *)(d), (char *)(s), (size_t)(n))
#define pltfm_strcmp(s1, s2)                strcmp((char *)(s1), (char *)(s2))
#define pltfm_strncmp(s1, s2, n)            strncmp((char *)(s1), (char *)(s2), (size_t)(n))
#define pltfm_strcat(d, s)                  strcat((char *)(d), (char *)(s))
#define pltfm_strncat(d, s, n)              strncat((char *)(d), (char *)(s), (size_t)(n))
#define pltfm_strchr(s, c)                  strchr((char *)(s), (int)(c))
#define pltfm_strrchr(s, c)                 strrchr((char *)(s), (int)(c))
#define pltfm_strstr(s, ss)                 strstr((char *)(s), (char *)(ss))

/* Abstractions for functions converting between numbers and strings. */
#define pltfm_strtoul(s, endptr, base)      strtoul((char *)(s), (char **)(endptr), (int)(base))
    
/* Abstractions for Memory manipulation functions. */
#define pltfm_memchr(b, c, n)               memchr((b), (int)(c), (size_t)(n))
#define pltfm_memset(b, c, n)               memset((b), (int)(c), (size_t)(n))
#define pltfm_memcmp(b1, b2, n)             memcmp((b1), (b2), (size_t)(n))
#define pltfm_memcpy(d, s, n)               memcpy((d), (s), (size_t)(n))
#define pltfm_memmove(d, s, n)              memmove((d), (s), (size_t)(n))
#define pltfm_malloc(size)                  malloc((size_t)(size))
#define pltfm_calloc(n, size)               calloc((size_t)(n), (size_t)(size))
#define pltfm_free(b)                       free((b))

/* Abstractions for Random Number Generator functions. */
#define pltfm_srand(seed)                   srand((unsigned int)(seed))
#define pltfm_rand()                        rand()

/* Safe string functions. */
#define pltfm_strcpy_s(d, s, d_size)        d[d_size - 1] = '\0';\
                                            strncpy((char *)(d), (char *)(s), (size_t)(d_size - 1))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_CLIB_ABSTRACTION_ */
