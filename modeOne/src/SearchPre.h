#ifndef __SEARCHPRE_H__
#define __SEARCHPRE_H__

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <set>
#include <string>
#include <vector>

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;
 
//候选词结构体
struct candidate{
    //词语
    string word = "";
    //频率
    int freq = 0;
    //距离
    int distan = 0;
   
    candidate(){};
    candidate(const std::string& n, int f, int d) : word(n), freq(f), distan(d) {};
};

class SearchPre{
public:
    //单例模式初始化
    static SearchPre* getInstance();
    //销毁类
    static void destroy();
    //拆解输入序列
    void splitQuery(const string& enterQuery);

    vector<struct candidate> generateAndSortCandidate(const string& enterQuery);

private:
    //删除拷贝构造函数和赋值运算符函数
    SearchPre(const SearchPre & rhs) = delete;
    SearchPre & operator=(const SearchPre & rhs) = delete; 
    //创建类，读取索引文件和字典文件
    SearchPre(const string & dictPath ,const string & indexPath);
    vector<pair<string, int>> _dict;
    map<string,set<int>> _index;
    std::vector<string> _words;
    static SearchPre* _search;

};
#endif
