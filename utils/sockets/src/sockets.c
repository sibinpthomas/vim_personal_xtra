#include "platform.h"
#include "logger.h"
#include "sockets_internal.h"
#include "sockets.h"


#define NUM_DIGITS_66535                    5

/* This variable decides if the socket connections requested in this application
 * are of the server or client kind. */
DECL_STATIC BOOL_T g_is_server;

/* Server's listening socket. */
DECL_STATIC SOCKET_HDL g_listen_sock;

/* Structure to hold address of server's listening socket. */
DECL_STATIC struct sockaddr_in g_server_addr; 

/* This variable is set at init and sets the upper limit for connection
 * requests. */
DECL_STATIC UINT16 g_max_connections;


/* Public Functions. */
API_RESULT sockets_init(BOOL_T is_server, UINT16 server_port, 
                        UINT16 max_connections, DECL_CONST CHAR* server_ip_addr)
{
    struct addrinfo hints, *res = NULL;
    char server_port_str[NUM_DIGITS_66535+1] = "";
    
    if ((server_port < 1024) || (max_connections < 1))
    {
        /* Why 1024?
         * Read this -> https://tools.ietf.org/html/rfc6335#section-6 */
        SOCK_ERR(("Sockets init failed - Invalid Params."));
        return SOCKETS_INVALID_FUNC_PARAMS;
    }

    pltfm_socket_startup(SOCK_ERR, SOCKETS_INIT_FAILED);

    /* Set Socket connections config variables.  */
    g_max_connections = max_connections;
    g_is_server = is_server;

    /* Fetching server address details. */
    pltfm_memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_NUMERICSERV; 
    sprintf(server_port_str, "%hu", server_port);

    if (getaddrinfo(server_ip_addr, server_port_str, &hints, &res) != 0)
    {
        SOCK_ERR(("Sockets - getaddrinfo() failed."));
        return SOCKETS_INIT_FAILED;
    }

    if (res != NULL)
    {
        /* Setting server address details. */
        pltfm_memcpy(&g_server_addr, res->ai_addr, sizeof(g_server_addr));
    }
    else
    {
        SOCK_ERR(("getaddrinfo() returned empty results list."));
        return SOCKETS_INIT_FAILED;
    }

    /* Free the linked list. */
    freeaddrinfo(res);

    if (is_server)
    {
        g_listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (INVALID_SOCKET == g_listen_sock)
        {
            SOCK_ERR(("Socket creation failed."));
            return SOCKETS_ACCEPT_FAILED;
        }

        /* Bind socket to local port. */
        if(SOCKET_ERROR == bind(g_listen_sock,
                                (struct sockaddr *)&g_server_addr,
                                sizeof(g_server_addr)))
        {
            SOCK_ERR(("Socket binding failed."));
            return SOCKETS_BIND_FAILED;
        }

        /* Listen on the socket for new connections. */
        if(SOCKET_ERROR == listen(g_listen_sock, max_connections))
        {
            SOCK_ERR(("Listening on socket failed."));
            return SOCKETS_LISTEN_FAILED;
        }
    }

    return API_SUCCESS;
}

API_RESULT sockets_deinit(void)
{
    if (g_is_server)
    {
        /* Freeing the listening socket. */
        socket_free(g_listen_sock);
        g_listen_sock = INVALID_SOCKET;
    }

    pltfm_socket_cleanup(SOCK_ERR, SOCKETS_DEINIT_FAILED);

    return API_SUCCESS;
}

API_RESULT socket_alloc(SOCKET_HDL* hdl)
{
    if (NULL == hdl)
    {
        SOCK_ERR(("Socket allocation failed - Invalid Params."));
        return SOCKETS_INVALID_FUNC_PARAMS;
    }

    if (g_is_server)
    {
        *hdl = accept(g_listen_sock, NULL, NULL);
        if(INVALID_SOCKET == *hdl)
        {
            SOCK_ERR(("Accept() on socket failed."));
            return SOCKETS_ACCEPT_FAILED;
        }
    }
    else
    {
        *hdl = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (INVALID_SOCKET == *hdl)
        {
            SOCK_ERR(("Client Socket creation failed."));
            return SOCKETS_CREATE_FAILED;
        }

        if(SOCKET_ERROR == connect(*hdl, (struct sockaddr *) &g_server_addr, sizeof(g_server_addr)))
        {
            SOCK_ERR(("Connect() on socket failed."));
            return SOCKETS_CONNECT_FAILED;
        }
    }

    return API_SUCCESS;
}

API_RESULT socket_free(SOCKET_HDL hdl)
{
    shutdown(hdl, SHUT_RDWR);
    pltfm_closesocket(hdl);

    return API_SUCCESS;
}

