#include <gnt/gnt_socket.h>
#include <gnt/gnt_net.h>

#include <assert.h>

#define DECLARE
GSOCKET gnt_net_open_by_proto(GNT_NETOPT* a_pstOpt, const GNT_NETPROTODEF* a_pstProto);
int gnt_net_get_arg(GNT_NETOPT* a_pstOpt, const char* a_pszArg);

#if defined (_WIN32) || defined (_WIN64)

#pragma warning(disable:4996)

#endif

static GNT_NETPROTODEF gs_astProtos[]	=
{
	{GNT_NET_PROTO_TCP, 	GNT_NET_ID_TCP, 	1,	1},
	{GNT_NET_PROTO_UDP, 	GNT_NET_ID_UDP,		1,	1},
	{GNT_NET_PROTO_UNIX, 	GNT_NET_ID_UNIX, 	1,	1},
	{GNT_NET_PROTO_PIPE, 	GNT_NET_ID_PIPE, 	1,	1},
	{GNT_NET_PROTO_FILE, 	GNT_NET_ID_FILE, 	1,	1},
	{GNT_NET_PROTO_SHM, 	GNT_NET_ID_SHM, 	0,	1},
	{GNT_NET_PROTO_HTTP, 	GNT_NET_ID_THTTP, 	1,	1},
};

static GNT_NETTYPEDEF gs_astTypes[] =
{
	{GNT_NET_TYPE_STREAM, 		SOCK_STREAM},
	{GNT_NET_TYPE_DGRAM, 		SOCK_DGRAM},
	{GNT_NET_TYPE_SEQPACKET, 	SOCK_SEQPACKET},
	{GNT_NET_TYPE_RAW,			SOCK_RAW},
	{GNT_NET_TYPE_RDM, 			SOCK_RDM},
#if !defined (_WIN32) && !defined (_WIN64)
	{GNT_NET_TYPE_PACKET, 	SOCK_PACKET},
#endif
};

///////////////////////////////////

/* helper used internal. */
/** *******************************************************************
*	@brief  not implemented now.
*	@param
*	@return
*	@retval
*********************************************************************/
int gnt_net_decode(INOUT char* a_pszDst, INOUT int* a_piDst, IN const char* a_pszSrc, IN int a_iSrc);



/** *******************************************************************
*	@brief  not implemented now.
*	@param
*	@return
*	@retval
*********************************************************************/
int gnt_net_encode(INOUT char* a_pszDst, INOUT int* a_piDst, IN const char* a_pszSrc, IN int a_iSrc);


////////////////////////////////////////////

int gnt_net_is_number(const char* a_pszHost)
{
	size_t iSize;

	iSize	=	strspn(a_pszHost, GNT_NET_STR_NUMBER);

	if( iSize==strlen(a_pszHost) )
		return 1;
	else
		return 0;
}

int gnt_net_str2inet(const char* a_pszAddr, struct sockaddr_in* a_pstIn)
{
	char szHost[GNT_NET_ADDR_LEN];
#if !defined (_WIN32) && !defined (_WIN64)
	char szBuff[GNT_NET_BUFF_LEN];
	struct hostent stHost;
	int iErr;
#endif
	struct hostent* pstRet=NULL;
	char* pszPort;
	unsigned short wPort;

	assert( a_pszAddr && a_pstIn);

    memset(a_pstIn, 0, sizeof(*a_pstIn));

	a_pstIn->sin_family	=	AF_INET;

	pszPort	=	strchr( a_pszAddr, GNT_NET_CHAR_PORT );

	if( pszPort )
	{
		if( pszPort - a_pszAddr >= GNT_NET_ADDR_LEN )
			return -1;

		memcpy(szHost, a_pszAddr, pszPort - a_pszAddr);
		szHost[pszPort - a_pszAddr]	=	'\0';

		pszPort++;
	}
	else
	{
		if( strlen(a_pszAddr) >= GNT_NET_ADDR_LEN )
			return -1;

		strcpy(szHost, a_pszAddr);

		pszPort	=	NULL;
	}

	if( pszPort )
		wPort	=	(unsigned short)atoi(pszPort);
	else
		wPort	=	0;

	a_pstIn->sin_port	=	HTONS(wPort);

	if( gnt_net_is_number(szHost) ) /* it is a numbering address. */
	{
		if (inet_aton(szHost,&a_pstIn->sin_addr) == 0)
		{
			return -1;
		}
	}
	else
	{
#if defined (_WIN32) || defined (_WIN64)
		pstRet	=	gethostbyname(szHost);
#else
		gethostbyname_r(szHost, &stHost, szBuff, GNT_NET_BUFF_LEN, &pstRet, &iErr);
#endif
		if( NULL==pstRet )
			return -1;

		a_pstIn->sin_addr=	*(struct in_addr*) pstRet->h_addr_list[0];
	}

	return 0;
}

