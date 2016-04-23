#ifndef POOL_H
#define POOL_H
#include <bits/stdc++.h>
#include "RULE.h"

using namespace std;

class Pool
{
        public:
                Pool(map<string, Rule*> Rules);

        private:
                map<string, Rule*> rules;
};
Pool :: Pool(map<string, Rule*> Rules)
{
        rules = Rules;
}

#endif // POOL_H
