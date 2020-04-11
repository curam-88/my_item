/* $Id: socket.c 1.1 1995/01/01 07:11:14 cthuang Exp $
 *
 * This module has been modified by Radim Kolar for OS/2 emx
 */

/***********************************************************************
  module:       socket.c
  program:      popclient
  SCCS ID:      @(#)socket.c    1.5  4/1/94
  programmer:   Virginia Tech Computing Center
  compiler:     DEC RISC C compiler (Ultrix 4.1)
  environment:  DEC Ultrix 4.3 
  description:  UNIX sockets code.
 ***********************************************************************/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int Socket(const char *host, int clientPort)
{
    int sock;
    unsigned long inaddr;
    struct sockaddr_in ad;
    struct hostent *hp;
    //初始化地址
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    //把主机名转化为数字
    inaddr = inet_addr(host);
    if (inaddr != INADDR_NONE)//如果数字不为空则主机名正确
        memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));
    else//根基host求得主机名传给sockaddr_in 结构
    {
        hp = gethostbyname(host);
        if (hp == NULL)
            return -1;
        memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);
    }
    //设置socket的端口
    ad.sin_port = htons(clientPort);
    //创建socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)//失败
        return sock;
    if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0)//建立连接
        return -1;
    return sock;
}

