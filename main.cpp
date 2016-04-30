#include <iostream>
#include <fstream>
#include "LeftFactoring.h"
#include "RulesParser.h"
#include "TableRule.h"
#include "Fol_fst.h"
#include "Match.h"
#define f first
#define s second
using namespace std;
int maini();
int main()
{
        maini();
        RulesParser * rParser = new RulesParser;
        rParser -> match("input.txt");
        ofstream fs("out.txt");
        rParser -> printGrammar(fs);
        LeftFactoring * factor = new LeftFactoring(rParser);
        factor -> simulate();
        Fol_fst * fol_fst = new Fol_fst(rParser);
        Table table;
        fol_fst -> parseTable(table);
        ofstream out("parse_table.txt");
        fol_fst->printTable(out,table);
        Match * matcher = new Match(table, rParser -> start -> name);
        matcher -> readAndMatch("tokens.txt");
        return 0;
}

