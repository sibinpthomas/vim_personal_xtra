#include "platform.h"
#include "logger.h"
#include "sockets_internal.h"
#include "socket_test_utils.h"

#ifdef SOCKETS_LOG_ENABLE
void log_socket_details(SOCKET_HDL hdl, BOOL_T is_server)
{
    socklen_t len;
    struct sockaddr_storage addr;
    char ipstr[INET6_ADDRSTRLEN];
    int port;

    len = sizeof(addr);
    if (SOCKET_ERROR != getsockname(hdl, (struct sockaddr *) &addr, &len))
    {
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);                

        if (is_server)
        {
            SOCK_INF(("Server Socket IP address: %s\n", ipstr));
            SOCK_INF(("Server Socket port      : %d\n", port));
        }
        else
        {
            SOCK_INF(("Client Socket IP address: %s\n", ipstr));
            SOCK_INF(("Client Socket port      : %d\n", port));
        }
    }

    len = sizeof(addr);
    if (SOCKET_ERROR != getpeername(hdl, (struct sockaddr *) &addr, &len))
    {
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);                

        SOCK_INF(("Peer IP address: %s\n", ipstr));
        SOCK_INF(("Peer port      : %d\n", port));
    }
}

#ifdef WINDOWS

struct socket_error_code_desc
{
    INT16_F code;
    DECL_CONST CHAR* desc;
};

