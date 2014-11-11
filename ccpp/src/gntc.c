#include "gntc/gntc.h"

#include "pal/gnt_net.h"
#include "pal/gnt_socket.h"
#include "pal/gnt_os.h"

int gntc_init()
{
    return 0;
}

int gntc_connect(char *url,float timeout)
{
    return 0;
}

int gntc_disconnect(int hdl)
{
    return 0;
}

int gntc_reconnect(int hdl)
{
    return 0;
}

int gntc_sendbuf(int hdl,const void *buf, size_t len, float timeout)
{
    return 0;
}

int gntc_recvbuf(int hdl, void *buf, size_t len,float timeout)
{
    return 0;
}

int gntc_uninit(int hdl)
{
    return 0;
}