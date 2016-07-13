#include "platform.h"
#include "gtest/gtest.h"
#include "logger.h"

extern "C"
{
    #include "circular_buffer_mt.c"
}

#define BUFSIZE                             (8u)

API_RESULT is_buffer_full(CBUF_HDL hdl)
{
    API_RESULT retval;

    CBUF_LIST_MUTEX_LOCK();
    retval = (is_cbuf_full(hdl)) ? API_SUCCESS : API_FAILURE;
    CBUF_LIST_MUTEX_UNLOCK();

    return retval;
}

API_RESULT is_buffer_empty(CBUF_HDL hdl)
{
    API_RESULT retval;

    CBUF_LIST_MUTEX_LOCK();
    retval = (is_cbuf_empty(hdl)) ? API_SUCCESS : API_FAILURE;
    CBUF_LIST_MUTEX_UNLOCK();

    return retval;
}

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

class Test_cbuf: public ::Circular_Buffer_Wbox_Test
{
    virtual void SetUp()
    {
        ASSERT_TRUE(API_SUCCESS == cbuf_init());
        ASSERT_TRUE(API_SUCCESS == cbuf_alloc(buf, (UINT16)sizeof(buf), &hdl));
    }

    virtual void TearDown()
    {
        ASSERT_TRUE(API_SUCCESS == cbuf_free(hdl));
        ASSERT_EQ(API_SUCCESS, cbuf_deinit());
    }

    protected:
        CBUF_HDL hdl;
        UINT8 buf[BUFSIZE];
};

class Test_cbuf_give: public ::Test_cbuf { };
class Test_cbuf_give_Neg: public ::Test_cbuf { };

class Test_cbuf_get: public ::Test_cbuf { };
class Test_cbuf_get_Neg: public ::Test_cbuf { };

TEST_F(Test_cbuf_give_Neg, invalid_input)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = sizeof(data);
    UINT16 length_0 = 0;
    UINT16 length_max = BUFSIZE;

    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, NULL, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, NULL, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, NULL, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_NULL_HDL, data, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS, data, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(CBUF_MT_NUM_BUFFERS+1, data, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, NULL, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_give(hdl, data, &length_max, CBUF_XACT|CBUF_WAIT));
}

TEST_F(Test_cbuf_get_Neg, invalid_input)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = sizeof(data);
    UINT16 length_0 = 0;
    UINT16 length_max = BUFSIZE;

    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, NULL, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, NULL, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, NULL, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_NULL_HDL, data, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS, data, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(CBUF_MT_NUM_BUFFERS+1, data, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &length_max, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &length_max, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &length_max, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &length_max, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &data_len, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &data_len, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &data_len, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &data_len, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, NULL, &data_len, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, NULL, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, NULL, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, NULL, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, NULL, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, NULL, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, &length_0, 0));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, &length_0, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, &length_0, CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, &length_0, CBUF_PEEK));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, &length_0, CBUF_XACT|CBUF_WAIT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, &length_max, CBUF_XACT));
    EXPECT_FALSE(API_SUCCESS == cbuf_get(hdl, data, &length_max, CBUF_XACT|CBUF_WAIT));
}

#define WRITE_THREAD_NUM_ARGS           (5u)
#define READ_THREAD_NUM_ARGS            (5u)

void cleanup_handler(void *arg)
{
    CBUF_LIST_MUTEX_UNLOCK_VOID();
}

void* write_thread_func(void *t_param)
{
    void* (*param_array)[WRITE_THREAD_NUM_ARGS] = (void * (*)[WRITE_THREAD_NUM_ARGS])t_param;
    CBUF_HDL hdl = *((CBUF_HDL *)(*param_array)[0]);
    UINT8* data = (UINT8 *)(*param_array)[1];
    UINT16 data_len = *((UINT32 *)(*param_array)[2]);
    UINT16 incr = *((UINT16 *)(*param_array)[3]);
    UINT16* p_incr = (UINT16 *)(*param_array)[3];
    UINT8 flags = *((UINT8 *)(*param_array)[4]);
    UINT16_F i;
    UINT16 give_len;

    EXPECT_EQ(0, pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL));
    EXPECT_EQ(0, pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL));

    pthread_cleanup_push(cleanup_handler, p_incr); 

    CBUF_MT_TRC(("\n------------- Increment of %u. -------------\n", incr));
    *p_incr = 0xFAFAu;
    for (i = incr; i < data_len; i+=incr)
    {
        EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (give_len = incr, &give_len), flags)) &&
                    (incr == give_len));
    }
    CBUF_MT_TRC(("Final write in Write thread.\n"));
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, data, (give_len = incr, &give_len), 0));
    EXPECT_LE(give_len, incr);
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, data, (give_len = incr, &give_len), flags));
    EXPECT_EQ(give_len, incr);

    CBUF_MT_TRC(("\nWrite thread coming to a close.\n"));
    *p_incr = 0xAFAFu;
    pthread_cleanup_pop(0);
    return NULL;
}

class Test_cbuf_give_blk_any : public ::Test_cbuf,
                      public ::testing::WithParamInterface<UINT16> { };

