#include "NFABuilder.h"
#include <stack>
#include <iostream>
using namespace std;
NFABuilder::NFABuilder(InputTokenizer& inTok1)
{
    inTok = inTok1;                             /*save input tokenizer*/
}

NFABuilder::~NFABuilder()
{
    //dtor
}

/* RE to NFA building */

/*
 * a function that returns the * closure for a graph.
 * returns :   the kleene Closure of a graph g
 * params  :   the graph we want to make kleene closure for
 */
graph * NFABuilder::kleenClosure(graph * g)
{
    /* the kleenClosure of a graph will look like this
    * n1--e-->g->......g--e-->n2
    * |       |<--e---|      |
    * |------->---e--->------|
    */

    Node * n1 = new Node,* n2 = new Node;
    n1->addEdge(g->start,EPSILON);
    n1->addEdge(n2,EPSILON);
    g->end->addEdge(g->start,EPSILON);
    g->end->addEdge(n2,EPSILON);
    g->start = n1;g->end = n2;
    return g;
}

/* a function that returns the kleen plus closure of a graph g */
graph * NFABuilder::kleenPlus(graph * g)
{
    /* the plus should look like this
    *        g->......g
    *        |<--e---|
    */
    g->end->addEdge(g->start,EPSILON);
    return g;
}

/* return alternation between two graphs */
graph * NFABuilder::alter(graph * g1, graph * g2)
{
    /*  the alternated sequence should look like this
        n1--e-->g1......g1--e-->n2
        |---e-->g2......g2--e->|
    */
    Node * nStart = new Node,* nEnd = new Node;
    nStart->addEdge(g1->start,EPSILON);
    nStart->addEdge(g2->start,EPSILON);
    g1->end->addEdge(nEnd,EPSILON);
    g2->end->addEdge(nEnd,EPSILON);

    graph * gNew = new graph;
    gNew->start = nStart;gNew->end = nEnd;
    return gNew;
}

/* unused alternation for the sake of test and optimization*/
graph * NFABuilder::alter2(graph * g1, graph * g2)
{
    /*
    g1.............g1
    |-->g2.....g2->|
    */
    g1->start->addEdge(g2->start,EPSILON);
    g2->end->addEdge(g1->end,EPSILON);
    return g1;
}

/* concatenation between two graphs */
graph * NFABuilder::concat(graph * g1, graph * g2)
{
    /*  connect the first of the second graph to the end
        of the first graph
    */
    g1->end->addEdge(g2->start,EPSILON);
    g1->end = g2->end;
    return g1;
}

/*
    construct a graph that matches one character 'a'
    this is the minimal building unit of a NFA graph.
*/
graph * NFABuilder::construct(char a)
{
    /*
        n1----a---->n2
    */
    graph * g1 = new graph;                         /* initialize the graph and the two nodes*/
    Node * n1 = new Node,* n2 = new Node;

    n1->addEdge(n2,a);  /*build the graph*/
    g1->start = n1;
    g1->end = n2;

    return g1;
}
/*
    convert RE expression to a NFA graph using infix to prefix algorithm
    params: lhs: left hand side of the expression which defines what scanner will
    output when it matches this expression.
    str: the RE expression we want to build a NFA for.
 */
