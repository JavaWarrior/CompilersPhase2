#include "LeftFactoring.h"
//Constructor.
LeftFactoring::LeftFactoring(RulesParser *r)
{
        //Set the Rules Table.
        rules = r->RulesMap;
        root = r;
}
//Printing Function.
void LeftFactoring :: printRules(vector<Rule *> r)
{
        //Output File.
        ofstream file;
        file.open("LeftFactoring.txt");
        /*Printing Rules as follow:
          Rule ::= P1 | P2 | P3
        */
        for(int i = 0; i < (int) r.size(); i++)
        {
                file << r[i] ->name << " ::= ";
                cout << r[i] ->name << " ::= ";
                for(int j = 0; j < r[i] -> productions.size(); j++)
                {
                        if(j)
                        {
                                file << "| ";
                                printf("| ");
                        }
                        for(int k = 0; k < r[i] -> productions[j].size(); k++)
                        {
                                file << r[i] -> productions[j][k] ->name << " ";
                                cout << r[i] -> productions[j][k] ->name << " ";
                        }
                }
                file << "\n";
                cout << "\n";
        }
        file.close();
}
//Longest common prefix Function.
vector<Rule*> LeftFactoring :: lcp(vector<vector<Rule*> > r)
{
        vector<Rule*> ans;
        //Max occurance and its index.
        int idx = -1, ma = 1;
        for(int i = 0; i < (int) r.size(); i++)
        {
                int m = 1;
                Rule *cur = r[i][0];
                for(int j = i + 1; j < (int) r.size(); j++)
                {
                        Rule *alt = r[j][0];
                        if(cur->name == alt->name)
                        {
                                m++;
                        }
                }
                //Update max and index.
                if(m > ma)
                {
                        ma = m;
                        idx = i;
                }
        }
        //If there is a common prefix between two or more prodcutions.
        if(ma > 1)
        {
                int len = 0;
                bool done = false;

                /*Find if there are another productions could be added
                to the prefix and still has the same Max.
                */
                while(true)
                {
                        len++;
                        for(int i = 0; i < (int) r.size(); i++)
                        {
                                //IF current Rule doesn't have the same first production discard.
                                if(i == idx || r[i][0] != r[idx][0])
                                {
                                        continue;
                                }
                                //If length of the longest prefix reached.
                                if(len >= r[i].size() ||
                                    r[i][len] != r[idx][len])
                                {
                                        done = true;
                                        break;
                                }
                        }
                        if(done)break;
                }
                //Vector of the longest common prefix.
                if(len)
                {
                        for(int i = 0; i < len; i++)
                        {
                                ans.pb(r[idx][i]);
                        }
                }
        }
        return ans;
}
//Rebuilding Function.
Rule *LeftFactoring :: reBuild(vector<Rule*> prefix, Rule *r)
{
        //Prefix length
        int len = prefix.size();
        string newName = r -> name;
        //Find unused name.
        while(rules.find(newName) != rules.end())
        {
                newName += "\'";
        }
        //Creat new Rule and set its attributes.
        Rule *child = new Rule(Rule :: nonTerminal, newName);
        //The old productions.
        vector<vector<Rule*> > oldProductions;
        prefix.pb(child);
        oldProductions.pb(prefix);
        prefix.pop_back();
        for(int i = 0; i < (int) r -> productions.size(); i++)
        {
                //If this production conatins the prefix
                if(r -> productions[i][0] == prefix[0])
                {
                        //Find the left productions.
                        vector<Rule*> alt;
                        for(int j = (int) r ->productions[i].size() - 1; j >= len; j--)
                        {
                                alt.pb(r ->productions[i][j]);
                        }
                        if(alt.size() == 0)
                        {
                                alt.pb(new Rule(Rule ::EPSILON, "\\L"));
                        }
                        reverse(alt.begin(), alt.end());
                        //Add the left productions to the new Rule.
                        child -> productions.pb(alt);
                }
                else
                {
                        //Add this productions to the old one.
                        oldProductions.pb(r -> productions[i]);
                }
        }
        //Set the old Rule productions with the updated one.
        r->productions = oldProductions;
        return child;
}
//Left Factoring Function.
Rule *LeftFactoring :: leftFactor(Rule *r)
{
        //Longest common prefix.
        vector<Rule*> v = lcp(r -> productions);
        if(v.size() > 0)
        {
                //Creat new rule and build it and rebuild the old one.
                Rule *child = reBuild(v, r);
                return child;
        }
        else
        {
                return NULL;
        }
}
//Simulation Function.
void LeftFactoring :: simulate()
{
        //Vector of non Terminal rules.
        vector<Rule*> v;
        for(auto it = rules.begin(); it != rules.end(); it++)
        {
                Rule *r = it -> S;
                if(r ->nodeType == Rule ::nonTerminal)
                {
                        v.pb(r);
                }
        }

        //While still have rules could be left factored.
        bool done = true;
        while(done)
        {
                done = false;
                for(int i = 0; i < (int) v.size(); i++)
                {
//                        cout << v[i]->name << "\n";
//                                for(int j = 0; j < v[i]->productions.size(); j++)
//                                {
//                                        for(int k = 0; k < v[i]->productions[j].size(); k++)
//                                        {
//                                                cout << v[i]->productions[j][k]->name << " ";
//                                        }
//                                        cout << " | ";
//                                }
//                                cout << "\n";
                        //The new Rule which equals null if it doesn't exist.
                        Rule *r = leftFactor(v[i]);
                        if(r != NULL)
                        {
                                done = true;
                                rules[r->name] = r;
                                v.pb(r);
                        }
                }
                root->RulesMap = rules;
        }
        //Print The Grammer.
        printRules(v);
}
