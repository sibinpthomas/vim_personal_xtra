#include "platform.h"
#include "gtest/gtest.h"
#include "logger.h"

#define BUFSIZE                             (8u)
#define TX_ATTEMPT_LIMIT                    (5u)
#define RX_ATTEMPT_LIMIT                    (5u)
#define RCS_ROOT                            "resources/"

class Test_Utils_Test : public ::testing::Test
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

        virtual void SetUp()
        {
        }

        virtual void TearDown()
        {
        }
};

class Test_get_user_confirmation: public ::Test_Utils_Test { };
class Test_compare_files: public ::Test_Utils_Test { };

TEST_F(Test_get_user_confirmation, t01)
{
    UINT8 buf[16] = {0};
    UINT16_F i;

    for (i = 0; i < sizeof(buf); i++)
    {
        buf[i] = i + 1;
    }

    log_simple(("SIMPLE\n"));
    log_buffer(buf, sizeof(buf));
    log_trace(("TRACE\n"));
    log_info(("INFO\n"));
    log_warning(("WARNING\n"));
    log_error(("ERROR\n"));
    log_critical(("CRITICAL\n"));
    log_temp(("TEMPORARY\n"));
    log_simple_temp(("SIMPLE TEMPORARY\n"));
    log_buffer_temp(buf, sizeof(buf));
}
