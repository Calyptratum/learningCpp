#include "SplitTool.h"
#include <dirent.h>
#include <sys/stat.h>

#include <algorithm>
#include <ios>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using cppjieba::Jieba;
SplitTool::SplitTool(const string & filepath)
:_jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH){
    createDictCN(filepath);

}

vector<string> SplitTool::cut(const string &sentence)
{
    vector<string> result;
    _jieba.CutForSearch(sentence, result);
    return result;
}

size_t getByteNum_UTF8(const char byte){
    int byteNum = 0;
    for(size_t i = 0; i < 6; ++i)
    {
        if(byte & (1 << (7 - i)))
            ++byteNum;
        else
            break;
    }
    return byteNum == 0 ? 1 : byteNum;
}

void SplitTool::createDictCN(const string & filepath){
    //打开目录
    DIR* dir = opendir(filepath.c_str());
    if(!dir){
        std::cerr<<"error in open directory"<<filepath<<"\n";
        exit(-1);
    }

    struct dirent* entry;
    while((entry = readdir(dir))!=nullptr){
        //忽"."和".."
        if(entry->d_name[0] == '.'){
            continue;
        }

        //构建完整路径
        string fullpath  =filepath + "/" + entry->d_name;

        //检查是否为文件
        struct stat filestat;
        if(stat(fullpath.c_str(),&filestat)==0 &&S_ISREG(filestat.st_mode)){
            _paths.push_back(fullpath);
        }

    }
    closedir(dir);
    //读取文件完成
    
    std::ifstream ifs;
    string line;
    string word;
    for(auto& file: _paths){
        ifs.open(file,std::ios::ate);
        if(!ifs.is_open()){
            std::cerr<<"open failed\n";
            return;
        }
        size_t length = ifs.tellg();
        ifs.seekg(std::ios_base::beg);
        char* buff = new char[length+1];
        ifs.read(buff,length+1);
        string txt(buff);
        delete []buff;

        vector<string> tmp = cut(txt);

        for(auto & ele :tmp){
            if(getByteNum_UTF8(ele[0]==3)){
                auto exist = _dictCN.find(ele);
                if(exist != _dictCN.end()){
                    ++_dictCN[ele];
                }else{
                    _dictCN.insert(std::make_pair(ele,1));
                }
            }
        }
    }
    ifs.close();
}
