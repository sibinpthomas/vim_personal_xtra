#ifndef _H_TEST_UTILS_
#define _H_TEST_UTILS_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef enum {TEST_PASS, TEST_FAIL} TEST_RESULTS;

TEST_RESULTS get_user_confirmation(void);

TEST_RESULTS compare_files(const char* , const char* );

void test_delay(double );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_TEST_UTILS_ */