graph * NFABuilder::toPostfix(string lhs, string str)
{
#ifdef DEBUG_MODE
    cout<<lhs<<" : "<<str<<endl;    /* DEBUG */
#endif
    stack <char> ops;               /* operations stack */
    stack <graph *> expr;           /* current evaluated expression stack */
    int ind =0 ;                    /* current input string pointer */

    while(ind < str.size())         /* if there's more in the expression */
    {
        if(str[ind] == '|')         /* if this is an alternation operation */
        {
                                    /* do all the higher priority operations before inserting this operation */
                                    /* i.e. '*,+' */
            while(!ops.empty() && ops.top() != '|' && ops.top() != '(')
            {
                char op = ops.top();                /* get the current operation */
                expr.push(doOperation(op,expr));    /* execute it on the current graph */
                ops.pop();                          /* remove it from stack */
            }
            ops.push('|');                          /* finally we push '|' to the stack */
        }
        else if(str[ind] == '*')
        {
            /* '*' is the highest priority so it's executed instantly */
            expr.push(doOperation('*',expr));
        }
        else if(str[ind] == '+')
        {
            expr.push(doOperation('+',expr));
        }
        else if(str[ind] == '(')
        {
            /* if we have a '(' that's not a matching character i.e. \( */
            if(ind > 0 && str[ind-1] != '|' && str[ind-1] != '(')
            {
                /* if the preceding character isn't '|' or '(' it's a concatenation operation */
                ops.push('c');
            }
            /* we push '(' to mark the parentheses starting */
            ops.push('(');
        }
        else if(str[ind] == ')')
        {
            /* if we reached end of ')' */
            while(ops.top() != '(')
            {
                /*
                    while there's operations after the occurrence of '('
                    do them because they are higher priorities now.
                */
                char op = ops.top();
                expr.push(doOperation(op,expr));
                ops.pop();
            }
            ops.pop(); //remove '('
        }
        else if(str[ind] == '\\')
        {
            /*lookahead character */
            graph * g1;
            if(str[ind+1] == 'L')
            {
                /* \L marks an epsilon */
                g1 = construct(EPSILON);
            }
            else
            {
                /* anything other than that continue the graph naturally */
                /* need check */
                g1 = construct(str[ind+1]);
            }

            expr.push(g1);
            /* add expression to the stack */
            if(ind>0)
                if(str[ind-1] != '|' && str[ind-1] != '(')
                {
                    ops.push('c');
                    /* check if it's concatenation */
                }
            /* advance pointer by one because we eaten extra character here */
            ind++;
        }
        else
        {
            /* normal character */
            graph * g1 = construct(str[ind]);
            /* construct a unit graph for this character */
            expr.push(g1);
            if(ind>0)
                if(str[ind-1] != '|' && str[ind-1] != '(')
                {
                    ops.push('c');
                }
        }
        ind++;
        /* advance string pointer */
    }

    while(!ops.empty())
    {
        /* when we reach end of stack we do our low priority operations */
        expr.push(doOperation(ops.top(),expr));
        ops.pop();
    }
    if(expr.size() != 1)
    {
        /* check if expression is valid */
        cout<<"error in building postfix: "<<lhs<<" : "<<str<<endl;
    }

    /* get the final expression */
    graph * g = expr.top();

    /* our constructing algorithm assures that the last element is always the
    only accepting node. can be proved by induction. */
    g->end->isFinal = true; //set last Node to be final
    finalNodes[g->end] = lhs;   /* save this node in finalNodes */
    return g;                   /* return the resulting graph */
}


/* do the operation on the stack of expressions */
graph * NFABuilder::doOperation(char op, stack <graph *> &expr)
{
    switch (op){
    case '*':
        {
            graph * temp = expr.top();expr.pop();
            return kleenClosure(temp);
        }
    case 'c':
        {
            graph * temp1 = expr.top();expr.pop();
            graph * temp2 = expr.top();expr.pop();
            return concat(temp2,temp1);
        }
    case '|':
        {
            graph * temp1 = expr.top();expr.pop();
            graph * temp2 = expr.top();expr.pop();
            return alter(temp2,temp1);
            return alter2(temp2,temp1);
        }
    case '+':
        {
            graph * temp = expr.top();expr.pop();
            return kleenPlus(temp);
        }
    default:
        cout << "ERROR" <<endl;
    }
    graph * notReached;
    return notReached;
}
/* end of RE to NFA */

/* building graph from multiple NFAs */

/* connect two NFAs graphs each matches a different string*/
graph * NFABuilder::connect(graph * g1,graph * g2)
{
    g1->start->addEdge(g2->start,EPSILON);          /* connect g1 with g2 from the first node using */
    return g1;                                      /* epsilon edge */
}

/*
    first NFA graph
    this function is different from the normal connect that it first creates a dummy starting node
    so that when normal connect is used the first node is used to connect multiple NFAs
*/

graph * NFABuilder::connect2(graph * g1)
{
    Node * n1 = new Node;
    n1->addEdge(g1->start,EPSILON);     /* add epsilon edge between the first dummy node and the starting of this graph */
    g1->start = n1;
    return g1;
}
/* end of connecting NFAs */

/* interfacing functions */


/* return a graph in vector form with priorities */
aGraph * NFABuilder::buildFinal()
{
    buildPriority();
    return adapt( pntrGraph = build());
}

/*
    this function adapts NFA graph from the pointers form to
    a numbered vector form (adjacency list)
    params: graph g1 the graph we wan't to adapt
*/
aGraph * NFABuilder::adapt(graph * g1)
{
    int ind = 0;                    /* current node index */
    map<Node *, int> numbering;     /* numbering of each node */
    aGraph * g = new aGraph;        /* create the adjacency list */
    Node * n1 = g1->start;          /* start with the first node (dummy node) */
    stack < Node *> stk;            /* stack for dfs */
    stk.push(n1);                   /* start algorithm with first node */
    set<int> marker;                /* check if the current node is marked */
    while(!stk.empty())
    {
        /* while there's other nodes */
        n1 = stk.top();stk.pop();
        if(numbering.find(n1) == numbering.end())
        {
            /* if node isn't numbered yet number it */
            numbering[n1] = ind++;
            if(n1->isFinal)
            {
                finalNodesA[numbering[n1]] = finalNodes[n1];
                /* if it's a final node add it to the final numbered nodes */
            }
            marker.insert(numbering[n1]);   /* mark this node */
        }
        /* if the list doesn't have current node */
        if(numbering[n1] + 1 > g->size())
            /* resize the list to contain the current node */
            g->resize(numbering[n1]+1);
        for(int i = 0 ; i < n1->edges.size() ; i++)
        {
            /* traverse connected nodes */
            Node * n2 = n1->edges[i].first;
            if(numbering.find(n2) == numbering.end())
            {
                /* if the node connected by this edge is not numbered yet
                    number it so we can put it in the adjacency list */
                numbering[n2] = ind++;
                if(n2->isFinal)
                {
                    finalNodesA[numbering[n2]] = finalNodes[n2];

                }
            }
            /* add this edge to the adjacency list */
            (*g)[numbering[n1]].push_back(make_pair(numbering[n2],n1->edges[i].second));
            if(marker.find(numbering[n2]) == marker.end())
            {
                /* if the current node isn't traversed yet mark it and push it to stack */
                stk.push(n2);
                marker.insert(numbering[n2]);   /* mark this node */
            }
        }
    }
#ifdef DEBUG_MODE
    cout<<"graph size: "<<ind<<endl;
    cout<<"final size: "<<finalNodesA.size()<<endl;
#endif // DEBUG_MODE
    return g;
}

