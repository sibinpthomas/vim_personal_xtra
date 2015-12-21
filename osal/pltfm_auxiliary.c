#include "platform.h"

/* It is the responsibility of the caller to ensure that 'buf' points to a
 * buffer with a capacity of at least 'nbytes'.  */
UINT16 _htole_bufn(UINTMAX hnum, UINT8 *buf, UINT16 nbytes)
{
    UINT16_F i;

    /* 
     * Return immediately if -
     *      - pointer to buffer is invalid, or,
     *      - nbytes is zero
     */
    if ((NULL == buf) || (0 == nbytes)) return 0;

    for (i = 0; i < (UINT16_F)nbytes; i++)
    {
        *((UINT8 *)((buf) + i)) = (UINT8)((hnum) >> (8 * i));
    }

    return nbytes;
}

/* It is the responsibility of the caller to ensure that 'buf' points to a
 * buffer with a capacity of at least 'nbytes'.  */
UINT16 _htobe_bufn(UINTMAX hnum, UINT8 *buf, UINT16 nbytes)
{
    UINT16_F i;

    /* 
     * Return immediately if -
     *      - pointer to buffer is invalid, or,
     *      - nbytes is zero
     */
    if ((NULL == buf) || (0 == nbytes)) return 0;

    for (i = 0; i < (UINT16_F)nbytes; i++)
    {
        *((UINT8 *)((buf) + (nbytes - 1) - i)) = (UINT8)((hnum) >> (8 * i));
    }

    return nbytes;
}

API_RESULT assign_2h(void *hnum, UINTMAX tmp_h, UINT16 nbytes)
{
    switch (nbytes)
    {
        case 1:
            *(UINT8 *)hnum = tmp_h;
            return API_SUCCESS;
        case 2:
            *(UINT16 *)hnum = tmp_h;
            return API_SUCCESS;
        case 3:
        case 4:
            *(UINT32 *)hnum = tmp_h;
            return API_SUCCESS;
        default:
            if (sizeof(UINTMAX) == nbytes)
            {
                *(UINTMAX *)hnum = tmp_h;
                return API_SUCCESS;
            }
            return API_FAILURE;
    }
}

/* It is the responsibility of the caller to ensure that 'hnum' points to a
 * field of at least 'nbytes' width.  */
UINT16 _letoh_bufn(UINT8 *buf, void *hnum, UINT16 nbytes)
{
    UINT16_F i;
    UINTMAX tmp_h;

    /* 
     * Return immediately if -
     *      - pointer to buffer is invalid, or,
     *      - pointer to hnum is invalid, or,
     *      - nbytes is greater than the maximum size of an unsigned number on
     *              this platfrom, or,
     *      - nbytes is zero
     */
    if ((NULL == buf) || (NULL == hnum) ||
            (nbytes > (UINT16)sizeof(UINTMAX)) || (0 == nbytes))
    {
        return 0;
    }

    tmp_h = 0;
    for (i = 0; i < (UINT16_F)nbytes; i++)
    {
        tmp_h |= buf[i] << (8 * i);
    }

    return (API_SUCCESS == assign_2h(hnum, tmp_h, nbytes)) ? nbytes : 0;
}

/* It is the responsibility of the caller to ensure that 'hnum' points to a
 * field of at least 'nbytes' width.  */
UINT16 _betoh_bufn(UINT8 *buf, void *hnum, UINT16 nbytes)
{
    UINT16_F i;
    UINTMAX tmp_h;

    /* 
     * Return immediately if -
     *      - pointer to buffer is invalid, or,
     *      - pointer to hnum is invalid, or,
     *      - nbytes is greater than the maximum size of an unsigned number on
     *              this platfrom, or,
     *      - nbytes is zero
     */
    if ((NULL == buf) || (NULL == hnum) ||
            (nbytes > (UINT16)sizeof(UINTMAX)) || (0 == nbytes))
    {
        return 0;
    }

    tmp_h = 0;
    for (i = 0; i < (UINT16_F)nbytes; i++)
    {
        tmp_h = (tmp_h << 8) | buf[i];
    }

    return (API_SUCCESS == assign_2h(hnum, tmp_h, nbytes)) ? nbytes : 0;
}
