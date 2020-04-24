#include "contloc.h"


using namespace std;

Contloc::Contloc() {
    this->contloc = "";
    this->codOperacion = "";
    this->instruc = new Instruccion();
}

string Contloc::getContloc() {
    return this->contloc;
}

void Contloc::setContloc(std::string contloc) {
    this->contloc = contloc;
}

string Contloc::getCodOperacion() {
    return this->codOperacion;
}

void Contloc::setCodOperacion(std::string codOperacion) {
    this->codOperacion = codOperacion;
}

Instruccion& Contloc::getInstruc() {
    return *instruc;
}

void Contloc::setInstruc(Instruccion* instruc) {
    this->instruc = instruc;
}

bool Contloc::validarDirectiva(std::string directiva, std::string* directivas)
{
    for(int i = 0; i < 20; i++) {
        if(directiva == directivas[i]) {
            return true;
        }
    }
    return false;
}


ostream& operator << (std::ostream& out, const Contloc& contloc){

}

istream& operator >> (std::istream& is, Contloc& contloc){

}

Contloc::~Contloc()
{
    //dtor
}
