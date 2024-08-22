//#include "SettingConf.h"
#include "SplitTool.h"
#include "DictProducer.h"
#include <iostream>
#include "SearchPre.h"
#include <map>
int main()
{   
    
    /*SplitTool sp("./yuliaoCN");
    for(auto & ele : sp._dictCN){
        cout<<ele.first<<" "<<ele.second<<"\n";
    }

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
    set<candidate> test(SearchPre::getInstance()->generateAndSortCandidate(input));
    if(test.empty()){
        std::cout<<"no candidate found\n";
    }
    int count =0;
    for(auto it = test.begin();it != test.end() && count <10;++it,++count){
        std::cout<<(*it).word<<" ";
    }
    std::cout<<"\n";

    SearchPre::destroy();
    return 0;
}

