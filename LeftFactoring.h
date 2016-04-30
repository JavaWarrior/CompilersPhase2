#ifndef LEFTFACTORING_H
#define LEFTFACTORING_H
#include <bits/stdc++.h>
#include "RULE.h"
#include "Match.h"
#include "RulesParser.h"
using namespace std;
class LeftFactoring
{
        public:
                RulesParser *root;
                map<string, Rule*> rules;       //table of Rules.
                LeftFactoring(RulesParser *r);   //Constructor.
                void printRules(vector<Rule*> r);       //Print Rules Console and File.
                vector<Rule*> lcp(vector<vector<Rule*> > r);    //Longest Common Prefix.
                Rule *reBuild(vector<Rule*> prefix, Rule *r);   //rebuild the old Rule and build new one.
                Rule *leftFactor(Rule *r);      //Left factoring the given Rule.
                void simulate();        //Simulation funtion.
        protected:
        private:
};

#endif // LEFTFACTORING_H
