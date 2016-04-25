#include "RulesParser.h"
#include <fstream>
#include <sstream>
#include "Errors.h"
#include <iostream>
#include <cstdlib>
#include <stack>

using namespace std;

//#define DEBUG_MODE_PARSER

RulesParser::RulesParser()
{
    //ctor

}

RulesParser::~RulesParser()
{
    //dtor
}


/* external functions */
void RulesParser::match(char * path )
{
    ifstream inpFile (path);
    string line;

    lineCounter = 0;

    typedef enum {STR, EQUAL, OR, NA, NEWLINE} statesEnum;
    statesEnum curState = NA, lastState = NA;
    string token,lastToken;

    Rule * curRule;

    while(getline(inpFile,line))
    {

        stringstream ss(addSpaces(line));

        while(ss>>token)
        {
#ifdef DEBUG_MODE_PARSER
            cout << token << endl;
#endif // DEBUG_MODE_PARSER
            if(token == "::=")
            {
                if(curState != STR)
                {
                    /* ::= str */
                    return Errors::printErrorWithLine(EXPECTED_STRING_BEFORE_EQUAL, lineCounter, cout);
                }
                else
                {
                    lastState = curState;
                    curState = EQUAL;

                    if(RulesMap.find(lastToken) != RulesMap.end())  /* rule was defined before ? */
                    {
                        curRule = RulesMap[lastToken];
                        curRule ->nodeType = Rule::nonTerminal;   /* change this node to nonTerminal */
                    }
                    else{
                        curRule = new Rule(Rule::nonTerminal,lastToken);          /* define new rule */
                        RulesMap[lastToken] = curRule;                         /* assign it to our map */
                    }
                    if(start == 0)
                    {
                        start = curRule;
                    }
                }
            }
            else if (token == "|")
            {
                if(curState != NEWLINE && curState != STR)
                {
                    return Errors::printErrorWithLine(EXPECTED_STRING_BEFORE_OR, lineCounter, cout);
                }
                lastState = curState;
                curState = OR;
            }
            else
            {
                /* normal string */
                if(curState == OR || curState == EQUAL)
                {
                    Rule * r;
                    if(RulesMap.find(token) != RulesMap.end())
                    {
                        r = RulesMap[token];
                    }
                    else
                    {
                        r = new Rule(Rule::terminal, token);
                        RulesMap[token] = r;
                    }
                    curRule->productions.resize(curRule->productions.size()+1);
                    curRule->productions[curRule->productions.size()-1].push_back(r);
                }
                else if (curState == NA || curState == NEWLINE)
                {
                    /* start of file do nothing */
                }
                else
                {
                    Rule * r;
                    if(RulesMap.find(token) != RulesMap.end())
                    {
                        r = RulesMap[token];
                    }
                    else
                    {
                        r = new Rule(Rule::terminal, token);
                        RulesMap[token] = r;
                    }
                    curRule->productions[curRule->productions.size()-1].push_back(r);
                }
                lastToken = token;
                curState = STR;
            }
        }
        if(curState == OR)
        {
            return Errors::printErrorWithLine(EXPECTED_STRING_BEFORE_OR, lineCounter, cout);
        }else if (curState == EQUAL)
        {
            return Errors::printErrorWithLine(EXPECTED_STRING_BEFORE_EQUAL, lineCounter, cout);
        }
        curState = NEWLINE;
        lineCounter++;
    }
    checkEpsilon();
    eliminateLeftRecursion();
}

void RulesParser::printGrammar(ostream & out)
{
    out << "start state: "<<start->name<<endl;

    Rule * r;
    for(map<string,Rule *>::iterator it = RulesMap.begin(); it != RulesMap.end() ; it++)
    {
        r = it->second;
        out<<r->name<<" :"<<endl;
        for(int i =0 ; i < r->productions.size() ; i++)
        {
            for(int j =0 ; j < r->productions[i].size() ; j++)
            {
                out<<r->productions[i][j]->name<<" ";
            }
            out << endl;
        }
        out << endl;
    }
}


string RulesParser::addSpaces(string str)
{
    for(int i = 0 ; i < str.size() ; i++)
    {
        if(i < str.size()-1 && str[i] == '\\' && str[i+1] == 'L')
        {
            i++;
        }
        else if(str[i] == '|' &&(i > 0 && str[i-1] != '\\'))
        {
            str.insert(i," ");
            str.insert(i+1," ");
            i += 2;
        }
        else if(isCap(str[i]))
        {
            if(i>0 && !isCap(str[i-1]))
            {
                str.insert(i," ");
                i++;
            }else if( i < str.size()-1 && !isCap(str[i+1]))
            {
                str.insert(i+1," ");
                i++;
            }
        }
    }
    return str;
}

