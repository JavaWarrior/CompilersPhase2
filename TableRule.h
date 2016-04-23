#ifndef TABLERULE_H
#define TABLERULE_H

#include "Rule.h"
#include <map>

class TableRule : public Rule
{
    public:
        map<Rule *, TableRule *> tableRow;

        TableRule(Rule * r) : Rule(r->nodeType,r->name){}

        TableRule * getProduction(Rule * r)
        {
            if(tableRow.find(r) != tableRow.end())
                return tableRow[r];
            else
                return NULL;
        }
    protected:

    private:

};

#endif // TABLERULE_H
