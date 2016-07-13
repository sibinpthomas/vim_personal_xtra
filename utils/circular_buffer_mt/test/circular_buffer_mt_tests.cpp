#include "platform.h"
#include "gtest/gtest.h"
#include "test_utils.h"
#include "logger.h"
#include "circular_buffer_mt.h"
#include "circular_buffer_mt_error_codes_str.h"

#define BUFSIZE                             (8u)
#define TX_ATTEMPT_LIMIT                    (5u)
#define RX_ATTEMPT_LIMIT                    (5u)
#define RCS_ROOT                            "resources/"
#define D_FI_RD_THREAD_NUM_PARAMS           (6u)
#define F_FI_RD_THREAD_NUM_PARAMS           (8u)

const CHAR* test_files[] = {"Sinkarimelam_1.mp3",
                            "Core_V4.0.pdf",
                            "high_on_life.jpg"};

ERR_DIC_ELEM err_codes_arr[] = {
    CIRCULAR_BUFFER_MT_ERROR_CODES
};

class Circular_Buffer_Test_Base : public ::testing::Test
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

class Test_cbuf_init: public ::Circular_Buffer_Test_Base { };

TEST_F(Test_cbuf_init, basic)
{
    EXPECT_TRUE(API_SUCCESS == cbuf_init());
}

class Test_cbuf_deinit: public ::Circular_Buffer_Test_Base { };

TEST_F(Test_cbuf_deinit, basic)
{
    EXPECT_TRUE(API_SUCCESS == cbuf_deinit());
}

class Circular_Buffer_Test : public ::Circular_Buffer_Test_Base
{
    virtual void SetUp()
    {
        ASSERT_TRUE(API_SUCCESS == cbuf_init());
    }

    virtual void TearDown()
    {
        ASSERT_TRUE(API_SUCCESS == cbuf_deinit());
    }
};

class Test_cbuf_alloc_Neg: public ::Circular_Buffer_Test { };
class Test_cbuf_alloc: public ::Circular_Buffer_Test { };

class Test_cbuf_free_Neg: public ::Circular_Buffer_Test { };
class Test_cbuf_free: public ::Circular_Buffer_Test { };

TEST_F(Test_cbuf_alloc_Neg, invalid_input)
{
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc(NULL, 0, NULL));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc((UINT8 *)2, 0, NULL));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc((UINT8 *)2, 1, NULL));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc(NULL, 2, NULL));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc(NULL, 3, NULL));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc(NULL, 1, (CBUF_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc(NULL, 0, (CBUF_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc(NULL, 4, (CBUF_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc(NULL, 3, (CBUF_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc((UINT8 *)2, 1, (CBUF_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc((UINT8 *)2, 0, (CBUF_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc((UINT8 *)2, 4, NULL));
    EXPECT_FALSE(API_SUCCESS == cbuf_alloc((UINT8 *)2, 9, NULL));
}

TEST_F(Test_cbuf_alloc, valid_input)
{
    CBUF_HDL hdl;
    UINT8 buf[100];

    EXPECT_TRUE(API_SUCCESS == cbuf_alloc(buf, (UINT16)sizeof(buf), &hdl));
    log_info(("Handle is 0x%04x\n", hdl));
}

TEST_F(Test_cbuf_alloc, until_fail)
{
    CBUF_HDL hdl;
    UINT8 buf[100];
    UINT16 cbuf_count = 0;

    for (cbuf_count = 0; cbuf_count < (UINT16)(~(0u)); cbuf_count++)
    {
        if (API_SUCCESS != cbuf_alloc(buf, (UINT16)sizeof(buf), &hdl))
        {
            break;
        }
        log_info(("Handle is 0x%04x\n", hdl));
    }

    EXPECT_FALSE(cbuf_count == (UINT16)(~(0u)));
}

TEST_F(Test_cbuf_free_Neg, invalid_input)
{
    EXPECT_FALSE(API_SUCCESS == cbuf_free(0));
    EXPECT_FALSE(API_SUCCESS == cbuf_free(3)); /* > CBUF_MT_NUM_BUFFERS */
    EXPECT_FALSE(API_SUCCESS == cbuf_free(8));
}

TEST_F(Test_cbuf_free, valid_input)
{
    CBUF_HDL hdl;
    UINT8 buf[100];

    EXPECT_TRUE(API_SUCCESS == cbuf_alloc(buf, (UINT16)sizeof(buf), &hdl));
    log_info(("Handle is 0x%04x\n", hdl));
    EXPECT_TRUE(API_SUCCESS == cbuf_free(hdl));
}

class Test_cbuf: public ::Circular_Buffer_Test
{
    virtual void SetUp()
    {
        ASSERT_TRUE(API_SUCCESS == cbuf_init());
        ASSERT_TRUE(API_SUCCESS == cbuf_alloc(buf, (UINT16)sizeof(buf), &hdl));
    }

    virtual void TearDown()
    {
        ASSERT_TRUE(API_SUCCESS == cbuf_free(hdl));
        ASSERT_TRUE(API_SUCCESS == cbuf_deinit());
    }

    protected:
        CBUF_HDL hdl;
        UINT8 buf[BUFSIZE];
};

class Test_cbuf_give_nblk_any : public ::Test_cbuf,
                      public ::testing::WithParamInterface<UINT16> { };


TEST_P(Test_cbuf_give_nblk_any, fill_cbuf_to_capacity)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = 0;
    UINT16 incr = GetParam();
    UINT16_F i;

    log_trace(("\n------------- Increment of %u. -------------\n", incr));
    for (i = 0; i < sizeof(data) - (incr - 1); i+=incr)
    {
        EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (data_len = incr, &data_len), 0)) &&
                    (incr == data_len));
    }
    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, data, (data_len = incr, &data_len), 0));
    EXPECT_EQ((BUFSIZE - 1) % incr, data_len);
}


TEST_P(Test_cbuf_give_nblk_any, fill_to_capacity_read)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = 0;
    UINT16 incr = GetParam();
    UINT16_F i;

    log_trace(("\n------------- Increment of %u. -------------\n", incr));
    for (i = 0; i < sizeof(data) - (incr - 1); i+=incr)
    {
        EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (data_len = incr, &data_len), 0)) &&
                    (incr == data_len));
    }

    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, data, (data_len = incr, &data_len), 0));
    EXPECT_EQ((BUFSIZE - 1) % incr, data_len);

    EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, data, (data_len = sizeof(data), &data_len), 0));
    EXPECT_EQ(sizeof(data), data_len);

    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, data, (data_len = incr, &data_len), 0));
    EXPECT_EQ(incr, data_len);
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_give_nblk_any,
                        ::testing::Range((UINT16)1, (UINT16)(BUFSIZE)));

