#include "platform.h"
#include "gtest/gtest.h"
#include "test_utils.h"
#include "logger.h"
#include "time_utils.h"

class Time_Utils_Tests : public ::testing::Test
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

class Test_get_current_time_Neg: public ::Time_Utils_Tests { };

TEST_F(Test_get_current_time_Neg, invalid_input)
{
    EXPECT_EQ(API_FAILURE, get_current_time(NULL));
}

class Test_get_current_time: public ::Time_Utils_Tests { };

TEST_F(Test_get_current_time, valid_input_01)
{
    TIME_SPAN ct;

    EXPECT_EQ(API_SUCCESS, get_current_time(&ct));
    EXPECT_EQ(API_SUCCESS, get_current_time(&ct));
    printf("\nSec - %u", ct.sec);
    printf("\nU Sec - %u", ct.usec);
    printf("\n");
}

TEST_F(Test_get_current_time, stress_01)
{
    TIME_SPAN ct;
    UINT16_F i;

    for (i = 0; i < 1000000; i++)
    {
        EXPECT_EQ(API_SUCCESS, get_current_time(&ct));
        EXPECT_LT(ct.usec, MILLION);
    }
}

class Test_get_time_difference_Neg: public ::Time_Utils_Tests { };

TEST_F(Test_get_time_difference_Neg, invalid_input_01)
{
    TIME_SPAN in, lt, diff;

    EXPECT_EQ(API_FAILURE, get_time_difference(NULL, NULL, NULL));
    EXPECT_EQ(API_FAILURE, get_time_difference(NULL, NULL, &diff));
    EXPECT_EQ(API_FAILURE, get_time_difference(NULL, &lt, NULL));
    EXPECT_EQ(API_FAILURE, get_time_difference(NULL, &lt, &diff));
    EXPECT_EQ(API_FAILURE, get_time_difference(&in, NULL, NULL));
    EXPECT_EQ(API_FAILURE, get_time_difference(&in, NULL, &diff));
    EXPECT_EQ(API_FAILURE, get_time_difference(&in, &lt, NULL));
}

TEST_F(Test_get_time_difference_Neg, invalid_input_02)
{
    TIME_SPAN in, lt, diff;

    in.sec = 111;
    in.usec = 321;

    lt.sec = 111;
    lt.usec = 321;
    EXPECT_EQ(API_SUCCESS, get_time_difference(&in, &lt, &diff));

    lt.usec = 321 + 1;
    EXPECT_EQ(API_SUCCESS, get_time_difference(&in, &lt, &diff));

    lt.usec = 321 - 1;
    EXPECT_EQ(API_FAILURE, get_time_difference(&in, &lt, &diff));

    lt.sec = 111 - 1;
    lt.usec = 321;
    EXPECT_EQ(API_FAILURE, get_time_difference(&in, &lt, &diff));

    lt.sec = 111 + 1;
    lt.usec = 321;
    EXPECT_EQ(API_SUCCESS, get_time_difference(&in, &lt, &diff));
}

TEST_F(Test_get_time_difference_Neg, invalid_input_03)
{
    TIME_SPAN in, lt, diff;

    in.sec = 111;
    in.usec = 0;

    lt.sec = 111;
    lt.usec = 0;
    EXPECT_EQ(API_SUCCESS, get_time_difference(&in, &lt, &diff));

    lt.usec = 0 + 1;
    EXPECT_EQ(API_SUCCESS, get_time_difference(&in, &lt, &diff));

    lt.sec = 111 - 1;
    lt.usec = (UINT32)MILLION - 1;
    EXPECT_EQ(API_FAILURE, get_time_difference(&in, &lt, &diff));

    lt.sec = 111 - 1;
    lt.usec = 0;
    EXPECT_EQ(API_FAILURE, get_time_difference(&in, &lt, &diff));

    lt.sec = 111 + 1;
    lt.usec = 0;
    EXPECT_EQ(API_SUCCESS, get_time_difference(&in, &lt, &diff));
}

class Test_get_time_difference: public ::Time_Utils_Tests { };

