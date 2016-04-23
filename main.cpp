#include <iostream>
#include "RulesParser.h"
#include "TableRule.h"
using namespace std;

int main()
{
    RulesParser * rParser = new RulesParser;
    rParser->match("input.txt");
    rParser->printGrammar(cout);
    return 0;
}
