//
//  gntc.h
//  gntc
//
//  Created by apollo on 15/5/18.
//  Copyright (c) 2015年 cz. All rights reserved.
//

#ifndef gntc_gntc_h
#define gntc_gntc_h


int connect(char *url);

int deconnect(int fd);

int send(fd,const char *buf,size_t len);

int recv(fd,char *buf,size_t *max);

int connectb(char *url);

int deconnectb(int fd);

int sendb(fd,const char *buf,size_t len);

int recvb(fd,char *buf,size_t *max);

#endif
