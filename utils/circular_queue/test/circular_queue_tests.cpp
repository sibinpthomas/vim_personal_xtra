#include "platform.h"
#include "gtest/gtest.h"
#include "test_utils.h"
#include "logger.h"
#include "circular_queue.h"

#define BUFSIZE                             (8u)
#define TX_ATTEMPT_LIMIT                    (5u)
#define RX_ATTEMPT_LIMIT                    (5u)
#define RCS_ROOT                            "resources/"

class Circular_Queue_Test : public ::testing::Test
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
            ASSERT_TRUE(API_SUCCESS == cque_init());
        }

        virtual void TearDown()
        {
            ASSERT_TRUE(API_SUCCESS == cque_deinit());
        }
};

class Test_cque_alloc_Neg: public ::Circular_Queue_Test { };
class Test_cque_alloc: public ::Circular_Queue_Test { };

class Test_cque_free_Neg: public ::Circular_Queue_Test { };
class Test_cque_free: public ::Circular_Queue_Test { };

TEST_F(Test_cque_alloc_Neg, invalid_input)
{
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 0, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 0, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 1, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 1, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 2, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 2, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 3, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 3, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 4, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 4, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 9, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 0, 9, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 0, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 0, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 1, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 1, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 2, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 2, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 3, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 3, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 4, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 4, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 9, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 1, 9, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 0, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 0, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 1, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 1, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 2, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 2, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 3, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 3, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 4, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 4, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 9, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc(NULL, 22, 9, (CQUE_HDL *)2));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 0, 0, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 0, 1, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 0, 2, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 0, 3, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 0, 4, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 0, 9, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 1, 0, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 1, 1, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 1, 2, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 1, 3, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 1, 4, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 1, 9, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 22, 0, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 22, 1, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 22, 2, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 22, 3, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 22, 4, NULL));
    EXPECT_FALSE(API_SUCCESS == cque_alloc((UINT8 *)2, 22, 9, NULL));
}

TEST_F(Test_cque_alloc, valid_input)
{
    CQUE_HDL hdl;
    UINT8 buf[100];

    EXPECT_TRUE(API_SUCCESS == cque_alloc(buf, sizeof(buf[0]), (UINT16)sizeof(buf), &hdl));
    log_info(("Handle is 0x%04x\n", hdl));
}

TEST_F(Test_cque_alloc, until_fail)
{
    CQUE_HDL hdl;
    UINT8 buf[100];
    UINT16 cque_count = 0;

    for (cque_count = 0; cque_count < (UINT16)(~(0u)); cque_count++)
    {
        if (API_SUCCESS != cque_alloc(buf, sizeof(buf[0]), (UINT16)sizeof(buf), &hdl))
        {
            break;
        }
        log_info(("Handle is 0x%04x\n", hdl));
    }

    EXPECT_FALSE(cque_count == (UINT16)(~(0u)));
}

TEST_F(Test_cque_free_Neg, invalid_input)
{
    EXPECT_FALSE(API_SUCCESS == cque_free(0));
    EXPECT_FALSE(API_SUCCESS == cque_free(3)); /* > CQUE_NUM_QUEUES */
    EXPECT_FALSE(API_SUCCESS == cque_free(8));
}

TEST_F(Test_cque_free, valid_input)
{
    CQUE_HDL hdl;
    UINT8 buf[100];

    EXPECT_TRUE(API_SUCCESS == cque_alloc(buf, sizeof(buf[0]), (UINT16)sizeof(buf), &hdl));
    log_info(("Handle is 0x%04x\n", hdl));
    EXPECT_TRUE(API_SUCCESS == cque_free(hdl));
}

class Test_cque_push_Neg: public ::Circular_Queue_Test { };
class Test_cque_push: public ::Circular_Queue_Test
{
    virtual void SetUp()
    {
        ASSERT_TRUE(API_SUCCESS == cque_init());
        ASSERT_TRUE(API_SUCCESS == cque_alloc(buf, sizeof(buf[0]), (UINT16)sizeof(buf), &hdl));
    }

