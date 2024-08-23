#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include "./cppjieba/Jieba.hpp"
using namespace cppjieba;

#include <vector>
#include <string>
#include <unordered_map>
using std::string;
using std::vector;

//分词工具类
class SplitTool
{
    const string DICT_PATH = "./cppjieba/dict/jieba.dict.utf8";
    const string HMM_PATH = "./cppjieba/dict/hmm_model.utf8";
    const string USER_DICT_PATH = "./cppjieba/dict/user.dict.utf8";
    const string IDF_PATH = "./cppjieba/dict/idf.utf8";
    const string STOP_WORD_PATH = "./cppjieba/dict/stop_words.utf8";
public:
    SplitTool(const string & filepath);

    void createDictCN(const string &);

    vector<string> cut(const string &);
    void storeDictCN(const string & filepath);
    void cleanCnMaterial(const string & str);
public:
    string _raw;
    vector<string> _paths;
    map<string,int> _dictCN;
private:
    Jieba _jieba;
};

#endif


