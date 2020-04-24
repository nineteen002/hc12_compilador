#include "instruccion.h"

using namespace std;

Instruccion::Instruccion() {
    etiqueta = "";
    mnemonico = "";
    comentario = "";
    operandos = "";
    numeroOperandos = 0;
}

Instruccion::Instruccion(std::string etiqueta, std::string mnemonico, int numeroOperandos, std::string comentario) {
    this->etiqueta = etiqueta;
    this->mnemonico = mnemonico;
    this->numeroOperandos = numeroOperandos;
    this->comentario = comentario;
}

string Instruccion::getEtiqueta() {
    return etiqueta;
}

void Instruccion::setEtiqueta(std::string etiqueta) {
    this->etiqueta = etiqueta;
}

string Instruccion::getMnemonico() {
    return this->mnemonico;
}

void Instruccion::setMnemonico(std::string mnemonico) {
    this->mnemonico = mnemonico;
}

string Instruccion::getComentario() {
    return this->comentario;
}

void Instruccion::setComentario(std::string comentario) {
    this->comentario = comentario;
}

int Instruccion::getNumeroOperandos() {
    return this->numeroOperandos;
}

void Instruccion::setNumeroOperandos(int numeroOperandos) {
    this->numeroOperandos = numeroOperandos;
}

string Instruccion::getOperandos() {
    return this->operandos;
}

void Instruccion::setOperandos(std::string operandos) {
    this->operandos = operandos;
}

bool Instruccion::hasEtiqueta() {
    if(this->etiqueta == "") {
        return false;
    }
    return true;
}

bool Instruccion::validateMnemonico(std::string mnemonico, string* mnemonicos) {
    for(int i = 0; i < 25; i++) {
        if(mnemonico == mnemonicos[i]) {
            return true;
        }
    }
    return false;
}

int Instruccion::retrieveOperand(int indexOp) {
    int i = 0, index = 0, res;
    string op =  getOperandos();
    string op1;

    if(string::npos != op.find(',')) {
        while(i < strlen(operandos.c_str())) {
            if(operandos[i] == ',') {
                op1 = op.substr(i+1,strlen(operandos.c_str()));
                index++;
            }

            if(indexOp == index){
                if(numeroOperandos == index + 1){
                    break;
                }
                else if(index == 0){
                    op1 = op.substr(0, op.find(','));
                }
                else{
                    op1 = op1.substr(0, op.find(','));
                }
                break;
            }
            i++;
        }
    }else{
        op1 = op;
    }

    //CONVERT IT TO DEC
    if(op1[0] == '['){ //IF IT HAS A [
        op1 = op1.substr(1,strlen(op1.c_str()));
    }
    if(op1[0] == '\''){ //IF ITS A LETTER
        return (int) op1[1];
    }
    else if(op1[0] == '$'){ //HEX TO DEC
        op1.erase(0, op1.find('$')+ 1);
        res = std::stoi(op1, 0,16);
    }
    else if(op1[0] == '@') { //OCT TO DEC
        op1.erase(0, op1.find('@')+ 1);
        res = stoi(op1, 0,8);
    }
    else if(op1[0] == '%') { //BIN TO DEC
        op1.erase(0, op1.find('%')+ 1);
        res = stoi(op1, 0,2);
        }
    else{
        res = atoi(op1.c_str());
    }
    return res;
}

string Instruccion::retrieveStringOperand(int indexOp) {
    int i = 0, index = 0, res;
    string op =  getOperandos();
    string op1;

    if(string::npos != op.find(',')) {
        while(i < strlen(operandos.c_str())) {
            if(operandos[i] == ',') {
                if(indexOp == index && indexOp == 0){
                    return "";
                }
                op1 = op.substr(i+1,strlen(operandos.c_str()));
                index++;
            }

            if(indexOp == index){
                if(numeroOperandos == index + 1){
                    break;
                }
                else if(index == 0){
                    op1 = op.substr(0, op.find(','));
                }
                else{
                    op1 = op1.substr(0, op.find(','));
                }
                break;
            }
            i++;
        }
    }else{
        op1 = op;
    }

    return op1;
}


Instruccion& Instruccion::operator=(const Instruccion& instruc) {
    this->etiqueta = instruc.etiqueta;
    this->mnemonico = instruc.mnemonico;
    this->operandos = instruc.operandos;
    this->comentario = instruc.comentario;
    this->numeroOperandos = instruc.numeroOperandos;

    return *this;
}

ostream& operator << (ostream& out, const Instruccion& instruc) {
    out << "Etiqueta: " << instruc.etiqueta << endl;
    out << "Mnemonico: " << instruc.mnemonico << endl;
    out << "Operandos: " << instruc.operandos << endl;
    out << "Comentario: " << instruc.comentario <<endl;
    return out;
}

istream& operator >> (istream& is, Instruccion& instruc) {
    int i, j = 0, num = 0;
    bool c = false;
    string label, line,mnemonico, operandos, comentario;

    getline(is,label, '\t');
    getline(is, line, '\n');

    //IF THERE IS A SPACE ANYWHERE IN THE LINE, THEN DIVIDE IT.
    if(string::npos != line.find(' ') || string::npos != line.find('\t')) {
        //THE MNEMONIC IS FROM THE START OF THE LINE UNTIL A SPACE
        mnemonico = line.substr(0, line.find(' '));
        string del = " ";
        line.erase(0, line.find(' ') + del.length());
        //THE REST OF THE LINE ASSIGN IT TO OPERANDOS
        operandos = line;
    } else {
        mnemonico = line;
    }

    //IF THERE IS A ; THEN FROM THERE ITS A COMMENT
    for(i = 0; i < strlen(operandos.c_str()); i++) {
        if(c) {
            comentario += operandos[i];
        }
        if(operandos[i] == ';') {
            c = true;
            j = i;
        }
    }

    // CALCULATE NUMNER OF OPERANDS
    if(strlen(operandos.c_str()) > 0) {
        num = 1;
        for(int i = 0; i < strlen(operandos.c_str()); i++) {
            if(operandos[i] == ',') {
                num++;
            }
        }
    }

    if(j!= 0){
        operandos = operandos.substr(0,j);
    }

    instruc.setEtiqueta(label);
    instruc.setMnemonico(mnemonico);
    instruc.setOperandos(operandos);
    instruc.setNumeroOperandos(num);
    instruc.setComentario(comentario);

    return is;
}

Instruccion::~Instruccion() {
    //dtor
}

