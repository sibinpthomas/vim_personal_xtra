#include "platform.h"
#include "gtest/gtest.h"
#include "test_utils.h"
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
    log_info(("Press \"Yes\"\n"));
    EXPECT_TRUE(TEST_PASS == get_user_confirmation());
}

TEST_F(Test_get_user_confirmation, t02)
{
    log_info(("Press \"No\"\n"));
    EXPECT_TRUE(TEST_FAIL == get_user_confirmation());
}

TEST_F(Test_compare_files, t01)
{
    EXPECT_TRUE(TEST_PASS == compare_files("resources/c_experiments.c", "resources/c_experiments_same.c"));
    EXPECT_TRUE(TEST_PASS == compare_files("resources/c_experiments_same.c", "resources/c_experiments.c"));

    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments.c", "resources/c_experiments_less_1.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments.c", "resources/c_experiments_less_1.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments.c", "resources/c_experiments_less_1.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments_less_1.c", "resources/c_experiments.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments_less_1.c", "resources/c_experiments.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments_less_1.c", "resources/c_experiments.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments.c", "resources/c_experiments_more_1.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments.c", "resources/c_experiments_more_1.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments.c", "resources/c_experiments_more_1.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments_more_1.c", "resources/c_experiments.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments_more_1.c", "resources/c_experiments.c"));
    EXPECT_TRUE(TEST_FAIL == compare_files("resources/c_experiments_more_1.c", "resources/c_experiments.c"));
}
