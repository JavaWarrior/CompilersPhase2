#ifndef ERRORS_H
#define ERRORS_H
#include <ostream>
/* errors */
        typedef enum {
            EXPECTED_STRING_BEFORE_EQUAL,
            EXPECTED_STRING_BEFORE_OR,
            EXTRA_STRING_BEFORE_OR,
            MISSING_OPERATOR
            } erroEnum;

class Errors
{
    public:

        Errors();
        virtual ~Errors() {}
        static void printError(erroEnum errorNum, int lineNum, ostream & str)
        {
            str<<" ** error: line: "<<lineNum<<" :";
            switch(errorNum)
            {
            case EXPECTED_STRING_BEFORE_EQUAL:
                str<<"expected string before '::=' "<<endl;
                break;
            case EXPECTED_STRING_BEFORE_OR:
                str<<"expected string before '|' "<<endl;
                break;
            case EXTRA_STRING_BEFORE_OR:
                str<<"extra string before | "<<endl;
                break;
            case MISSING_OPERATOR:
                str<<"missing operator " <<endl;
                break;
            }
        }
    protected:

    private:
};

#endif // ERRORS_H
