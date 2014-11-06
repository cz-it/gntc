#ifndef GNT_NET_H_
#define GNT_NET_H_

#include "gnt_socket.h"

#define GNT_NET_WAIT_INFINITE		WAIT_INFINITE

#define GNT_NET_CHAR_ARG			'?'
#define GNT_NET_CHAR_EQ				'='
#define GNT_NET_CHAR_SEP			'&'
#define GNT_NET_CHAR_PORT			':'
#define GNT_NET_CHAR_ESC			'%'

#define GNT_NET_STR_PROTO			"://"
#define GNT_NET_STR_NUMBER			"1234567890."

#define GNT_NET_PROTO_TCP			"tcp"
#define GNT_NET_PROTO_UDP			"udp"
#define GNT_NET_PROTO_UNIX			"unix"
#define GNT_NET_PROTO_PIPE			"pipe"
#define GNT_NET_PROTO_FILE			"file"
#define GNT_NET_PROTO_SHM			"shm"
#define GNT_NET_PROTO_HTTP			"http"

#define GNT_NET_ID_TCP				1
#define GNT_NET_ID_UDP				2
#define GNT_NET_ID_UNIX				3
#define GNT_NET_ID_PIPE				4
#define GNT_NET_ID_FILE				5
#define GNT_NET_ID_SHM				6
#define GNT_NET_ID_THTTP			7

#define GNT_NET_ARG_REUSE			"reuse"
#define GNT_NET_ARG_TYPE			"type"
	
#define GNT_NET_TYPE_STREAM			"stream"
#define GNT_NET_TYPE_DGRAM			"dgram"
#define GNT_NET_TYPE_SEQPACKET		"seqpacket"
#define GNT_NET_TYPE_RDM			"rdm"
#define GNT_NET_TYPE_RAW			"raw"
#define GNT_NET_TYPE_PACKET			"packet"

#define GNT_NET_DEF_PROTO			GNT_NET_PROTO_TCP

#define HTONS(x)				htons((x))
#define HTONL(x)				htonl((x))
#define NTOHS(x)				ntohs((x))
#define NTOHL(x)				ntohl((x))

#define GNT_NET_PROTO_LEN			16
#define GNT_NET_ADDR_LEN			128
#define GNT_NET_NAME_LEN			32
#define GNT_NET_VALUE_LEN			256
#define GNT_NET_MAX_ARGS			16
#define GNT_NET_BUFF_LEN			65535

struct tagNetArg
{
	char szName[GNT_NET_NAME_LEN];
	char szValue[GNT_NET_VALUE_LEN];
};

typedef struct tagNetArg		GNT_NETARG;

struct tagNetOpt
{
	char szProto[GNT_NET_PROTO_LEN];
	char szAddr[GNT_NET_ADDR_LEN];
	int iArgs;
	GNT_NETARG	args[GNT_NET_MAX_ARGS];
};

typedef struct tagNetOpt		GNT_NETOPT;

struct tagNetProtoDef
{
	char* pszName;
	int iID;
	int fSelectable;
	int fAvail;
};

typedef struct tagNetProtoDef		GNT_NETPROTODEF;

struct tagNetTypeDef
{
	char* pszType;
	int iType;
};

typedef struct tagNetTypeDef		GNT_NETTYPEDEF;

union uniNetAddr
{
	struct sockaddr_in stIn;

#if !defined(_WIN32) && !defined(_WIN64)
	struct sockaddr_un stUn;
#endif
};

typedef union uniNetAddr			GNT_NETADDR;

