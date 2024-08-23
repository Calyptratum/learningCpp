#include "SocketIO.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

SocketIO::SocketIO(int fd):_fd(fd){

}

SocketIO::~SocketIO(){
    close(_fd);
}

int SocketIO::readn(char* buf,int len){//返回值说明：-1错误，正数值：读取成功的长度
    int left = len;//left 剩余长度
    char* pstr = buf;//读取到的信息
    int ret = 0;//目前已读长度

    while(left>0){
        ret = read(_fd,pstr,left);
        if(ret == -1 && errno == EINTR){//系统中断
            continue;
        }
        else if(-1 == ret){//读取错误
            perror("read error -1");
            return -1; 
        }
        else if(0 == ret){
            break;
        }else{
            pstr += ret;
            left -= ret;
        }
    }
    return len-left;
}

int SocketIO::readLine(char* buf,int len){//为了处理类似http使用\r\n的协议
    int left = len-1;
    char* pstr = buf;
    int ret = 0,total =0;

    while(left >0){
        //使用MSG_PEEK不会清空缓冲区的数据，只会进行拷贝操作
        ret = recv(_fd,pstr,left,MSG_PEEK);
        if(-1 == ret && errno ==EINTR){//系统中断
            continue;
        }
        else if(-1 == ret){
            perror("readLine error !\n");
            return -1;
        }
        else if(0 == ret){
            break;
        }
        else{
            for(int idx = 0;idx < ret;++idx){
                if(pstr[idx] == '\n'){
                    int sz = idx+1;
                    readn(pstr,sz);
                    pstr += sz;
                    *pstr = '\0';//为什么不去掉\n？http协议是以\r\n结尾，去掉可能无法正常解析
                }
            }
            readn(pstr,ret);
            total += ret;
            pstr += ret;
            left -= ret;
        }

    }
    *pstr = '\n';
    return total -left;
}


int SocketIO::writen(const char* buf,int len){
    int left =  len;
    const char* pstr = buf;
    int ret;
    
    while(left > 0){
        ret =  write(_fd,pstr,left);
        if(-1==ret && errno ==EINTR){
            continue;
        }
        else if(-1 == ret){
            perror("write file error\n");
            return -1;
        }
        else if(0 == ret){
            break;
        }else{
            pstr += ret;
            left -= ret;
        }
    }
    return len-left;
}
