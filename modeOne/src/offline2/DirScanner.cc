#include "DirScanner.h"

#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
DirScanner::DirScanner(const string & dir){
    traverse(dir);
}

DirScanner::~DirScanner(){

}

void DirScanner::traverse(const string & dir){
    //打开目录流
    DIR* dirent = opendir(dir.c_str());
    if(!dirent){
        std::cerr<<"open directory :"<<dir<<"failed!\n";
        exit(-1);
    }

    struct dirent* entry;
    entry = readdir(dirent);
    while(entry != nullptr){
        if(entry->d_name[0]=='.'){
            continue;
        }
        //获得完整路径
        string fullPath = dir +"/"+entry->d_name;
        
        struct stat fileStat;
        if(stat(fullPath.c_str(),&fileStat) == 0 && S_ISREG(fileStat.st_mode)){
            _filePaths.emplace_back(fullPath);
        }
    }
    closedir(dirent);
}