#if !defined (_WIN32) && !defined (_WIN64)
int gnt_net_str2unix(const char* a_pszAddr, struct sockaddr_un* a_pstUn)
{
	int iLen;

	assert( a_pszAddr && a_pstUn);

	a_pstUn->sun_family	=	AF_UNIX;

	iLen	=	strlen(a_pszAddr);

	if( iLen >= UNIX_PATH_MAX )
		return -1;

	memcpy(a_pstUn->sun_path, a_pszAddr, iLen);
	a_pstUn->sun_path[iLen]	=	'\0';

	return 0;
}
#endif

int gnt_net_decode(char* a_pszDst, int* a_piDst, const char* a_pszSrc, int a_iSrc)
{
	assert( a_pszDst && a_piDst && a_pszSrc && a_iSrc );

	return 0;
}

int gnt_net_encode(char* a_pszDst, int* a_piDst, const char* a_pszSrc, int a_iSrc)
{
	assert( a_pszDst && a_piDst && a_pszSrc && a_iSrc );

	return 0;
}

const GNT_NETPROTODEF* gnt_net_find_proto(const char* a_pszProto)
{
	int i;

	assert( a_pszProto);

	for(i=0; i<(int)(sizeof(gs_astProtos)/sizeof(GNT_NETPROTODEF)); i++)
	{
		if( 0==strcmp(gs_astProtos[i].pszName, a_pszProto ) )
			return gs_astProtos + i;
	}

	return NULL;
}

const GNT_NETTYPEDEF* gnt_net_find_type(const char* a_pszType)
{
	int i;

	assert( a_pszType );

	for(i=0; i<(int)(sizeof(gs_astTypes)/sizeof(GNT_NETTYPEDEF)); i++)
	{
		if( 0==strcmp(gs_astTypes[i].pszType, a_pszType ) )
			return gs_astTypes + i;
	}

	return NULL;
}

const GNT_NETARG* gnt_net_find_arg(GNT_NETOPT* a_pstOpt, const char* a_pszName)
{
	int i;

	assert( a_pstOpt && a_pszName );

	for(i=0; i<a_pstOpt->iArgs; i++)
	{
		if( 0==strcmp(a_pstOpt->args[i].szName, a_pszName) )
			return a_pstOpt->args + i;
	}

	return NULL;
}

int gnt_net_get_arg(GNT_NETOPT* a_pstOpt, const char* a_pszArg)
{
	size_t iLen;
	size_t iName;
	size_t iValue;
	char* pszNext;
	char* pszValue;

	assert( a_pstOpt );

	if( !a_pszArg )
		return 0;

	a_pstOpt->iArgs	=	0;

	do
	{
		pszNext	=	strchr(a_pszArg, GNT_NET_CHAR_SEP);

		if( pszNext )
		{
			iLen	=	pszNext - a_pszArg;
			pszNext++;
		}
		else
		{
			iLen	=	strlen(a_pszArg);
			pszNext	=	NULL;
		}

		if( iLen<=0 )
			break;

		pszValue=	memchr(a_pszArg, GNT_NET_CHAR_EQ, iLen);

		if( pszValue )
		{
			iName	=	pszValue - a_pszArg;
			iValue	=	iLen - iName - 1;

			pszValue++;
		}
		else
		{
			iName	=	iLen;
			iValue	=	0;

			pszValue	=	NULL;
		}

		if( iName>=GNT_NET_NAME_LEN || iValue>=GNT_NET_VALUE_LEN )
			return -1;

		if( iName > 0 )
		{
			memcpy( a_pstOpt->args[a_pstOpt->iArgs].szName, a_pszArg, iName);
			a_pstOpt->args[a_pstOpt->iArgs].szName[iName]	=	0;
		}
		else
			a_pstOpt->args[a_pstOpt->iArgs].szName[0]	=	0;

		if( iValue > 0 )
		{
			memcpy( a_pstOpt->args[a_pstOpt->iArgs].szValue, pszValue, iValue);
			a_pstOpt->args[a_pstOpt->iArgs].szValue[iValue]	=	0;
		}
		else
			a_pstOpt->args[a_pstOpt->iArgs].szValue[0]	=	0;

		a_pszArg	=	pszNext;

		a_pstOpt->iArgs++;
	}
	while(a_pszArg);

	return 0;
}