class Test_cbuf_give_nblk_xact : public ::Test_cbuf,
                      public ::testing::WithParamInterface<UINT16> { };


TEST_P(Test_cbuf_give_nblk_xact, fill_cbuf_to_capacity)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = 0;
    UINT16 incr = GetParam();
    UINT16_F i;

    log_trace(("\n------------- Increment of %u. -------------\n", incr));
    for (i = 0; i < sizeof(data) - (incr - 1); i+=incr)
    {
        EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (data_len = incr, &data_len), CBUF_XACT)) &&
                    (incr == data_len));
    }
    EXPECT_TRUE(CBUF_MT_NOT_ENUF_SPACE == cbuf_give(hdl, data, (data_len = incr, &data_len), CBUF_XACT));
}


TEST_P(Test_cbuf_give_nblk_xact, fill_to_capacity_read)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT16 data_len = 0;
    UINT16 incr = GetParam();
    UINT16_F i;
    API_RESULT retval;

    log_trace(("\n------------- Increment of %u. -------------\n", incr));
    for (i = 0; i < sizeof(data) - (incr - 1); i+=incr)
    {
        EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (data_len = incr, &data_len), CBUF_XACT)) &&
                    (incr == data_len));
    }

    if ((incr > 1) && (incr < (BUFSIZE - 1)))
    {
        retval = cbuf_give(hdl, data, (data_len = ((BUFSIZE - 1) % incr), &data_len), CBUF_XACT);
        EXPECT_EQ(API_SUCCESS, retval);
    }

    EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, data, (data_len = sizeof(data), &data_len), CBUF_XACT));
    EXPECT_EQ(sizeof(data), data_len);

    EXPECT_TRUE(API_SUCCESS == cbuf_give(hdl, data, (data_len = incr, &data_len), CBUF_XACT));
    EXPECT_EQ(incr, data_len);
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_give_nblk_xact,
                        ::testing::Range((UINT16)1, (UINT16)(BUFSIZE)));


class Test_cbuf_get_nblk_any : public ::Test_cbuf,
                      public ::testing::WithParamInterface<UINT16> { };


TEST_P(Test_cbuf_get_nblk_any, read_dry)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT8 recv_buf[BUFSIZE - 1] = {0};
    UINT16 data_len = 0;
    UINT16 incr = GetParam();
    UINT16_F i;

    log_trace(("\n************* Increment of %u. *************\n", incr));

    EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (data_len = sizeof(data), &data_len), CBUF_XACT)));
    EXPECT_EQ(sizeof(data), data_len);

    for (i = 0; i < sizeof(data) - (incr - 1); i+=incr)
    {
        EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &recv_buf[i], (data_len = incr, &data_len), 0)) &&
                    (incr == data_len));
        log_buffer(&recv_buf[i], data_len);
    }
    EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, &recv_buf[i], (data_len = incr, &data_len), 0));
    EXPECT_EQ((BUFSIZE - 1) % incr, data_len);
    log_buffer(&recv_buf[i], data_len);
    EXPECT_EQ(0, pltfm_memcmp(data, recv_buf, sizeof(data)));
}


TEST_P(Test_cbuf_get_nblk_any, read_dry_write)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT8 recv_buf[BUFSIZE - 1] = {0};
    UINT16 data_len = 0;
    UINT16 incr = GetParam();
    UINT16_F i;

    log_trace(("\n------------- Increment of %u. -------------\n", incr));

    EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (data_len = sizeof(data), &data_len), CBUF_XACT)));
    EXPECT_EQ(sizeof(data), data_len);

    for (i = 0; i < sizeof(data) - (incr - 1); i+=incr)
    {
        EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &recv_buf[i], (data_len = incr, &data_len), 0)) &&
                    (incr == data_len));
        log_buffer(&recv_buf[i], data_len);
    }
    EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, &recv_buf[i], (data_len = incr, &data_len), 0));
    EXPECT_EQ((BUFSIZE - 1) % incr, data_len);
    log_buffer(&recv_buf[i], data_len);
    EXPECT_EQ(0, pltfm_memcmp(data, recv_buf, sizeof(data)));

    pltfm_memset(recv_buf, 0, sizeof(recv_buf));
    EXPECT_NE(0, pltfm_memcmp(data, recv_buf, sizeof(data)));
    EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (data_len = sizeof(data), &data_len), CBUF_XACT)));
    EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, recv_buf, (data_len = sizeof(data), &data_len), 0));
    EXPECT_EQ(sizeof(data), data_len);
    log_buffer(recv_buf, data_len);
    EXPECT_EQ(0, pltfm_memcmp(data, recv_buf, sizeof(data)));
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_get_nblk_any,
                        ::testing::Range((UINT16)1, (UINT16)(BUFSIZE)));

