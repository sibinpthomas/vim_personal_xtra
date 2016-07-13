#include "platform.h"
#include "test_utils.h"

#define FILE_BLK_SIZE                       (0xFFFFu)

TEST_RESULTS compare_files(const char* f1, const char* f2)
{
    FILE *fp1, *fp2;
    UINT8 blk1[FILE_BLK_SIZE];
    UINT8 blk2[FILE_BLK_SIZE];
    UINT32 read_len1;
    UINT32 read_len2;

    fp1 = fopen(f1, "rb");
    fp2 = fopen(f2, "rb");

    do
    {
        read_len1 = fread(blk1, 1, FILE_BLK_SIZE, fp1);
        read_len2 = fread(blk2, 1, FILE_BLK_SIZE, fp2);

        if (read_len1 == read_len2)
        {
            if (0 != pltfm_memcmp(blk1, blk2, read_len1))
            {
                return TEST_FAIL;
            }
        }
        else
        {
            return TEST_FAIL;
        }
    }
    while((read_len1 > 0) && (read_len2 > 0));

    if (read_len1 == read_len2)
    {
        return TEST_PASS;
    }
    else
    {
        return TEST_FAIL;
    }
}

TEST_RESULTS get_user_confirmation(void)
{
    char result;

    while (1)
    {
        printf("\nIs this right?(y/n)?\n");
        scanf("%c", &result);
        if (('y' == result) || ('Y' == result))
        {
            return TEST_PASS;
        }
        else if (('n' == result) || ('N' == result))
        {
            return TEST_FAIL;
        }
    }
}

void test_delay(double delay_s)
{
    /* TODO - Should compensate for all the extra processing such as the
     *        floating point multiplications? */
    if (delay_s > 0)
    {
        /* Delay in seconds. */
        pltfm_sleep(delay_s);
        delay_s -= (double)(int)delay_s;
    }

    delay_s *= 1000;
    if (delay_s > 0)
    {
        /* Delay in milli-seconds. */
        pltfm_msleep(delay_s);
        delay_s -= (double)(int)delay_s;
    }

    delay_s *= 1000;
    if (delay_s > 0)
    {
        /* Delay in micro-seconds. */
        pltfm_usleep(delay_s);
    }
}
