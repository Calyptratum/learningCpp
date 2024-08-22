#ifndef __PagelibProcess_H__
#define __PagelibProcess_H__

#define LOGGER_LEVEL LL_WARN 

#include "tinyxml2.h"
#include "../simhash/include/simhash/Simhasher.hpp"

#include <dirent.h>
#include <sys/stat.h>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using std::string;
using std::vector;
using std::pair;
using std::cout;
using std::unordered_map;
using std::map;

class Item{
public:
    string title;
    string link;
    string description;
    string content;

    void print() const {
        cout << "Title: " << title << "\n";
        cout << "Link: " << link << "\n";
        cout << "Description: " << description << "\n";
        cout << "Content: " << content << "\n";
    }
};

class PageLibProcess
{
public:

    ~PageLibProcess(){}
    
    static PageLibProcess* getInstance(const string & path);

    static void destory();

    void storeRawOnDisk(const string& filename);
    void cutRedundantPage(const string& src, const string& des);  //页面去重，simhash算法  
    void createOffsetlib(const string & filename); //构建页面偏移库
    void buildInvertIndexMap(); //构建倒排索引库
    void storeOffset(const string& filename);
    void storeWebIndex(const string& filename);
private:

    PageLibProcess(string path)
    :_directory(path)
    {}
    
    PageLibProcess & operator=(const PageLibProcess & rhs) = delete;
    
    PageLibProcess(const PageLibProcess & rhs) = delete;

private:
    static PageLibProcess* _pgLB;
    string _directory;
    
    vector<uint64_t> _simhasheLib;
    std::map<int, pair<int, int>> _offset;
};

#endif // __PagelibProcess_H__
