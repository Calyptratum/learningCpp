#include "SearchPre.h"
#include <algorithm>
#include <ctype.h>

SearchPre* SearchPre::_search = nullptr;

struct candidate;

SearchPre* SearchPre::getInstance(){
    if(!_search){
        _search = new SearchPre("./dict.txt","./index.bat");
        std::cout<<"getInstance success\n";
    }
    return _search;
}

SearchPre::~SearchPre(){
}
 
void SearchPre::destroy(){
    if(_search){
        delete  _search;
        _search = nullptr;
    }
}

SearchPre::SearchPre(const string & dictPath,const string & indexPath){
    //加载字典
    std::ifstream ifsDict(dictPath);
    if(!ifsDict.is_open()){
        std::cerr<<"open "<<dictPath<<"failed!\n";
        return ;
    }
    string line;
    string key;
    int value;
    while(std::getline(ifsDict,line)){
        std::istringstream iss(line);
        while(iss>>key>>value){
            _dict.emplace_back(std::make_pair(key,value));
        }
    }
    ifsDict.close();
    std::cout<<"dict sucess\n";
    //加载索引
    std::ifstream ifsIndex(indexPath);
    if(!ifsIndex.is_open()){
        std::cerr<<"open"<<indexPath<<"failed!\n";
        return ;
    }

    while(std::getline(ifsIndex,line)){
        std::istringstream issI(line);
        if(issI>>key){
            while(issI>>value){
                _index[key].insert(value);
            }
        }
    }
    ifsIndex.close();
    std::cout<<"index sucess\n";
}


static size_t nBytesCode(const char ch) {
    if (ch & (1 << 7)) {
        int nBytes = 1;
        for (int idx = 0; idx != 6; ++idx) {
            if (ch & (1 << (6 - idx))) {
                ++nBytes;   
            } else {
                break;
            }
        }
//        std::cout<<"get ok\n";
        return nBytes;
    }
    return 1;
}

static size_t length(const string &str) {
    size_t ilen = 0;
    for (size_t idx = 0; idx < str.size(); ++idx) {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
   // std::cout<<"get length ok\n";
    return ilen;
}

static int triple_min(const int &a, const int &b, const int &c)
 {  
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
 }

static int editDistance(const string & lhs, const string & rhs) {
    //计算最小编辑距离-包括处理中英文
    size_t lhs_len = length(lhs);
    size_t rhs_len = length(rhs);
    int editDist[lhs_len + 1][rhs_len + 1];

    for(size_t idx = 0; idx <= lhs_len; ++idx) {
        editDist[idx][0] = idx;
    }
    for(size_t idx = 0; idx <= rhs_len; ++idx) {
        editDist[0][idx] = idx;
    }
    
    string sublhs, subrhs;
    for(size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx) {
        size_t nBytes = nBytesCode(lhs[lhs_idx]);
        sublhs = lhs.substr(lhs_idx, nBytes);
        lhs_idx += (nBytes - 1);
        for(size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx) {
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += (nBytes - 1);
            if(sublhs == subrhs) {
                editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
            } else {
                editDist[dist_i][dist_j] = triple_min(
                    editDist[dist_i][dist_j - 1] + 1,
                    editDist[dist_i - 1][dist_j] + 1,
                    editDist[dist_i - 1][dist_j - 1] + 1
                );
            }
        }
    }
//    std::cout<<"get distance ok\n";
    return editDist[lhs_len][rhs_len];
}

//enterQuery:输入序列，这个函数对输入序列进行拆分
void SearchPre::splitQuery(const string& enterQuery){

    for(size_t i = 0;i< enterQuery.size();i++){
        char c = enterQuery[i];
        if(isspace(c)){
            continue;
        }else if((unsigned char)c >= 0x80){ //非ASCII字符
            //对中文字符进行分割处理
            string chineseStr;
            chineseStr +=c;
            if(i+1<enterQuery.size()) chineseStr += enterQuery[i+1];
            if(i+2<enterQuery.size()) chineseStr += enterQuery[i+2];
            _words.push_back(chineseStr);
            i+=2;
            continue;
        }else if(isalnum(c)){
            //英文或数字
            _words.push_back(string(1,c));
        }

    std::cout<<"get splitQuery ok\n";
    }
}

set<struct candidate> SearchPre::generateAndSortCandidate( const string & enterQuery){
    set<int> resultSet;
    for(auto & word :_words){
        auto it = _index.find(word);
        if(it != _index.end()){
            std::cout<<"Found word in here "<<word<<"\n";
            if(resultSet.empty()){
                resultSet.insert(it->second.begin(),it->second.end());       
            }
        }else{
            std::cout<<"can't find word here"<<word<<"\n";
        }
    }
    std::set<struct candidate> recommend;
    struct candidate temp = {enterQuery,0,0};
    recommend.insert(temp);
    for(int idx :resultSet){
        if(idx >= 0 && idx <(int)_dict.size()){
            struct candidate tmp= {_dict[idx].first,_dict[idx].second,editDistance(_dict[idx].first,enterQuery)};
            recommend.insert(tmp);
        }
    }
    _words.clear();
    return recommend;
}
