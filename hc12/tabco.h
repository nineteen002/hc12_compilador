#ifndef TABCO_H
#define TABCO_H

#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

class Tabco
{
    public:
        Tabco();

        std::string getMnemonico();
        void setMnemonico(std::string);

        std::string getModoDirec();
        void setModoDirecc(std::string);

        int getlongInstr();
        void setLongInstr(int);

        std::string getCodOperacion();
        void setCodOperacion(std::string);

        int getLengthCop();
        void setLengthCop(int);

        Tabco& operator = (const Tabco& t);
        friend std::ostream& operator << (std::ostream& out, const Tabco& table);
        friend std::istream& operator >> (std::istream& is, Tabco& table);

        virtual ~Tabco();

    private:

        std::string mnemonico;
        std::string modoDirec;
        int longInstr;
        int lengthCop;
        std::string codOperacion;
};

#endif // TABCO_H