class Test_cbuf_get_nblk_xact : public ::Test_cbuf,
                      public ::testing::WithParamInterface<UINT16> { };


TEST_P(Test_cbuf_get_nblk_xact, read_dry)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT8 recv_buf[BUFSIZE - 1] = {0};
    UINT16 data_len = 0;
    UINT16 incr = GetParam();
    UINT16_F i;

    log_trace(("\n************* Increment of %u. *************\n", incr));

    EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (data_len = sizeof(data), &data_len), CBUF_XACT)));
    EXPECT_EQ(sizeof(data), data_len);

    for (i = 0; (i + incr) < sizeof(data); i+=incr)
    {
        EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &recv_buf[i], (data_len = incr, &data_len), CBUF_XACT)) &&
                    (incr == data_len));
        log_buffer(&recv_buf[i], data_len);
    }
    EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, &recv_buf[i], (data_len = (sizeof(data) - i), &data_len), CBUF_XACT));
    EXPECT_EQ((sizeof(data) - i), data_len);
    log_buffer(&recv_buf[i], data_len);
    EXPECT_EQ(0, pltfm_memcmp(data, recv_buf, sizeof(data)));
}


TEST_P(Test_cbuf_get_nblk_xact, read_dry_write)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
    UINT8 recv_buf[BUFSIZE - 1] = {0};
    UINT16 data_len = 0;
    UINT16 incr = GetParam();
    UINT16_F i;

    log_trace(("\n------------- Increment of %u. -------------\n", incr));

    EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (data_len = sizeof(data), &data_len), CBUF_XACT)));
    EXPECT_EQ(sizeof(data), data_len);

    for (i = 0; (i + incr) < sizeof(data); i+=incr)
    {
        EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &recv_buf[i], (data_len = incr, &data_len), CBUF_XACT)) &&
                    (incr == data_len));
        log_buffer(&recv_buf[i], data_len);
    }
    EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, &recv_buf[i], (data_len = (sizeof(data) - i), &data_len), CBUF_XACT));
    EXPECT_EQ((sizeof(data) - i), data_len);
    log_buffer(&recv_buf[i], data_len);
    EXPECT_EQ(0, pltfm_memcmp(data, recv_buf, sizeof(data)));

    pltfm_memset(recv_buf, 0, sizeof(recv_buf));
    EXPECT_NE(0, pltfm_memcmp(data, recv_buf, sizeof(data)));
    EXPECT_TRUE((API_SUCCESS == cbuf_give(hdl, data, (data_len = sizeof(data), &data_len), CBUF_XACT)));
    EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, recv_buf, (data_len = sizeof(data), &data_len), 0));
    EXPECT_EQ(sizeof(data), data_len);
    log_buffer(recv_buf, data_len);
    EXPECT_EQ(0, pltfm_memcmp(data, recv_buf, sizeof(data)));
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_get_nblk_xact,
                        ::testing::Range((UINT16)1, (UINT16)(BUFSIZE)));

class Test_cbuf_get_basic: public ::Circular_Buffer_Test
{
    virtual void SetUp()
    {
        UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};
        UINT16 data_len = sizeof(data);

        ASSERT_TRUE(API_SUCCESS == cbuf_init());
        ASSERT_TRUE(API_SUCCESS == cbuf_alloc(buf, (UINT16)sizeof(buf), &hdl));
        ASSERT_TRUE(API_SUCCESS == cbuf_give(hdl, data, &data_len, CBUF_XACT));
    }

    virtual void TearDown()
    {
        ASSERT_TRUE(API_SUCCESS == cbuf_free(hdl));
        ASSERT_TRUE(API_SUCCESS == cbuf_deinit());
    }

    protected:
        CBUF_HDL hdl;
        UINT8 buf[BUFSIZE];
};

TEST_F(Test_cbuf_get_basic, valid_input_01)
{
    UINT8 buf[BUFSIZE] = {0};
    UINT16 buf_size = sizeof(buf);

    log_buffer(buf, BUFSIZE);
    EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, buf, &buf_size, 0)) && ((BUFSIZE - 1) == buf_size));
    log_buffer(buf, BUFSIZE - 1);
}

TEST_F(Test_cbuf_get_basic, valid_input_02)
{
    UINT8 buf[BUFSIZE] = {0};
    UINT8 pos = 0;
    UINT16 buf_size = 1;

    log_buffer(buf, BUFSIZE);
    EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &buf[pos], &buf_size, 0)) &&
                (1 == buf_size));
    pos += 1;
    EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &buf[pos], &buf_size, 0)) &&
                (1 == buf_size));
    pos += 1;
    EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &buf[pos], &buf_size, 0)) &&
                (1 == buf_size));
    pos += 1;
    EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &buf[pos], &buf_size, 0)) &&
                (1 == buf_size));
    pos += 1;
    EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &buf[pos], &buf_size, 0)) &&
                (1 == buf_size));
    pos += 1;
    EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &buf[pos], &buf_size, 0)) &&
                (1 == buf_size));
    pos += 1;
    EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &buf[pos], &buf_size, 0)) &&
                (1 == buf_size));
    pos += 1;
    EXPECT_TRUE((API_SUCCESS == cbuf_get(hdl, &buf[pos], &buf_size, 0)) &&
                (0 == buf_size));
    log_buffer(buf, BUFSIZE - 1);
}


