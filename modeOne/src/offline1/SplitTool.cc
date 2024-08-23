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

    storeDictCN("./dictCN.txt");
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



bool isChineseCharacter(const std::string &str, size_t pos)
{
    if (pos + 2 >= str.size())
        return false; // 需要至少3个字节

    unsigned char c1 = str[pos];
    unsigned char c2 = str[pos + 1];
    unsigned char c3 = str[pos + 2];

    // UTF-8 处理
    if (c1 < 0x80)
        return false; // ASCII范围内的字符

    // 汉字的 UTF-8 编码范围 (3字节)
    if (c1 >= 0xE4 && c1 <= 0xE9)
    {
        if ((c2 & 0xC0) == 0x80 && (c3 & 0xC0) == 0x80)
        {
            return true; // 这范围包含了常用汉字
        }
    }
    return false;
}

void SplitTool::cleanCnMaterial(const string & str)
{
    size_t pos = 0;

    while (pos < str.size())
    {
        if (isChineseCharacter(str, pos))
        {
            _raw.push_back(str[pos]);
            _raw.push_back(str[pos + 1]);
            _raw.push_back(str[pos + 2]);
            pos += 3; // 跳过完整的汉字
        }
        else
        {
            pos++;
        }
    }
    std::cout << "cleanCnMaterial Success!\n";
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
        cleanCnMaterial(txt);

        vector<string> tmp = cut(_raw);

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

void SplitTool::storeDictCN(const string & filepath){
    std::ofstream ofs(filepath);
    if(!ofs){
        std::cerr<<"open file to write failed\n";
        return ;
    }
    for(const auto & ele : _dictCN){
        ofs<<ele.first<<" "<<ele.second<<"\n";
    }
    ofs.close();
}
