#ifndef ERRORS_H
#define ERRORS_H
#include <ostream>
/* errors */
        typedef enum {
            EXPECTED_STRING_BEFORE_EQUAL,
            EXPECTED_STRING_BEFORE_OR,
            EXTRA_STRING_BEFORE_OR,
            MISSING_OPERATOR,
            WRONG_GRAMMAR_ALL_LR
            } erroEnum;

class Errors
{
    public:

        Errors();
        virtual ~Errors() {}
        static void printErrorWithLine(erroEnum errorNum, int lineNum, ostream & str)
        {
            str<<" ** error: line: "<<lineNum<<" :";
            ErrorCodeToStr(errorNum,str);
        }
        static void printErrorWithString(erroEnum errorNum, string arr, ostream & str)
        {
            str<<" ** error: in: "<<arr<<" :";
            ErrorCodeToStr(errorNum,str);
        }

    protected:

    private:
        static void ErrorCodeToStr(erroEnum errorNum,ostream & str)
        {
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
            case WRONG_GRAMMAR_ALL_LR:
                str<<"wrong grammar, all productions is left recursive."<<endl;
                break;
            }
        }
};

#endif // ERRORS_H
