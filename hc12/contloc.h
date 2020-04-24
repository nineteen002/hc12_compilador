#ifndef CONTLOC_H
#define CONTLOC_H

#include "instruccion.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

class Contloc
{
    public:
        Contloc();

        std::string getContloc();
        void setContloc(std::string);

        std::string getCodOperacion();
        void setCodOperacion(std::string);

        Instruccion& getInstruc();
        void setInstruc(Instruccion*);

        bool validarDirectiva(std::string, std::string *directivas);
        std::string calculatePC(std::string, int);

        friend std::ostream& operator << (std::ostream& out, const Contloc& contloc);
        friend std::istream& operator >> (std::istream& is, Contloc& contloc);

        virtual ~Contloc();

    protected:

    private:
        std::string contloc;
        std::string codOperacion;
        Instruccion* instruc;
};

#endif // CONTLOC_H