/*
    a function that builds the priority map for each expression
    this is used in dfa build.
    priorities of keywords has the highest priorities by default and
    its inter-priorities are determined by the order they were submitted to
    the input file.
    priorities of other tokens is determined by their order in file.
*/
void NFABuilder::buildPriority()
{
    int ind= 0 ;
    for(int i = 0 ; i < inTok.kwds.size() ; i++)
    {
        priorities[inTok.kwds[i]] = ind++;
    }
    for(int i = 0 ; i < inTok.tokens_ordered.size() ; i++)
    {
        priorities[inTok.tokens_ordered[i]] = ind++;
    }
#ifdef DEBUG_MODE
    for( map<string,int>::iterator i =priorities.begin() ; i != priorities.end() ; i++)
    {
        cout<<i->first<< " : "<<i->second<<endl;
    }
#endif // DEBUG_MODE
}
/*  the core function for building the graph
    it returns the final NFA graph
*/
graph * NFABuilder::build()
{
    std::map <std::string,std::string>::iterator it = inTok.tokens.begin();
    /* let's start with the first toke in the tokens list */
    graph * g1 = connect2(toPostfix(it->first, it->second));
    /* make a core graph from this first token */
    for( ++it;it != inTok.tokens.end() ;it++)
    {
        /* connect next matching graph for this token with the current graph */
        g1 = connect(g1,toPostfix(it->first,it->second));
    }
    for(std::vector <std::string>::iterator it2 = inTok.pncs.begin() ;it2 != inTok.pncs.end() ;it2++)
    {
        /* connect next matching graph for this punctuation with the current graph */
        g1 = connect(g1,toPostfix(*it2,*it2));
    }
    for(std::vector <std::string>::iterator it2 = inTok.kwds.begin() ;it2 != inTok.kwds.end() ;it2++)
    {
        /* connect next matching graph for this keyword with the current graph */
        g1 = connect(g1,toPostfix(*it2,*it2));
    }
    return g1;
}

/* end of interfacing */

/* node functions */
void Node::addEdge(Node * n1, char inp)
{
    edges.push_back(make_pair(n1,inp));
}
/* end of node functions */

/* test and debug funcs */
void NFABuilder::test()
{
//    string exp = "a+(d|c)*";
    string exp = "(a|b)((a)|(b)|(1))*";
    graph * g =  toPostfix("lol", exp);
//    printGraph(g);
    printAGraph(adapt(g));
}

void NFABuilder::printGraph(graph * g)
{
    Node * n1 = g->start;
    stack<Node *> stk1,stk2;
    stk1.push(n1);
    int ind = 0;
    cout<<ind++<<endl;
    while(!stk1.empty() || !stk2.empty())
    {
        while(!stk1.empty())
        {
            n1 = stk1.top();stk1.pop();
            for(int i =0 ; i < n1->edges.size() ; i++)
            {
                pair<Node *,char> edge = n1->edges[i];
                stk2.push(edge.first);
                cout<<ind++<<" : "<<edge.second<<"\t";
            }
        }
        cout <<endl;
        while(!stk2.empty())
        {
            n1 = stk2.top();stk2.pop();
            for(int i =0 ; i < n1->edges.size() ; i++)
            {
                pair<Node *,char> edge = n1->edges[i];
                stk1.push(edge.first);
                cout<<ind++<<" : "<<edge.second<<"\t";
            }
        }
        cout<<endl;
    }
}

void NFABuilder::printAGraph(aGraph * g)
{
    for(int i =0 ; i < g->size() ; i++)
    {
        cout << "node "<< i<<endl;
        for(int j =0 ; j < (*g)[i].size() ; j++)
        {
            if((*g)[i][j].second == 0)
                cout<<"\tto "<<(*g)[i][j].first<<" inp:"<<"eps"<<endl;
            else
                cout<<"\tto "<<(*g)[i][j].first<<" inp:"<<(*g)[i][j].second<<endl;
        }
        cout<<endl;
    }
}
/* end of test and debug */