#if GTEST_HAS_COMBINE
class Test_cbuf_data_fidelity: public ::Circular_Buffer_Test,
                      public ::testing::WithParamInterface< ::std::tr1::tuple<int, int, int> >
{
    virtual void SetUp()
    {
        UINT32 i;

        src_snk_len = 0xFFFFFFF - 0xFFFF;
        ASSERT_TRUE(API_SUCCESS == cbuf_init());
        ASSERT_TRUE(NULL != (src = (UINT8 *)pltfm_malloc(src_snk_len)));
        ASSERT_TRUE(NULL != (snk = (UINT8 *)pltfm_calloc(src_snk_len, 1)));

        for (i = 0; i < src_snk_len; i++) 
        {
            src[i] = i + 1;
        }

        ASSERT_FALSE(0 == pltfm_memcmp(src, snk, src_snk_len));

        wr_flags = 0;
        rd_flags = 0;
    }

    virtual void TearDown()
    {
        pltfm_free(snk);
        pltfm_free(src);
        ASSERT_TRUE(API_SUCCESS == cbuf_deinit());
    }

    protected:
        UINT8 *src;
        UINT8 *snk;
        UINT32 src_snk_len;
        UINT8 wr_flags;
        UINT8 rd_flags;
};

void* d_fi_read_thread_func(void *t_param)
{
    void* (*param_array)[D_FI_RD_THREAD_NUM_PARAMS] = (void * (*)[D_FI_RD_THREAD_NUM_PARAMS])t_param;
    CBUF_HDL hdl = *((CBUF_HDL *)(*param_array)[0]);
    UINT32 src_snk_len = *((UINT32 *)(*param_array)[1]);
    UINT8 *snk = (UINT8 *)(*param_array)[2];
    UINT8 rd_flags = *((UINT8 *)(*param_array)[3]);
    UINT8 buf_len = *((UINT8 *)(*param_array)[4]);
    UINT16 *read_retry_count = (UINT16 *)(*param_array)[5];
    UINT32 r_num_total = 0;
    UINT16 read_len;
    UINT16 prev_read_len;
    UINT16 get_len = 1;
    API_RESULT retval;

    while (r_num_total < src_snk_len)
    {
        prev_read_len = read_len;
        do
        {
            read_len = (UINT16)pltfm_rand();
        } while ((0 == read_len) || (read_len > (buf_len - 5)));

        if (0 == get_len)
        {
            read_len = prev_read_len>>1;
            if (0 == read_len)
            {
                read_len = prev_read_len;
            }
        }
        if ((UINT32)read_len > (src_snk_len - r_num_total))
        {
            read_len = src_snk_len - r_num_total;
        }
        get_len = read_len;

        /* Read from CBUF. */
        retval = cbuf_get(hdl, &snk[r_num_total], &get_len, rd_flags);

        if (API_SUCCESS == retval)
        {
            if (IS_BITNUM_SET(rd_flags, CBUF_XACT_BIT))
            {
                EXPECT_EQ(get_len, read_len);
            }
            else
            {
                EXPECT_LE(get_len, read_len);
            }
        }
        else
        {
            EXPECT_EQ(CBUF_MT_NOT_ENUF_DATA, retval);
        }

        if ((retval != API_SUCCESS) || ((API_SUCCESS == retval) && (0 == get_len)))
        {
            *read_retry_count += 1;
        }
        else
        {
            r_num_total += get_len;
        }

        pltfm_thread_yield(); 
    }

    return NULL;
}

