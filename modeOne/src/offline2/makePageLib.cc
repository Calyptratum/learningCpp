#include "PageLibProcess.h"

#include <iostream>


using std::cout;

int main()
{
    string path = "../netpages";
    PageLibProcess::getInstance(path)->storeRawOnDisk("../data/RawPagelib.dat");
    PageLibProcess::getInstance(path)->cutRedundantPage("../data/RawPagelib.dat", "../data/Pagelib.xml");
    PageLibProcess::getInstance(path)->createOffsetlib("/root/searchEgine/modeOne/src/data/Pagelib.dat");
    PageLibProcess::getInstance(path)->storeOffset("../data/offset.dat");
    PageLibProcess::getInstance(path)->buildInvertIndexMap("../data/Pagelib.dat");
    PageLibProcess::getInstance(path)->storeWebIndex("../data/invertIndex.dat");
    PageLibProcess::getInstance(path)->destory();
    return 0;
}

