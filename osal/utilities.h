/*
 * Generic utility macro functions.
 */

#ifndef _H_UTILITIES_
#define _H_UTILITIES_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Bit manipulation primitives. */
#define BIT(n)                              ((unsigned)(n))
#define EXTRACT_BITNUM(val, bitnum)         (((val) >> (bitnum)) & 1)
#define SET_BITNUM(val, bitnum)             ((val) |= (1 << (bitnum)))
#define CLR_BITNUM(val, bitnum)             ((val) &= (~(1 << (bitnum))))

/* Macro to calculate the length of arrays irrespective of their types. */
#define LEN_OF_ARRAY(array)                 (sizeof((array))/sizeof((array)[0]))

/* Compile time assert declaration. */
#define CT_ASSERT_ON(condition)             extern void ct_assert(char [sizeof(char[1 - 2*!!(condition)])])
/* Compile time assert statement. */
#define CT_ASSERT_STMT_ON(condition)        ((void)sizeof(char[1 - 2*!!(condition)]))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_UTILITIES_ */
