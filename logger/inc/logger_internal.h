
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifdef LOGGER

    extern FILE *log_f_hdl;

    int _log(DECL_CONST char *format, ...);
    int _hex_dump(DECL_CONST UINT8* buffer, UINT32 buf_len);

#endif /* LOGGER */

#ifdef __cplusplus
}
#endif /* __cplusplus */
