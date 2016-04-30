#ifndef DFABUILDER_H
#define DFABUILDER_H

#include "NFABuilder.h"

typedef std::vector<Node *> nodeSet;
class DFABuilder
{
    public:
        DFABuilder(NFABuilder * NFA1);
        virtual ~DFABuilder();
        graph * build();
    protected:

    private:
        NFABuilder * NFA;
        nodeSet * e_closure(nodeSet * n);
        nodeSet * getNext(nodeSet * n, char inp);
};

#endif // DFABUILDER_H
