#ifndef MATCH_H
#define MATCH_H
#define pb push_back
#define mp make_pair
#define F first
#define S second
#include "RULE.h"
#include <bits/stdc++.h>

using namespace std;

class Match
{
        public:
                //Constructor.
                Match(map<pair<string, string>, vector<pair<string, int> > > t, string r);
                //print output in consol.
                void print(vector<string> out);

                //Simulation function.
                void readAndMatch(string path);
                //Print in file
                void printFile(vector<string> out);
                //push the rules in the stack.
                void pushRules(vector<pair<string, int> > v);
                string root; // root rule
                string matcher(string in);      //matching funtion
                stack<pair<string, int> > st;   //stack of rules.
                vector<pair<string, int> > findRule(string r, string in);       //find the rule in the table
                map<pair<string, string>, vector<pair<string, int> > > table;   //Rules table.
        protected:
        private:

};

#endif // MATCH_H
