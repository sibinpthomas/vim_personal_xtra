#include "platform.h"
#include "gtest/gtest.h"
#include "logger.h"

extern "C"
{
    #include "circular_buffer.c"
}

#define BUFSIZE                             (8u)

class Circular_Buffer_Wbox_Test : public ::testing::Test
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

class Test_cbuf_give_Neg: public ::Circular_Buffer_Wbox_Test { };
class Test_cbuf_give: public ::Circular_Buffer_Wbox_Test,
                      public ::testing::WithParamInterface<int>
{
    virtual void SetUp()
    {
        UINT16_F i;

        ASSERT_TRUE(API_SUCCESS == cbuf_init());
        ASSERT_TRUE(API_SUCCESS == cbuf_alloc(buf, (UINT16)sizeof(buf), &hdl));
        pltfm_memset(buf, 0xFF, (UINT16)sizeof(buf));
        pltfm_memset(flush, 0xA5, (UINT16)sizeof(flush));
        print_cbuf_data(hdl);
        for (i = 0; i < sizeof(data); i++)
        {
            data[i] = i + 1;
        }
        log_info(("\n"));
    }

    virtual void TearDown()
    {
        ASSERT_TRUE(API_SUCCESS == cbuf_deinit());
    }

    protected:
        CBUF_HDL hdl;
        UINT8 buf[BUFSIZE];
        UINT8 flush[BUFSIZE];
        UINT8 data[BUFSIZE * 5];
};

TEST_P(Test_cbuf_give, valid_input_full)
{
    UINT8 *dt = data;
    UINT8 initial_len = GetParam();

    if ((initial_len > 0) && (initial_len < BUFSIZE))
    {
        EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, flush, initial_len));
        EXPECT_TRUE(initial_len == cbuf_get(hdl, flush, initial_len));
        print_cbuf_data(hdl);
    }

    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, BUFSIZE - 1));
    print_cbuf_data(hdl);
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, dt, BUFSIZE - 1));
}

TEST_P(Test_cbuf_give, valid_input_full_01)
{
    UINT8 *dt = data;
    UINT8 initial_len = (UINT8)GetParam();

    if ((initial_len > 0) && (initial_len < BUFSIZE))
    {
        EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, flush, 1));
        EXPECT_TRUE(1 == cbuf_get(hdl, flush, 1));
        EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, flush, initial_len));
        EXPECT_TRUE(initial_len == cbuf_get(hdl, flush, initial_len));
        print_cbuf_data(hdl);
    }

    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, BUFSIZE - 1));
    print_cbuf_data(hdl);
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, dt, BUFSIZE - 1));
}

TEST_P(Test_cbuf_give, valid_input_1)
{
    UINT8 quanta = 1;
    UINT8 *dt = data;
    UINT8 initial_len = (UINT8)GetParam();

    if ((initial_len > 0) && (initial_len < BUFSIZE))
    {
        EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, flush, initial_len));
        EXPECT_TRUE(initial_len == cbuf_get(hdl, flush, initial_len));
        print_cbuf_data(hdl);
    }

    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    print_cbuf_data(hdl);
}

TEST_P(Test_cbuf_give, valid_input_2)
{
    UINT8 quanta = 2;
    UINT8 *dt = data;
    UINT8 initial_len = (UINT8)GetParam();

    if ((initial_len > 0) && (initial_len < BUFSIZE))
    {
        EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, flush, initial_len));
        EXPECT_TRUE(initial_len == cbuf_get(hdl, flush, initial_len));
        print_cbuf_data(hdl);
    }

    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    print_cbuf_data(hdl);
}

TEST_P(Test_cbuf_give, valid_input_4)
{
    UINT8 quanta = 4;
    UINT8 *dt = data;
    UINT8 initial_len = (UINT8)GetParam();

    if ((initial_len > 0) && (initial_len < BUFSIZE))
    {
        EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, flush, initial_len));
        EXPECT_TRUE(initial_len == cbuf_get(hdl, flush, initial_len));
        print_cbuf_data(hdl);
    }

    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    dt += quanta;
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, dt, quanta));
    print_cbuf_data(hdl);
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_give,
                        ::testing::Range(0, (int)(8)));
