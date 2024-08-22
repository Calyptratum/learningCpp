#include "DictProducer.h"
#include "SplitTool.h"
#include "CutEnglishWord.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <algorithm>

//初始化静态成员
DictProducer* DictProducer::_dict = nullptr;
std::mutex DictProducer::_mutex;

bool cmpByValue(const std::pair<string,int> & lhs,const std::pair<string,int> & rhs){
    return lhs.second > rhs.second;
}



DictProducer::DictProducer(){
    //_setingConf = new SettingConf(setConf);
    //todo,从配置信息中读取信息后决定英文单词分词的读取路径及保存路径
    _cutWordEn = new CutEnglishWord("./yuliao");
    _splitT = new SplitTool("./yuliaoCN");
    _dictory.assign(_splitT->_dictCN.begin(),_splitT->_dictCN.end());
    vector<std::pair<string,int>> tmp(_cutWordEn->_dict.begin(),_cutWordEn->_dict.end());
    _dictory.insert(_dictory.end(),tmp.begin(),tmp.end());
    sort(_dictory.begin(),_dictory.end(),cmpByValue);
    store("./dict.txt");
}

void DictProducer::store(const string & filename){
    std::ofstream ofs(filename);
    if(!ofs){
        std::cerr<<"ofs open"<<filename<<"error!\n";
        return ;
    }

    for(const auto & ele :_dictory){
        ofs<<ele.first<<" "<<ele.second<<"\n";
    }
    ofs.close();
}

void DictProducer::indexCreate(){
    std::ifstream ifs("./dict.txt");
        if(!ifs.is_open()){
            std::cerr<<"open ./dict.txt failed!\n";
            return ;
        }
    string line;
    int lineIndex = 0; // 记录行索引
    while (getline(ifs, line))
    {
        // 使用字符串流提取单词
        istringstream iss(line);
        string word;
        // 逐个单词读取并存储到索引中
        while (iss >> word)
        {
            // 如果单词是数字，则跳过
            if (isdigit(word[0]))
            {
                continue; // 跳过数字
            }

            // 处理英文单词
            if (isalpha(word[0]))
            { // 如果第一个字符是字母，处理为英文单词
                for (char c : word)
                {
                    _idx[string(1, c)].insert(lineIndex); // 拆分成字母并存入索引
                }
            }
            else
            { // 处理中文词语
                for (size_t i = 0; i < word.size(); ++i)
                {
                    // 处理 UTF-8 编码的汉字
                    // 获取字符的字节长度
                    size_t charLength = 0;
                    if ((word[i] & 0x80) == 0)
                    {
                        charLength = 1; // ASCII字符
                    }
                    else if ((word[i] & 0xE0) == 0xC0)
                    {
                        charLength = 2; // 中文字符（2字节）
                    }
                    else if ((word[i] & 0xF0) == 0xE0)
                    {
                        charLength = 3; // 中文字符（3字节）
                    }
                    else if ((word[i] & 0xF8) == 0xF0)
                    {
                        charLength = 4; // 中文字符（4字节）
                    }
                    else
                    {
                        // 无效字符，跳过
                        charLength = 1; // 默认处理单字节
                    }

                    // 提取汉字并插入索引
                    string character = word.substr(i, charLength);
                    _idx[character].insert(lineIndex);
                    i += charLength - 1; // 跳过当前字符的字节数
                }
            }
        }
        lineIndex++; // 行索引递增
    }
    ifs.close();
}


void DictProducer::storeIndex(){
    int fd = open("./index.bat",O_WRONLY|O_CREAT|O_APPEND,0666);
    if(fd<0){
        std::cerr<<"error create index.bat in DictProducer::storeIndex!\n";
        return ;
    }
    for(const auto & entry : _idx){
        const string & word = entry.first;
        const set<int> & indices = entry.second;

        std::ostringstream oss;
        oss<< word;

        for(int index : indices){
            oss<<" "<<index;
        }
        oss<<"\n";
        string output = oss.str();
        write(fd,output.c_str(),output.size());
    }
    close(fd);
}

DictProducer::~DictProducer(){
    
    delete _cutWordEn;
    _cutWordEn = nullptr;
    delete _splitT;
    _splitT = nullptr;
}

void DictProducer::destroy(){
    delete _dict;
    _dict = nullptr;
}

DictProducer* DictProducer::getInstance(){
    if(!_dict){
        std::lock_guard<std::mutex> lock(_mutex);
        if(!_dict){
            _dict = new DictProducer();
        }
    }
    return _dict;
}


