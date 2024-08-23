#include "Socket.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>



Socket::Socket(){
    
    _fd = ::socket(AF_INET,SOCK_STREAM,0);
    if(_fd < 0){
        perror("error create a socket!\n");
        return ;
    }
}

Socket::Socket(int fd):_fd(fd){

}

Socket::~Socket(){
    close(_fd);

}

int Socket::getFd()const{
    return  _fd;
}

void Socket::shutAndCloseWrite(){
    //关闭写端
    //明确关闭写操作，告知对方不会再有数据发送，方便对方确认数据接收完成
    //避免资源浪费，防止以外或者恶意的写操作
    int ret  = shutdown(_fd,SHUT_WR);

    if(ret <0 ){
        perror("shutdown write failed!\n");
        return ;
    }
}
