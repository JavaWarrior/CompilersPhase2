#include "RulesParser.h"
#include <fstream>
#include <sstream>
#include "Errors.h"
#include <iostream>

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

    int lineCounter = 0;

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
                    return Errors::printError(EXPECTED_STRING_BEFORE_EQUAL, lineCounter, cout);
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
                    return Errors::printError(EXPECTED_STRING_BEFORE_OR, lineCounter, cout);
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
            return Errors::printError(EXPECTED_STRING_BEFORE_OR, lineCounter, cout);
        }else if (curState == EQUAL)
        {
            return Errors::printError(EXPECTED_STRING_BEFORE_EQUAL, lineCounter, cout);
        }
        curState = NEWLINE;
        lineCounter++;
    }

    checkEpsilon();
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
