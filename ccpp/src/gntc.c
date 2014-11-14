#include "gntc/gntc.h"

#include "pal/gnt_net.h"
#include "pal/gnt_socket.h"
#include "pal/gnt_os.h"


int gntc_init(gntc_line *line)
{
    gnt_socket_init(2,2);
    line->recvbuf = line->sendbuf = NULL;
    line->recvbufcap = line->recvbuflen = 0;
    line->sendbufcap = line->sendbuflen = 0;
    line->sfd = -1;
    return 0;
}

int gntc_connect(char *url,float timeout)
{
    // first do connect to server with socket
    int sfd = gnt_net_connect(url,timeout);
    
    if (sfd<0) {
        return -2;
    }
    
    // second send connect request and recive response
    
    // check if invalied
    
    return -1;
}

int gntc_disconnect(gntc_line *line)
{
    // first send disconnect request
    
    // close fd
    
    return -1;
}

int gntc_reconnect(gntc_line *line)
{
    // first send reconnect request
    
    // deal with response
    
    return -1;
}

int gntc_set_recvbuf(gntc_line *line,size_t len)
{
    if (line->recvbufcap >= len){
        line->recvbufcap = len;
    } else {
        char *buf =realloc(line->recvbuf,len);
        if (buf == NULL) {
            return -1;
        }
        line->recvbuf = buf;
        line->recvbufcap = len;
    }
    
    return 0;
}

int gntc_set_sendbuf(gntc_line *line,size_t len)
{
    if (line->sendbufcap >= len){
        line->sendbufcap = len;
    } else {
        char *buf =realloc(line->sendbuf,len);
        if (buf == NULL) {
            return -1;
        }
        line->sendbuf = buf;
        line->sendbufcap = len;
    }
    
    return 0;
}

int gntc_sendbuf(gntc_line *line,const void *buf, size_t len, float timeout)
{
    return -1;
}

int gntc_recvbuf(gntc_line *line, void *buf, size_t len,float timeout)
{
    return -1;
}

int gntc_uninit(gntc_line *line)
{
    if (line->recvbuf != NULL) {
        free(line->recvbuf);
    }
    line->recvbuf  = NULL;
    
    if (line->sendbuf != NULL) {
        free(line->sendbuf);
    }
    line->sendbuf = NULL;
    line->sendbufcap = line->sendbuflen = 0;
    line->recvbufcap = line->recvbuflen = 0;
    return 0;
}