TEST_P(Test_cbuf_data_fidelity, random_write_read)
{
    UINT8 buf[0xFFFF];
    UINT16 buf_len = sizeof(buf);
    UINT16 seed = (UINT16)(::std::tr1::get<0>(GetParam()) + 1);
    UINT16 rand_len = 10;
    UINT16 prev_rand_len;
    UINT16 give_len = 1;
    API_RESULT retval;
    UINT32 src_consumed = 0;
    BOOL_T b_memcmp;
    pltfm_thread_t t_id;
    void *param_array[D_FI_RD_THREAD_NUM_PARAMS];
    CBUF_HDL hdl;
    UINT16 write_retry_count = 0;
    UINT16 read_retry_count = 0;
    struct timespec start, stop, elapsed; 

    pltfm_srand(seed);
    pltfm_rand();
    pltfm_srand(pltfm_rand());

    wr_flags = (UINT8)(::std::tr1::get<1>(GetParam()));
    rd_flags = (UINT8)(::std::tr1::get<2>(GetParam()));

    if (IS_BITNUM_SET(wr_flags, CBUF_XACT_BIT) && 
            IS_BITNUM_SET(wr_flags, CBUF_WAIT_BIT) &&
            IS_BITNUM_SET(rd_flags, CBUF_XACT_BIT) &&
            IS_BITNUM_SET(rd_flags, CBUF_WAIT_BIT))
    {
        log_info(("Both read and write are in WAIT and XACT mode.\n"));
        return;
    }

    ASSERT_TRUE(API_SUCCESS == cbuf_alloc(buf, buf_len, &hdl));
    pltfm_memset(buf, 0xFF, buf_len);

    param_array[0] = &hdl;
    param_array[1] = &src_snk_len;
    param_array[2] = snk;
    param_array[3] = &rd_flags;
    param_array[4] = &buf_len;
    param_array[5] = &read_retry_count;

    ASSERT_NE(-1,  clock_gettime(CLOCK_REALTIME, &start));

    ASSERT_TRUE(0 == pltfm_thread_create(&t_id, NULL, d_fi_read_thread_func, &param_array));

    log_info(("\n------------------- Start of test %u, flags W:%u R:%u ---------------------\n", 
                seed, wr_flags, rd_flags));
    while (src_consumed < src_snk_len)
    {
        prev_rand_len = rand_len;
        do
        {
            rand_len = (UINT16)pltfm_rand();
        } while ((0 == rand_len) || (rand_len > (buf_len - 5)));

        if (0 == give_len)
        {
            rand_len = prev_rand_len>>1;
            if (0 == rand_len)
            {
                rand_len = prev_rand_len;
            }
        }
        if ((UINT32)rand_len > (src_snk_len - src_consumed))
        {
            rand_len = src_snk_len - src_consumed;
        }
        give_len = rand_len;

        /* Write to CBUF. */
        retval = cbuf_give(hdl, &src[src_consumed], &give_len, wr_flags);

        if (API_SUCCESS == retval)
        {
            if (IS_BITNUM_SET(wr_flags, CBUF_XACT_BIT))
            {
                EXPECT_EQ(give_len, rand_len);
            }
            else
            {
                EXPECT_LE(give_len, rand_len);
            }
        }
        else
        {
            EXPECT_EQ(CBUF_MT_NOT_ENUF_SPACE, retval);
            if (CBUF_MT_NOT_ENUF_SPACE != retval)
            {
                log_error(("\nGive Len = %u\n", give_len));
                log_error(("HDL = %u\n", hdl));
            }
        }

        if ((retval != API_SUCCESS) || ((API_SUCCESS == retval) && (0 == give_len)))
        {
            write_retry_count += 1;
            give_len = 0; /* To signal write failure when (retval != API_SUCCESS) */
        }
        else
        {
            src_consumed += give_len;
        }

        pltfm_thread_yield(); 
    }
    log_info(("\n******************* End of test %u, flags W:%u R:%u, %u *********************\n",
              seed, wr_flags, rd_flags, src_snk_len));

    /* Wait for receive thread to terminate. */
    ASSERT_TRUE(0 == pltfm_thread_join(t_id, NULL));

    ASSERT_NE(-1,  clock_gettime(CLOCK_REALTIME, &stop));
    elapsed.tv_sec = stop.tv_sec - start.tv_sec;
    elapsed.tv_nsec = stop.tv_nsec - start.tv_nsec;
    if (elapsed.tv_nsec < 0)
    {
        elapsed.tv_sec--;
        elapsed.tv_nsec += 1000000000L;
    }

    b_memcmp = (0 == pltfm_memcmp(src, snk, src_snk_len))? BOOL_TRUE: BOOL_FALSE;
    EXPECT_TRUE(BOOL_TRUE == b_memcmp);

    if (BOOL_FALSE == b_memcmp)
    {
        log_info(("Source Buffer - \n"));
        log_buffer(src, src_snk_len);
        log_info(("Sink Buffer - \n"));
        log_buffer(snk, src_snk_len);
    }
    else
    {
        log_temp(("Read retry count (%u) - %u\n", rd_flags, read_retry_count));
        log_temp(("Write retry count (%u) - %u\n", wr_flags, write_retry_count));
        log_temp(("Elapsed Time (0x%02x|0x%02x) - %2ld.%03ld s\n",
                    rd_flags, wr_flags,
                    (long)elapsed.tv_sec, elapsed.tv_nsec/(1000 * 1000)));
    }

    ASSERT_TRUE(API_SUCCESS == cbuf_free(hdl));
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_data_fidelity,
                        ::testing::Combine(::testing::Range(2, 0xFF, 0x3F),
                                           ::testing::Values(CBUF_NBLK|CBUF_ANY, CBUF_WAIT|CBUF_ANY,
                                                             CBUF_NBLK|CBUF_XACT, CBUF_WAIT|CBUF_XACT),
                                           ::testing::Values(CBUF_NBLK|CBUF_ANY, CBUF_WAIT|CBUF_ANY,
                                                             CBUF_NBLK|CBUF_XACT, CBUF_WAIT|CBUF_XACT)));


class Test_cbuf_file_fidelity: public ::Circular_Buffer_Test,
                      public ::testing::WithParamInterface< ::std::tr1::tuple<const CHAR *, int, int> >
{
    virtual void SetUp()
    {
        const CHAR *_file_name = (const CHAR *)(::std::tr1::get<0>(GetParam()));
        UINT16 seed;

        ASSERT_EQ(0, pltfm_mutex_init(&final_file_size_mutex, NULL));
        pltfm_strcpy(file_name, RCS_ROOT);
        pltfm_strcpy(w_file_name, RCS_ROOT "test_");
        seed = file_name[0] + file_name[1] + file_name[2];

        pltfm_strcat(file_name, _file_name);
        fpr = fopen(file_name, "rb");
        ASSERT_FALSE(NULL == fpr);
        pltfm_strcat(w_file_name, _file_name);
        fpw = fopen(w_file_name, "wb");
        ASSERT_FALSE(NULL == fpw);

        ASSERT_TRUE(API_SUCCESS == cbuf_init());
        pltfm_memset(buf, 0xFF, (UINT16)sizeof(buf));
        pltfm_srand(seed);
        pltfm_rand();
        do 
        {
            buf_size = (UINT16)(pltfm_rand());
        }
        while (0 == buf_size);
        pltfm_srand(pltfm_rand());
        ASSERT_TRUE(API_SUCCESS == cbuf_alloc(buf, buf_size, &hdl));

        wr_flags = (UINT8)(::std::tr1::get<1>(GetParam()));
        rd_flags = (UINT8)(::std::tr1::get<2>(GetParam()));
    }

    virtual void TearDown()
    {
        ASSERT_EQ(0, pltfm_mutex_destroy(&final_file_size_mutex));
        ASSERT_TRUE(API_SUCCESS == cbuf_free(hdl));
        ASSERT_TRUE(API_SUCCESS == cbuf_deinit());
    }

    protected:
        FILE *fpw, *fpr;
        CBUF_HDL hdl;
        UINT8 buf[0xFFFF];
        UINT16 buf_size;
        UINT8 src[0xFFFF];
        UINT8 snk[0xFFFF];
        UINT8 wr_flags;
        UINT8 rd_flags;
        CHAR file_name[100];
        CHAR w_file_name[100];
        pltfm_mutex_t final_file_size_mutex;
};

