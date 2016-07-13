#include "platform.h"
#include "gtest/gtest.h"
#include "logger.h"


class LoggerInit : public ::testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            ASSERT_TRUE(API_SUCCESS == init_logger());
        }

        static void TearDownTestCase()
        {
            shutdown_logger();
        }
};


class Platform_HtoBuf_Test : public LoggerInit
{
    protected:
        UINT8 buffer[8];

        virtual void SetUp()
        {
            pltfm_memset(buffer, 0xFF, sizeof(buffer));
        }

        virtual void TearDown()
        {
            log_buffer(buffer, sizeof(buffer));
            log_simple(("\n"));
        }
};


class Platform_BuftoH_Test : public LoggerInit
{
    protected:
        UINT8 buffer[8];

        virtual void SetUp()
        {
            UINT16_F i;

            for (i = 0; i < sizeof(buffer); i++)
            {
                buffer[i] = 0x11 * (i + 1);
            }
            log_buffer(buffer, sizeof(buffer));
            log_simple(("\n"));
        }
};


class HtoLE_1s_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n**** Host to Little Endian buffer tests - 1 host byte. ****\n\n"));
        }
};


class HtoBE_1s_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n#### Host to Big Endian buffer tests - 1 host byte. ####\n\n"));
        }
};


class HtoLE_2s_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n**** Host to Little Endian buffer tests - 2 host bytes. ****\n\n"));
        }
};


class HtoBE_2s_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n#### Host to Big Endian buffer tests - 2 host bytes. ####\n\n"));
        }
};


class HtoLE_3s_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n**** Host to Little Endian buffer tests - 3 host bytes. ****\n\n"));
        }
};


class HtoBE_3s_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n#### Host to Big Endian buffer tests - 3 host bytes. ####\n\n"));
        }
};


class HtoLE_4s_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n**** Host to Little Endian buffer tests - 4 host bytes. ****\n\n"));
        }
};


class HtoBE_4s_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n#### Host to Big Endian buffer tests - 4 host bytes. ####\n\n"));
        }
};


class HtoLE_noN_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n**** Host to Little Endian buffer tests. ****\n\n"));
        }
};


class HtoBE_noN_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n#### Host to Big Endian buffer tests. ####\n\n"));
        }
};


class HtoLE_noN_ret_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n**** Host to Little Endian buffer tests, with return value. ****\n\n"));
        }
};


class HtoBE_noN_ret_Test : public Platform_HtoBuf_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_HtoBuf_Test::SetUpTestCase();
            log_simple(("\n#### Host to Big Endian buffer tests, with return value. ####\n\n"));
        }
};


class HtoLE_Neg_Test : public LoggerInit
{
    protected:
        static void SetUpTestCase()
        {
            LoggerInit::SetUpTestCase();
            log_simple(("\n**** Host to Little Endian buffer Negative (-ve) tests. ****\n\n"));
        }
};


class HtoBE_Neg_Test : public LoggerInit
{
    protected:
        static void SetUpTestCase()
        {
            LoggerInit::SetUpTestCase();
            log_simple(("\n**** Host to Big Endian buffer Negative (-ve) tests. ****\n\n"));
        }
};


class LEtoH_1s_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n**** Little Endian buffer to Host tests - 1 host byte. ****\n\n"));
        }
};


class BEtoH_1s_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n#### Big Endian buffer to Host tests - 1 host byte. ####\n\n"));
        }
};


class LEtoH_2s_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n**** Little Endian buffer to Host tests - 2 host byte. ****\n\n"));
        }
};


class BEtoH_2s_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n#### Big Endian buffer to Host tests - 2 host byte. ####\n\n"));
        }
};


class LEtoH_3s_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n**** Little Endian buffer to Host tests - 4 host byte. ****\n\n"));
        }
};


class BEtoH_3s_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n#### Big Endian buffer to Host tests - 4 host byte. ####\n\n"));
        }
};


class LEtoH_4s_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n**** Little Endian buffer to Host tests - 4 host byte. ****\n\n"));
        }
};


class BEtoH_4s_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n#### Big Endian buffer to Host tests - 4 host byte. ####\n\n"));
        }
};


class LEtoH_noN_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n**** Host to Little Endian buffer tests. ****\n\n"));
        }
};


class BEtoH_noN_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n#### Host to Big Endian buffer tests. ####\n\n"));
        }
};


class LEtoH_noN_ret_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n**** Host to Little Endian buffer tests, with return value. ****\n\n"));
        }
};


class BEtoH_noN_ret_Test : public Platform_BuftoH_Test
{
    protected:
        static void SetUpTestCase()
        {
            Platform_BuftoH_Test::SetUpTestCase();
            log_simple(("\n#### Host to Big Endian buffer tests, with return value. ####\n\n"));
        }
};


class LEtoH_Neg_Test : public LoggerInit
{
    protected:
        static void SetUpTestCase()
        {
            LoggerInit::SetUpTestCase();
            log_simple(("\n**** Host to Little Endian buffer Negative (-ve) tests. ****\n\n"));
        }
};


class BEtoH_Neg_Test : public LoggerInit
{
    protected:
        static void SetUpTestCase()
        {
            LoggerInit::SetUpTestCase();
            log_simple(("\n**** Host to Big Endian buffer Negative (-ve) tests. ****\n\n"));
        }
};