DECL_STATIC DECL_CONST struct socket_error_code_desc code_desc[] =
{     /*{ code, "desc" }*/
    { 10004, "WSAEINTR: Interrupted function call. \nA blocking operation was"
             "interrupted by a call to WSACancelBlockingCall." },
    { 10013, "WSAEACCES: Permission denied. \nAn attempt was made to access a "
             "socket in a way forbidden by its access permissions. An example "
             "is using a broadcast address for sendto without broadcast "
             "permission being set using setsockopt(SO_BROADCAST). \n"
             "Another possible reason for the WSAEACCES error is that when the "
             "bind function is called (on Windows NT 4 SP4 or later), another "
             "application, service, or kernel mode driver is bound to the same "
             "address with exclusive access." },
    { 10014, "WSAEFAULT: Bad address. \nThe system detected an invalid pointer "
             "address in attempting to use a pointer argument of a call. This "
             "error occurs if an application passes an invalid pointer value, "
             "or if the length of the buffer is too small. For instance, if "
             "the length of an argument, which is a sockaddr structure, is "
             "smaller than the sizeof(sockaddr)." },
    { 10022, "WSAEINVAL: Invalid argument. \nSome invalid argument was "
             "supplied (for example, specifying an invalid level to the "
             "setsockopt function). In some instances, it also refers to the "
             "current state of the socket-for instance, calling accept on a "
             "socket that is not listening." },
    { 10024, "WSAEMFILE: Too many open files. \nToo many open sockets. "
             "Each implementation may have a maximum number of socket handles "
             "available, either globally, per process, or per thread." },
    { 10035, "WSAEWOULDBLOCK: Resource temporarily unavailable. \n"
             "This error is returned from operations on nonblocking sockets "
             "that cannot be completed immediately, for example recv when no "
             "data is queued to be read from the socket. It is a nonfatal "
             "error, and the operation should be retried later. It is normal "
             "for WSAEWOULDBLOCK to be reported as the result from calling "
             "connect on a nonblocking SOCK_STREAM socket, since some time "
             "must elapse for the connection to be established." },
    { 10036, "WSAEINPROGRESS: Operation now in progress. \nA blocking "
             "operation is currently executing. Windows Sockets only allows a "
             "single blocking operation-per- task or thread-to be outstanding, "
             "and if any other function call is made (whether or not it "
             "references that or any other socket) the function fails with the "
             "WSAEINPROGRESS error." },
    { 10037, "WSAEALREADY: Operation already in progress. \nAn operation was "
             "attempted on a nonblocking socket with an operation already in "
             "progress-that is, calling connect a second time on a nonblocking"
             " socket that is already connecting, or canceling an asynchronous "
             "request (WSAAsyncGetXbyY) that has already been canceled or "
             "completed." },
    { 10038, "WSAENOTSOCK: Socket operation on nonsocket. \nAn operation was "
             "attempted on something that is not a socket. Either the socket "
             "handle parameter did not reference a valid socket, or for "
             "select, a member of an fd_set was not valid." },
    { 10039, "WSAEDESTADDRREQ: Destination address required. \nA required "
             "address was omitted from an operation on a socket. For "
             "example, this error is returned if sendto is called with the "
             "remote address of ADDR_ANY." },
    { 10040, "WSAEMSGSIZE: Message too long. \nA message sent on a datagram "
             "socket was larger than the internal message buffer or some "
             "other network limit, or the buffer used to receive a datagram "
             "was smaller than the datagram itself." },
    { 10041, "WSAEPROTOTYPE: Protocol wrong type for socket. \nA protocol "
             "was specified in the socket function call that does not "
             "support the semantics of the socket type requested. For "
             "example, the ARPA Internet UDP protocol cannot be specified "
             "with a socket type of SOCK_STREAM." },
    { 10042, "WSAENOPROTOOPT: Bad protocol option. \nAn unknown, invalid or "
             "unsupported option or level was specified in a getsockopt or "
             "setsockopt call." },
    { 10043, "WSAEPROTONOSUPPORT: Protocol not supported. \nThe requested "
             "protocol has not been configured into the system, or no "
             "implementation for it exists. For example, a socket call "
             "requests a SOCK_DGRAM socket, but specifies a stream "
             "protocol." },
    { 10044, "WSAESOCKTNOSUPPORT: Socket type not supported. \nThe support "
             "for the specified socket type does not exist in this address "
             "family. For example, the optional type SOCK_RAW might be "
             "selected in a socket call, and the implementation does not "
             "support SOCK_RAW sockets at all." },
    { 10045, "WSAEOPNOTSUPP: Operation not supported. \nThe attempted "
             "operation is not supported for the type of object referenced. "
             "Usually this occurs when a socket descriptor to a socket that "
             "cannot support this operation is trying to accept a connection "
             "on a datagram socket." },
    { 10046, "WSAEPFNOSUPPORT: Protocol family not supported. \nThe protocol "
             "family has not been configured into the system or no "
             "implementation for it exists. This message has a slightly "
             "different meaning from WSAEAFNOSUPPORT. However, it is "
             "interchangeable in most cases, and all Windows Sockets "
             "functions that return one of these messages also specify "
             "WSAEAFNOSUPPORT." },
    { 10047, "WSAEAFNOSUPPORT: Address family not supported by protocol "
             "family. \nAn address incompatible with the requested protocol "
             "was used. All sockets are created with an associated address "
             "family (that is, AF_INET for Internet Protocols) and a generic "
             "protocol type (that is, SOCK_STREAM). This error is returned "
             "if an incorrect protocol is explicitly requested in the socket "
             "call, or if an address of the wrong family is used for a "
             "socket, for example, in sendto." },
    { 10048, "WSAEADDRINUSE: Address already in use. \nTypically, only one "
             "usage of each socket address (protocol/IP address/port) is "
             "permitted. This error occurs if an application attempts to "
             "bind a socket to an IP address/port that has already been used "
             "for an existing socket, or a socket that was not closed "
             "properly, or one that is still in the process of closing. For "
             "server applications that need to bind multiple sockets to the "
             "same port number, consider using setsockopt (SO_REUSEADDR)." },
    { 10049, "WSAEADDRNOTAVAIL: Cannot assign requested address. \nThe "
             "requested address is not valid in its context. This normally "
             "results from an attempt to bind to an address that is not "
             "valid for the local computer. This can also result from "
             "connect, sendto, WSAConnect, WSAJoinLeaf, or WSASendTo when "
             "the remote address or port is not valid for a remote computer "
             "(for example, address or port 0)." },
    { 10050, "WSAENETDOWN: Network is down. \nA socket operation encountered "
             "a dead network. This could indicate a serious failure of the "
             "network system (that is, the protocol stack that the Windows "
             "Sockets DLL runs over), the network interface, or the local "
             "network itself." },
    { 10051, "WSAENETUNREACH: Network is unreachable. \nA socket operation "
             "was attempted to an unreachable network. This usually means "
             "the local software knows no route to reach the remote host." },
    { 10052, "WSAENETRESET: Network dropped connection on reset. \nThe "
             "connection has been broken due to keep-alive activity "
             "detecting a failure while the operation was in progress. It "
             "can also be returned by setsockopt if an attempt is made to "
             "set SO_KEEPALIVE on a connection that has already failed." },
    { 10053, "WSAECONNABORTED: Software caused connection abort. \nAn "
             "established connection was aborted by the software in your "
             "host computer, possibly due to a data transmission time-out or "
             "protocol error." },
    { 10054, "WSAECONNRESET: Connection reset by peer. \nAn existing "
             "connection was forcibly closed by the remote host. This "
             "normally results if the peer application on the remote host is "
             "suddenly stopped, the host is rebooted, the host or remote "
             "network interface is disabled, or the remote host uses a hard "
             "close. This error may also result if a connection was broken due "
             "to keep-alive activity detecting a failure. "
             "Operations that were in progress fail with WSAENETRESET. "
             "Subsequent operations fail with WSAECONNRESET." },
    { 10055, "WSAENOBUFS: No buffer space available. \nAn operation on a "
             "socket could not be performed because the system lacked "
             "sufficient buffer space or because a queue was full." },
    { 10056, "WSAEISCONN: Socket is already connected. \nA connect request "
             "was made on an already-connected socket. Some implementations "
             "also return this error if sendto is called on a connected "
             "SOCK_DGRAM socket (for SOCK_STREAM sockets, the to parameter "
             "in sendto is ignored) although other implementations treat "
             "this as a legal occurrence." },
    { 10057, "WSAENOTCONN: Socket is not connected. \nA request to send or "
             "receive data was disallowed because the socket is not "
             "connected and (when sending on a datagram socket using sendto) "
             "no address was supplied. Any other type of operation might "
             "also return this error-for example, setsockopt setting "
             "SO_KEEPALIVE if the connection has been reset." },
    { 10058, "WSAESHUTDOWN: Cannot send after socket shutdown. \nA request "
             "to send or receive data was disallowed because the socket had "
             "already been shut down in that direction with a previous "
             "shutdown call. By calling shutdown a partial close of a socket "
             "is requested, which is a signal that sending or receiving, or "
             "both have been discontinued." },
    { 10060, "WSAETIMEDOUT: Connection timed out. \nA connection attempt "
             "failed because the connected party did not properly respond "
             "after a period of time, or the established connection failed "
             "because the connected host has failed to respond." },
    { 10061, "WSAECONNREFUSED: Connection refused. \nNo connection could be "
             "made because the target computer actively refused it. This "
             "usually results from trying to connect to a service that is "
             "inactive on the foreign host-that is, one with no server "
             "application running." },
    { 10064, "WSAEHOSTDOWN: Host is down. \nA socket operation failed "
             "because the destination host is down. A socket operation "
             "encountered a dead host. Networking activity on the local host "
             "has not been initiated. These conditions are more likely to be "
             "indicated by the error WSAETIMEDOUT." },
    { 10065, "WSAEHOSTUNREACH: No route to host. \nA socket operation was "
             "attempted to an unreachable host. See WSAENETUNREACH."},
    { 10067, "WSAEPROCLIM: Too many processes. \nA Windows Sockets "
             "implementation may have a limit on the number of applications "
             "that can use it simultaneously.WSAStartup may fail with this "
             "error if the limit has been reached." },
    { 10091, "WSASYSNOTREADY: Network subsystem is unavailable. \nThis error "
             "is returned by WSAStartup if the Windows Sockets "
             "cannot function because the underlying system it uses is "
             "currently unavailable. Users should check: \nThat the "
             "appropriate Windows Sockets DLL file is in the current path. "
             "\nIf there is more than one Winsock DLL on your system, "
             "be sure the first one in the path is appropriate for the "
             "network subsystem currently loaded." },
    { 10092, "WSAVERNOTSUPPORTED: Winsock.dll version out of range. \nThe "
             "current Windows Sockets implementation does not support the "
             "Windows Sockets specification version requested by the "
             "application. Check that no old Windows Sockets DLL files are "
             "being accessed." },
    { 10093, "WSANOTINITIALISED: Successful WSAStartup not yet performed. "
             "\nEither the application has not called WSAStartup or "
             "WSAStartup failed. The application may be accessing a socket "
             "that the current active task does not own (that is, trying to "
             "share a socket between tasks), or WSACleanup has been called "
             "too many times." },
    { 10101, "WSAEDISCON: Graceful shutdown in progress. \nReturned by "
             "WSARecv and WSARecvFrom to indicate that the remote party has "
             "initiated a graceful shutdown sequence." },
    { 10109, "WSATYPE_NOT_FOUND: Class type not found. \nThe specified class "
             "was not found."},
    { 11001, "WSAHOST_NOT_FOUND: Host not found. \nNo such host is known. "
             "The name is not an official host name or alias, or it cannot "
             "be found in the database(s) being queried. This error may also "
             "be returned for protocol and service queries, and means that "
             "the specified name could not be found in the relevant "
             "database." },
    { 11002, "WSATRY_AGAIN: Nonauthoritative host not found. \nThis is "
             "usually a temporary error during host name resolution and "
             "means that the local server did not receive a response from an "
             "authoritative server. A retry at some time later may be "
             "successful." },
    { 11003, "WSANO_RECOVERY: This is a nonrecoverable error. \nThis "
             "indicates that some sort of nonrecoverable error occurred "
             "during a database lookup. This may be because the database "
             "files (for example, BSD-compatible HOSTS, SERVICES, or "
             "PROTOCOLS files) could not be found, or a DNS request was "
             "returned by the server with a severe error." },
    { 11004, "WSANO_DATA: Valid name, no data record of requested type. "
             "\nThe requested name is valid and was found in the database, "
             "but it does not have the correct associated data being "
             "resolved for. The usual example for this is a host "
             "name-to-address translation attempt (using gethostbyname or "
             "WSAAsyncGetHostByName) which uses the DNS (Domain Name "
             "Server). An MX record is returned but no A record-indicating "
             "the host itself exists, but is not directly reachable." }
};

    #ifndef get_last_error
        #define get_last_error()                    WSAGetLastError()
    #endif /* get_last_error */
#define get_socket_error_desc(err_code)     win_socket_err_desc((err_code))

DECL_STATIC DECL_CONST CHAR *win_socket_err_desc(int err_code)
{
    UINT16_F i;

    for (i = 0; i < LEN_OF_ARRAY(code_desc); i++)
    {
        if (code_desc[i].code == err_code)
        {
            return code_desc[i].desc;
        }
    }

    return "Error code not in database!!!";
}
#else /* WINDOWS */
    #ifndef get_last_error
        #define get_last_error()                    errno
    #endif /* get_last_error */
    #define get_socket_error_desc(err_code)     strerror((err_code))
#endif /* WINDOWS*/

void log_socket_error(void)
{
    int err_code = get_last_error();

    SOCK_ERR(("Error code = %d", err_code));
    SOCK_ERR((get_socket_error_desc(err_code)));
}

#endif /* SOCKETS_LOG_ENABLE */
