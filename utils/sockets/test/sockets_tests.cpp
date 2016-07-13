#include "platform.h"
#include "gtest/gtest.h"
#include "test_utils.h"
#include "logger.h"
#include "sockets.h"

class Sockets_Tests : public ::testing::Test
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

class Test_socket_init_Neg: public ::Sockets_Tests { };

TEST_F(Test_socket_init_Neg, invalid_input)
{
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 512, 0, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 512, 0, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 1023, 0, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 1023, 0, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 512, 0, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 512, 0, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 1023, 0, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 1023, 0, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 512, 10, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 512, 10, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 1023, 10, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 1023, 10, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 512, 10, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 512, 10, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 1023, 10, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 1023, 10, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 1024, 0, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 1024, 0, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 1234, 0, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 1234, 0, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 65000, 0, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_TRUE, 65000, 0, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 1024, 0, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 1024, 0, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 1234, 0, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 1234, 0, "127.0.0.1"));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 65000, 0, NULL));
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, sockets_init(BOOL_FALSE, 65000, 0, "127.0.0.1"));
}

class Test_socket_alloc_Neg: public ::Sockets_Tests { };
TEST_F(Test_socket_alloc_Neg, invalid_input)
{
    EXPECT_EQ(SOCKETS_INVALID_FUNC_PARAMS, socket_alloc(NULL));
}

TEST_F(Test_socket_alloc_Neg, invalid_setup)
{
    SOCKET_HDL hdl;

#ifdef WINDOWS
    EXPECT_EQ(SOCKETS_CREATE_FAILED, socket_alloc(&hdl));
#else /* WINDOWS */
    EXPECT_EQ(SOCKETS_CONNECT_FAILED, socket_alloc(&hdl));
#endif /* WINDOWS */
}
