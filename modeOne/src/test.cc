//#include "SettingConf.h"
//#include "SplitTool.h"
//#include "DictProducer.h"
#include <iostream>
#include "SearchPre.h"
//#include <map>
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
/*
    DictProducer::getInstance();
    DictProducer::getInstance()->indexCreate();
    DictProducer::getInstance()->storeIndex();



    DictProducer::destroy();
*/
    std::string input;
    std::cout<<"please enter a string :";
    std::getline(std::cin, input);

    SearchPre::getInstance();
    SearchPre::getInstance()->splitQuery(input);
    vector<candidate> test(SearchPre::getInstance()->generateAndSortCandidate(input));
    if(test.empty()){
        std::cout<<"no candidate found\n";
    }
    for(auto it = test.begin();it<test.begin()+10 &&it != test.end();++it){
        std::cout<<(*it).word<<" ";
    }
    std::cout<<"\n";
    return 0;
}