TEST_F(Test_get_time_difference, valid_input_01)
{
    TIME_SPAN in, lt, diff;

    in.sec = 111;
    in.usec = 321;

    lt.sec = 111;
    lt.usec = 321;
    EXPECT_EQ(API_SUCCESS, get_time_difference(&in, &lt, &diff));
    EXPECT_EQ(diff.sec, 0);
    EXPECT_EQ(diff.usec, 0);

    lt.usec = 321 + 1;
    EXPECT_EQ(API_SUCCESS, get_time_difference(&in, &lt, &diff));
    EXPECT_EQ(diff.sec, 0);
    EXPECT_EQ(diff.usec, 1);

    in.sec = 111;
    in.usec = 1;

    lt.sec = 112;
    lt.usec = 0;
    EXPECT_EQ(API_SUCCESS, get_time_difference(&in, &lt, &diff));
    EXPECT_EQ(diff.sec, 0);
    EXPECT_EQ(diff.usec, ((UINT32)MILLION) - 1);

    in.sec = 111;
    in.usec = ((UINT32)MILLION) - 1;

    lt.sec = 112;
    lt.usec = 0;
    EXPECT_EQ(API_SUCCESS, get_time_difference(&in, &lt, &diff));
    EXPECT_EQ(diff.sec, 0);
    EXPECT_EQ(diff.usec, 1);
}

class Test_get_elapsed_time_Neg: public ::Time_Utils_Tests { };

TEST_F(Test_get_elapsed_time_Neg, invalid_input_01)
{
    TIME_SPAN ref, diff;

    EXPECT_EQ(API_FAILURE, get_elapsed_time(NULL, NULL));
    EXPECT_EQ(API_FAILURE, get_elapsed_time(NULL, &diff));
    EXPECT_EQ(API_FAILURE, get_elapsed_time(&ref, NULL));
}

TEST_F(Test_get_elapsed_time_Neg, invalid_input_02)
{
    TIME_SPAN ref, diff;

    EXPECT_EQ(API_SUCCESS, get_current_time(&ref));
    ref.sec += 10;
    EXPECT_EQ(API_FAILURE, get_elapsed_time(&ref, &diff));
}

class Test_get_elapsed_time: public ::Time_Utils_Tests { };

TEST_F(Test_get_elapsed_time, basic)
{
    TIME_SPAN ref, diff;

    EXPECT_EQ(API_SUCCESS, get_current_time(&ref));
    EXPECT_EQ(API_SUCCESS, get_elapsed_time(&ref, &diff));
}

TEST_F(Test_get_elapsed_time, timed_test_1_sec)
{
    TIME_SPAN ref, diff;
    UINT16_F i;

    for (i = 0; i < 10; i++)
    {
        EXPECT_EQ(API_SUCCESS, get_current_time(&ref));
        pltfm_sleep(1);
        EXPECT_EQ(API_SUCCESS, get_elapsed_time(&ref, &diff));

        printf("\nsec: %u, usec: %u\n", diff.sec, diff.usec);
    }
}

TEST_F(Test_get_elapsed_time, timed_test_1_msec)
{
    TIME_SPAN ref, diff;
    UINT16_F i;

    for (i = 0; i < 10; i++)
    {
        EXPECT_EQ(API_SUCCESS, get_current_time(&ref));
        pltfm_msleep(1);
        EXPECT_EQ(API_SUCCESS, get_elapsed_time(&ref, &diff));

        printf("\nsec: %u, usec: %u\n", diff.sec, diff.usec);
    }
}

TEST_F(Test_get_elapsed_time, timed_test_50_usec)
{
    TIME_SPAN ref, diff;
    UINT16_F i;

    for (i = 0; i < 10; i++)
    {
        EXPECT_EQ(API_SUCCESS, get_current_time(&ref));
        pltfm_usleep(50);
        EXPECT_EQ(API_SUCCESS, get_elapsed_time(&ref, &diff));

        printf("\nsec: %u, usec: %u\n", diff.sec, diff.usec);
    }
}
