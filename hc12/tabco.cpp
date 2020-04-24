#include "Tabco.h"

using namespace std;

Tabco::Tabco() {
    this->mnemonico = "";
    this->longInstr = 0;
    this->modoDirec = "";
    this->codOperacion = "";
}

string Tabco::getMnemonico() {
    return this->mnemonico;
}

void Tabco::setMnemonico(string mnemonico) {
    this->mnemonico = mnemonico;
}

string Tabco::getModoDirec() {
    return this->modoDirec;
}

void Tabco::setModoDirecc(std::string modoDirec) {
    this->modoDirec = modoDirec;
}

int Tabco::getlongInstr() {
    return this->longInstr;
}

void Tabco::setLongInstr(int longInstr) {
    this->longInstr = longInstr;
}

int Tabco::getLengthCop() {
    return this->lengthCop;
}

void Tabco::setLengthCop(int lengthCop) {
    this->lengthCop = lengthCop;
}

string Tabco::getCodOperacion() {
    return this->codOperacion;
}

void Tabco::setCodOperacion(std::string codOperacion) {
    this->codOperacion = codOperacion;
}

Tabco& Tabco::operator=(const Tabco& t) {
    this->mnemonico = t.mnemonico;
    this->modoDirec = t.modoDirec;
    this->codOperacion = t.codOperacion;
    this->longInstr = t.longInstr;

    return *this;
}

ostream& operator << (ostream& out, const Tabco& table) {
    out << table.mnemonico << '|';
    out << table.modoDirec << '|';
    out << table.longInstr << '|';
    out << table.codOperacion << '|';
    out << table.lengthCop << endl;
    return out;
}

istream& operator>> (istream& is, Tabco& table) {
    string va;
    int v;

    getline(is, va, '|');
    table.setMnemonico(va);

    getline(is, va, '|');
    table.setModoDirecc(va);

    getline(is, va, '|');
    v = atoi(va.c_str());
    table.setLongInstr(v);

    getline(is, va, '|');
    table.setCodOperacion(va);

    getline(is, va);
    v = atoi(va.c_str());
    table.setLengthCop(v);

    return is;
}

Tabco::~Tabco() {
    //dtor
}
