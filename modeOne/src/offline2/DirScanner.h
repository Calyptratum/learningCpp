#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include <string>
#include <vector>
using std::string;
using std::vector;

class DirScanner{
public:
    DirScanner(const string & dir);
    ~DirScanner();
private:
    void traverse(const string & dir);
public:
    vector<std::string> _filePaths;
};
#endif
