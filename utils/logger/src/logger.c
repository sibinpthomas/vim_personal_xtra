#include "platform.h"
#include "logger.h"

#define NUM_BYTES_PER_LINE                  (8u)
FILE *log_f_hdl = NULL;

API_RESULT init_logger(void)
{
#ifdef LOG_TO_STDOUT
    log_f_hdl = stdout;
#else /* LOG_TO_STDOUT */
    log_f_hdl = fopen("log.txt", "w");
#endif /* LOG_TO_STDOUT */

    if (NULL == log_f_hdl)
    {
        return API_FAILURE;
    }

    return API_SUCCESS;
}

void shutdown_logger(void)
{
#ifndef LOG_TO_STDOUT
    if (log_f_hdl != NULL)
    {
        fclose(log_f_hdl);
        log_f_hdl = NULL;
    }
#endif /* LOG_TO_STDOUT */
}

int _log(DECL_CONST char *format, ...)
{
    va_list var_args;
    int num_chars;

    /* Return immediately if log file handle is invalid. */
    if (NULL == log_f_hdl) return 0;

    va_start(var_args, format);
    num_chars = vfprintf(log_f_hdl, format, var_args);
    va_end(var_args);
    fflush(log_f_hdl);

    return num_chars;
}

int _hex_dump(DECL_CONST UINT8* buffer, UINT32 buf_len)
{
    UINT16_F i, j;
    int num_chars;

    /* Return immediately if log file handle is invalid. */
    if (NULL == log_f_hdl) return 0;

    num_chars = fprintf(log_f_hdl, HEX_DUMP_PREFIX "--- Dumping %d bytes ---", (int)buf_len);
    for (i = 0; i < buf_len; i+=NUM_BYTES_PER_LINE)
    {   
        num_chars += fprintf(log_f_hdl, HEX_DUMP_PREFIX);
        for (j = 0; (j < NUM_BYTES_PER_LINE) && ((i + j) < buf_len); j++)
        { 
            num_chars += fprintf(log_f_hdl, "%02X ", buffer[i+j]);
        }
        /* Pad last line with spaces if there were less than NUM_BYTES_PER_LINE
         * bytes in the last line. */
        for ( ; j < NUM_BYTES_PER_LINE; j++)
        {
            num_chars += fprintf(log_f_hdl, "   ");
        }

        /* Displaying printable characters. */
        num_chars += fprintf(log_f_hdl, "  ");
        for (j = 0; (j < NUM_BYTES_PER_LINE) && ((i + j) < buf_len); j++)
        { 
            num_chars += fprintf(log_f_hdl, "%c", (((buffer[i+j] >= 0x20) && (buffer[i+j] <= 0x7E)) ? buffer[i+j] : '.'));
        }
    }
    num_chars += fprintf(log_f_hdl, "\n");
    fflush(log_f_hdl);

    return num_chars;
}
