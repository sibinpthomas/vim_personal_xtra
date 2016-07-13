#include "platform.h"
#include "gtest/gtest.h"
#include "logger.h"
#include "socket_test_utils.h"

int g_err_code; 
#define get_last_error()        g_err_code 

extern "C"
{
    #include "socket_test_utils.c"
}

class Sockets_Test_Utils_Test : public ::testing::Test
{
    public:
        static void SetUpTestCase()
        {
            ASSERT_TRUE(API_SUCCESS == init_logger());
        }

        static void TearDownTestCase()
        {
            shutdown_logger();
        }
};

class Test_socket_err_desc : public ::Sockets_Test_Utils_Test,
                      public ::testing::WithParamInterface<int> { };

TEST_P(Test_socket_err_desc, valid_input)
{
    g_err_code = GetParam();  

    log_socket_error();
    printf("\n");
}

#ifdef WINDOWS
INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_socket_err_desc,
                        ::testing::Range(10004, 10102));

INSTANTIATE_TEST_CASE_P(InstantiationTwo,
                        Test_socket_err_desc,
                        ::testing::Range(11000, 11005));
#else /* WINDOWS */
INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_socket_err_desc,
                        ::testing::Range(0, 140));
#endif /* WINDOWS */