void* f_fi_read_thread_func(void *t_param)
{
    void* (*param_array)[F_FI_RD_THREAD_NUM_PARAMS] = (void * (*)[F_FI_RD_THREAD_NUM_PARAMS])t_param;
    CBUF_HDL hdl = *((CBUF_HDL *)(*param_array)[0]);
    UINT8 *snk = (UINT8 *)(*param_array)[1];
    UINT8 rd_flags = *((UINT8 *)(*param_array)[2]);
    UINT16 buf_size = *((UINT16 *)(*param_array)[3]);
    FILE *fpw = (FILE *)(*param_array)[4];
    UINT32* final_file_size = (UINT32 *)(*param_array)[5];
    UINT16* read_retry_count = (UINT16 *)(*param_array)[6];
    pltfm_mutex_t *mutex = (pltfm_mutex_t *)(*param_array)[7];
    UINT32 r_num_total = 0;
    UINT16 read_len;
    UINT16 prev_read_len;
    UINT16 get_len = 1;
    API_RESULT retval;
    UINT32 file_size = 0;

    log_info(("\n********** In f_fi_read_thread_func() *****************\n"));
    while ((0 == file_size) || (r_num_total < file_size))
    {
        prev_read_len = read_len;
        do
        {
            read_len = (UINT16)pltfm_rand();
        } while ((0 == read_len) || (read_len > (buf_size - 5)));

        if (0 == get_len)
        {
            read_len = prev_read_len>>1;
            if (0 == read_len)
            {
                read_len = prev_read_len;
            }
        }
        if ((0 != file_size) && ((UINT32)read_len > (file_size - r_num_total)))
        {
            read_len = file_size - r_num_total;
        }
        get_len = read_len;

        /* Read from CBUF. */
        retval = cbuf_get(hdl, snk, &get_len, rd_flags);

        if (API_SUCCESS == retval)
        {
            if (IS_BITNUM_SET(rd_flags, CBUF_XACT_BIT))
            {
                EXPECT_EQ(get_len, read_len);
            }
            else
            {
                EXPECT_LE(get_len, read_len);
            }
        }
        else
        {
            EXPECT_TRUE((CBUF_MT_NOT_ENUF_DATA == retval) || 
                        (CBUF_MT_SUSPENDED_WRITE == retval));
            if (CBUF_MT_SUSPENDED_WRITE == retval)
            {
                get_len = buf_size - 1;
                retval = cbuf_get(hdl, snk, &get_len, 0);
                EXPECT_EQ(API_SUCCESS, retval);
                if ((API_SUCCESS == retval) && (get_len > 0))
                {
                    fwrite(snk, 1, get_len, fpw);
                }
                break;
            }
        }

        if ((retval != API_SUCCESS) || ((API_SUCCESS == retval) && (0 == get_len)))
        {
            read_retry_count += 1;
        }
        else
        {
            r_num_total += get_len;
            fwrite(snk, 1, get_len, fpw);
        }

        pltfm_thread_yield(); 

        EXPECT_EQ(0, pltfm_mutex_lock(mutex)); 
        file_size = *final_file_size;
        EXPECT_EQ(0, pltfm_mutex_unlock(mutex)); 
    }

    return NULL;
}

