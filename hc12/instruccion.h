#ifndef INSTRUCCION_H
#define INSTRUCCION_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

class Instruccion {

  public:
    Instruccion();
    Instruccion(std::string, std::string, int, std::string);

    std::string getEtiqueta();
    void setEtiqueta(std::string);

    std::string getMnemonico();
    void setMnemonico(std::string);

    std::string getComentario();
    void setComentario(std::string);

    std::string getOperandos();
    void setOperandos(std::string);

    int getNumeroOperandos();
    void setNumeroOperandos(int);
    void calculateOperands();
    int numBytes();

    int retrieveOperand(int);
    std::string retrieveStringOperand(int);

    bool hasEtiqueta();
    bool validateMnemonico(std::string, std::string *mnemonicos);

    Instruccion& operator = (const Instruccion& instruc);
    friend std::ostream& operator << (std::ostream& out, const Instruccion& instruc);
    friend std::istream& operator >> (std::istream& is, Instruccion& instruc);

    virtual ~Instruccion();

  private:
    std::string etiqueta;
    std::string mnemonico;
    std::string operandos;
    //list::<std::string> operandos;
    int numeroOperandos;
    std::string comentario;
};

#endif // INSTRUCCION_H
