#include "CutEnglishWord.h"

#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <sstream>

CutEnglishWord::CutEnglishWord(const string & filepath){
    
    readDirectory(filepath);
    read();

}

CutEnglishWord::~CutEnglishWord(){

}

void CutEnglishWord::cleanAndPushWord(string & word){
    string result;
    for (auto & c : word) {
        if (::isalpha((unsigned char)c)) {
                result += tolower(c);
        }
    }
    word = result;
    pushWord(word);    
}

void CutEnglishWord::pushWord(const string & word){
    if (!word.empty()) {
            _dict[word]++;
    }
}

void CutEnglishWord::readDirectory(const string & filepath){
    //打开目录
    DIR* dir = opendir(filepath.c_str());
    if(!dir){
        std::cerr<<"error in open directory:"<<filepath<<"\n";
        exit(1);
    }

    struct dirent* entry;
    while((entry = readdir(dir))!= nullptr){
        if(entry->d_name[0]=='.'){
            continue;
        }
        //构建完整路径
        string fullpath = filepath + "/" + entry->d_name;
        
        //检查是否为文件
        struct stat filestat;
        if (stat(fullpath.c_str(), &filestat) == 0 && S_ISREG(filestat.st_mode)){
            _paths.push_back(fullpath); // 将路径存入vector
        }
    }
    closedir(dir);
}

void CutEnglishWord::read() {
    std::ifstream ifs;
    string line;
    string word;
    for (auto & ele : _paths) {
        ifs.open(ele.c_str());
        if (!ifs.is_open()) {
            std::cerr << "open file error: " << ele << "\n";
            exit(-1);
        }

        while (std::getline(ifs, line)) {
            for (char& c : line) {
                //将其他非字母字符、数字替换为空格
                if (!::isalnum(c)) {
                    c = ' '; // 替换非字母数字字符为空格
                }
            }

            std::istringstream iss(line);
            while (iss >> word) {
                cleanAndPushWord(word);
            }
        }
        ifs.close(); // 关闭文件流
    }
}

