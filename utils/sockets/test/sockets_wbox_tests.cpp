#include "platform.h"
#include "gtest/gtest.h"
#include "logger.h"

extern "C"
{
    #include "sockets.c"
}

class Sockets_Wbox_Test : public ::testing::Test
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
