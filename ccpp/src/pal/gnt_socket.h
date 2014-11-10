#ifndef GNT_SOCKET_H_
#define GNT_SOCKET_H_


#include "gnt_os.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#if defined(_WIN32) || defined(_WIN64)
	#include <ws2tcpip.h>
#else
	#include <sys/select.h>
	#include <sys/socket.h>
	#include <sys/un.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
    #include <arpa/inet.h>
#endif





#define GSOCKET_WAIT_INFINITE	-1

#define GSOCKET_TIMEOUT			0x01
#define GSOCKET_INPROGRESS		0x02


/* PA Method Start */

#if defined(_WIN32) || defined(_WIN64)

	#define socketerrno				WSAGetLastError()
	#define SOCKETEWOULDBLOCK		WSAEWOULDBLOCK
	#define SOCKETEAGAIN			WSAEWOULDBLOCK
	#define SOCKETINTR              WSAEINTR
    typedef SOCKET GSOCKET;

#else	/* #if defined(_WIN32) || defined(_WIN64) */

	#define socketerrno				errno
	#define SOCKETEWOULDBLOCK		EWOULDBLOCK
	#define SOCKETEAGAIN			EAGAIN
	#define SOCKETINTR              EINTR
	typedef int GSOCKET;

    #define INVALID_SOCKET  -1

#endif


#define MS_TO_TV(tv, ms)		do										\
								{										\
									(tv).tv_sec = (ms)/1000; 			\
									(tv).tv_usec = ((ms) % 1000)*1000;	\
								} while(0)

#define TV_TO_MS(ms, tv)		ms = (tv).tv_sec*1000 + (tv).tv_usec/1000

#ifdef __cplusplus

extern "C"
{
#endif

#if defined(_WIN32) || defined(_WIN64)

	typedef unsigned long in_addr_t;

	int inet_aton(const char* cp, struct in_addr* inp);
	struct in_addr inet_makeaddr(in_addr_t net, in_addr_t host);
	in_addr_t inet_netof(struct in_addr in);
	in_addr_t inet_lnaof(struct in_addr in);

#if _MSC_VER < 1500
	const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt);
#else
	#define inet_ntop InetNtop
#endif
#endif


#if defined(_WIN32) || defined(_WIN64)
const char *gnt_inet_ntop(int af, const void *src,
						  char *dst, socklen_t cnt);
#else
#define gnt_inet_ntop inet_ntop
#endif

// #define gnt_inet_ntop inet_ntop

/** *******************************************************************
*	@brief init the socket api.
*	@param[in] a_iMajor the major version of the socket api lib.
*	@param[in] a_iMinor the minor version of the socket api lib.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_init(IN int a_iMajor, IN int a_iMinor);


/** *******************************************************************
*	@brief finitialize the socket api.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_fini(void);

/** *******************************************************************
*	@brief open a socket descriptor.
*	@param[in] a_iAf address family, see socket().
*	@param[in] a_iType the socket type, see socket().
*	@param[in] a_iProto the protocol used by the socket, see socket().
*	@return the descriptor of the socket.
*	@retval -1 for failure.
*	@retval <>-1 the descriptor of the socket.
*********************************************************************/
GSOCKET gnt_socket_open(IN int a_iAf, IN int a_iType, IN int a_iProto);


/** *******************************************************************
*	@brief open a tcp socket descriptor.
*	@return the descriptor of the tcp socket.
*	@retval -1 for failure.
*	@retval <>-1 the descriptor of the tcp socket.
*********************************************************************/
GSOCKET gnt_socket_open_tcp(void);


/** *******************************************************************
*	@brief open a udp socket descriptor.
*	@return the descriptor of the udp socket.
*	@retval -1 for failure.
*	@retval <>-1 the descriptor of the udp socket.
*********************************************************************/
GSOCKET gnt_socket_open_udp(void);


/** *******************************************************************
*	@brief open a unix socket descriptor.
*	@param[in] a_iType the type of unix
*	@return the descriptor of the unix socket.
*	@retval -1 for failure.
*	@retval <>-1 the descriptor of the unix socket.
*********************************************************************/
GSOCKET gnt_socket_open_unix(IN int a_iType);



/** *******************************************************************
*	@brief set the socket attribute to reuse network address.
*	@param[in] a_iSock descriptor of the socket to set attribute.
*	@param[in] a_iIsReuseAddr set/unset reuse addr attribute, 0 for unset, 1 for set.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_reuse_addr(IN GSOCKET a_iSock, IN int a_iIsReuseAddr);



/** *******************************************************************
*	@brief set the socket attribute to nonblock style.
*	@param[in] a_iSock descriptor of the socket to set attribute.
*	@param[in] a_iIsNonblock set/unset the socket to blocking(0) or unblocking(1) mode.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_set_nonblock(IN GSOCKET a_iSock, IN int a_iIsNonblock);



/** *******************************************************************
*	@brief start connect to a remote address, maybe success immidiately or late.
*	@param[in] a_iSock descriptor of the socket.
*	@param[in] a_pstAddr the remote network address to connect.
*	@param[in] a_iLen the data size of a_pstAddr.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*	@retval GSOCKET_INPROGRESS this operation is in progress.
*********************************************************************/
int gnt_socket_start_connect(IN GSOCKET a_iSock, IN struct sockaddr* a_pstAddr, IN int a_iLen);


/** *******************************************************************
*	@brief check whether the earlier connect operation is finished.
*	@param[in] a_iSock descriptor of the socket.
*	@param[in] a_iTimeout the milliseconds to wait before the connect is finished.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*	@retval GSOCKET_TIMEOUT this operation is timeout, the connection is not finished.
*********************************************************************/
int gnt_socket_check_connect(IN GSOCKET a_iSock, IN int a_iTimeout);



