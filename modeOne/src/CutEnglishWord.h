#ifndef __CUTENGLISHWORD_H__
#define __CUTENGLISHWORD_H__

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class CutEnglishWord{
public:
    CutEnglishWord(const string & filepath);
    ~CutEnglishWord();
private:
    void readDirectory(const string & filepath);
    void read();
    void cleanAndPushWord(string & word);
    void pushWord(const string & word);
public:
    vector<string> _paths;
    map<string,int> _dict;
};
#endif