    virtual void TearDown()
    {
        ASSERT_TRUE(API_SUCCESS == cque_free(hdl));
        ASSERT_TRUE(API_SUCCESS == cque_deinit());
    }

    protected:
        CQUE_HDL hdl;
        UINT8 buf[BUFSIZE];
};

TEST_F(Test_cque_push, valid_input)
{
    UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};

    EXPECT_TRUE(API_SUCCESS == cque_push(hdl, data, sizeof(data)));
}

class Test_cque_pop: public ::Circular_Queue_Test
{
    virtual void SetUp()
    {
        UINT8 data[BUFSIZE - 1] = {1, 2, 3, 4, 5, 6, 7};

        ASSERT_TRUE(API_SUCCESS == cque_init());
        ASSERT_TRUE(API_SUCCESS == cque_alloc(buf, sizeof(buf[0]), (UINT16)sizeof(buf), &hdl));
        ASSERT_TRUE(API_SUCCESS == cque_push(hdl, data, sizeof(data)));
    }

    virtual void TearDown()
    {
        ASSERT_TRUE(API_SUCCESS == cque_free(hdl));
        ASSERT_TRUE(API_SUCCESS == cque_deinit());
    }

    protected:
        CQUE_HDL hdl;
        UINT8 buf[BUFSIZE];
};

TEST_F(Test_cque_pop, valid_input_01)
{
    UINT8 buf[BUFSIZE] = {0};

    log_buffer(buf, BUFSIZE);
    EXPECT_TRUE((BUFSIZE - 1) == cque_pop(hdl, buf, sizeof(buf)));
    log_buffer(buf, BUFSIZE - 1);
}

TEST_F(Test_cque_pop, valid_input_02)
{
    UINT8 buf[BUFSIZE] = {0};
    UINT8 pos = 0;

    log_buffer(buf, BUFSIZE);
    EXPECT_TRUE(1 == cque_pop(hdl, &buf[pos], 1));
    pos += 1;
    EXPECT_TRUE(1 == cque_pop(hdl, &buf[pos], 1));
    pos += 1;
    EXPECT_TRUE(1 == cque_pop(hdl, &buf[pos], 1));
    pos += 1;
    EXPECT_TRUE(1 == cque_pop(hdl, &buf[pos], 1));
    pos += 1;
    EXPECT_TRUE(1 == cque_pop(hdl, &buf[pos], 1));
    pos += 1;
    EXPECT_TRUE(1 == cque_pop(hdl, &buf[pos], 1));
    pos += 1;
    EXPECT_TRUE(1 == cque_pop(hdl, &buf[pos], 1));
    pos += 1;
    EXPECT_TRUE(0 == cque_pop(hdl, &buf[pos], 1));
    log_buffer(buf, BUFSIZE - 1);
}

class Test_cque_data_fidelity: public ::Circular_Queue_Test,
                      public ::testing::WithParamInterface<int>
{
    virtual void SetUp()
    {
        UINT32 i;

        src_snk_len = 0xFFFFFFF - 0xFFFF;
        ASSERT_TRUE(API_SUCCESS == cque_init());
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
        ASSERT_TRUE(API_SUCCESS == cque_deinit());
    }

    protected:
        UINT8 *src;
        UINT8 *snk;
        UINT32 src_snk_len;
};