TEST_P(Test_cbuf_file_fidelity, random_write_read)
{
    pltfm_thread_t t_id;
    void *param_array[F_FI_RD_THREAD_NUM_PARAMS];
    UINT16 read_len;
    API_RESULT retval;
    UINT16 give_len = 1;
    BOOL_T retry = BOOL_FALSE;
    UINT16 f_read_len;
    UINT16 write_retry_count = 0;
    UINT16 read_retry_count = 0;
    struct timespec start, stop, elapsed; 
    TEST_RESULTS comp_result;
    UINT32 file_size = 0;
    UINT32 final_file_size = 0;

    if (IS_BITNUM_SET(wr_flags, CBUF_XACT_BIT) && 
            IS_BITNUM_SET(wr_flags, CBUF_WAIT_BIT) &&
            IS_BITNUM_SET(rd_flags, CBUF_XACT_BIT) &&
            IS_BITNUM_SET(rd_flags, CBUF_WAIT_BIT))
    {
        log_info(("Both read and write are in WAIT and XACT mode.\n"));
        return;
    }

    param_array[0] = &hdl;
    param_array[1] = snk;
    param_array[2] = &rd_flags;
    param_array[3] = &buf_size;
    param_array[4] = fpw;
    param_array[5] = &final_file_size;
    param_array[6] = &read_retry_count;
    param_array[7] = &final_file_size_mutex;

    ASSERT_NE(-1,  clock_gettime(CLOCK_REALTIME, &start));

    ASSERT_TRUE(0 == pltfm_thread_create(&t_id, NULL, f_fi_read_thread_func, &param_array));

    log_info(("\n------------------- Start of test %s, flags W:%u R:%u ---------------------\n",
                file_name, wr_flags, rd_flags));
    while (1)
    {
        if (BOOL_FALSE == retry)
        {
            do
            {
                read_len = ((UINT16)pltfm_rand() % (buf_size - 5));
            }
            while(0 == read_len);
            f_read_len = fread(src, 1, read_len, fpr);
            if (0 == f_read_len)
            {
                break;
            }
        }
        give_len = f_read_len;
        retval = cbuf_give(hdl, src, &give_len, wr_flags);
        if ((API_SUCCESS != retval) || (give_len < f_read_len))
        {
            while (CBUF_MT_NOT_ENUF_SPACE == retval)
            {
                pltfm_thread_yield(); 
                give_len >>= 1; 
                give_len += 1; 
                write_retry_count += 1;
                retval = cbuf_give(hdl, src, &give_len, wr_flags);
            }
            if ((API_SUCCESS == retval) && (give_len > 0))
            {
                file_size += give_len;
                f_read_len -= give_len;
                pltfm_memmove(src, &src[give_len], f_read_len);
            }
            if ((retval != API_SUCCESS) && (retval != CBUF_MT_NOT_ENUF_SPACE))
            {
                log_warning(("HDL = %u\nSrc = %p\nGive Len = %u\nWR_FLAGS = %u\nbuf_size = %u\n",
                            hdl, src, give_len, wr_flags, buf_size));
            }
            if (0 != f_read_len)
            {
                retry = BOOL_TRUE;
                write_retry_count += 1;
            }
            else
            {
                retry = BOOL_FALSE;
            }
        }
        else
        {
            file_size += give_len;
            retry = BOOL_FALSE;
        }

        pltfm_thread_yield(); 
    }
    ASSERT_EQ(0, pltfm_mutex_lock(&final_file_size_mutex));
    final_file_size = file_size; 
    ASSERT_EQ(0, pltfm_mutex_unlock(&final_file_size_mutex));

    /* Indicate that the write has come to a stop in case the read thread is
     * using blocking cbuf_get(). */
    if (IS_BITNUM_SET(rd_flags, CBUF_WAIT_BIT))
    {
        EXPECT_TRUE(API_SUCCESS == cbuf_suspend_write(hdl, BOOL_TRUE));
    }

    log_info(("\n******************* End of test %s, flags W:%u R:%u *********************\n",
                file_name, wr_flags, rd_flags));

    /* Wait for receive thread to terminate. */
    ASSERT_TRUE(0 == pltfm_thread_join(t_id, NULL));

    ASSERT_NE(-1,  clock_gettime(CLOCK_REALTIME, &stop));
    elapsed.tv_sec = stop.tv_sec - start.tv_sec;
    elapsed.tv_nsec = stop.tv_nsec - start.tv_nsec;
    if (elapsed.tv_nsec < 0)
    {
        elapsed.tv_sec--;
        elapsed.tv_nsec += 1000000000L;
    }

    fclose(fpr);
    fclose(fpw);

    comp_result = compare_files(file_name, w_file_name);
    EXPECT_TRUE(TEST_PASS == comp_result);
    if (TEST_PASS == comp_result)
    {
        log_temp(("Read retry count (%u) - %u\n", rd_flags, read_retry_count));
        log_temp(("Write retry count (%u) - %u\n", wr_flags, write_retry_count));
        log_temp(("Elapsed Time (0x%02x|0x%02x) - %2ld.%03ld s\n",
                    rd_flags, wr_flags,
                    (long)elapsed.tv_sec, elapsed.tv_nsec/(1000 * 1000)));
    }
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_file_fidelity,
                        ::testing::Combine(::testing::ValuesIn(test_files),
                                           ::testing::Values(CBUF_NBLK|CBUF_ANY, CBUF_WAIT|CBUF_ANY,
                                                             CBUF_NBLK|CBUF_XACT, CBUF_WAIT|CBUF_XACT),
                                           ::testing::Values(CBUF_NBLK|CBUF_ANY, CBUF_WAIT|CBUF_ANY,
                                                             CBUF_NBLK|CBUF_XACT, CBUF_WAIT|CBUF_XACT)));

#else /* GTEST_HAS_COMBINE */

class Test_cbuf_data_fidelity: public ::Circular_Buffer_Test,
                      public ::testing::WithParamInterface<int>
{
    virtual void SetUp()
    {
        UINT32 i;

        src_snk_len = 0xFFFFFFF - 0xFFFF;
        ASSERT_TRUE(API_SUCCESS == cbuf_init());
        ASSERT_TRUE(NULL != (src = (UINT8 *)pltfm_malloc(src_snk_len)));
        ASSERT_TRUE(NULL != (snk = (UINT8 *)pltfm_calloc(src_snk_len, 1)));

        for (i = 0; i < src_snk_len; i++) 
        {
            src[i] = i + 1;
        }

        ASSERT_FALSE(0 == pltfm_memcmp(src, snk, src_snk_len));
    }

    virtual void TearDown()
    {
        pltfm_free(snk);
        pltfm_free(src);
        ASSERT_TRUE(API_SUCCESS == cbuf_deinit());
    }

    protected:
        UINT8 *src;
        UINT8 *snk;
        UINT32 src_snk_len;
};

