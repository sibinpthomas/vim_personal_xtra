/*
 * Abstractions for shell (empty) Thread handling primitives.
 */

#ifndef _H_NULL_OS_
#define _H_NULL_OS_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * Threads related.
 */
int null_thread_create(INT32* , const INT32* ,
                         void *(*sr) (void *), void * );
int null_thread_cancel(INT32 );
int null_thread_join(INT32 , void ** );
int null_thread_attr_init(INT32 t_at);
int null_thread_attr_destroy(INT32 t_at);
int null_thread_yield(void);
int null_thread_mutex_init(INT32* , const INT32* );
int null_thread_mutex_destroy(INT32* );
int null_thread_mutex_lock(INT32* );
int null_thread_mutex_unlock(INT32* );
int null_thread_cond_init(INT32* , const INT32* );
int null_thread_cond_destroy(INT32* );
int null_thread_cond_wait(INT32* , INT32* );
int null_thread_cond_signal(INT32* );

/*
 * Sleep related.
 */
void null_sleep (UINT32 );
void null_msleep(UINT32 );
void null_usleep(UINT32 );

/*
 * Sockets related.
 */
#define AF_INET                             1
#define SOCK_STREAM                         2
#define IPPROTO_TCP                         3
#define AI_NUMERICSERV                      4

struct sockaddr
{
    unsigned short    sa_family;
    char              sa_data[14];
};

struct in_addr
{
    unsigned long s_addr;
};

struct sockaddr_in
{
    short            sin_family;
    unsigned short   sin_port;
    struct in_addr   sin_addr;
    char             sin_zero[8];
};

typedef unsigned                            socklen_t;
struct addrinfo
{
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};

int getaddrinfo(const char *, const char *,
               const struct addrinfo *,
               struct addrinfo **);

void freeaddrinfo(struct addrinfo *);

int socket(int , int , int );
int accept(int , struct sockaddr *, socklen_t *);
int bind(int , const struct sockaddr *, socklen_t );
int listen(int , int );
int connect(int , const struct sockaddr *, socklen_t );
int shutdown(int , int );

/*
 * Sockets test related.
 */
#define INET6_ADDRSTRLEN                    (48)

extern int errno;

typedef unsigned short sa_family_t;
struct sockaddr_storage {
    sa_family_t  ss_family;
};

int getsockname(int , struct sockaddr *, socklen_t *);
UINT16 ntohs(UINT16 netshort);
const char *inet_ntop(int , const void *, char *, socklen_t );
int getpeername(int , struct sockaddr *, socklen_t *);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _H_NULL_OS_ */
