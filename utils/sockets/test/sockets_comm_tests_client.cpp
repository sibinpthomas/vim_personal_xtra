#include "platform.h"
#include "gtest/gtest.h"
#include "test_utils.h"
#include "logger.h"
#include "sockets.h"
#include "sockets_pl_internal.h"
#include "socket_test_utils.h"

#define BUFSIZE                             (5u)
#define NUM_SOCKETS                         (2u)
#define SERVER_PORT                         (12678u)
#define STRINGIZE(x)                        #x
#define STR_INTRMDT(y)                      STRINGIZE(y)
#define RCS_ROOT                            STR_INTRMDT(TEST_ROOT) STRINGIZE(/resources/)

class Sockets_Comm_Test_Client : public ::testing::Test
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
            ASSERT_EQ(API_SUCCESS, sockets_init(BOOL_FALSE, SERVER_PORT, NUM_SOCKETS, NULL));

            log_info((" Client Ready to make connections at server port %d.\n", SERVER_PORT));
        }

        virtual void TearDown()
        {
            ASSERT_TRUE(API_SUCCESS == sockets_deinit());
        }
};

class Test_socket_socket_comm: public ::Sockets_Comm_Test_Client { };
TEST_F(Test_socket_socket_comm, duplex_comm)
{
    SOCKET_HDL client_sock[NUM_SOCKETS];
    UINT16_F i;

    UINT8 send_buf[][BUFSIZE] = {
        {1, 2, 3, 4, 5},
        {0xF1, 0xF2, 0xF3, 0xF4, 0xF5},
        {0xC1, 0xC2, 0xC3, 0xC4, 0xC5},
    };
    UINT8 recv_buf[BUFSIZE] = {0};

    for (i = 0; i < NUM_SOCKETS; i++)
    {
        ASSERT_EQ(API_SUCCESS, socket_alloc(&client_sock[i]));
    }

    /* Client -> Server */
    ASSERT_NE(SOCKET_ERROR, socket_send(client_sock[0], send_buf[0], BUFSIZE, 0));
    log_info((" Sent message.\r\n"));

    /* Client -> Server */
    ASSERT_NE(SOCKET_ERROR, socket_send(client_sock[1], send_buf[2], BUFSIZE, 0));
    log_info((" Sent message.\r\n"));

    /* Server -> Client */
    ASSERT_NE(SOCKET_ERROR, socket_recv(client_sock[1], recv_buf, BUFSIZE, 0));
    EXPECT_EQ(0, pltfm_memcmp(recv_buf, send_buf[2], BUFSIZE));
    log_info((" Received sent message.\r\n"));

    /* Server -> Client */
    ASSERT_NE(SOCKET_ERROR, socket_recv(client_sock[0], recv_buf, BUFSIZE, 0));
    EXPECT_EQ(0, pltfm_memcmp(recv_buf, send_buf[1], BUFSIZE));
    log_info((" Received sent message.\r\n"));

    /* Server -> Client - Read corresponding to 0 length write */
    ASSERT_LE(socket_recv(client_sock[1], recv_buf, BUFSIZE, 0), 0);
    log_socket_error();
    log_info((" Received sent message.\r\n"));

    /* Server -> Client - Read 0 length */
    ASSERT_LE(socket_recv(client_sock[0], recv_buf, 0, 0), 0);
    log_socket_error();
    log_info((" Received sent message.\r\n"));

    for (i = 0; i < NUM_SOCKETS; i++)
    {
        ASSERT_EQ(API_SUCCESS, socket_free(client_sock[i]));
    }
}

