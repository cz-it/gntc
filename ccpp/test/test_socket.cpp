#include "test_socket.h"

#include <stdlib.h>
#include <string.h>
int main (int argc,char *argv[])
{
/*
	int ret = gnt_socket_init(2,2);
		if (ret != 0)
	{
		LOG_D("socket init error");
		exit(-1);
	}

	GSOCKET sfd = gnt_socket_open_tcp();
	if (sfd < 0) {
		LOG_D("socket init error");
		exit(-1);
	}
	gnt_socket_set_nonblock(sfd,1);
*/

	gnt_socket_init(2,2);
	int ret =-1;
	GSOCKET sfd =gnt_net_connect("tcp://127.0.0.1:9527?reuse=1", 5000);
	if (sfd <0)	{
		LOG_D("net init error");
		exit(-1);
	}
	gnt_net_set_recvbuff(sfd,1024);
	gnt_net_set_sendbuff(sfd,1024);

	char *dataBuf =(char *) malloc(1024);
	memset(dataBuf,'\0',1024);
	char *data="nimei";
	ret = gnt_net_sendall(sfd,data,strlen(data),5000);
	if (ret <0 ) {
		LOG_D("net gnt_net_sendall error with %d:%s",socketerrno,strerror(socketerrno));
		exit(-1);	
	}
	ret = gnt_net_recvall(sfd,dataBuf,strlen(data),5000);
	if (ret <0 ) {
		LOG_D("net gnt_net_recvall error");
		exit(-1);	
	}
	LOG_D("get data :%s",dataBuf);
	gnt_net_close(sfd);
	free(dataBuf);
	return 0;
}