#ifndef Fol_fst_H
#define Fol_fst_H
#define f first
#define s second
#include <bits/stdc++.h>
#include "RulesParser.h"
typedef map<pair<string,string> , vector<pair<string,int> > >  Table;
using namespace std;

class Fol_fst
{
    public :
        void First(string s,set<string> &res,int k);
        void Follow(string s,set<string> &res);
        void parseTable(Table & table);
        void printTable(ostream & out,Table table);
        Fol_fst(RulesParser *r);

    private :
        RulesParser *rulesParser ;


};
#endif