/* Host to Little Endian - 1 byte host variable. */
TEST_F(HtoLE_1s_Test, htole_1n_1s)
{
    UINT8 byte = 0xa5;

    _htole_bufn(byte, buffer, 1);
    EXPECT_EQ(0xa5, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
}

TEST_F(HtoLE_1s_Test, htole_2n_1s)
{
    UINT8 byte = 0xa5;

    _htole_bufn(byte, buffer, 2);
    EXPECT_EQ(0xa5, buffer[0]);
    EXPECT_EQ(0x00, buffer[1]);
    EXPECT_EQ(0xFF, buffer[2]);
}

TEST_F(HtoLE_1s_Test, htole_3n_1s)
{
    UINT8 byte = 0xa5;

    _htole_bufn(byte, buffer, 3);
    EXPECT_EQ(0xa5, buffer[0]);
    EXPECT_EQ(0x00, buffer[1]);
    EXPECT_EQ(0x00, buffer[2]);
    EXPECT_EQ(0xFF, buffer[3]);
}

TEST_F(HtoLE_1s_Test, htole_4n_1s)
{
    UINT8 byte = 0xa5;

    _htole_bufn(byte, buffer, 4);
    EXPECT_EQ(0xa5, buffer[0]);
    EXPECT_EQ(0x00, buffer[1]);
    EXPECT_EQ(0x00, buffer[2]);
    EXPECT_EQ(0x00, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
}

/* Host to Big Endian - 1 byte host variable. */
TEST_F(HtoBE_1s_Test, htobe_1n_1s)
{
    UINT8 byte = 0xa5;

    _htobe_bufn(byte, buffer, 1);
    EXPECT_EQ(0xa5, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
}

TEST_F(HtoBE_1s_Test, htobe_2n_1s)
{
    UINT8 byte = 0xa5;

    _htobe_bufn(byte, buffer, 2);
    EXPECT_EQ(0x00, buffer[0]);
    EXPECT_EQ(0xa5, buffer[1]);
    EXPECT_EQ(0xFF, buffer[2]);
}

TEST_F(HtoBE_1s_Test, htobe_3n_1s)
{
    UINT8 byte = 0xa5;

    _htobe_bufn(byte, buffer, 3);
    EXPECT_EQ(0x00, buffer[0]);
    EXPECT_EQ(0x00, buffer[1]);
    EXPECT_EQ(0xa5, buffer[2]);
    EXPECT_EQ(0xFF, buffer[3]);
}

TEST_F(HtoBE_1s_Test, htobe_4n_1s)
{
    UINT8 byte = 0xa5;

    _htobe_bufn(byte, buffer, 4);
    EXPECT_EQ(0x00, buffer[0]);
    EXPECT_EQ(0x00, buffer[1]);
    EXPECT_EQ(0x00, buffer[2]);
    EXPECT_EQ(0xa5, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
}

/* Host to Little Endian - 2 bytes host variable. */
TEST_F(HtoLE_2s_Test, htole_1n_2s)
{
    UINT16 hword = 0x12E9;

    _htole_bufn(hword, buffer, 1);
    EXPECT_EQ(0xE9, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
}

TEST_F(HtoLE_2s_Test, htole_2n_2s)
{
    UINT16 hword = 0x12E9;

    _htole_bufn(hword, buffer, 2);
    EXPECT_EQ(0xE9, buffer[0]);
    EXPECT_EQ(0x12, buffer[1]);
    EXPECT_EQ(0xFF, buffer[2]);
}

TEST_F(HtoLE_2s_Test, htole_3n_2s)
{
    UINT16 hword = 0x12E9;

    _htole_bufn(hword, buffer, 3);
    EXPECT_EQ(0xE9, buffer[0]);
    EXPECT_EQ(0x12, buffer[1]);
    EXPECT_EQ(0x00, buffer[2]);
    EXPECT_EQ(0xFF, buffer[3]);
}

TEST_F(HtoLE_2s_Test, htole_4n_2s)
{
    UINT16 hword = 0x12E9;

    _htole_bufn(hword, buffer, 4);
    EXPECT_EQ(0xE9, buffer[0]);
    EXPECT_EQ(0x12, buffer[1]);
    EXPECT_EQ(0x00, buffer[2]);
    EXPECT_EQ(0x00, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
}

/* Host to Big Endian - 2 bytes host variable. */
TEST_F(HtoBE_2s_Test, htobe_1n_2s)
{
    UINT16 hword = 0x12E9;

    H2BE_BUFN(hword, buffer, 1);
    EXPECT_EQ(0xE9, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
}

TEST_F(HtoBE_2s_Test, htobe_2n_2s)
{
    UINT16 hword = 0x12E9;

    H2BE_BUFN(hword, buffer, 2);
    EXPECT_EQ(0x12, buffer[0]);
    EXPECT_EQ(0xE9, buffer[1]);
    EXPECT_EQ(0xFF, buffer[2]);
}

TEST_F(HtoBE_2s_Test, htobe_3n_2s)
{
    UINT16 hword = 0x12E9;

    H2BE_BUFN(hword, buffer, 3);
    EXPECT_EQ(0x00, buffer[0]);
    EXPECT_EQ(0x12, buffer[1]);
    EXPECT_EQ(0xE9, buffer[2]);
    EXPECT_EQ(0xFF, buffer[3]);
}

TEST_F(HtoBE_2s_Test, htobe_4n_2s)
{
    UINT16 hword = 0x12E9;

    H2BE_BUFN(hword, buffer, 4);
    EXPECT_EQ(0x00, buffer[0]);
    EXPECT_EQ(0x00, buffer[1]);
    EXPECT_EQ(0x12, buffer[2]);
    EXPECT_EQ(0xE9, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
}

/* Host to Little Endian - 3 bytes host variable. */
TEST_F(HtoLE_3s_Test, htole_1n_3s)
{
    UINT32 word = 0x00A934C1;

    H2LE_BUFN(word, buffer, 1);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
}

TEST_F(HtoLE_3s_Test, htole_2n_3s)
{
    UINT32 word = 0x00A934C1;

    H2LE_BUFN(word, buffer, 2);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0x34, buffer[1]);
    EXPECT_EQ(0xFF, buffer[2]);
}

TEST_F(HtoLE_3s_Test, htole_3n_3s)
{
    UINT32 word = 0x00A934C1;

    H2LE_BUFN(word, buffer, 3);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0x34, buffer[1]);
    EXPECT_EQ(0xA9, buffer[2]);
    EXPECT_EQ(0xFF, buffer[3]);
}

TEST_F(HtoLE_3s_Test, htole_4n_3s)
{
    UINT32 word = 0x00A934C1;

    H2LE_BUFN(word, buffer, 4);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0x34, buffer[1]);
    EXPECT_EQ(0xA9, buffer[2]);
    EXPECT_EQ(0x00, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
}

/* Host to Big Endian - 3 bytes host variable. */
TEST_F(HtoBE_3s_Test, htobe_1n_3s)
{
    UINT32 word = 0x00A934C1;

    _htobe_bufn(word, buffer, 1);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
}

TEST_F(HtoBE_3s_Test, htobe_2n_3s)
{
    UINT32 word = 0x00A934C1;

    _htobe_bufn(word, buffer, 2);
    EXPECT_EQ(0x34, buffer[0]);
    EXPECT_EQ(0xC1, buffer[1]);
    EXPECT_EQ(0xFF, buffer[2]);
}

TEST_F(HtoBE_3s_Test, htobe_3n_3s)
{
    UINT32 word = 0x00A934C1;

    _htobe_bufn(word, buffer, 3);
    EXPECT_EQ(0xA9, buffer[0]);
    EXPECT_EQ(0x34, buffer[1]);
    EXPECT_EQ(0xC1, buffer[2]);
    EXPECT_EQ(0xFF, buffer[3]);
}

TEST_F(HtoBE_3s_Test, htobe_4n_3s)
{
    UINT32 word = 0x00A934C1;

    _htobe_bufn(word, buffer, 4);
    EXPECT_EQ(0x00, buffer[0]);
    EXPECT_EQ(0xA9, buffer[1]);
    EXPECT_EQ(0x34, buffer[2]);
    EXPECT_EQ(0xC1, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
}

/* Host to Little Endian - 4 bytes host variable. */
TEST_F(HtoLE_4s_Test, htole_1n_4s)
{
    UINT32 word = 0xF10934C1;

    H2LE_BUFN(word, buffer, 1);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
}

TEST_F(HtoLE_4s_Test, htole_2n_4s)
{
    UINT32 word = 0xF10934C1;

    H2LE_BUFN(word, buffer, 2);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0x34, buffer[1]);
    EXPECT_EQ(0xFF, buffer[2]);
}

TEST_F(HtoLE_4s_Test, htole_3n_4s)
{
    UINT32 word = 0xF10934C1;

    H2LE_BUFN(word, buffer, 3);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0x34, buffer[1]);
    EXPECT_EQ(0x09, buffer[2]);
    EXPECT_EQ(0xFF, buffer[3]);
}

TEST_F(HtoLE_4s_Test, htole_4n_4s)
{
    UINT32 word = 0xF10934C1;

    H2LE_BUFN(word, buffer, 4);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0x34, buffer[1]);
    EXPECT_EQ(0x09, buffer[2]);
    EXPECT_EQ(0xF1, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
}

/* Host to Big Endian - 4 bytes host variable. */
TEST_F(HtoBE_4s_Test, htobe_1n_4s)
{
    UINT32 word = 0xF10934C1;

    _htobe_bufn(word, buffer, 1);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
}

TEST_F(HtoBE_4s_Test, htobe_2n_4s)
{
    UINT32 word = 0xF10934C1;

    _htobe_bufn(word, buffer, 2);
    EXPECT_EQ(0x34, buffer[0]);
    EXPECT_EQ(0xC1, buffer[1]);
    EXPECT_EQ(0xFF, buffer[2]);
}

TEST_F(HtoBE_4s_Test, htobe_3n_4s)
{
    UINT32 word = 0xF10934C1;

    _htobe_bufn(word, buffer, 3);
    EXPECT_EQ(0x09, buffer[0]);
    EXPECT_EQ(0x34, buffer[1]);
    EXPECT_EQ(0xC1, buffer[2]);
    EXPECT_EQ(0xFF, buffer[3]);
}

TEST_F(HtoBE_4s_Test, htobe_4n_4s)
{
    UINT32 word = 0xF10934C1;

    _htobe_bufn(word, buffer, 4);
    EXPECT_EQ(0xF1, buffer[0]);
    EXPECT_EQ(0x09, buffer[1]);
    EXPECT_EQ(0x34, buffer[2]);
    EXPECT_EQ(0xC1, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
}

/* Host variable size detection and copy to buffer - Little Endian.  */
TEST_F(HtoLE_noN_Test, htole_1s)
{
    UINT8 byte = 0xa5;

    H2LE_BUF(byte, buffer);
    EXPECT_EQ(0xa5, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
}

TEST_F(HtoLE_noN_Test, htole_2s)
{
    UINT16 hword = 0x12E9;

    H2LE_BUF(hword, buffer);
    EXPECT_EQ(0xE9, buffer[0]);
    EXPECT_EQ(0x12, buffer[1]);
    EXPECT_EQ(0xFF, buffer[2]);
}

TEST_F(HtoLE_noN_Test, htole_4s)
{
    UINT32 word = 0xF10934C1;

    H2LE_BUF(word, buffer);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0x34, buffer[1]);
    EXPECT_EQ(0x09, buffer[2]);
    EXPECT_EQ(0xF1, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
}

/* Host variable size detection and copy to buffer - Big Endian.  */
TEST_F(HtoBE_noN_Test, htobe_1s)
{
    UINT8 byte = 0xa5;

    H2BE_BUF(byte, buffer);
    EXPECT_EQ(0xa5, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
}

TEST_F(HtoBE_noN_Test, htobe_2s)
{
    UINT16 hword = 0x12E9;

    H2BE_BUF(hword, buffer);
    EXPECT_EQ(0x12, buffer[0]);
    EXPECT_EQ(0xE9, buffer[1]);
    EXPECT_EQ(0xFF, buffer[2]);
}

TEST_F(HtoBE_noN_Test, htobe_4s)
{
    UINT32 word = 0xF10934C1;

    H2BE_BUF(word, buffer);
    EXPECT_EQ(0xF1, buffer[0]);
    EXPECT_EQ(0x09, buffer[1]);
    EXPECT_EQ(0x34, buffer[2]);
    EXPECT_EQ(0xC1, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
}

/* Testing Return value. */
TEST_F(HtoLE_noN_ret_Test, htole_ret_01)
{
    UINT8 byte = 0xa5;
    UINT16 hword = 0x12E9;
    UINT32 word = 0xF10934C1;
    UINT16_F num = 0;

    num = H2LE_BUF(byte, buffer);
    EXPECT_EQ(0xa5, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
    EXPECT_EQ(1, num);

    num += H2LE_BUF(hword, &buffer[1]);
    EXPECT_EQ(0xE9, buffer[1]);
    EXPECT_EQ(0x12, buffer[2]);
    EXPECT_EQ(0xFF, buffer[3]);
    EXPECT_EQ(3, num);

    num += H2LE_BUF(word, &buffer[3]);
    EXPECT_EQ(0xC1, buffer[3]);
    EXPECT_EQ(0x34, buffer[4]);
    EXPECT_EQ(0x09, buffer[5]);
    EXPECT_EQ(0xF1, buffer[6]);
    EXPECT_EQ(0xFF, buffer[7]);
    EXPECT_EQ(7, num);
}

TEST_F(HtoLE_noN_ret_Test, htole_ret_02)
{
    UINT8 byte = 0xa5;
    UINT16 hword = 0x12E9;
    UINT32 word = 0xF10934C1;
    UINT16_F num = 0;

    num = H2LE_BUF(word, buffer);
    EXPECT_EQ(0xC1, buffer[0]);
    EXPECT_EQ(0x34, buffer[1]);
    EXPECT_EQ(0x09, buffer[2]);
    EXPECT_EQ(0xF1, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
    EXPECT_EQ(4, num);

    num += H2LE_BUF(hword, &buffer[4]);
    EXPECT_EQ(0xE9, buffer[4]);
    EXPECT_EQ(0x12, buffer[5]);
    EXPECT_EQ(0xFF, buffer[6]);
    EXPECT_EQ(6, num);

    num += H2LE_BUF(byte, &buffer[6]);
    EXPECT_EQ(0xa5, buffer[6]);
    EXPECT_EQ(0xFF, buffer[7]);
    EXPECT_EQ(7, num);
}

TEST_F(HtoLE_Neg_Test, htole_neg_01)
{
    UINT8 byte = 0xa5;
    UINT16 hword = 0x12E9;
    UINT32 word = 0xF10934C1;
    UINT16_F num = 9;
    UINT8 *buf = NULL;

    num = H2LE_BUF(word, buf);
    EXPECT_EQ(0, num);
    num = H2LE_BUF(byte, buf);
    EXPECT_EQ(0, num);
    num = H2LE_BUF(hword, buf);
    EXPECT_EQ(0, num);
}

TEST_F(HtoLE_Neg_Test, htole_neg_literal)
{
    UINT16_F num = 9;
    UINT8 *buf = NULL;

    num = H2LE_BUF(5u, buf);
    EXPECT_EQ(0, num);
}

TEST_F(HtoBE_noN_ret_Test, htobe_ret_01)
{
    UINT8 byte = 0xa5;
    UINT16 hword = 0x12E9;
    UINT32 word = 0xF10934C1;
    UINT16_F num = 0;

    num = H2BE_BUF(byte, buffer);
    EXPECT_EQ(0xa5, buffer[0]);
    EXPECT_EQ(0xFF, buffer[1]);
    EXPECT_EQ(1, num);

    num += H2BE_BUF(hword, &buffer[1]);
    EXPECT_EQ(0x12, buffer[1]);
    EXPECT_EQ(0xE9, buffer[2]);
    EXPECT_EQ(0xFF, buffer[3]);
    EXPECT_EQ(3, num);

    num += H2BE_BUF(word, &buffer[3]);
    EXPECT_EQ(0xF1, buffer[3]);
    EXPECT_EQ(0x09, buffer[4]);
    EXPECT_EQ(0x34, buffer[5]);
    EXPECT_EQ(0xC1, buffer[6]);
    EXPECT_EQ(0xFF, buffer[7]);
    EXPECT_EQ(7, num);
}

TEST_F(HtoBE_noN_ret_Test, htobe_ret_02)
{
    UINT8 byte = 0xa5;
    UINT16 hword = 0x12E9;
    UINT32 word = 0xF10934C1;
    UINT16_F num = 0;

    num = H2BE_BUF(word, buffer);
    EXPECT_EQ(0xF1, buffer[0]);
    EXPECT_EQ(0x09, buffer[1]);
    EXPECT_EQ(0x34, buffer[2]);
    EXPECT_EQ(0xC1, buffer[3]);
    EXPECT_EQ(0xFF, buffer[4]);
    EXPECT_EQ(4, num);

    num += H2BE_BUF(hword, &buffer[4]);
    EXPECT_EQ(0x12, buffer[4]);
    EXPECT_EQ(0xE9, buffer[5]);
    EXPECT_EQ(0xFF, buffer[6]);
    EXPECT_EQ(6, num);

    num += H2BE_BUF(byte, &buffer[6]);
    EXPECT_EQ(0xa5, buffer[6]);
    EXPECT_EQ(0xFF, buffer[7]);
    EXPECT_EQ(7, num);
}

TEST_F(HtoBE_Neg_Test, htobe_neg_01)
{
    UINT8 byte = 0xa5;
    UINT16 hword = 0x12E9;
    UINT32 word = 0xF10934C1;
    UINT16_F num = 9;
    UINT8 *buf = NULL;

    num = H2BE_BUF(word, buf);
    EXPECT_EQ(0, num);
    num = H2BE_BUF(hword, buf);
    EXPECT_EQ(0, num);
    num = H2BE_BUF(byte, buf);
    EXPECT_EQ(0, num);
}

/* Little Endian to Host - 1 byte host variable. */
TEST_F(LEtoH_1s_Test, letoh_1n_1s)
{
    UINT8 byte = 0x00;

    _letoh_bufn(buffer, &byte, 1);
    EXPECT_EQ(0x11, byte);

    byte = 0x00;
    LE2H_BUFN(buffer, byte, 1);
    EXPECT_EQ(0x11, byte);
}

TEST_F(LEtoH_1s_Test, letoh_2n_1s)
{
    UINT8 byte = 0x00;

    _letoh_bufn(buffer, &byte, 2);
    EXPECT_EQ(0x11, byte);

    byte = 0x00;
    LE2H_BUFN(buffer, byte, 2);
    EXPECT_EQ(0x11, byte);

    EXPECT_EQ(0x2211, *(UINT16 *)&byte);
}

TEST_F(LEtoH_1s_Test, letoh_3n_1s)
{
    UINT8 byte = 0x00;

    _letoh_bufn(buffer, &byte, 3);
    EXPECT_EQ(0x11, byte);

    byte = 0x00;
    LE2H_BUFN(buffer, byte, 3);
    EXPECT_EQ(0x11, byte);

    log_simple(("\nVal = 0x%08x\n", *(UINT32 *)&byte));
    EXPECT_EQ(0x00332211, *(UINT32 *)&byte & ((1 << 24) - 1));
}

TEST_F(LEtoH_1s_Test, letoh_4n_1s)
{
    UINT8 byte = 0x00;

    _letoh_bufn(buffer, &byte, 4);
    EXPECT_EQ(0x11, byte);

    byte = 0x00;
    LE2H_BUFN(buffer, byte, 4);
    EXPECT_EQ(0x11, byte);

    EXPECT_EQ(0x44332211, *(UINT32 *)&byte);
}

/* Big Endian to Host - 1 byte host variable. */
TEST_F(BEtoH_1s_Test, betoh_1n_1s)
{
    UINT8 byte = 0x00;

    _betoh_bufn(buffer, &byte, 1);
    EXPECT_EQ(0x11, byte);

    byte = 0x00;
    BE2H_BUFN(buffer, byte, 1);
    EXPECT_EQ(0x11, byte);
}

TEST_F(BEtoH_1s_Test, betoh_2n_1s)
{
    UINT8 byte = 0x00;

    _betoh_bufn(buffer, &byte, 2);
    EXPECT_EQ(0x22, byte);

    byte = 0x00;
    BE2H_BUFN(buffer, byte, 2);
    EXPECT_EQ(0x22, byte);

    EXPECT_EQ(0x1122, *(UINT16 *)&byte);
}

TEST_F(BEtoH_1s_Test, betoh_3n_1s)
{
    UINT8 byte = 0x00;

    _betoh_bufn(buffer, &byte, 3);
    EXPECT_EQ(0x33, byte);

    byte = 0x00;
    BE2H_BUFN(buffer, byte, 3);
    EXPECT_EQ(0x33, byte);

    log_simple(("\nVal = 0x%08x\n", *(UINT32 *)&byte));
    EXPECT_EQ(0x00112233, *(UINT32 *)&byte & ((1 << 24) - 1));
}

TEST_F(BEtoH_1s_Test, betoh_4n_1s)
{
    UINT8 byte = 0x00;

    _betoh_bufn(buffer, &byte, 4);
    EXPECT_EQ(0x44, byte);

    byte = 0x00;
    BE2H_BUFN(buffer, byte, 4);
    EXPECT_EQ(0x44, byte);

    EXPECT_EQ(0x11223344, *(UINT32 *)&byte);
}

/* Little Endian to Host - 2 bytes host variable. */
TEST_F(LEtoH_2s_Test, letoh_1n_2s)
{
    UINT16 hword = 0x0000;

    _letoh_bufn(buffer, &hword, 1);
    EXPECT_EQ(0x11, hword);

    hword = 0x00;
    LE2H_BUFN(buffer, hword, 1);
    EXPECT_EQ(0x11, hword);
}

TEST_F(LEtoH_2s_Test, letoh_2n_2s)
{
    UINT16 hword = 0x0000;

    _letoh_bufn(buffer, &hword, 2);
    EXPECT_EQ(0x2211, hword);

    hword = 0x00;
    LE2H_BUFN(buffer, hword, 2);
    EXPECT_EQ(0x2211, hword);
}

TEST_F(LEtoH_2s_Test, letoh_3n_2s)
{
    UINT16 hword = 0x0000;

    _letoh_bufn(buffer, &hword, 3);
    EXPECT_EQ(0x2211, hword);

    hword = 0x00;
    LE2H_BUFN(buffer, hword, 3);
    EXPECT_EQ(0x2211, hword);

    log_simple(("\nVal = 0x%08x\n", *(UINT32 *)&hword));
    EXPECT_EQ(0x00332211, *(UINT32 *)&hword & ((1 << 24) - 1));
}

TEST_F(LEtoH_2s_Test, letoh_4n_2s)
{
    UINT16 hword = 0x0000;

    _letoh_bufn(buffer, &hword, 4);
    EXPECT_EQ(0x2211, hword);

    hword = 0x00;
    LE2H_BUFN(buffer, hword, 4);
    EXPECT_EQ(0x2211, hword);

    EXPECT_EQ(0x44332211, *(UINT32 *)&hword);
}

/* Big Endian to Host - 2 bytes host variable. */
TEST_F(BEtoH_2s_Test, betoh_1n_2s)
{
    UINT16 hword = 0x0000;

    _betoh_bufn(buffer, &hword, 1);
    EXPECT_EQ(0x11, hword);

    hword = 0x00;
    BE2H_BUFN(buffer, hword, 1);
    EXPECT_EQ(0x11, hword);
}

TEST_F(BEtoH_2s_Test, betoh_2n_2s)
{
    UINT16 hword = 0x0000;

    _betoh_bufn(buffer, &hword, 2);
    EXPECT_EQ(0x1122, hword);

    hword = 0x00;
    BE2H_BUFN(buffer, hword, 2);
    EXPECT_EQ(0x1122, hword);
}

TEST_F(BEtoH_2s_Test, betoh_3n_2s)
{
    UINT16 hword = 0x0000;

    _betoh_bufn(buffer, &hword, 3);
    EXPECT_EQ(0x2233, hword);

    hword = 0x00;
    BE2H_BUFN(buffer, hword, 3);
    EXPECT_EQ(0x2233, hword);

    log_simple(("\nVal = 0x%08x\n", *(UINT32 *)&hword));
    EXPECT_EQ(0x00112233, *(UINT32 *)&hword & ((1 << 24) - 1));
}

TEST_F(BEtoH_2s_Test, betoh_4n_2s)
{
    UINT16 hword = 0x0000;

    _betoh_bufn(buffer, &hword, 4);
    EXPECT_EQ(0x3344, hword);

    hword = 0x00;
    BE2H_BUFN(buffer, hword, 4);
    EXPECT_EQ(0x3344, hword);

    EXPECT_EQ(0x11223344, *(UINT32 *)&hword);
}

/* Little Endian to Host - 3 bytes host variable. */
TEST_F(LEtoH_3s_Test, letoh_1n_3s)
{
    UINT32 word = 0x00000000;

    _letoh_bufn(buffer, &word, 1);
    EXPECT_EQ(0x11, word);

    word = 0x00;
    LE2H_BUFN(buffer, word, 1);
    EXPECT_EQ(0x11, word);
}

TEST_F(LEtoH_3s_Test, letoh_2n_3s)
{
    UINT32 word = 0x00000000;

    _letoh_bufn(buffer, &word, 2);
    EXPECT_EQ(0x2211, word);

    word = 0x00;
    LE2H_BUFN(buffer, word, 2);
    EXPECT_EQ(0x2211, word);
}

TEST_F(LEtoH_3s_Test, letoh_3n_3s)
{
    UINT32 word = 0x00000000;

    _letoh_bufn(buffer, &word, 3);
    EXPECT_EQ(0x332211, word);

    word = 0x00;
    LE2H_BUFN(buffer, word, 3);
    EXPECT_EQ(0x332211, word);

    log_simple(("\nVal = 0x%08x\n", *(UINT32 *)&word));
    EXPECT_EQ(0x00332211, *(UINT32 *)&word & ((1 << 24) - 1));
}

TEST_F(LEtoH_3s_Test, letoh_4n_3s)
{
    UINT32 word = 0x00000000;

    _letoh_bufn(buffer, &word, 4);
    EXPECT_EQ(0x44332211, word);

    word = 0x00;
    LE2H_BUFN(buffer, word, 4);
    EXPECT_EQ(0x44332211, word);

    EXPECT_EQ(0x44332211, *(UINT32 *)&word);
}

/* Big Endian to Host - 3 bytes host variable. */
TEST_F(BEtoH_3s_Test, betoh_1n_3s)
{
    UINT32 word = 0x00000000;

    _betoh_bufn(buffer, &word, 1);
    EXPECT_EQ(0x11, word);

    word = 0x00;
    BE2H_BUFN(buffer, word, 1);
    EXPECT_EQ(0x11, word);
}

TEST_F(BEtoH_3s_Test, betoh_2n_3s)
{
    UINT32 word = 0x00000000;

    _betoh_bufn(buffer, &word, 2);
    EXPECT_EQ(0x1122, word);

    word = 0x00;
    BE2H_BUFN(buffer, word, 2);
    EXPECT_EQ(0x1122, word);
}

TEST_F(BEtoH_3s_Test, betoh_3n_3s)
{
    UINT32 word = 0x00000000;

    _betoh_bufn(buffer, &word, 3);
    EXPECT_EQ(0x112233, word);

    word = 0x00;
    BE2H_BUFN(buffer, word, 3);
    EXPECT_EQ(0x112233, word);

    log_simple(("\nVal = 0x%08x\n", *(UINT32 *)&word));
    EXPECT_EQ(0x00112233, *(UINT32 *)&word & ((1 << 24) - 1));
}

TEST_F(BEtoH_3s_Test, betoh_4n_3s)
{
    UINT32 word = 0x00000000;

    _betoh_bufn(buffer, &word, 4);
    EXPECT_EQ(0x11223344, word);

    word = 0x00;
    BE2H_BUFN(buffer, word, 4);
    EXPECT_EQ(0x11223344, word);

    EXPECT_EQ(0x11223344, *(UINT32 *)&word);
}

/* Little Endian to Host - 4 bytes host variable. */
TEST_F(LEtoH_4s_Test, letoh_1n_4s)
{
    UINT32 word = 0x00000000;

    _letoh_bufn(buffer, &word, 1);
    EXPECT_EQ(0x11, word);

    word = 0x00;
    LE2H_BUFN(buffer, word, 1);
    EXPECT_EQ(0x11, word);
}

TEST_F(LEtoH_4s_Test, letoh_2n_4s)
{
    UINT32 word = 0x00000000;

    _letoh_bufn(buffer, &word, 2);
    EXPECT_EQ(0x2211, word);

    word = 0x00;
    LE2H_BUFN(buffer, word, 2);
    EXPECT_EQ(0x2211, word);
}

TEST_F(LEtoH_4s_Test, letoh_3n_4s)
{
    UINT32 word = 0x00000000;

    _letoh_bufn(buffer, &word, 3);
    EXPECT_EQ(0x332211, word);

    word = 0x00;
    LE2H_BUFN(buffer, word, 3);
    EXPECT_EQ(0x332211, word);

    log_simple(("\nVal = 0x%08x\n", *(UINT32 *)&word));
    EXPECT_EQ(0x00332211, *(UINT32 *)&word & ((1 << 24) - 1));
}

TEST_F(LEtoH_4s_Test, letoh_4n_4s)
{
    UINT32 word = 0x00000000;

    _letoh_bufn(buffer, &word, 4);
    EXPECT_EQ(0x44332211, word);

    word = 0x00;
    LE2H_BUFN(buffer, word, 4);
    EXPECT_EQ(0x44332211, word);

    EXPECT_EQ(0x44332211, *(UINT32 *)&word);
}

/* Big Endian to Host - 4 bytes host variable. */
TEST_F(BEtoH_4s_Test, betoh_1n_4s)
{
    UINT32 word = 0x00000000;

    _betoh_bufn(buffer, &word, 1);
    EXPECT_EQ(0x11, word);

    word = 0x00;
    BE2H_BUFN(buffer, word, 1);
    EXPECT_EQ(0x11, word);
}

TEST_F(BEtoH_4s_Test, betoh_2n_4s)
{
    UINT32 word = 0x00000000;

    _betoh_bufn(buffer, &word, 2);
    EXPECT_EQ(0x1122, word);

    word = 0x00;
    BE2H_BUFN(buffer, word, 2);
    EXPECT_EQ(0x1122, word);
}

TEST_F(BEtoH_4s_Test, betoh_3n_4s)
{
    UINT32 word = 0x00000000;

    _betoh_bufn(buffer, &word, 3);
    EXPECT_EQ(0x112233, word);

    word = 0x00;
    BE2H_BUFN(buffer, word, 3);
    EXPECT_EQ(0x112233, word);

    log_simple(("\nVal = 0x%08x\n", *(UINT32 *)&word));
    EXPECT_EQ(0x00112233, *(UINT32 *)&word & ((1 << 24) - 1));
}

TEST_F(BEtoH_4s_Test, betoh_4n_4s)
{
    UINT32 word = 0x00000000;

    _betoh_bufn(buffer, &word, 4);
    EXPECT_EQ(0x11223344, word);

    word = 0x00;
    BE2H_BUFN(buffer, word, 4);
    EXPECT_EQ(0x11223344, word);

    EXPECT_EQ(0x11223344, *(UINT32 *)&word);
}

/* Host variable size detection and copy from buffer - Little Endian.  */
TEST_F(LEtoH_noN_Test, letoh_1s)
{
    UINT8 byte = 0;

    LE2H_BUF(buffer, byte);
    EXPECT_EQ(0x11, byte);
}

TEST_F(LEtoH_noN_Test, letoh_2s)
{
    UINT16 hword = 0;

    LE2H_BUF(buffer, hword);
    EXPECT_EQ(0x2211, hword);
}

TEST_F(LEtoH_noN_Test, letoh_4s)
{
    UINT32 word = 0;

    LE2H_BUF(buffer, word);
    EXPECT_EQ(0x44332211, word);
}

/* Host variable size detection and copy from buffer - Big Endian.  */
TEST_F(BEtoH_noN_Test, betoh_1s)
{
    UINT8 byte = 0;

    BE2H_BUF(buffer, byte);
    EXPECT_EQ(0x11, byte);
}

TEST_F(BEtoH_noN_Test, betoh_2s)
{
    UINT16 hword = 0;

    BE2H_BUF(buffer, hword);
    EXPECT_EQ(0x1122, hword);
}

TEST_F(BEtoH_noN_Test, betoh_4s)
{
    UINT32 word = 0;

    BE2H_BUF(buffer, word);
    EXPECT_EQ(0x11223344, word);
}

/* Testing Return value. */
TEST_F(LEtoH_noN_ret_Test, letoh_ret_01)
{
    UINT8 byte = 0;
    UINT16 hword = 0;
    UINT32 word = 0;
    UINT16_F num = 0;

    num = LE2H_BUF(buffer, byte);
    EXPECT_EQ(0x11, byte);
    EXPECT_EQ(1, num);

    num += LE2H_BUF(&buffer[1], hword);
    EXPECT_EQ(0x3322, hword);
    EXPECT_EQ(3, num);

    num += LE2H_BUF(&buffer[3], word);
    EXPECT_EQ(0x77665544, word);
    EXPECT_EQ(7, num);
}

TEST_F(LEtoH_noN_ret_Test, letoh_ret_02)
{
    UINT8 byte = 0;
    UINT16 hword = 0;
    UINT32 word = 0;
    UINT16_F num = 0;

    num = LE2H_BUF(buffer, word);
    EXPECT_EQ(0x44332211, word);
    EXPECT_EQ(4, num);

    num += LE2H_BUF(&buffer[4], hword);
    EXPECT_EQ(0x6655, hword);
    EXPECT_EQ(6, num);

    num += LE2H_BUF(&buffer[6], byte);
    EXPECT_EQ(0x77, byte);
    EXPECT_EQ(7, num);
}

TEST_F(LEtoH_Neg_Test, letoh_neg_buf)
{
    UINT8 byte = 0;
    UINT16 hword = 0;
    UINT32 word = 0;
    UINT16_F num = 0;
    UINT8 *buf = NULL;

    num = LE2H_BUF(buf, word);
    EXPECT_EQ(0, num);

    num += LE2H_BUF(buf, hword);
    EXPECT_EQ(0, num);

    num += LE2H_BUF(buf, byte);
    EXPECT_EQ(0, num);
}

TEST_F(LEtoH_Neg_Test, letoh_neg_hnum)
{
    UINT16_F num = 0;
    UINT8 buf[8];

    num = _letoh_bufn(buf, NULL, 4);
    EXPECT_EQ(0, num);

    num += _letoh_bufn(buf, NULL, 2);
    EXPECT_EQ(0, num);

    num += _letoh_bufn(buf, NULL, 1);
    EXPECT_EQ(0, num);
}

TEST_F(LEtoH_Neg_Test, letoh_neg_nbyte_eq0)
{
    UINT8 byte = 0;
    UINT16 hword = 0;
    UINT32 word = 0;
    UINT16_F num = 0;
    UINT8 buf[8];

    num = _letoh_bufn(buf, &word, 0);
    EXPECT_EQ(0, num);

    num += _letoh_bufn(buf, &hword, 0);
    EXPECT_EQ(0, num);

    num += _letoh_bufn(buf, &byte, 0);
    EXPECT_EQ(0, num);
}

TEST_F(LEtoH_Neg_Test, letoh_neg_nbyte_gt)
{
    UINT8 byte = 0;
    UINT16 hword = 0;
    UINT32 word = 0;
    UINT16_F num = 0;
    UINT8 buf[8];

    num = _letoh_bufn(buf, &word, (sizeof(UINTMAX) + 1));
    EXPECT_EQ(0, num);

    num += _letoh_bufn(buf, &hword, (sizeof(UINTMAX) + 1));
    EXPECT_EQ(0, num);

    num += _letoh_bufn(buf, &byte, (sizeof(UINTMAX) + 1));
    EXPECT_EQ(0, num);
}

TEST_F(BEtoH_noN_ret_Test, betoh_ret_01)
{
    UINT8 byte = 0;
    UINT16 hword = 0;
    UINT32 word = 0;
    UINT16_F num = 0;

    num = BE2H_BUF(buffer, byte);
    EXPECT_EQ(0x11, byte);
    EXPECT_EQ(1, num);

    num += BE2H_BUF(&buffer[1], hword);
    EXPECT_EQ(0x2233, hword);
    EXPECT_EQ(3, num);

    num += BE2H_BUF(&buffer[3], word);
    EXPECT_EQ(0x44556677, word);
    EXPECT_EQ(7, num);
}

TEST_F(BEtoH_noN_ret_Test, betoh_ret_02)
{
    UINT8 byte = 0;
    UINT16 hword = 0;
    UINT32 word = 0;
    UINT16_F num = 0;

    num = BE2H_BUF(buffer, word);
    EXPECT_EQ(0x11223344, word);
    EXPECT_EQ(4, num);

    num += BE2H_BUF(&buffer[4], hword);
    EXPECT_EQ(0x5566, hword);
    EXPECT_EQ(6, num);

    num += BE2H_BUF(&buffer[6], byte);
    EXPECT_EQ(0x77, byte);
    EXPECT_EQ(7, num);
}

TEST_F(BEtoH_Neg_Test, betoh_neg_buf)
{
    UINT8 byte = 0;
    UINT16 hword = 0;
    UINT32 word = 0;
    UINT16_F num = 0;
    UINT8 *buf = NULL;

    num = BE2H_BUF(buf, word);
    EXPECT_EQ(0, num);

    num += BE2H_BUF(buf, hword);
    EXPECT_EQ(0, num);

    num += BE2H_BUF(buf, byte);
    EXPECT_EQ(0, num);
}

TEST_F(BEtoH_Neg_Test, betoh_neg_hnum)
{
    UINT16_F num = 0;
    UINT8 buf[8];

    num = _betoh_bufn(buf, NULL, 4);
    EXPECT_EQ(0, num);

    num += _betoh_bufn(buf, NULL, 2);
    EXPECT_EQ(0, num);

    num += _betoh_bufn(buf, NULL, 1);
    EXPECT_EQ(0, num);
}

TEST_F(BEtoH_Neg_Test, betoh_neg_nbyte_eq0)
{
    UINT8 byte = 0;
    UINT16 hword = 0;
    UINT32 word = 0;
    UINT16_F num = 0;
    UINT8 buf[8];

    num = _betoh_bufn(buf, &word, 0);
    EXPECT_EQ(0, num);

    num += _betoh_bufn(buf, &hword, 0);
    EXPECT_EQ(0, num);

    num += _betoh_bufn(buf, &byte, 0);
    EXPECT_EQ(0, num);
}

TEST_F(BEtoH_Neg_Test, betoh_neg_nbyte_gt)
{
    UINT8 byte = 0;
    UINT16 hword = 0;
    UINT32 word = 0;
    UINT16_F num = 0;
    UINT8 buf[8];

    num = _betoh_bufn(buf, &word, (sizeof(UINTMAX) + 1));
    EXPECT_EQ(0, num);

    num += _betoh_bufn(buf, &hword, (sizeof(UINTMAX) + 1));
    EXPECT_EQ(0, num);

    num += _betoh_bufn(buf, &byte, (sizeof(UINTMAX) + 1));
    EXPECT_EQ(0, num);
}

class Strstrip_Test : public ::testing::Test
{
    protected:
        static void SetUpTestCase()
        {
            ASSERT_TRUE(API_SUCCESS == init_logger());
        }

        static void TearDownTestCase()
        {
            shutdown_logger();
        }
};

TEST_F(Strstrip_Test, Neg_01)
{
    CHAR *str = NULL;
    EXPECT_EQ(NULL, pltfm_strstrip(str));
}

TEST_F(Strstrip_Test, valid_01)
{
    {
        CHAR str[] = "";
        EXPECT_EQ(str, pltfm_strstrip(str));
    }
    {
        CHAR str[] = " ";
        EXPECT_EQ(&str[1], pltfm_strstrip(str));
    }
    {
        CHAR str[] = "\n";
        EXPECT_EQ(&str[1], pltfm_strstrip(str));
    }
    {
        CHAR str[] = "\t";
        EXPECT_EQ(&str[1], pltfm_strstrip(str));
    }
    {
        CHAR str[] = "       ";
        EXPECT_EQ(&str[7], pltfm_strstrip(str));
    }
    {
        CHAR str[] = "  \n\r  ";
        EXPECT_EQ(&str[6], pltfm_strstrip(str));
    }
    {
        CHAR str[] = "\t \t \t\n\r\t";
        EXPECT_EQ(&str[8], pltfm_strstrip(str));
    }
    {
        CHAR str[] = "a";
        EXPECT_EQ(str, pltfm_strstrip(str));
    }
    {
        CHAR str[] = " a";
        EXPECT_EQ(&str[1], pltfm_strstrip(str));
    }
    {
        CHAR str[] = " \n\t\r \ta";
        EXPECT_EQ(&str[6], pltfm_strstrip(str));
    }
    {
        CHAR str[] = "a \t\r";
        EXPECT_EQ(str, pltfm_strstrip(str));
        EXPECT_EQ(1, pltfm_strlen(str));
    }
    {
        CHAR str[] = " a\t\r\t\n   ";
        CHAR *s = pltfm_strstrip(str);
        EXPECT_EQ(&str[1], s);
        EXPECT_EQ(1, pltfm_strlen(s));
    }
    {
        CHAR str[] = " \n\t\r \ta  \t\r\n  ";
        CHAR *s = pltfm_strstrip(str);
        EXPECT_EQ(&str[6], s);
        EXPECT_EQ(1, pltfm_strlen(s));
    }
    {
        CHAR str[] = "a  \t\r\rfgh \t\r";
        EXPECT_EQ(str, pltfm_strstrip(str));
        EXPECT_EQ(9, pltfm_strlen(str));
    }
    {
        CHAR str[] = " aaasd\t\nasd\t\r\t\n   ";
        CHAR *s = pltfm_strstrip(str);
        EXPECT_EQ(&str[1], s);
        EXPECT_EQ(10, pltfm_strlen(s));
    }
    {
        CHAR str[] = " \n\t\r \ta \t\t\r\nfrag \t\r\n  ";
        CHAR *s = pltfm_strstrip(str);
        EXPECT_EQ(&str[6], s);
        EXPECT_EQ(10, pltfm_strlen(s));
    }
}
