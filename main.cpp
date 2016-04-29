#include <iostream>
#include "RulesParser.h"
#include "TableRule.h"
#include <fstream>
using namespace std;

int main()
{
    RulesParser * rParser = new RulesParser;
    rParser->match("input(LL1).txt");
//    rParser->match("input.txt");
    ofstream fuck("out.out");
//    rParser->printGrammar(cout);
    rParser->printGrammar(fuck);
    return 0;
}

