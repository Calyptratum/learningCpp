#include <string>

#include <map>

using std::map;
using std::string;

class SettingConf{
public:
    SettingConf(const string & filepath);
    ~SettingConf();
public:
    std::map<string,string> _setting;

private:
    void read(const string & filename);
    string clean(const string& str);
};
