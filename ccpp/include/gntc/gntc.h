#ifndef GNTC_GNTC_H_
#define GNTC_GNTC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
struct gntc_line_t
{
    char *recvbuf;
    char *sendbuf;
    size_t sendbuflen;
    size_t recvbuflen;
    size_t sendbufcap;
    size_t recvbufcap;
    int sfd;
};
typedef struct gntc_line_t gntc_line;
    
extern int gntc_init(gntc_line *line);

extern int gntc_connect(char *url,float timeout);

extern int gntc_disconnect(gntc_line *line);

extern int gntc_reconnect(gntc_line *line);
    
extern int gntc_set_recvbuf(gntc_line *line,size_t len);
    
extern int gntc_set_sendbuf(gntc_line *line,size_t len);

extern int gntc_sendbuf(gntc_line *line,const void *buf, size_t len, float timeout);

extern int gntc_recvbuf(gntc_line *line, void *buf, size_t len,float timeout);

extern int gntc_uninit(gntc_line *line);

#ifdef __cplusplus
}
#endif


#endif