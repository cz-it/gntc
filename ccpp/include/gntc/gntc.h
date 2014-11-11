#ifndef GNTC_GNTC_H_
#define GNTC_GNTC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
struct xxx_t
{
    int sfd;
};
typedef struct xxx_t xxx;
    
extern int gntc_init();

extern int gntc_connect(char *url,float timeout);

extern int gntc_disconnect(int hdl);

extern int gntc_reconnect(int hdl);

extern int gntc_sendbuf(int hdl,const void *buf, size_t len, float timeout);

extern int gntc_recvbuf(int hdl, void *buf, size_t len,float timeout);

extern int gntc_uninit(int hdl);

#ifdef __cplusplus
}
#endif


#endif