int gnt_net_get_opt(GNT_NETOPT* a_pstOpt, const char* a_pszUri)
{
	const char* pszProto;
	const char* pszArg;
	size_t iLen;

	assert( a_pstOpt && a_pszUri );

	pszProto	=	strstr(a_pszUri, GNT_NET_STR_PROTO);

	if( pszProto )
	{
		if( pszProto - a_pszUri >= GNT_NET_PROTO_LEN || pszProto == a_pszUri )
			return -1;

		memcpy(a_pstOpt->szProto, a_pszUri, pszProto - a_pszUri);

		a_pstOpt->szProto[pszProto - a_pszUri]	=	'\0';

		a_pszUri	=	pszProto + strlen(GNT_NET_STR_PROTO);
	}
	else
	{
		strcpy(a_pstOpt->szProto, GNT_NET_DEF_PROTO);
	}

	pszArg		=	strchr(a_pszUri, GNT_NET_CHAR_ARG);

	if( pszArg )
		iLen	=	pszArg - a_pszUri;
	else
		iLen	=	strlen(a_pszUri);

	if( iLen>=GNT_NET_ADDR_LEN )
		return -1;

	a_pstOpt->iArgs	=	0;

	if( iLen>0 )
	{
		memcpy(a_pstOpt->szAddr, a_pszUri, iLen);
		a_pstOpt->szAddr[iLen]	=	0;
	}
	else
	{
		a_pstOpt->szAddr[0]	=	0;
	}

	if( pszArg )
	{
		pszArg++;

		return gnt_net_get_arg(a_pstOpt, pszArg);
	}

	return 0;
}

int gnt_net_set_nonblock(GSOCKET a_iSock, int a_iIsNonblock)
{
	return gnt_socket_set_nonblock(a_iSock, a_iIsNonblock);
}

int gnt_net_set_sendbuff(GSOCKET a_iSock, int a_iSize)
{
	return gnt_socket_set_sendbuff(a_iSock, a_iSize);
}

int gnt_net_set_recvbuff(GSOCKET a_iSock, int a_iSize)
{
	return gnt_socket_set_recvbuff(a_iSock, a_iSize);
}

int gnt_net_close(GSOCKET a_iSock)
{
	return gnt_socket_close(a_iSock);
}

GSOCKET gnt_net_open_by_proto(GNT_NETOPT* a_pstOpt, const GNT_NETPROTODEF* a_pstProto)
{
	int iType;
	const GNT_NETARG* pstArg;
	const GNT_NETTYPEDEF* pstType;

	assert( a_pstOpt && a_pstProto );

	switch( a_pstProto->iID )
	{
	case GNT_NET_ID_UDP:
		return gnt_socket_open_udp();

	case GNT_NET_ID_THTTP:
	case GNT_NET_ID_TCP:
		return gnt_socket_open_tcp();

	case GNT_NET_ID_UNIX:
		iType	=	SOCK_STREAM;

		pstArg	=	gnt_net_find_arg(a_pstOpt, GNT_NET_ARG_TYPE);

		if( pstArg )
		{
			pstType	=	gnt_net_find_type(pstArg->szValue);

			if( pstType )
				iType	=	pstType->iType;
		}

		return gnt_socket_open_unix(SOCK_STREAM);

	case GNT_NET_ID_PIPE:
	case GNT_NET_ID_SHM:
	case GNT_NET_ID_FILE:
		return (GSOCKET)-1;
	default:
		break;
	}

	return (GSOCKET)-1;
}

GSOCKET gnt_net_open(const char* a_pszUri)
{
	GNT_NETOPT stOpt;
	const GNT_NETPROTODEF* pstProto;

	if( -1==gnt_net_get_opt(&stOpt, a_pszUri) )
		return (GSOCKET)-1;

	pstProto	=	gnt_net_find_proto(stOpt.szProto);

	if( NULL==pstProto )
		return (GSOCKET)-1;

	return gnt_net_open_by_proto(&stOpt, pstProto);
}

