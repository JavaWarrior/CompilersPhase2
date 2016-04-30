#include "InputTokenizer.h"
#include <iostream> //for cout
#include <fstream>  //for file
#include <sstream>
using namespace std;

InputTokenizer::InputTokenizer(void)
{
    //ctor
}

InputTokenizer::~InputTokenizer()
{
    //dtor
}

void InputTokenizer::parseFile(char * fileName)
{
    ifstream inFile(fileName); //open file
    string line;
    if(inFile.is_open()){
        while(getline(inFile,line)){
            parseLine(line);
        }
    }

}

void InputTokenizer::parseLine(string line)
{
    clrSpcs(line);
    int expInd, defInd;
    if(line[0] == '{')
    {
        parseKwd(line);
    }
    else if(line[0] == '[')
    {
        parsePunc(line);
    }
    else if( (expInd = line.find(":")) != string::npos)
    {
        //check that this is an expression not a definition with ':' in it.
        if( (defInd = line.find("=")) != string::npos)
        {
            if(expInd < defInd)
            {//occurrence of ':' is before '=' -> it's expression
                parseExp(line,expInd);
            }
            else
            {
                parseDef(line,defInd);
            }
        }
        else
        {
            parseExp(line,expInd);
        }
    }
    else if( (defInd = line.find("=")) != string::npos)
    {
        parseDef(line,defInd);
    }
    else
    {
        cout<<"error cannot parse input file"<<endl;
    }

}

void InputTokenizer::clrSpcs(string& line)
{
    //clear unnecessary spaces from beginning.
    int i =0 ;
    for(i = 0 ; i < line.size(); i++)
    {
        if(line[i] != ' '&& line[i] != '\t')
        {
            break;
        }
    }
    if(i < line.size())
        line = line.substr(i,string::npos);
    //clear from the end
    for(i = line.size()-1 ; i > -1; i--)
    {
        if(line[i] != ' '&& line[i] != '\t')
        {
            break;
        }
    }
    if(i>-1)
        line = line.substr(0,i+1);
}

void InputTokenizer::parseDef(string line, int pos)
{
    //token = regExp
    string lhs = noSpaces(line.substr(0,pos)),rhs = noSpaces(line.substr(pos+1));
    rhs = expand(rhs);
    regDefn[lhs] = rhs;
}

void InputTokenizer::parseExp(string line, int pos)
{
    //token : exp
    tokens[noSpaces(line.substr(0,pos))] = substitue(line.substr(pos+1));
    tokens_ordered.push_back(noSpaces(line.substr(0,pos)));
}

void InputTokenizer::parseKwd(string line)
{
    stringstream ss (line.substr(1,line.size()-2));//ignore parentheses
    string next;
    while(ss>>next)
    {
        kwds.push_back(next);
    }
}

void InputTokenizer::parsePunc(string line)
{
    stringstream ss (line.substr(1,line.size()-2));//ignore parentheses

    string next;
    while(ss>>next)
    {
        pncs.push_back(next);
        tokens_ordered.push_back(next);
    }
}

void InputTokenizer::printContents(void)
{
    //this function mainly used for debugging
    cout<<"defs:"<<endl;
    for(map<string,string>::iterator it = regDefn.begin(); it != regDefn.end(); it++)
    {
        cout<<it->first<<" = "<<it->second<<endl;
    }
    cout<<"\nexpressions:"<<endl;
    for(map<string,string>::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        cout<<it->first<<" : "<<it->second<<endl;
    }
    cout<<"\nkwds:"<<endl;
    for(vector<string>::iterator it = kwds.begin() ; it != kwds.end() ; it++)
    {
        cout<<*it<<endl;
    }
    cout<<"\npunc:"<<endl;
    for(vector<string>::iterator it = pncs.begin() ; it != pncs.end() ; it++)
    {
        cout<<*it<<endl;
    }
}

string InputTokenizer::noSpaces(string str)
{
    string temp = "";
    for(int i =0 ; i < str.size() ;i++)
    {
        if(str[i] != ' ')
        {
            temp+=str[i];
        }
    }
    return temp;
}

string InputTokenizer::expand(string  str)
{
    string temp = "";
    for(int i =0 ; i < str.size() ; i++)
    {
        if(str[i] == '-' && str[i-1] != '\\')
        {
            temp.insert(temp.size()-1,"(");
            for(char start = str[i-1]+1 ; start <= str[i+1] ; start++)
            {
                temp += '|';
                temp += start;
            }
            i++; //advance the ending character
            temp += ')';
        }
        else
        {
            temp += str[i];
        }
    }
    return temp;
}

string InputTokenizer::substitue(string str)
{
    string finalStr, temp;
    str = addSpaces(str);
    stringstream ss(str);
    while(ss>>temp)
    {
        if(regDefn.find(temp) != regDefn.end() ){
            finalStr += substitue(regDefn[temp]);//recursive call        }
        }
        else
        {
            finalStr += noSpaces(temp);
        }

    }
    return finalStr;

}

string InputTokenizer::addSpaces(string str)
{
    string delms = "|\\()*+";
    for(int i = 0 ; i < str.size() ; i++)
    {
        for(int j = 0 ; j < delms.size() ; j++)
        {
            if(str[i] == delms[j])
            {
                if(i>0 && str[i-1] != ' ')
                {
                    str.insert(i," ");
                }
                if(str[i+1] != ' ' && str[i] != '\\' )
                {
                    str.insert(i+1," ");
                }
            }
        }
    }
    return str;
}