void RulesParser::checkEpsilon()
{
    if(RulesMap.find("\\L") != RulesMap.end())
    {
        RulesMap["\\L"]->nodeType = Rule::EPSILON;
    }
    else
    {
        RulesMap["\\L"] = new Rule(Rule::EPSILON,"\\L");
    }

}

void RulesParser::removeDirectLR(Rule * r)
{
    /* get productions */
    Rule::productions_t &prods = r->productions;

    /*name a new Rule to be used in elimination of LR */
    string x (r->name);
    x = getNewName(x);

    Rule * extraRule = new Rule(Rule::nonTerminal,x);

    stack <vector<Rule *> > stk;

    /* first we check if there's left recursion */
    for(int i = 0 ; i < prods.size();i++)
    {
        if(prods[i].size() > 0 && prods[i][0] == r )
        {
            /* if left recursion in this production */
            stk.push(prods[i]);
            prods.erase(prods.begin() + i);
            i--;
        }
    }

    if(prods.size() == 0)
    {
        /* all productions is left recursive */
        return Errors::printErrorWithString(WRONG_GRAMMAR_ALL_LR, r->name, cout);
    }
    if(stk.size() == 0 )
    {
        delete extraRule;
        return; /* no Left recursion */
    }
    for(int i =0 ;i < prods.size() ; i++)
    {
        if(prods[i][0]->nodeType == Rule::EPSILON)
            prods[i].erase(prods[i].begin());   /* if the node has epsilon in it the remove it then only A' will be inserted */
        prods[i].push_back(extraRule);  //A -> B1A'|...|BnA'
    }
    while(!stk.empty())
    {
        vector< Rule *> LR = stk.top();stk.pop();
        //A' -> aA'|e
        LR.erase(LR.begin());
        LR.push_back(extraRule);
        extraRule->productions.push_back(LR);
    }
    vector<Rule *> vec; vec.push_back(RulesMap["\\L"] );
    extraRule->productions.push_back(vec); /* insert epsilon */
    RulesMap[extraRule->name] = extraRule;
}

/* generate new name for this Rule that doesn't conflict with other rules */
string RulesParser::getNewName(string x)
{
    while(RulesMap.find(x) != RulesMap.end())
    {
        x += '\''; /* add random number to the string */
    }
    return x;
}

/* eliminate direct and direct left recursion */
void RulesParser::eliminateLeftRecursion()
{
    vector<Rule *> orderedRules;    /* numbering of rules */
    for(map<string,Rule *>::iterator it = RulesMap.begin() ; it != RulesMap.end() ;it++)
    {   /* if rule is non terminal number it */
        if(it->second->nodeType == Rule::nonTerminal){
            orderedRules.push_back(it->second);
        }

    }
    /* algorithm stated in ref book */
    for(int i =0  ; i < orderedRules.size() ; i++)
    {
        for( int  j = 0 ; j < i ; j++)
        {
            /* substitute every resolved rule j into i */
            substitute(orderedRules[i], orderedRules[j]);
        }
        /* remove LR after substitution to mark rule i as LR free */
        removeDirectLR(orderedRules[i]);

    }
}

/* substitutes Rule to into Rule from */
void RulesParser::substitute(Rule * from, Rule * to)
{
    stack <pair<vector<Rule *>, int> > stk;
    /* stack to mark affected productions */
    for(int i = 0; i < from->productions.size() ; i++)
    {
        for(int j = 0 ; j < from->productions[i].size() ; j++)
        {
            if(from->productions[i][j] == to)
            {
                stk.push(make_pair(from->productions[i],j));
                from->productions.erase(from->productions.begin() + i);
                i--;
                break;
            }
        }
    }
    while(!stk.empty())
    {
        vector<Rule *> vc = stk.top().first, suffix,prefix;
        int idx = stk.top().second;
        stk.pop();

        /* if there's prefix and it isn't epsilon */
        if(idx > 0 && vc[0]->nodeType==Rule::EPSILON)
            prefix = vector<Rule*>(vc.begin(), vc.begin() + idx -1);
        /* if there's a suffix and it isn't epsilon */
        if(idx < vc.size() -1 && vc[idx+1]->nodeType==Rule::EPSILON)
            suffix = vector<Rule*>(vc.begin()+idx+1,vc.end());

        vector<Rule *> * tempVec;
        for(int i = 0  ; i < to->productions.size() ; i++)
        {
            tempVec = new vector<Rule *>(prefix.begin(),prefix.end());  /* add prefix */
            tempVec->insert(tempVec->end(),to->productions[i].begin(), to->productions[i].end()); /* substitute */
            tempVec->insert(tempVec->end(),suffix.begin(), suffix.end()); /* add suffix */
            /* add new production to the from rule */
            from->productions.push_back(*tempVec);
        }
    }
}
