#include "SettingConf.h"

#include <fstream>
#include <iostream>
#include <sstream>

using std::cout;

string SettingConf::clean(const string & str){
    size_t head = str.find_first_not_of("\t\n\f\r\v ");
    size_t tail = str.find_last_not_of("\t\n\f\r\v ");
    return (head == std::string::npos)?"":str.substr(head,(tail-head+1));

}

void SettingConf::read(const string & filename){
    //创建打开文件流
    std::ifstream ifs(filename);
    if(!ifs.is_open()){
        cout<<"打开文件失败\n";
        return ;
    }

    string line;
    while(getline(ifs,line)){
        
        size_t delimitPos = line.find(":");
        if(delimitPos != std::string::npos){
            string key = line.substr(0,delimitPos);
            string value = line.substr(delimitPos+1);

            key = clean(key);
            value = clean(value);

            _setting[key] = value;
        }
    }

    ifs.close();
}

SettingConf::SettingConf(const string & filepath){
    read(filepath);
}

SettingConf::~SettingConf(){

}