GSOCKET gnt_net_listen(const char* a_pszUri, int a_iBacklog)
{
	GNT_NETOPT stOpt;
	GNT_NETADDR stAddr;
	const GNT_NETARG* pstArg;
	const GNT_NETPROTODEF* pstProto;
	GSOCKET s;
	int fReuseAddr;

	if( -1==gnt_net_get_opt(&stOpt, a_pszUri) )
		return (GSOCKET)-1;

	pstProto	=	gnt_net_find_proto(stOpt.szProto);

	if( NULL==pstProto )
		return (GSOCKET)-1;

	s	=	gnt_net_open_by_proto(&stOpt, pstProto);

	if( -1==s )
		return (GSOCKET)-1;

	pstArg	=	gnt_net_find_arg(&stOpt, GNT_NET_ARG_REUSE);

	if( pstArg )
		fReuseAddr	=	atoi(pstArg->szValue);
	else
		fReuseAddr	=	0;

	if( fReuseAddr )
		gnt_socket_reuse_addr(s, fReuseAddr);

	switch(pstProto->iID)
	{
	case GNT_NET_ID_THTTP:
	case GNT_NET_ID_TCP:
		if( gnt_net_str2inet(stOpt.szAddr, &stAddr.stIn) < 0 )
			break;

		if( gnt_socket_listen(s, (struct sockaddr*)&stAddr.stIn, sizeof(stAddr.stIn), a_iBacklog)<0 )
			break;
		else
			return s;

		break;

	case GNT_NET_ID_UDP:
		if( gnt_net_str2inet(stOpt.szAddr, &stAddr.stIn) < 0 )
			break;

		if( bind(s, (struct sockaddr*)&stAddr.stIn, sizeof(stAddr.stIn))<0 )
			break;
		else
			return s;

		break;

#if !defined (_WIN32) && !defined (_WIN64)
	case GNT_NET_ID_UNIX:
		if( gnt_net_str2unix(stOpt.szAddr, &stAddr.stUn) < 0 )
			break;

		unlink(stOpt.szAddr);

		if( gnt_socket_listen(s, (struct sockaddr*)&stAddr.stUn, sizeof(stAddr.stUn), a_iBacklog)<0 )
			break;
		else
			return s;

		break;

#endif

	default:
		break;
	}

	gnt_net_close(s);

	return (GSOCKET)-1;
}

GSOCKET gnt_net_connect(const char* a_pszUri, int a_iTimeout)
{
	GNT_NETOPT stOpt;
	GNT_NETADDR stAddr;
	const GNT_NETPROTODEF* pstProto;
	GSOCKET s;

	if( -1==gnt_net_get_opt(&stOpt, a_pszUri) )
		return (GSOCKET)-1;


	pstProto	=	gnt_net_find_proto(stOpt.szProto);

	if( NULL==pstProto )
		return (GSOCKET)-1;

	s	=	gnt_net_open_by_proto(&stOpt, pstProto);

	if( -1==s )
		return (GSOCKET)-1;

	gnt_net_set_nonblock(s, 1);

	switch(pstProto->iID)
	{
	case GNT_NET_ID_TCP:
	case GNT_NET_ID_UDP:
		if( gnt_net_str2inet(stOpt.szAddr, &stAddr.stIn) < 0 )
			break;

		if( gnt_socket_connect(s, (struct sockaddr*)&stAddr.stIn, sizeof(stAddr.stIn), a_iTimeout)<0 )
			break;
		else
			return s;

		break;

#if !defined (_WIN32) && !defined (_WIN64)
	case GNT_NET_ID_UNIX:
		if( gnt_net_str2unix(stOpt.szAddr, &stAddr.stUn) < 0 )
			break;

		if( gnt_socket_connect(s, (struct sockaddr*)&stAddr.stUn, sizeof(stAddr.stUn), a_iTimeout)<0 )
			break;
		else
			return s;

		break;
#endif

	default:
		break;
	}

	gnt_net_close(s);

	return (GSOCKET)-1;
}

GSOCKET gnt_net_accept(GSOCKET a_iSock, struct sockaddr* a_pstAddr, int* a_piLen, int a_iTimeout)
{
	return gnt_socket_accept(a_iSock, a_pstAddr, a_piLen, a_iTimeout);
}

int gnt_net_send(GSOCKET a_iSock, const char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	return gnt_socket_send(a_iSock, a_pszBuff, a_iLen, a_iTimeout);
}

int gnt_net_recv(GSOCKET a_iSock, char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	return gnt_socket_recv(a_iSock, a_pszBuff, a_iLen, a_iTimeout);
}

int gnt_net_sendall(GSOCKET a_iSock, const char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	return gnt_socket_sendall(a_iSock, a_pszBuff, a_iLen, a_iTimeout);
}

int gnt_net_recvall(GSOCKET a_iSock, char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	return gnt_socket_recvall(a_iSock, a_pszBuff, a_iLen, a_iTimeout);
}

