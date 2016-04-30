#ifndef NFA2DFA_H
#define NFA2DFA_H

#include <bits/stdc++.h>
#include "InputTokenizer.h"
#include "NFABuilder.h"

#define pb push_back
#define mp make_pair
#define f first
#define s second

#define MAX_STATE 1000
#define MAX_INPUT 250

using namespace std;
//representation of NFA state
class NFA_state
{
    public:
        int transitions[MAX_INPUT][MAX_STATE];
        NFA_state()
        {
            for(int i=0;i<MAX_INPUT;i++)
                for(int j=0;j<MAX_STATE;j++)
                    transitions[i][j]=-1;
        }
}*NFA_states;
// representation of DFA state
struct DFA_state
{
    bool finalState;
    string finalDFA_name;
    bitset<MAX_STATE>NFA2DFA_states; // NFA states that is composed in one DFA state
    bitset<MAX_STATE>transitions[MAX_INPUT]; // transition NFA states of this DFA state on input i=0,1,2, ... MAX_INPUT
    int Transitions[MAX_INPUT];// (DFA) transition from current DFA state to -> Transitions[i] on input i where i=0,1,2, ... MAX_INPUT
    int prio;
};

set<int>NFA_finalStates;
vector<int> DFA_finalStates;
vector<DFA_state*> DFA_states;
stack<int> unmarked_DFA_states;
std::map<int,std::string> finalNodesA;
std::map<std::string,int> priorities;

//set of NFA states reachable from NFA state "state" on epsilon transitions alone
//or closure(s) and save it in "closure"
void epsilonClosure(int state,bitset<MAX_STATE> & closure);

//set of NFA states reachable from some NFA state "state" in "T"
// on epsilon transitions alone
void epsilonClosure(bitset<MAX_STATE> T,bitset<MAX_STATE> & closure);

// returns a bitset representing the set of states the NFA could be in after moving
// from state "from" on input symbol "input"
void NFA_move(int from,int input,bitset<MAX_STATE> &b);

//set of NFA states to which there is a transition on input symbol "input" from some NFA state T[i] in T
void NFA_move(bitset<MAX_STATE> T,int input,bitset<MAX_STATE> &b);

//method that converts NFA to DFA
void NFA2DFA();

//method that minimize DFA converted to minimized DFA states
void minimize();

#endif // OUTPUTPARSER_H
