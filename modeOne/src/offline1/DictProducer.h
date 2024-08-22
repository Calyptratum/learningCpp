#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__
//#include "CutEnglishWord.h"
//#include "SettingConf.h"

#include <map>
#include <vector>
#include <set>
#include <string>
#include <mutex>

using std::map;
using std::vector;
using std::set;
using std::string;

class CutEnglishWord;
class SplitTool;
//class SettingConf;

class DictProducer{
public:
    static DictProducer* getInstance();
    
    static void destroy();

    void store(const string & filename);
    
    void indexCreate();
    
    void storeIndex();

private:
    DictProducer();
    ~DictProducer();
    DictProducer & operator=(const DictProducer & rhs) = delete ;
    DictProducer(const DictProducer & rhs) = delete ;

private:
    //SettingConf* _setingConf;
    CutEnglishWord* _cutWordEn;
    SplitTool* _splitT;
    static DictProducer* _dict;
    static std::mutex _mutex;
    vector<std::pair<string,int>> _dictory;
    map<string ,set<int>> _idx;
};

#endif
