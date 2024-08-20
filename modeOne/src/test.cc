//#include "SettingConf.h"
//#include "SplitTool.h"
#include "DictProducer.h"
#include <iostream>
#include <map>
int main()
{   /*
    SettingConf setcon("setting.conf");
    for(const auto& ele : setcon._setting){
        std::cout<<ele.first <<" "<<ele.second<<"\n";
    }*/
    /*
    SplitTool sp("./yuliaoCN");
    for(auto & ele : sp._dictCN){
        cout<<ele.first<<" "<<ele.second<<"\n";
    }*/

    DictProducer::getInstance();
    DictProducer::getInstance()->indexCreate();
    DictProducer::getInstance()->storeIndex();
    DictProducer::destroy();

    return 0;
}