TEST_P(Test_cque_data_fidelity, random_write_read)
{
    CQUE_HDL hdl;
    UINT8 buf[0xFFFF];
    UINT16 seed = (UINT16)GetParam() + 1;
    UINT16 rand_len;
    UINT16 tx_count = 0;
    UINT16 rx_count = 0;
    API_RESULT retval;
    UINT16 r_num;
    UINT32 src_consumed = 0;
    UINT32 r_num_total = 0;
    BOOL_T b_memcmp;

    ASSERT_TRUE(API_SUCCESS == cque_alloc(buf, sizeof(buf[0]), sizeof(buf), &hdl));
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
            rand_len = (UINT16)pltfm_rand();
            if ((UINT32)rand_len > (src_snk_len - src_consumed))
            {
                rand_len = src_snk_len - src_consumed;
            }
            retval = cque_push(hdl, &src[src_consumed], rand_len);
            if (API_SUCCESS == retval)
            {
                src_consumed += rand_len;
            }
            tx_count++;
        }
        while((API_SUCCESS != retval) && (tx_count < TX_ATTEMPT_LIMIT) &&
                (src_consumed < src_snk_len));

        rx_count = 0;
        do
        {
            rand_len = (UINT16)pltfm_rand();
            r_num = cque_pop(hdl, &snk[r_num_total], rand_len);
            rx_count++;
        }
        while((0 == r_num) && (rx_count < RX_ATTEMPT_LIMIT));

        r_num_total += r_num;
    }
    log_info(("\n******************* End of test %u, %u *********************\n", seed, src_snk_len));

    b_memcmp = (0 == pltfm_memcmp(src, snk, src_snk_len))? BOOL_TRUE: BOOL_FALSE;
    EXPECT_TRUE(BOOL_TRUE == b_memcmp);

    if (BOOL_FALSE == b_memcmp)
    {
        log_info(("Source Queue - \n"));
        log_buffer(src, src_snk_len);
        log_info(("Sink Queue - \n"));
        log_buffer(snk, src_snk_len);
    }

    ASSERT_TRUE(API_SUCCESS == cque_free(hdl));
}

class Test_cque_data_fidelity_elem_size_4: public ::Circular_Queue_Test,
                      public ::testing::WithParamInterface<int>
{
    virtual void SetUp()
    {
        UINT32 i;

        src_snk_len = 0xFFFFFFF - 0xFFFF;
        ASSERT_TRUE(API_SUCCESS == cque_init());
        ASSERT_TRUE(NULL != (src = (UINT32 *)pltfm_malloc(src_snk_len * sizeof(UINT32))));
        ASSERT_TRUE(NULL != (snk = (UINT32 *)pltfm_calloc(src_snk_len, sizeof(UINT32))));

        for (i = 0; i < src_snk_len; i++) 
        {
            src[i] = i + 1;
        }

        ASSERT_FALSE(0 == pltfm_memcmp(src, snk, src_snk_len * sizeof(UINT32)));
    }

    virtual void TearDown()
    {
        pltfm_free(snk);
        pltfm_free(src);
        ASSERT_TRUE(API_SUCCESS == cque_deinit());
    }

    protected:
        UINT32 *src;
        UINT32 *snk;
        UINT32 src_snk_len;
};

TEST_P(Test_cque_data_fidelity_elem_size_4, random_write_read)
{
    CQUE_HDL hdl;
    UINT32 buf[0xFFFF];
    UINT16 seed = (UINT16)GetParam() + 1;
    UINT16 rand_len;
    UINT16 tx_count = 0;
    UINT16 rx_count = 0;
    API_RESULT retval;
    UINT16 r_num;
    UINT32 src_consumed = 0;
    UINT32 r_num_total = 0;
    BOOL_T b_memcmp;

    ASSERT_TRUE(API_SUCCESS == cque_alloc(buf, sizeof(buf[0]), LEN_OF_ARRAY(buf), &hdl));
    pltfm_memset(buf, 0xFF, (UINT32)sizeof(buf));
    pltfm_srand(seed);
    pltfm_rand();
    pltfm_srand(pltfm_rand());

    log_info(("\n------------------- Start of test %u ---------------------\n", seed));
    while (r_num_total < src_snk_len)
    {
        tx_count = 0;
        do
        {
            rand_len = (UINT16)pltfm_rand();
            if ((UINT32)rand_len > (src_snk_len - src_consumed))
            {
                rand_len = src_snk_len - src_consumed;
            }
            retval = cque_push(hdl, &src[src_consumed], rand_len);
            if (API_SUCCESS == retval)
            {
                src_consumed += rand_len;
            }
            tx_count++;
        }
        while((API_SUCCESS != retval) && (tx_count < TX_ATTEMPT_LIMIT) &&
                (src_consumed < src_snk_len));

        rx_count = 0;
        do
        {
            rand_len = (UINT16)pltfm_rand();
            r_num = cque_pop(hdl, &snk[r_num_total], rand_len);
            rx_count++;
        }
        while((0 == r_num) && (rx_count < RX_ATTEMPT_LIMIT));

        r_num_total += r_num;
    }
    log_info(("\n******************* End of test %u, %u *********************\n", seed, src_snk_len));

    b_memcmp = (0 == pltfm_memcmp(src, snk, src_snk_len * sizeof(UINT32)))? BOOL_TRUE: BOOL_FALSE;
    EXPECT_TRUE(BOOL_TRUE == b_memcmp);

    if (BOOL_FALSE == b_memcmp)
    {
        log_info(("Source Queue - \n"));
        log_buffer((UINT8 *)src, src_snk_len * sizeof(UINT32));
        log_info(("Sink Queue - \n"));
        log_buffer((UINT8 *)snk, src_snk_len * sizeof(UINT32));
    }

    ASSERT_TRUE(API_SUCCESS == cque_free(hdl));
}

