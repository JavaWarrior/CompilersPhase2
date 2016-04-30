#include "Match.h"
using namespace std;
//Constructor.
Match :: Match(map<pair<string, string>, vector<pair<string, int> > > t, string r)
{
        Match :: table = t;
        Match :: root = r;
        st.push(mp("$", 1));
        st.push(mp(root, 0));
}

//Convert int to string.
string intToString(int n)
{
        string ans = "";
        while(n)
        {
                ans += n % 10 + '0';
                n /= 10;
        }
        reverse(ans.begin(), ans.end());
        return ans;
}
//Print Result Console.
void Match :: print(vector<string> out)
{
        for(int i = 0; i < (int)out.size(); i++)
        {
                puts(out[i].c_str());
        }
}
//Print Result in File.
void Match :: printFile(vector<string> out)
{
        ofstream file;
        file.open("RulesOutput.txt");
        for(int i = 0; i < (int)out.size(); i++)
        {
                file << out[i] << "\n";
        }
        file.close();
}
//Search for the Rule in the Table as table[X, token].
vector<pair<string, int> > Match :: findRule(string r, string in)
{
        //IF founded return the Rule.
        if(table.find(mp(r, in)) != table.end())
        {
                return table[mp(r, in)];
        }
        else
        {
                //Return empty vector.
                vector<pair<string, int> >  dummy;
                return dummy;
        }
}
//Push founded rules in the stack.
void Match :: pushRules(vector<pair<string, int> > v)
{
        for(int i = (int) v.size() - 1; i >= 0; i--)
        {
                st.push(v[i]);
        }
}

//Matching Function.
string Match :: matcher(string in)
{
        string ans = "";
        while(!st.empty())
        {
                pair<string, int> rule = st.top();
                st.pop();
                //If the Rule in non Terminal.
                if(rule.S == 0)
                {
                        //Find The Corrsponding Rule in the table.
                        vector<pair<string, int> > rules = findRule(rule.F, in);
                        //If founded.
                        if(rules.size() > 0)
                        {
                                if(rules.size() == 1)
                                {
                                        //If the State is SYNCH.
                                        if(rules[0].S == 4)
                                        {
                                                continue;
                                        }
                                }
                                pushRules(rules);
                                ans += rule.F;
                                ans += " --> ";
                        }
                        //If doesn't found any States.
                        else
                        {
                                //Print Error.
                                ans = "Error:(illegal ";
                                ans += rule.F;
                                ans += ") -- discard ";
                                ans += in;
                                if(in == "$")
                                {
                                        ans += "\n";
                                        continue;
                                }
                                st.push(rule);
                                return ans;
                        }
                        //Update Answer.
                        for(int i = 0; i < (int) rules.size(); i++)
                        {
                                ans += " ";
                                ans += rules[i].F;
                        }
                }
                //If non Terminal State.
                else if(rule.S == 1)
                {
                        //If top of stack doesn't match the current token.
                        if(rule.F != in)
                        {
                                ans = "Error: missing ";
                                ans += rule.F;
                                ans += ", inserted\n";
                                if(rule.F == "$")
                                {
                                        st.push(rule);
                                        return ans;
                                }

                        }
                        else
                        {
                                //If the end of input.
                                if(in == "$")
                                {
                                        ans += "\nAccepted\n";
                                }
                                return ans;
                        }
                }
                ans += "\n";
        }
        st.push(mp(root, 0));
        return "";
}
void Match :: readAndMatch(string path)
{
        //Input File.
        ifstream file(path.c_str());
        int line = 0;
        if(file.is_open())
        {
                string in;
                vector<string> out;
                //Scan Line.
                while(getline(file, in))
                {
                        line++;
                        //Find Matching result.
                        string res = matcher(in);
                        //Output string.
                        string outer = intToString(line);
                        outer += " - ";
                        outer += in;
                        outer += "\n";
                        outer += res;
                        outer += "\n\n";
                        out.pb(outer);
                }
                //Match the $ to print the left in the stack.
                out.pb(matcher("$"));
                print(out);
                printFile(out);
        }
        else
        {
                cout << "File doesn't exist!\n";
        }
}
