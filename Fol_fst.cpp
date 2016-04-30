#include <bits/stdc++.h>
#include "RULE.h"
#include "Fol_fst.h"
#include "RulesParser.h"
using namespace std;
Fol_fst::Fol_fst(RulesParser *r)
{
    rulesParser=r;
}
void Fol_fst::First(string s ,set<string> &res,int k)
{
    //vector that contains sub-result of the answer
    set<string> subRes[1000];
    //production rule for string s
    Rule * rule=rulesParser->RulesMap[s];
    // if X is terminal -> First(x) = {x}
    if(rule->nodeType==Rule::terminal||rule->nodeType==Rule::EPSILON)
    {
        // add x to first (X)
        res.insert(s);
        return ;
    }
    // if X is non-terminal .
    //if X → ε is a production, then add ε to FIRST(X).
    for(int i=0;i<rule->productions.size();i++)
    {
        if(rule->productions[i][0]->nodeType==Rule::EPSILON)
        {
            // add eps to First(X)
            res.insert("\\L");
            return ;
        }
        //If X is a non-terminal, and X → Y1 Y2 … Yk
        //is a production, then add a to FIRST(X)
        //if for some i, a is in FIRST(Yi),
        //and ε is in all of FIRST(Y1), …, FIRST(Yi-1).

        for(int j=0;j<rule->productions[i].size();j++)
        {
            bool found_eps=false;
            First(rule->productions[i][j]->name,subRes[k],k++);
            for(string s:subRes[k])
            {
                if(s=="\\L")found_eps=true;
                else
                    res.insert(s);
            }
            if(!found_eps)break;
            else if(found_eps&&j==rule->productions[i].size()-1)
                res.insert("\\L");
        }
    }
}
map<string,vector<string> > dependent;
void Fol_fst::Follow(string s,set<string> &res)
{
    Rule * rule=rulesParser->RulesMap[s];
    string start=rulesParser->start->name;
    set<string>subres_follow1,subres_follow2,subres_first;
    //If S is the start symbol -> $ is in FOLLOW(S)
    if(s==start)
        res.insert("$");
    //if A ->alpha B beta is a production rule
    // -> everything in FIRST(beta) is in FOLLOW(B) except eps
    Rule * r;
    for(auto it=rulesParser->RulesMap.begin();it!=rulesParser->RulesMap.end();it++)
    {
        r=it->second;
        //for all productions search for state s
        for(int i =0 ; i < r->productions.size() ; i++)
        {
            for(int j =0 ; j < r->productions[i].size() ; j++)
            {
                // if state s is found in rhs of production rules
                if(r->productions[i][j]->name==s)
                {
                    // if A->alpha B
                    if(j==r->productions[i].size()-1)
                    {
                        // all elements in Follow(A) is in Follow B also
                        if(r->name!=s&&(dependent.find(s)==dependent.end()||find(dependent[s].begin(),dependent[s].end(),r->name)==dependent[s].end()))
                        {
                            dependent[s].push_back(r->name);
                            subres_follow1.clear();
                            Follow(r->name,subres_follow1);
                            for(string sRF1:subres_follow1)
                                res.insert(sRF1);
                        }
                    }
                    else
                    {
                        // if A->alpha B beta then all elements in First (beta) is in Follow (B)
                        subres_first.clear();
                        First(r->productions[i][j+1]->name,subres_first,0);
                        for(string s_first:subres_first)
                        {
                            // if epsilon in First(beta) -> all elements in Follow(A) is in Follow (B)
                            if(s_first=="\\L")
                            {
                                if(r->name!=s&&(dependent.find(s)==dependent.end()||find(dependent[s].begin(),dependent[s].end(),r->name)==dependent[s].end()))
                                {
                                    dependent[s].push_back(r->name);
                                    subres_follow2.clear();
                                    Follow(r->name,subres_follow2);
                                    for(string sRF2:subres_follow2)
                                        res.insert(sRF2);
                                }
                            }
                            else
                                res.insert(s_first);
                        }
                    }
                }
            }
        }
    }
}

void Fol_fst::parseTable(Table & table)
{
    Rule * r;
    set<string>subres1,subres2;
    for(auto it=rulesParser->RulesMap.begin();it!=rulesParser->RulesMap.end();it++)
    {
        r=it->second;
        //A->alpha
        for(int i =0 ; i < r->productions.size() ; i++)
        {
            //for each terminal in first (alpha)
            subres1.clear();
            First(r->productions[i][0]->name,subres1,0);
            for(string s:subres1)
            {
                // if eps is in First(alpha)
                if(s=="\\L")
                {
                    // for each terminal in Follow(A)
                    dependent.clear();
                    subres2.clear();
                    Follow(r->name,subres2);
                    for(string ss:subres2)
                    {
                        //add A->alpha to M[A,ss]
                        //M[ r->name , ss ] = (r->name)  -> (r->productions[i][k]->name)
                        for(int k=0;k<r->productions[i].size();k++)
                        {
                            int f;
                            if(r->productions[i][k]->nodeType==Rule::terminal)f=1;
                            else if(r->productions[i][k]->nodeType==Rule::nonTerminal)f=0;
                            else if(r->productions[i][k]->nodeType==Rule::EPSILON)f=2;
                            table[make_pair(r->name,ss)].push_back(make_pair(r->productions[i][k]->name,f));
                        }
                    }
                }
                else
                {
                    // if this terminal isn't Eps
                    // add A->alpha to M[A,s]
                    //M[ r->name , s ] = (r->name)  -> (r->productions[i][k]->name)
                    for(int k=0;k<r->productions[i].size();k++)
                    {
                        int f;
                        if(r->productions[i][k]->nodeType==Rule::terminal)f=1;
                        else if(r->productions[i][k]->nodeType==Rule::nonTerminal)f=0;
                        else if(r->productions[i][k]->nodeType==Rule::EPSILON)f=2;
                        table[make_pair(r->name,s)].push_back(make_pair(r->productions[i][k]->name,f));
                    }

                }
            }
        }
    }
    for(auto it=rulesParser->RulesMap.begin();it!=rulesParser->RulesMap.end();it++)
    {
        r=it->second;
        // Panic-Mode Error Recovery
        if(r->nodeType==Rule::nonTerminal)
        {
            dependent.clear();
            set<string>subres3;
            Follow(r->name,subres3);
            for(string s:subres3)
            {
                //if (M[r->name, s] == "") M[r->name,s]="SYNCH"
                if(table[make_pair(r->name,s)].size()==0)
                    table[make_pair(r->name,s)].push_back(make_pair("SYNCH",4));
            }
        }
    }
}

//print the parsing table in out stream
void Fol_fst::printTable(ostream & out,Table table)
{
    for(auto it = table.begin();it!=table.end();it++)
    {
        out<<"Table[ "<<(it->f.f)<<" , "<<(it->f.s)<<" ]"<<endl<< (it->f.f) <<"---> ";
        for(int i=0;i<it->s.size();i++)
            out<<it->s[i].f;
        out<<"\n*******************************************\n";
    }
}
