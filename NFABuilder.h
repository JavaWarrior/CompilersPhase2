#ifndef NFABUILDER_H
#define NFABUILDER_H

/*
NFA Graph Builder
this code takes a RE from the input tokenizer and then returns a NFA graph
you can get the resulting graph by simply calling buildFinal();
for more read the main.cpp file.
*/

#include "InputTokenizer.h"     /* the main header file */
#include <vector>
#include <set>
#include <stack>


#define DEBUG_MODE
typedef std::vector<std::vector< std::pair <int, char> > > aGraph;
/* typedef for integration between NFA part and DFA part */

#define EPSILON 0
/*epsilon character to be used in graph representation */


class Node{
    /*
    the smallest unit of a graph a Node that keeps
    track of the next Nodes.
    */
public:
    void addEdge(Node * n1, char inp);
    bool isFinal = false;                           /* is this Node final ? */
    std::vector <std::pair<Node *,char> > edges;    /* connected Nodes */
};


struct graph {
    /* a graph is characterized by its starting and ending points*/
    Node * start, * end;
};

class NFABuilder
{
    public:
        NFABuilder(InputTokenizer& inTok);
        virtual ~NFABuilder();

        aGraph * buildFinal();
        void test();
        graph * pntrGraph;

        std::map<Node *,std::string> finalNodes;    /* what does every final Node define ? */
        std::map<int,std::string> finalNodesA;      /* the same as finalNodes but is accessed using Nodes numbering */
        std::map<std::string,int> priorities;       /* priority of every identifier */

        void printGraph(graph * g);
        void printAGraph(aGraph * g);


    protected:

    private:

        std::string delms = "|\\()*+";              /* language delimiters */

        InputTokenizer inTok;
        graph * kleenClosure(graph * g);
        graph * kleenPlus(graph * g);
        graph * concat(graph * g1,graph * g2);
        graph * alter(graph * g1,graph * g2);
        graph * alter2(graph * g1,graph * g2);
        graph * construct(char a);

        graph * toPostfix(std::string lhs, std::string x);
        graph * doOperation(char op, std::stack <graph *> &expr);
        graph * connect(graph* g1, graph* g2);
        graph * connect2(graph * g1);

        graph * build();
        aGraph * adapt(graph * g1);
        void buildPriority();
};

#endif // NFABUILDER_H
