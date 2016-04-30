#include "DFABuilder.h"

DFABuilder::DFABuilder(NFABuilder * NFA1)
{
    NFA = NFA1;
}

DFABuilder::~DFABuilder()
{
    //dtor
}
graph * DFABuilder::build()
{

}

nodeSet * DFABuilder::e_closure(nodeSet * n)
{
    return getNext(n, 0);
}

nodeSet * DFABuilder::getNext(nodeSet * n, char inp)
{

}