typedef struct
{
    CHAR ch;
    UINT32 num;
}T_STRUCT;

class Test_cque_data_fidelity_struct_01: public ::Circular_Queue_Test,
                      public ::testing::WithParamInterface<int>
{
    virtual void SetUp()
    {
        UINT32 i;

        src_snk_len = 0xFFFFFFF - 0xFFFF;
        ASSERT_TRUE(API_SUCCESS == cque_init());
        ASSERT_TRUE(NULL != (src = (T_STRUCT *)pltfm_malloc(src_snk_len * sizeof(T_STRUCT))));
        ASSERT_TRUE(NULL != (snk = (T_STRUCT *)pltfm_calloc(src_snk_len, sizeof(T_STRUCT))));

        for (i = 0; i < src_snk_len; i++) 
        {
            src[i].ch = (i % 26) + 65;
            src[i].num = i + 1;
        }

        ASSERT_FALSE(0 == pltfm_memcmp(src, snk, src_snk_len * sizeof(T_STRUCT)));
    }

    virtual void TearDown()
    {
        pltfm_free(snk);
        pltfm_free(src);
        ASSERT_TRUE(API_SUCCESS == cque_deinit());
    }

    protected:
        T_STRUCT *src;
        T_STRUCT *snk;
        UINT32 src_snk_len;
};

TEST_P(Test_cque_data_fidelity_struct_01, random_write_read)
{
    CQUE_HDL hdl;
    T_STRUCT buf[0xFFFF];
    UINT16 seed = (UINT16)GetParam() + 1;
    UINT16 rand_len;
    UINT16 tx_count = 0;
    UINT16 rx_count = 0;
    API_RESULT retval;
    UINT16 r_num;
    UINT32 src_consumed = 0;
    UINT32 r_num_total = 0;
    BOOL_T b_memcmp;

    ASSERT_TRUE(API_SUCCESS == cque_alloc(buf, sizeof(buf[0]), LEN_OF_ARRAY(buf), &hdl));
    pltfm_memset(buf, 0xFF, (UINT32)sizeof(buf));
    pltfm_srand(seed);
    pltfm_rand();
    pltfm_srand(pltfm_rand());

    log_info(("\n------------------- Start of test %u ---------------------\n", seed));
    while (r_num_total < src_snk_len)
    {
        tx_count = 0;
        do
        {
            rand_len = (UINT16)pltfm_rand();
            if ((UINT32)rand_len > (src_snk_len - src_consumed))
            {
                rand_len = src_snk_len - src_consumed;
            }
            retval = cque_push(hdl, &src[src_consumed], rand_len);
            if (API_SUCCESS == retval)
            {
                src_consumed += rand_len;
            }
            tx_count++;
        }
        while((API_SUCCESS != retval) && (tx_count < TX_ATTEMPT_LIMIT) &&
                (src_consumed < src_snk_len));

        rx_count = 0;
        do
        {
            rand_len = (UINT16)pltfm_rand();
            r_num = cque_pop(hdl, &snk[r_num_total], rand_len);
            rx_count++;
        }
        while((0 == r_num) && (rx_count < RX_ATTEMPT_LIMIT));

        r_num_total += r_num;
    }
    log_info(("\n******************* End of test %u, %u *********************\n", seed, src_snk_len));

    b_memcmp = (0 == pltfm_memcmp(src, snk, src_snk_len * sizeof(T_STRUCT)))? BOOL_TRUE: BOOL_FALSE;
    EXPECT_TRUE(BOOL_TRUE == b_memcmp);

    if (BOOL_FALSE == b_memcmp)
    {
        log_info(("Source Queue - \n"));
        log_buffer((UINT8 *)src, src_snk_len * sizeof(T_STRUCT));
        log_info(("Sink Queue - \n"));
        log_buffer((UINT8 *)snk, src_snk_len * sizeof(T_STRUCT));
    }

    ASSERT_TRUE(API_SUCCESS == cque_free(hdl));
}