class Test_socket_data_fidelity: public ::Sockets_Comm_Test_Client,
                      public ::testing::WithParamInterface<int>
{
    virtual void SetUp()
    {
        UINT32 i;

        setup_stage = 0;
        ASSERT_EQ(API_SUCCESS, sockets_init(BOOL_FALSE, SERVER_PORT, NUM_SOCKETS, NULL));
        setup_stage = 1;

        log_info((" Client Ready to make connections at server port %d.\n", SERVER_PORT));

        ASSERT_TRUE(API_SUCCESS == socket_alloc(&hdl));
        setup_stage = 2;

        src_snk_len = 0xFFFFFFF - 0xFFFF;
        ASSERT_TRUE(NULL != (src = (UINT8 *)pltfm_malloc(src_snk_len)));
        setup_stage = 3;
        ASSERT_TRUE(NULL != (snk = (UINT8 *)pltfm_calloc(src_snk_len, 1)));
        setup_stage = 4;

        for (i = 0; i < src_snk_len; i++) 
        {
            src[i] = i + 1;
        }

        ASSERT_FALSE(0 == pltfm_memcmp(src, snk, src_snk_len));
    }

    virtual void TearDown()
    {
        if (setup_stage >= 4) pltfm_free(src);
        if (setup_stage >= 3) pltfm_free(snk);
        if (setup_stage >= 2)
        {
            ASSERT_TRUE(API_SUCCESS == socket_free(hdl));
            hdl = INVALID_SOCKET;
        }
        if (setup_stage >= 1) ASSERT_TRUE(API_SUCCESS == sockets_deinit());
    }

    protected:
        UINT8 *src;
        UINT8 *snk;
        UINT32 src_snk_len;
        SOCKET_HDL hdl;
        UINT8 setup_stage;
};

TEST_P(Test_socket_data_fidelity, random_write_read)
{
    UINT16 seed = (UINT16)GetParam() + 1;
    UINT16 rand_len;
    int s_num;
    int r_num;
    UINT32 src_consumed = 0;
    UINT32 r_num_total = 0;
    BOOL_T b_memcmp;
    UINT8 r_conf = 99;

    pltfm_srand(seed);
    pltfm_rand();
    pltfm_srand(pltfm_rand());

    log_info(("\n------------------- Start of test %u ---------------------\n", seed));

    /* 
     * Receive at client data from server.
     */
    do
    {
        do
        {
            rand_len = (UINT16)pltfm_rand();
        }
        while(0 == rand_len);
        if ((UINT32)rand_len > (src_snk_len - r_num_total))
        {
            rand_len = src_snk_len - r_num_total;
        }
        r_num = socket_recv(hdl, &snk[r_num_total], rand_len, 0);
        if (r_num > 0)
        {
            r_num_total += r_num;
        }
        else
        {
            log_socket_error();
            log_error(("Recv returned -> %d", r_num));
            log_error(("When read len = %u", rand_len));
            log_error(("When total received number of bytes = %u\n", r_num_total));
            log_buffer(&snk[r_num_total-32], 64);
            ASSERT_EQ(r_num_total, src_snk_len);
            break;
        }
    }
    while(r_num_total < src_snk_len);

    /* Send confirmation for received data. */
    log_info(("Sending confirmation for received data.\n"));
    s_num = socket_send(hdl, &r_conf, 1, 0);
    EXPECT_EQ(1, s_num);

    b_memcmp = (0 == pltfm_memcmp(src, snk, src_snk_len))? BOOL_TRUE: BOOL_FALSE;
    if (BOOL_FALSE == b_memcmp)
    {
        UINT16_F i;

        EXPECT_EQ(r_num_total, src_snk_len);

        for (i = 0; i < src_snk_len; i++)
        {
            if (src[i] != snk[i])
            {
                log_error(("Mismatch at %u'th byte - %u != %u\n", i, src[i], snk[i]));
                break;
            }
        }
    }
    ASSERT_TRUE(BOOL_TRUE == b_memcmp);

    /* 
     * Send from client to server.
     */
    do
    {
        do
        {
            rand_len = (UINT16)pltfm_rand();
        }
        while(0 == rand_len);
        if ((UINT32)rand_len > (src_snk_len - src_consumed))
        {
            rand_len = src_snk_len - src_consumed;
        }
        s_num = socket_send(hdl, &src[src_consumed], rand_len, 0);
        if (s_num > 0)
        {
            src_consumed += s_num;
        }
        else
        {
            log_socket_error();
            log_error(("Send returned -> %d\n", s_num));
            log_buffer(&src[src_consumed-32], 64);
            ASSERT_EQ(src_consumed, src_snk_len);
            break;
        }
    }
    while(src_consumed < src_snk_len);

    /* Wait for receive confirmation. */
    log_info(("Waiting for receive confirmation.\n"));
    r_num = socket_recv(hdl, &r_conf, 1, 0);
    EXPECT_EQ(1, r_num);
    EXPECT_EQ(99, r_conf);

    log_info(("\n******************* End of test %u, %u *********************\n", seed, src_snk_len));
}

INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_socket_data_fidelity,
                        ::testing::Range(2, 0xFF, 0x3F));

