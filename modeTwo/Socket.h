#ifndef __SOCKET_H__
#define __SOCKET_H__

class Socket{
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int getFd()const;
};
#endif
