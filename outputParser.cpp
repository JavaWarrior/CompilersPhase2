#include "outputParser.h"
#include <fstream>
#include <iostream>
using namespace std;

outputParser::outputParser(aGraph * g1, std::map<int,std::string> * fs)
{
    g = g1;
    finalStates = fs;
}
void outputParser::match(char * path)
{
    ifstream inFile (path);
    ofstream outFile ("tokens.txt");
    string line;
    while(inFile >> line){
        int node = 0, lastMatched = -1, lastInd = -1,ind = 0,lastStart = 0;
#ifdef DEBUG_MODE
        cout<<line<<endl;
#endif // DEBUG_MODE
        while(node < g->size() )
        {
            if(ind == line.size())
            {
                //reached end of string
                if(lastMatched == -1 && lastInd != ind-1)
                {
                    //if could not match end of string
                    cout<<"error: end of string coultdn't be matched"<<endl;
                    outFile<<line.substr(lastStart,line.size()-lastStart)<<" : error"<<endl;

                    break;
                }
                else if(lastInd == ind -1)
                {
                    //if no more characters to parse
//                    outFile<<removeSlash((*finalStates)[lastMatched])<<endl;
                    outFile<<removeSlash((*finalStates)[lastMatched])<<endl;                    break;
                    break;
                }
                else
                {
                    //return to the last final state
//                    outFile<<removeSlash((*finalStates)[lastMatched])<<endl;
                    outFile<<removeSlash((*finalStates)[lastMatched])<<endl;
                    lastMatched = -1;
                    ind = lastInd+1;
                    node = 0;
                    lastStart = ind;
                }
            }
            int toNode = findEdge(node,line[ind]);
            if(toNode == -1)
            {
                if(lastMatched == -1)
                {
//                    cout<<"error"<<endl;
                    outFile<<line.substr(lastStart,line.size()-lastStart)<<" : error"<<endl;
                    break;
                }
                //no transition matching this state
//                outFile<<removeSlash((*finalStates)[lastMatched])<<endl;
                outFile<<removeSlash((*finalStates)[lastMatched])<<endl;
                lastMatched = -1;
                ind = lastInd+1;
                node = 0;
                lastStart = ind;
            }
            else if(finalStates->find(toNode) != finalStates->end())
            {
                lastMatched = toNode;
                lastInd = ind;
                node = toNode;
                ind++;
            }
            else
            {
                node = toNode;
                ind++;
            }
        }
    }
}
int outputParser::findEdge(int node, char ch)
{
    for(int i = 0;  i < (*g)[node].size(); i++)
    {
        if((*g)[node][i].second == ch)
        {
            if (!isDummy((*g)[node][i].first))
                return (*g)[node][i].first;
            break;
        }
    }
    return -1;
}
bool outputParser::isDummy(int node)
{
    for(int i = 0; i < (*g)[node].size() ; i++)
    {
         if((*g)[node][i].first != node)
        {
            return false;
        }
    }
    return true;
}
string outputParser::removeSlash(string s)
{
    if(s[0] == '\\')
    {
        return s.substr(1,s.size()-1);
    }
    return s;
}
outputParser::~outputParser()
{
    //dtor
}