TEST_P(Test_cbuf_give_blk_any, fill_cbuf_to_capacity)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = sizeof(data);
    UINT8 recv_buf[BUFSIZE - 1] = {0};
    UINT16 recv_len = sizeof(recv_buf);
    UINT16 incr = GetParam();
    pltfm_thread_t wr_t_id;
    void *wr_param_array[WRITE_THREAD_NUM_ARGS];
    UINT32 qfull_chk_cnt = 0;
    UINT8 flags = CBUF_WAIT|CBUF_ANY;

    wr_param_array[0] = &hdl;
    wr_param_array[1] = data;
    wr_param_array[2] = &data_len;
    wr_param_array[3] = &incr;
    wr_param_array[4] = &flags;
    ASSERT_TRUE(0 == pltfm_thread_create(&wr_t_id, NULL, write_thread_func, wr_param_array));

    while (API_SUCCESS != is_buffer_full(hdl))
    {
        pltfm_thread_yield(); 
    }

    /* Check if write thread has terminated. */
    EXPECT_EQ(0xFAFAu, incr);
    EXPECT_EQ(0, pltfm_thread_cancel(wr_t_id)); 
    EXPECT_EQ(0, pltfm_thread_join(wr_t_id, NULL));  
}

TEST_P(Test_cbuf_give_blk_any, fill_cbuf_to_capacity_read)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = sizeof(data);
    UINT8 recv_buf[BUFSIZE - 1] = {0};
    UINT16 recv_len = sizeof(recv_buf);
    UINT16 incr = GetParam();
    pltfm_thread_t wr_t_id;
    void *wr_param_array[WRITE_THREAD_NUM_ARGS];
    UINT16 get_len;
    API_RESULT retval;
    UINT8 flags = CBUF_WAIT|CBUF_ANY;

    wr_param_array[0] = &hdl;
    wr_param_array[1] = data;
    wr_param_array[2] = &data_len;
    wr_param_array[3] = &incr;
    wr_param_array[4] = &flags;
    ASSERT_TRUE(0 == pltfm_thread_create(&wr_t_id, NULL, write_thread_func, wr_param_array));

    while (API_SUCCESS != is_buffer_full(hdl))
    {
        pltfm_thread_yield(); 
    }

    retval = cbuf_get(hdl, recv_buf, (get_len = recv_len, &get_len), CBUF_ANY);
    EXPECT_EQ(API_SUCCESS, retval);
    EXPECT_GT(get_len, recv_len/2);

    /* Wait for write thread to terminate. */
    ASSERT_TRUE(0 == pltfm_thread_join(wr_t_id, NULL));
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_give_blk_any,
                        ::testing::Range((UINT16)1, (UINT16)(BUFSIZE)));

class Test_cbuf_give_blk_xact : public ::Test_cbuf,
                      public ::testing::WithParamInterface<UINT16> { };

TEST_P(Test_cbuf_give_blk_xact, fill_cbuf_to_capacity)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = sizeof(data);
    UINT16 incr = GetParam();
    pltfm_thread_t wr_t_id;
    void *wr_param_array[WRITE_THREAD_NUM_ARGS];
    UINT8 flags = CBUF_WAIT|CBUF_XACT;

    wr_param_array[0] = &hdl;
    wr_param_array[1] = data;
    wr_param_array[2] = &data_len;
    wr_param_array[3] = &incr;
    wr_param_array[4] = &flags;
    ASSERT_TRUE(0 == pltfm_thread_create(&wr_t_id, NULL, write_thread_func, wr_param_array));

    while (API_SUCCESS != is_buffer_full(hdl))
    {
        pltfm_thread_yield(); 
    }

    /* Check if write thread has terminated. */
    EXPECT_EQ(0, pltfm_thread_cancel(wr_t_id)); 
    EXPECT_EQ(0, pltfm_thread_join(wr_t_id, NULL));  
}

TEST_P(Test_cbuf_give_blk_xact, fill_cbuf_to_capacity_read)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = sizeof(data);
    UINT8 recv_buf[BUFSIZE - 1] = {0};
    UINT16 recv_len = sizeof(recv_buf);
    UINT16 incr = GetParam();
    pltfm_thread_t wr_t_id;
    void *wr_param_array[WRITE_THREAD_NUM_ARGS];
    UINT16 get_len;
    API_RESULT retval;
    UINT8 flags = CBUF_WAIT|CBUF_XACT;

    wr_param_array[0] = &hdl;
    wr_param_array[1] = data;
    wr_param_array[2] = &data_len;
    wr_param_array[3] = &incr;
    wr_param_array[4] = &flags;
    ASSERT_TRUE(0 == pltfm_thread_create(&wr_t_id, NULL, write_thread_func, wr_param_array));

    while (API_SUCCESS != is_buffer_full(hdl))
    {
        pltfm_thread_yield(); 
    }

    retval = cbuf_get(hdl, recv_buf, (get_len = recv_len, &get_len), CBUF_ANY);
    EXPECT_EQ(API_SUCCESS, retval);
    EXPECT_GT(get_len, recv_len/2);

    /* Wait for write thread to terminate. */
    ASSERT_TRUE(0 == pltfm_thread_join(wr_t_id, NULL));
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_give_blk_xact,
                        ::testing::Range((UINT16)1, (UINT16)(BUFSIZE)));

