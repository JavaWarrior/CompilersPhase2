#ifndef RULESPARSER_H
#define RULESPARSER_H

/** to do
    the first thing is to put spaces between | and any thing else
    then spaces between capital letters and anything else
 **/
#include "Rule.h"
#include <ostream>
class RulesParser
{
    public:

        Rule * start = 0;
        map < string, Rule *> RulesMap;

        RulesParser();
        void match(char * path);
        virtual ~RulesParser();

        void printGrammar(ostream &out);
    protected:

    private:
        string addSpaces(string str);
        bool isCap(char x){return (x >= 'A' && x <= 'Z') || x == '_';}
        void checkEpsilon();
};

#endif // RULESPARSER_H
