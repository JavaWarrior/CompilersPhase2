#ifndef RULESPARSER_H
#define RULESPARSER_H


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
        int lineCounter = 0;

        string addSpaces(string str);
        bool isCap(char x){return (x >= 'A' && x <= 'Z') || x == '_' ||(x >= '0' && x <= '9')||(x == '\'');}
        void checkEpsilon();

        string getNewName(string x);
        void removeDirectLR(Rule * r);
        void eliminateLeftRecursion();
        void substitute(Rule * from, Rule * to);
};

#endif // RULESPARSER_H
