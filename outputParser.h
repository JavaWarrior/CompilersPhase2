#ifndef OUTPUTPARSER_H
#define OUTPUTPARSER_H

#include "NFABuilder.h"

#include <map>


class outputParser
{
    public:
        void match(char * path);
        outputParser(aGraph * g1, std::map<int,std::string> *fs);
        virtual ~outputParser();
    protected:
    private:
    aGraph * g;
    std::map<int,std::string> *finalStates;
    int findEdge(int node,char ch);
    bool isDummy(int node);
    std::string removeSlash(std::string s);
};

#endif // OUTPUTPARSER_H