TEST_P(Test_cbuf_data_fidelity, random_write_read)
{
    CBUF_HDL hdl;
    UINT8 buf[0xFFFF];
    UINT16 seed = (UINT16)GetParam() + 1;
    UINT16 read_len;
    UINT16 tx_count = 0;
    UINT16 rx_count = 0;
    API_RESULT retval;
    UINT32 src_consumed = 0;
    UINT32 r_num_total = 0;
    BOOL_T b_memcmp;

    ASSERT_TRUE(API_SUCCESS == cbuf_alloc(buf, sizeof(buf), &hdl));
    pltfm_memset(buf, 0xFF, (UINT16)sizeof(buf));
    pltfm_srand(seed);
    pltfm_rand();
    pltfm_srand(pltfm_rand());

    log_info(("\n------------------- Start of test %u ---------------------\n", seed));
    while (r_num_total < src_snk_len)
    {
        tx_count = 0;
        do
        {
            read_len = (UINT16)pltfm_rand();
            if ((UINT32)read_len > (src_snk_len - src_consumed))
            {
                read_len = src_snk_len - src_consumed;
            }
            retval = cbuf_give(hdl, &src[src_consumed], &read_len, CBUF_XACT);
            if (API_SUCCESS == retval)
            {
                src_consumed += read_len;
            }
            tx_count++;
        }
        while((API_SUCCESS != retval) && (tx_count < TX_ATTEMPT_LIMIT) &&
                (src_consumed < src_snk_len));

        rx_count = 0;
        do
        {
            read_len = (UINT16)pltfm_rand();
            EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, &snk[r_num_total], &read_len, 0));
            rx_count++;
        }
        while((0 == read_len) && (rx_count < RX_ATTEMPT_LIMIT));

        r_num_total += read_len;
    }
    log_info(("\n******************* End of test %u, %u *********************\n", seed, src_snk_len));

    b_memcmp = (0 == pltfm_memcmp(src, snk, src_snk_len))? BOOL_TRUE: BOOL_FALSE;
    EXPECT_TRUE(BOOL_TRUE == b_memcmp);

    if (BOOL_FALSE == b_memcmp)
    {
        log_info(("Source Buffer - \n"));
        log_buffer(src, src_snk_len);
        log_info(("Sink Buffer - \n"));
        log_buffer(snk, src_snk_len);
    }

    ASSERT_TRUE(API_SUCCESS == cbuf_free(hdl));
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_data_fidelity,
                        ::testing::Range(2, 0xFF, 0x3F));


class Test_cbuf_file_fidelity: public ::Circular_Buffer_Test,
                      public ::testing::WithParamInterface<const CHAR *> { };

TEST_P(Test_cbuf_file_fidelity, random_write_read)
{
    CBUF_HDL hdl;
    FILE *fpw, *fpr;
    UINT8 buf[0xFFFF];
    UINT16 buf_size;
    UINT8 src[0xFFFF] = {0};
    UINT8 snk[0xFFFF] = {0};
    const CHAR *_file_name = GetParam();
    CHAR file_name[100] = RCS_ROOT;
    CHAR w_file_name[100] = RCS_ROOT "test_";
    UINT16 seed = file_name[0] + file_name[1] + file_name[2];
    UINT16 read_len;
    API_RESULT retval;
    BOOL_T retry = BOOL_FALSE;
    UINT16 f_read_len;

    pltfm_memset(buf, 0xFF, (UINT16)sizeof(buf));
    pltfm_srand(seed);
    pltfm_rand();
    buf_size = pltfm_rand();
    pltfm_srand(pltfm_rand());
    ASSERT_TRUE(API_SUCCESS == cbuf_alloc(buf, buf_size, &hdl));

    pltfm_strcat(file_name, _file_name);
    fpr = fopen(file_name, "rb");
    ASSERT_FALSE(NULL == fpr);
    pltfm_strcat(w_file_name, _file_name);
    fpw = fopen(w_file_name, "wb");
    ASSERT_FALSE(NULL == fpw);

    log_info(("\n------------------- Start of test %s ---------------------\n", file_name));
    do
    {
        if (BOOL_FALSE == retry)
        {
            do
            {
                read_len = ((UINT16)pltfm_rand()) % buf_size;
            }
            while(0 == read_len);
            f_read_len = fread(src, 1, read_len, fpr);
        }
        log_info(("Rand Len = %u\n", read_len));
        log_info(("Read Len = %u\n", f_read_len));
        retval = cbuf_give(hdl, src, &f_read_len, CBUF_XACT);
        if (API_SUCCESS != retval)
        {
            retry = BOOL_TRUE;
        }
        else
        {
            retry = BOOL_FALSE;
        }

        read_len = (UINT16)pltfm_rand();
        EXPECT_TRUE(API_SUCCESS == cbuf_get(hdl, snk, &read_len, 0));
        fwrite(snk, 1, read_len, fpw);
    }
    while((f_read_len > 0) || (read_len > 0));

    log_info(("\n******************* End of test %s *********************\n", file_name));
    fclose(fpr);
    fclose(fpw);

    EXPECT_TRUE(TEST_PASS == compare_files(file_name, w_file_name));
    ASSERT_TRUE(API_SUCCESS == cbuf_free(hdl));
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cbuf_file_fidelity,
                        ::testing::ValuesIn(test_files));
#endif /* GTEST_HAS_COMBINE */
