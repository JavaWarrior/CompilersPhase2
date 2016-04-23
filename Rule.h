#ifndef RULE_H
#define RULE_H

#include <vector>
#include <map>
#include <string>
using namespace std;

class Rule
{
    public:
        /* defining whether the Rule is Terminal or non-Terminal or a '$' */
        typedef enum {terminal, nonTerminal, finalNode, EPSILON} nodeTypeEnum;
        nodeTypeEnum nodeType;

        /* container of the productions of this rule*/
        typedef vector < vector < Rule *> > productions_t;
        productions_t productions;

        /* name of the node (string it contains)*/
        string name;
        /* constructor */
        Rule (nodeTypeEnum nT, string str):nodeType(nT), name(str){}
        ~Rule(){}


};

#endif // RULE_H