class Test_cque_file_fidelity: public ::Circular_Queue_Test,
                      public ::testing::WithParamInterface<const char *> { };

TEST_P(Test_cque_file_fidelity, random_write_read)
{
    CQUE_HDL hdl;
    FILE *fpw, *fpr;
    UINT8 buf[0xFFFF];
    UINT16 buf_size;
    UINT8 src[0xFFFF] = {0};
    UINT8 snk[0xFFFF] = {0};
    const char *_file_name = GetParam();
    char file_name[100] = RCS_ROOT;
    char w_file_name[100] = RCS_ROOT "test_";
    UINT16 seed = file_name[0] + file_name[1] + file_name[2];
    UINT16 rand_len;
    UINT16 rx_count = 0;
    API_RESULT retval;
    UINT16 r_num;
    BOOL_T retry = BOOL_FALSE;
    UINT32 read_len;

    pltfm_memset(buf, 0xFF, (UINT16)sizeof(buf));
    pltfm_srand(seed);
    pltfm_rand();
    buf_size = pltfm_rand();
    pltfm_srand(pltfm_rand());
    ASSERT_TRUE(API_SUCCESS == cque_alloc(buf, sizeof(buf[0]), buf_size, &hdl));

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
                rand_len = ((UINT16)pltfm_rand()) % buf_size;
            }
            while(0 == rand_len);
            read_len = fread(src, 1, rand_len, fpr);
        }
        log_info(("Rand Len = %u\n", rand_len));
        log_info(("Read Len = %u\n", read_len));
        retval = cque_push(hdl, src, read_len);
        if (API_SUCCESS != retval)
        {
            retry = BOOL_TRUE;
        }
        else
        {
            retry = BOOL_FALSE;
        }

        rand_len = (UINT16)pltfm_rand();
        r_num = cque_pop(hdl, snk, rand_len);
        fwrite(snk, 1, r_num, fpw);
    }
    while((read_len > 0) || (r_num > 0));

    log_info(("\n******************* End of test %s *********************\n", file_name));
    fclose(fpr);
    fclose(fpw);

    EXPECT_TRUE(TEST_PASS == compare_files(file_name, w_file_name));
    ASSERT_TRUE(API_SUCCESS == cque_free(hdl));
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cque_data_fidelity,
                        ::testing::Range(2, 0xFF, 0x3F));

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cque_data_fidelity_elem_size_4,
                        ::testing::Range(2, 0xFF, 0x3F));

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cque_data_fidelity_struct_01,
                        ::testing::Range(2, 0xFF, 0x3F));

const char* test_files[] = {"Sinkarimelam_1.mp3",
                            "Core_V4.0.pdf",
                            "high_on_life.jpg"};
INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_cque_file_fidelity,
                        ::testing::ValuesIn(test_files));