#ifdef __cplusplus
extern "C"
{
#endif


/** *******************************************************************
*	@brief judge whether the string is all composed by digit.
*	@param[in] a_pszHost the source string which need judge.
*	@return whether the string is all composed by digit.
*	@retval 1 if the string is all composed by digit.
*	@retval 0 if not.
*********************************************************************/
int gnt_net_is_number(IN const char* a_pszHost);


/** *******************************************************************
*	@brief convert a address string to struct sockaddr_in.
*	@param[in] a_pszAddr the address string to convert.
*	@param[out] a_pstIn the buffer to hold the struct sockaddr_in which is converted to.
*	@return whether the convert is succeeded.
*	@retval 0 if success.
*	@retval -1 if failure.
*********************************************************************/
int gnt_net_str2inet(IN const char* a_pszAddr, OUT struct sockaddr_in* a_pstIn);

#if !defined(_WIN32) && !defined(_WIN64)
/** *******************************************************************
*	@brief convert a address string to struct sockaddr_un.
*	@param[in] a_pszAddr the address string to convert.
*	@param[out] a_pstUn the buffer to hold the struct sockaddr_un which is converted to.
*	@return whether the convert is succeeded.
*	@retval 0 if success.
*	@retval -1 if failure.
*********************************************************************/
int gnt_net_str2unix(IN const char* a_pszAddr, OUT struct sockaddr_un* a_pstUn);
#endif





/** *******************************************************************
*	@brief  find prototype definition from name.
*	@param[in] a_pszProto the name of the prototype.
*	@return the prototype definition.
*	@retval NULL if the named prototype not found.
*	@retval !NULL pointer to prototype definition.
*********************************************************************/
const GNT_NETPROTODEF* gnt_net_find_proto(IN const char* a_pszProto);



/** *******************************************************************
*	@brief  find socket type definition from name.
*	@param[in] a_pszType the name of the socket type.
*	@return the prototype definition.
*	@retval NULL if the named socket type not found.
*	@retval !NULL pointer to socket type definition.
*********************************************************************/
const GNT_NETTYPEDEF* gnt_net_find_type(IN const char* a_pszType);



/** *******************************************************************
*	@brief  find argument from the GNT_NETOPT structure.
*	@param[in] a_pstOpt the GNT_NETOPT struct buffer, which is the source used to find argument.
*	@param[in] a_pszName the name of the argument.
*	@return the argument definition found.
*	@retval NULL if the named argument not found.
*	@retval !NULL pointer to the argument definition found.
*********************************************************************/
const GNT_NETARG* gnt_net_find_arg(IN GNT_NETOPT* a_pstOpt, IN const char* a_pszName);



/** *******************************************************************
*	@brief  make option list from the a URI string.
*	@param[out]  a_pstOpt the buffer for contain the option list.
*	@param[in]  a_pszUri the URI string.
*	@return whether this operation is succeeded.
*	@retval 0 for success.
*	@retval -1 for failure.
*********************************************************************/
int gnt_net_get_opt(OUT GNT_NETOPT* a_pstOpt, IN const char* a_pszUri);		/* get the option from the uri string, this is a helper, needn't use normally. */

/* some flags can set. */

/** *******************************************************************
*	@brief  set the nonblock mode for the communication descriptor.
*	@param[in]  a_iSock the communication descriptor.
*	@param[in]  a_iIsNonblocking Input, the non-blocking flags: 1 for yes, 0 for no.
*	@return whether this operation is succeeded.
*	@retval 0 for success.
*	@retval -1 for failure.
*********************************************************************/
int gnt_net_set_nonblock(IN GSOCKET a_iSock, IN int a_iIsNonblocking);			/* set the socket to be nonblock or not. */

/** *******************************************************************
*	@brief  set the send buffer for the communication descriptor.
*	@param[in]  a_iSock the communication descriptor.
*	@param[in]  a_iSize	the send buffer size.
*	@return whether this operation is succeeded.
*	@retval 0 for success.
*	@retval -1 for failure.
*********************************************************************/
int gnt_net_set_recvbuff(GSOCKET a_iSock, int a_iSize);



/** *******************************************************************
*	@brief  set the recv buffer for the communication descriptor.
*	@param[in]  a_iSock the communication descriptor.
*	@param[in]  a_iSize	the recv buffer size.
*	@return whether this operation is succeeded.
*	@retval 0 for success.
*	@retval -1 for failure.
*********************************************************************/
int gnt_net_set_sendbuff(GSOCKET a_iSock, int a_iSize);


/* create/destroy the socket. */

/** *******************************************************************
*	@brief  open a communication descriptor by a URI string. the URI like that: tcp://127.0.0.1:8888?reuse=1
*	@param[in]  a_pszUri the URI string used to create the communication descriptor.
*	@return the communication descriptor.
*	@retval  -1 if failed.
*	@retval  the communication descriptor if success.
*********************************************************************/
GSOCKET gnt_net_open(IN const char* a_pszUri);				/* create a socket use uri info. */



/** *******************************************************************
*	@brief  create a communication descriptor by a URI string and listen on it. the URI like that: tcp://127.0.0.1:8888?reuse=1
*	@param[in]  a_pszUri the URI string used to create the communication descriptor.
*	@param[in]  a_iBacklog the internal pre-finish listening queue length.
*	@return the communication descriptor.
*	@retval  -1 if failed.
*	@retval  the communication descriptor if success.
*********************************************************************/
GSOCKET gnt_net_listen(IN const char* a_pszUri, IN int a_iBacklog);		/* create a socket for listen. */



/** *******************************************************************
*	@brief  open a communication descriptor by a URI string and do connect. the URI like that: tcp://127.0.0.1:8888?reuse=1
*	@param[in]  a_pszUri the URI string used to create the communication descriptor.
*	@param[in]  a_iTimeout the max milliseconds can be used to wait for the finsishing of connection.
*	@return the communication descriptor.
*	@retval  -1 if failed.
*	@retval  the communication descriptor if success.
*********************************************************************/
GSOCKET gnt_net_connect(IN const char* a_pszUri, IN int a_iTimeout);		/* create a socket, and connect to the uri specified. */



/** *******************************************************************
*	@brief  accept a pre-finished connection on a listening descriptor.
*	@param[in] a_iSock the listening descriptor which the connection is created on.
*	@param[out] a_pstAddr the buffer used to hold the connection's source address.
*	@param[in,out] a_piLen it hold the buffer Length as input, actually data length on return.
*	@param[in] a_iTimeout  the max milliseconds can be used to accept the pre-finished connection.
*	@return the accepted communication descriptor.
*	@retval  -1 if failed.
*	@retval  the accepted communication descriptor if success.
*********************************************************************/
GSOCKET gnt_net_accept(IN GSOCKET a_iSock, OUT struct sockaddr* a_pstAddr, INOUT int* a_piLen, IN int a_iTimeout); /* accept a income connection. */



/** *******************************************************************
*	@brief  close a communication descriptor.
*	@param[in] a_iSock the communication descriptor which need to be closed.
*	@return whether this operation is succeeded.
*	@retval 0 if success.
*	@retval -1 if failure.
*********************************************************************/
int gnt_net_close(IN GSOCKET a_iSock);						/* close a socket. */


/* send/recv socket operations. */

/** *******************************************************************
*	@brief  send data through the specified communication descriptor.
*	@param[in]  a_iSock the communcation descriptor.
*	@param[in]  a_pszBuff the buffer which used to hold data to send.
*	@param[in]  a_iLen the length of the data to send.
*	@param[in]  a_iTimeout the max milliseconds can be used to send the data.
*	@return whether this operation is succeeded.
*	@retval -1 if failure.
*	@retval >=0 the data sent, by bytes.
*********************************************************************/
int gnt_net_send(IN GSOCKET a_iSock, IN const char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);



/** *******************************************************************
*	@brief  receive data from the specified communication descriptor.
*	@param[in]  a_iSock the communcation descriptor.
*	@param[out]  a_pszBuff the buffer which used to hold data received.
*	@param[in]  a_iLen the length of the buffer.
*	@param[in]  a_iTimeout the max milliseconds can be used to receivethe data.
*	@return whether this operation is succeeded.
*	@retval -1 if failure.
*	@retval >=0 the data received, by bytes.
*********************************************************************/
int gnt_net_recv(IN GSOCKET a_iSock, OUT char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);


/* sendall/recvall socket operations. */

/** *******************************************************************
*	@brief  send data through the specified communication descriptor as more as possible in the specified timeout period.
*	@param[in]  a_iSock the communcation descriptor.
*	@param[in]  a_pszBuff the buffer which used to hold data to send.
*	@param[in]  a_iLen the length of the data to send.
*	@param[in]  a_iTimeout the max milliseconds can be used to send the data.
*	@return whether this operation is succeeded.
*	@retval -1 if failure.
*	@retval >=0 the data sent, by bytes.
*********************************************************************/
int gnt_net_sendall(IN GSOCKET a_iSock, IN const char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);



/** *******************************************************************
*	@brief  receive data from the specified communication descriptor as more as possible in the specified timeout period.
*	@param[in]  a_iSock the communcation descriptor.
*	@param[out]  a_pszBuff the buffer which used to hold data received.
*	@param[in]  a_iLen the length of the buffer.
*	@param[in]  a_iTimeout the max milliseconds can be used to receivethe data.
*	@return whether this operation is succeeded.
*	@retval -1 if failure.
*	@retval >=0 the data received, by bytes.
*********************************************************************/
int gnt_net_recvall(IN GSOCKET a_iSock, OUT char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);

/* read/write operations. */

#ifdef __cplusplus
}
#endif

#endif /* GNT_NET_H */