void* read_thread_func(void *t_param)
{
    void* (*param_array)[READ_THREAD_NUM_ARGS] = (void * (*)[READ_THREAD_NUM_ARGS])t_param;
    CBUF_HDL hdl = *((CBUF_HDL *)(*param_array)[0]);
    UINT8* recv_buf = (UINT8 *)(*param_array)[1];
    UINT16 recv_len = *((UINT32 *)(*param_array)[2]);
    UINT16 incr = *((UINT16 *)(*param_array)[3]);
    UINT16* p_incr = (UINT16 *)(*param_array)[3];
    UINT8 flags = *((UINT8 *)(*param_array)[4]);
    UINT16_F i;
    UINT16 get_len;

    EXPECT_EQ(0, pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL));
    EXPECT_EQ(0, pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL));

    pthread_cleanup_push(cleanup_handler, p_incr); 

    CBUF_MT_TRC(("\n------------- Increment of %u. -------------\n", incr));
    *p_incr = 0xFAFAu;
    for (i = 0; (i + incr) < recv_len; i+=incr)
    {
        EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &recv_buf[i], (get_len = incr, &get_len), flags)) &&
                    (incr == get_len));
    }
    CBUF_MT_TRC(("Final read in Read thread.\n"));
    EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, &recv_buf[i], (get_len = incr, &get_len), 0));
    EXPECT_LE(get_len, incr);
    EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, recv_buf, (get_len = incr, &get_len), flags));
    EXPECT_EQ(get_len, incr);

    CBUF_MT_TRC(("\nRead thread coming to a close.\n"));
    *p_incr = 0xAFAFu;
    pthread_cleanup_pop(0);
    return NULL;
}

class Test_cbuf_get_blk_any : public ::Test_cbuf,
                      public ::testing::WithParamInterface<UINT16> { };

TEST_P(Test_cbuf_get_blk_any, read_dry)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = sizeof(data);
    UINT8 recv_buf[BUFSIZE - 1] = {0};
    UINT16 recv_len = sizeof(recv_buf);
    UINT16 incr = GetParam();
    pltfm_thread_t rd_t_id;
    void *rd_param_array[READ_THREAD_NUM_ARGS];
    UINT16 give_len;
    UINT8 flags = CBUF_WAIT|CBUF_ANY;

    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, data, (give_len = data_len, &give_len), CBUF_XACT));
    EXPECT_EQ(give_len, data_len);

    rd_param_array[0] = &hdl;
    rd_param_array[1] = recv_buf;
    rd_param_array[2] = &recv_len;
    rd_param_array[3] = &incr;
    rd_param_array[4] = &flags;
    ASSERT_TRUE(0 == pltfm_thread_create(&rd_t_id, NULL, read_thread_func, rd_param_array));

    while (API_SUCCESS != is_buffer_empty(hdl))
    {
        pltfm_thread_yield(); 
    }

    /* Check if read thread has terminated. */
    EXPECT_EQ(0xFAFAu, incr);
    EXPECT_EQ(0, pltfm_thread_cancel(rd_t_id)); 
    EXPECT_EQ(0, pltfm_thread_join(rd_t_id, NULL));  

    log_buffer(recv_buf, recv_len);
    EXPECT_EQ(0, pltfm_memcmp(data, recv_buf, sizeof(data)));
}

TEST_P(Test_cbuf_get_blk_any, read_dry_write)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = sizeof(data);
    UINT8 recv_buf[BUFSIZE - 1] = {0};
    UINT16 recv_len = sizeof(recv_buf);
    UINT16 incr = GetParam();
    pltfm_thread_t rd_t_id;
    void *rd_param_array[READ_THREAD_NUM_ARGS];
    UINT16 give_len;
    UINT8 flags = CBUF_WAIT|CBUF_ANY;

    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, data, (give_len = data_len, &give_len), CBUF_XACT));
    EXPECT_EQ(give_len, data_len);

    rd_param_array[0] = &hdl;
    rd_param_array[1] = recv_buf;
    rd_param_array[2] = &recv_len;
    rd_param_array[3] = &incr;
    rd_param_array[4] = &flags;
    ASSERT_TRUE(0 == pltfm_thread_create(&rd_t_id, NULL, read_thread_func, rd_param_array));

    while (API_SUCCESS != is_buffer_empty(hdl))
    {
        pltfm_thread_yield(); 
    }

    log_buffer(recv_buf, recv_len);
    EXPECT_EQ(0, pltfm_memcmp(data, recv_buf, sizeof(data)));
 
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, data, (give_len = data_len, &give_len), CBUF_XACT));
    EXPECT_EQ(give_len, data_len);

    /* Wait for write thread to terminate. */
    ASSERT_TRUE(0 == pltfm_thread_join(rd_t_id, NULL));
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_get_blk_any,
                        ::testing::Range((UINT16)1, (UINT16)(BUFSIZE)));
