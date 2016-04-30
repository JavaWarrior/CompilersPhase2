#ifndef INPUTTOKENIZER_H
#define INPUTTOKENIZER_H

#include <map>
#include <vector>
#include <string>

class InputTokenizer
{
    public:
        InputTokenizer(void);
        virtual ~InputTokenizer();
        void parseFile(char * filePath);
        void printContents(void);

        std::map <std::string,std::string> regDefn;
        std::map <std::string,std::string> tokens;
        std::vector <std::string> kwds;
        std::vector <std::string> pncs;
        std::vector <std::string> tokens_ordered;
    protected:

    private:


        void parseLine(std::string line);
        void clrSpcs(std::string &line);

        void parseExp(std::string line, int pos);
        void parseDef(std::string line, int pos);
        void parsePunc(std::string line);
        void parseKwd(std::string line);

        std::string expand(std::string  str);

        std::string noSpaces(std::string str);

        std::string substitue(std::string str);
        std::string addSpaces(std::string str);

};

#endif // INPUTTOKENIZER_H