class Test_socket_file_fidelity: public ::Sockets_Comm_Test_Client,
                      public ::testing::WithParamInterface<const char *>
{
    virtual void SetUp()
    {
        ASSERT_EQ(API_SUCCESS, sockets_init(BOOL_FALSE, SERVER_PORT, NUM_SOCKETS, NULL));

        log_info(("Client Ready to make connections at server port %d.\n", SERVER_PORT));

        ASSERT_TRUE(API_SUCCESS == socket_alloc(&hdl));
        ASSERT_TRUE(API_SUCCESS == socket_alloc(&conf_hdl));
    }

    virtual void TearDown()
    {
        ASSERT_TRUE(API_SUCCESS == socket_free(conf_hdl));
        conf_hdl = INVALID_SOCKET;
        ASSERT_TRUE(API_SUCCESS == socket_free(hdl));
        hdl = INVALID_SOCKET;

        ASSERT_TRUE(API_SUCCESS == sockets_deinit());
    }

    protected:
        SOCKET_HDL hdl;
        SOCKET_HDL conf_hdl;
};

TEST_P(Test_socket_file_fidelity, random_write_read)
{
    FILE *fpw, *fpr;
    UINT8 src[0xFFFF] = {0};
    UINT8 snk[0xFFFF] = {0};
    const char *_file_name = GetParam();
    char file_name[100] = RCS_ROOT;
    char w_file_name[100] = RCS_ROOT "test_cl_";
    UINT16 seed = file_name[0] + file_name[1] + file_name[2];
    int s_num;
    UINT16 s_num_tot;
    int r_num;
    UINT16 read_len;
    UINT16 rand_len;
    UINT8 r_conf = 99;
    size_t write_len;
    size_t write_len_tot;

    pltfm_srand(seed);
    pltfm_rand();
    pltfm_srand(pltfm_rand());

    pltfm_strcat(file_name, _file_name);
    fpr = fopen(file_name, "rb");
    ASSERT_FALSE(NULL == fpr);
    pltfm_strcat(w_file_name, _file_name);
    fpw = fopen(w_file_name, "wb");
    ASSERT_FALSE(NULL == fpw);

    log_info(("\n------------------- Start of test %s ---------------------\n", file_name));

    /*
     * Receiving file in raw binary format.
     */
    do
    {
        do
        {
            rand_len = (UINT16)pltfm_rand();
        }
        while(0 == rand_len);
        r_num = socket_recv(hdl, snk, rand_len, 0);
        if (r_num > 0)
        {
            write_len_tot = 0;
            while(write_len_tot < (size_t)r_num)
            {
                write_len = fwrite(&snk[write_len_tot], 1, (r_num - write_len_tot), fpw);
                write_len_tot += write_len;
            }
        }
        else
        {
            if (r_num < 0)
            {
                log_error(("Recv returned -> %d\n", r_num));
                log_socket_error();
            }

            break;
        }
    }
    while(1);
    fclose(fpw);
    shutdown(hdl, SHUT_RD);
    log_info(("Received file %s.", w_file_name));

    /*
     * Sending file in raw binary format.
     */
    do
    {
        do
        {
            rand_len = (UINT16)pltfm_rand();
        }
        while(0 == rand_len);
        read_len = fread(src, 1, rand_len, fpr);

        s_num_tot = 0;
        while (s_num_tot < read_len)
        {
            s_num = socket_send(hdl, &src[s_num_tot], (read_len - s_num_tot), 0);
            if (s_num > 0)
            {
                s_num_tot += (UINT16)s_num;
            }
            else
            {
                log_socket_error();
                log_error(("Send returned -> %d\n", s_num));
                ASSERT_GT(s_num, 0);
                break;
            }
        }
    }
    while(read_len > 0);
    fclose(fpr);
    shutdown(hdl, SHUT_WR);
    log_info(("Sent file %s.", file_name));

    /* Wait for receive confirmation. */
    log_info(("Waiting for receive confirmation.\n"));
    r_conf = 0;
    r_num = socket_recv(conf_hdl, &r_conf, 1, 0);
    EXPECT_EQ(1, r_num);
    EXPECT_EQ(99, r_conf);
    
    EXPECT_TRUE(TEST_PASS == compare_files(file_name, w_file_name));
}

const char* test_files[] = {"Sinkarimelam_1.mp3",
                            "Core_V4.0.pdf",
                            "high_on_life.jpg"};
INSTANTIATE_TEST_CASE_P(InstantiationOne,
                        Test_socket_file_fidelity,
                        ::testing::ValuesIn(test_files));
