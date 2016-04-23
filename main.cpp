#include <iostream>
#include "RulesParser.h"

using namespace std;

int main()
{
    RulesParser * rParser = new RulesParser;
    rParser->match("input.txt");
    rParser->printGrammar(cout);
    return 0;
}