/** *******************************************************************
*	@brief listen on specified socket.
*	@param[in] a_iSock descriptor of the socket.
*	@param[in] a_pstAddr the address which the socket listen on.
*	@param[in] a_iLen the data size of a_pstAddr.
*	@param[in] a_iBacklog the maximum finished but but accepted connection kept in the listening queue.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_listen(IN GSOCKET a_iSock, IN struct sockaddr* a_pstAddr, IN int a_iLen, IN int a_iBacklog);



/** *******************************************************************
*	@brief connect the specified socket to remote address.
*	@param[in] a_iSock descriptor of the socket.
*	@param[in] a_pstAddr the address which the socket connect to.
*	@param[in] a_iLen the data size of a_pstAddr.
*	@param[in] a_iTimeout the milliseconds to wait before the connect is finished.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_connect(IN GSOCKET a_iSock, IN struct sockaddr* a_pstAddr, IN int a_iLen, IN int a_iTimeout);



/** *******************************************************************
*	@brief accept a new socket from the listening socket.
*	@param[in] a_iSock descriptor of the socket for listening.
*	@param[out] a_pstAddr the buffer for hold the remote address of the new socket.
*	@param[in,out] a_piLen the buffer size of a_pstAddr as input, the data size for return.
*	@param[in] a_iTimeout the milliseconds to wait before the accept is finished.
*	@return the descriptor of the new accepted socket.
*	@retval -1 for failure.
*	@retval <>-1 the descriptor of the new accepted socket.
*********************************************************************/
GSOCKET gnt_socket_accept(IN GSOCKET a_iSock, OUT struct sockaddr* a_pstAddr, INOUT int* a_piLen, IN int a_iTimeout);




/** *******************************************************************
*	@brief close the specified socket.
*	@param[in] a_iSock descriptor of the socket to close.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_close(IN GSOCKET a_iSock);



/** *******************************************************************
*	@brief send data use the specified socket.
*	@param[in] a_iSock descriptor of the socket.
*	@param[in] a_pszBuff the data to be sent.
*	@param[in] a_iLen the data size.
*	@param[in] a_iTimeout the milliseconds to wait before the data is sent.
*	@return the bytes of data sent.
*	@retval -1 for failure.
*	@retval >=0  the bytes sent.
*********************************************************************/
int gnt_socket_send(IN GSOCKET a_iSock, IN const char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);




/** *******************************************************************
*	@brief receive data from the specified socket.
*	@param[in] a_iSock descriptor of the socket.
*	@param[out] a_pszBuff the buffer for hold data received.
*	@param[in,out] a_iLen the size of buffer as input, data size for return.
*	@param[in] a_iTimeout the milliseconds to wait before the data is received.
*	@return the bytes of data received.
*	@retval -1 for failure.
*	@retval >=0  the bytes received.
*********************************************************************/
int gnt_socket_recv(IN GSOCKET a_iSock, OUT char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);



/** *******************************************************************
*	@brief send data use the specified socket, familar with gnt_socket_send(), but try best to send all the data.
*	@param[in] a_iSock descriptor of the socket.
*	@param[in] a_pszBuff the data to be sent.
*	@param[in] a_iLen the data size.
*	@param[in] a_iTimeout the milliseconds to wait before the data is sent.
*	@return the bytes of data sent.
*	@retval -1 for failure.
*	@retval >=0  the bytes sent.
*********************************************************************/
int gnt_socket_sendall(IN GSOCKET a_iSock, IN const char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);



/** *******************************************************************
*	@brief receive data from the specified socket, familar with gnt_socket_recv(), but try best to received all data.
*	@param[in] a_iSock descriptor of the socket.
*	@param[out] a_pszBuff the buffer for hold data received.
*	@param[in,out] a_iLen the size of buffer as input, data size for return.
*	@param[in] a_iTimeout the milliseconds to wait before the data is received.
*	@return the bytes of data received.
*	@retval -1 for failure.
*	@retval >=0  the bytes received.
*********************************************************************/
int gnt_socket_recvall(IN GSOCKET a_iSock, OUT char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);




/** *******************************************************************
*	@brief  set the recv buffer size of the socket.
*	@param[in] a_iSock descriptor of the socket.
*	@param[in] a_iSize the new size of recived buffer.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_set_recvbuff(IN GSOCKET a_iSock, IN int a_iSize);



/** *******************************************************************
*	@brief  set the send buffer size of the socket.
*	@param[in] a_iSock descriptor of the socket.
*	@param[in] a_iSize the new size of send buffer.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_set_sendbuff(IN GSOCKET a_iSock, IN int a_iSize);


/** *******************************************************************
*	@brief get the socket type of the specified socket, SOCK_STREAM or SOCK_DGRAM etc.
*	@param[in] a_iSock descriptor of the socket.
*	@param[in] a_piType the pointer to buffer for hold the type.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_get_type(IN GSOCKET a_iSock, OUT int* a_piType);



/** *******************************************************************
*	@brief get information about whether the specified socket ia an acceptable socket.
*	@param[in] a_iSock descriptor of the socket.
*	@param[in] a_piAcceptConn  the pointer to buffer for hold the information whether this socket ia an acceptable socket.
*	@return whether the operation is succeed.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
int gnt_socket_get_acceptconn(IN GSOCKET a_iSock, OUT int* a_piAcceptConn);



#ifdef __cplusplus
}
#endif

/* PA Method End */


#endif /* GSOCKET_H */


