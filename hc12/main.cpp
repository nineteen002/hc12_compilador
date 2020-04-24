#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <bits/stdc++.h>
#include <ctype.h>

#include "instruccion.h"
#include "tabco.h"
#include "contloc.h"

using namespace std;
//GLOBAL VARIABLES

string tablaMnemonicos[25] = {"ABA", "ABX", "ABY", "ADCA", "ADCB","ADDA", "ADDB", "ADDD", "ANDA", "ANDB",
                              "ANDCC", "ASL", "ASLA", "ASLB", "ASLD", "ASR", "ASRA", "ASRB", "BCC", "BCLR",
                              "BNE", "LBNE", "IBNE", "JMP", "LDAA"
                             };

string tablaDirectivas[10] = {"ORG", "END", "START", "EQU", "DC.B", "BSZ", "FILL", "DC.W", "FCC", "FCB"};

//FUNCTIONS
void readP2();
void writeP3();
void writeP2(Instruccion& a1, Tabco*, int);
void writeTablaSimbolos(Instruccion& a1, int);
void writeTabSim();

// AUX FUNCTIONS
string getCOP(Instruccion &a1, Tabco*);
string relativeCOP(Instruccion &a1, Tabco*, int pc);
string indexedCOP(Instruccion &a1, Tabco*);
string formatCOP(string);
string getOffset(string, string);

Tabco* addressMode(Instruccion& instruc);
int modifyPC(Instruccion& a1, Tabco*, int);
bool operandSize_isValid(string, double, int);
string referenceTabSim(string);
bool checkETQ_OP(string op);

//CONVERT FUNCTIONS
string convertNumber(string, int);
int convertNumberDec(string, int);
string decToHex(int);
string decToHexSigned(int);

int main() {
    string tablaDirectivas[10] = {"ORG", "END", "START", "EQU", "DC.B", "BSZ", "FILL", "DC.W", "FCC", "FCB"};

    //EXECUTES TAB SIM BEFORE
    writeTabSim();

    //EXECUTES
    writeP3();

    //READS & WRITES FROM/TO P2
    //readP2();

    return 0;
}

//WRITE AND READ FUNCs
void writeP3() {
    Instruccion a1 = Instruccion();
    Instruccion a2 = Instruccion();
    Tabco* t1 = new Tabco();
    Contloc c1 = Contloc();

    int line = 0, pc = 0, temp_pc;
    bool validMnemonico, validDirectiva;
    string inFile = "P7.asm", outFile = "P7.lst", PC = "0", COP = "", ope;
    ifstream fin;
    ofstream fout;

    fout.open(outFile.c_str());

    if(!fout.is_open()) {
        cout << "Error opening file" << endl;
    } else {
        fin.open(inFile.c_str());
        //START READING THE P2 ARCHIVE
        if(!fin.is_open()) {
            cout << "Error opening file" << endl;
        } else {
            while(!fin.eof()) {
                fin >> a1;
                line++;
                COP = "";

                //VALIDATE INSTRUCTION
                validMnemonico = a1.validateMnemonico(a1.getMnemonico(), tablaMnemonicos);
                c1.setInstruc(&a1);

                //CHECKS OPERANDS
                if(validMnemonico) {
                    ope = referenceTabSim(a1.getOperandos()); //CHECKS
                    if(ope != "-1") {
                        if(ope[1] == 'x') {
                            ope.erase(0,2);
                        }
                        ope = '$' + ope;
                        a2 = a1;
                        a2.setOperandos(ope);
                        t1 = addressMode(a2);
                    } else {
                        t1 = addressMode(a1);
                    }
                } else {
                    //IF NOT A MNEMONIC VALIDATE IF ITS A DIRECTIVE
                    validDirectiva = c1.validarDirectiva(a1.getMnemonico(), tablaDirectivas);
                }
                //MAKES SURE PC IS NOT EMPTY
                if(PC == "") {
                    PC = "0";
                }

                //IF IT'S RELATIVE THEN GET ITS COP INDEPENDANTLY
                if(a1.getMnemonico() == "BNE" || a1.getMnemonico() == "LBNE" || a1.getMnemonico() == "IBNE") {
                    temp_pc = modifyPC(a1, t1, pc);
                    COP = relativeCOP(a1, t1, temp_pc);
                } else if(t1->getModoDirec() == "IDX" || t1->getModoDirec() == "IDX1" || t1->getModoDirec() == "IDX2" || t1->getModoDirec() == "[IDX2]" || t1->getModoDirec() == "[D,IDX]" ) {
                    COP = indexedCOP(a1,t1);
                } else {
                    COP = getCOP(a1, t1);
                }

                //WRITE TO FILE
                if(!validMnemonico && !validDirectiva) {
                    fout << "Mnemonico no valido, en linea " << line << endl;
                } else {
                    if(t1->getModoDirec() == "Invalid") {
                        fout << PC << "|ERROR|" << a1.getEtiqueta() << " " <<  a1.getMnemonico() << " " << a1.getOperandos() << endl;
                    } else if(validMnemonico) {
                        fout << PC << "|" << COP << "|" << a1.getEtiqueta() << " ";
                        fout << a1.getMnemonico() << " " << a1.getOperandos() << endl;
                    } else {
                        fout << PC << " " << getCOP(a1, t1) << "|" << a1.getEtiqueta() << " " <<  a1.getMnemonico() << " " << a1.getOperandos() << endl;
                    }
                }

                pc = modifyPC(a1, t1, pc);
                PC = decToHex(pc);
            }
        }
        fin.close();
    }
}

void readP2() {
    Instruccion a1 = Instruccion();
    Tabco* t1 = new Tabco();
    int line = 0;

    string inFile = "P2.asm";
    ifstream fin;

    //IF THE P2.LST EXISTS THEN REMOVE IT TO START CLEAN
    string outFile = "P2.lst";
    remove(outFile.c_str());

    fin.open(inFile.c_str());
    //START READING THE P2 ARCHIVE
    if(!fin.is_open()) {
        cout << "Error opening file" << endl;
    } else {
        while(!fin.eof()) {
            fin >> a1;
            line++;
            //WRITE TO P2.LST
            writeP2(a1, t1, line);
        }
    }
    fin.close();
}

void writeP2(Instruccion& a1, Tabco* t1, int line) {
    string outFile = "P2.lst";
    ofstream fout;
    bool validMnemonico;

    fout.open(outFile.c_str(), std::ios::app);

    if(!fout.is_open()) {
        cout << "Error opening file" << endl;
    } else {
        validMnemonico = a1.validateMnemonico(a1.getMnemonico(),tablaMnemonicos);

        //CHECKS OPERANDS
        if(validMnemonico) {
            // validOperand = addressingMode(a1);
            t1 = addressMode(a1);
        }
        //WRITE TO TABLA DE SIMBOLOS IF THERE IS AN ETIQUETA

        if(!validMnemonico) {
            fout << "Mnemonico no valido, en linea " << line << endl;
        } else {
            if(t1->getModoDirec() == "Invalid") {
                fout << "Operando(s) no valido, en linea " << line << endl;
            } else {
                fout << "Linea " << line << ", Modo de direccionamiento: " << t1->getModoDirec() << endl;
            }
        }
        fout << "___________________________________" << endl;
        fout << a1 << endl << endl;
    }
}

void writeTabSim() {
    Instruccion a1 = Instruccion();
    Tabco* t1 = new Tabco();
    Contloc c1 = Contloc();
    int line = 0, pc = 0;
    bool validMnemonico;
    string num, ope;

    string inFile = "P7.asm";
    ifstream fin;

    //IF THE P2.LST EXISTS THEN REMOVE IT TO START CLEAN
    string outFile = "TABSIM.txt";
    ofstream fout;
    remove(outFile.c_str());

    fin.open(inFile.c_str());
    fout.open(outFile.c_str());
    //START READING THE P2 ARCHIVE
    if(!fin.is_open() && !fout.is_open()) {
        cout << "Error opening file" << endl;
    } else {
        while(!fin.eof()) {
            fin >> a1;
            line++;

            //ADD TO TABSIM
            if(a1.hasEtiqueta()) {
                //IF ITS A MNEMONICO
                if(a1.validateMnemonico(a1.getMnemonico(), tablaMnemonicos)) {
                    t1 = addressMode(a1);
                    c1.setCodOperacion(t1->getCodOperacion());
                    fout << a1.getEtiqueta() << " -> " << decToHex(pc) << endl;
                }
                //IF ITS A DIRECTICA
                else if(a1.getMnemonico() == "EQU") {
                    num = decToHex(a1.retrieveOperand(0));
                    fout << a1.getEtiqueta() <<  " -> 0x0" << num << endl;
                } else {
                    fout << a1.getEtiqueta() << "-> " << decToHex(pc) << endl;
                }
            }

            //NEEDED TO UPDATE PC
            validMnemonico = a1.validateMnemonico(a1.getMnemonico(), tablaMnemonicos);
            if(validMnemonico) {
                ope = referenceTabSim(a1.getOperandos());
                if(ope != "-1") {
                    if(ope[1] == 'x') {
                        ope.erase(0,2);
                    }
                    ope = '$' + ope;
                    a1.setOperandos(ope);
                }
                t1 = addressMode(a1);
            }

            pc = modifyPC(a1, t1, pc);
        }
    }
    fin.close();
    fout.close();
}


//AUXILIAR FUNCS
string formatCOP(string cop) {
    string aux_cop = "";
    int j = 1;

    for(int i = 1; i <= strlen(cop.c_str()); i++) {
        if(j % 3 == 0 && i != 1) {
            if(cop[i-1] != ' ') {
                i--;
            }
            aux_cop += ' ';
            j++;
        } else if(cop[i-1] == ' ') {

        } else {
            aux_cop += cop[i-1];
            j++;
        }
    }
    return aux_cop;
}

string getCOP(Instruccion &a1, Tabco* t1) {
    Tabco t2 = Tabco();
    string cop = "", aux = "", op, c, aux_op;
    Contloc c1 = Contloc();
    int operando, cont, reps, n, bytesReserved, rr;
    string aux_cop;

    //IF ITS A MNEMONIC
    if(!c1.validarDirectiva(a1.getMnemonico(), tablaDirectivas)) {
        t2 = *t1;
        //CHECK IF THE OPERANDO IS A LABEL IN TABSIM
        aux_op = referenceTabSim(a1.getOperandos());
        //IF ITS NOT A LABEL THEN FOLLOW AS USUAL:
        if(aux_op != "-1") {
            if(aux_op[1] == 'x') {
                aux_op.erase(0,2);
            }
            operando = std::stoi(aux_op,0,16);
        } else {
            operando = convertNumberDec(a1.getOperandos(), a1.getNumeroOperandos());
        }

        //GET OPERANDO IN DECIMAL
        if(a1.getNumeroOperandos() > 0) {
            op = decToHex(operando); //CONVERT THE NUMBER TO HEX
            //FORMAT
            for(int i = op.length(); i < t1->getlongInstr() * 2; i++) {
                aux += "0";
                if(i % 2 == 0 && i != 0) {
                    aux += " ";
                }
            }
            aux += op;
        }

        // get base cop
        c = t1->getCodOperacion();
        n =  (t1->getLengthCop() - t1->getlongInstr()) * 2;
        cop = c.substr(0, n+1);
        cop += aux;

        //WRITE COP WITH SPACES
    } else {
        //FOR DC.B or FCC
        if((a1.getMnemonico() == "DC.B" || a1.getMnemonico() == "FCC") && a1.getNumeroOperandos() > 0) {
            for(int i = 0; i < a1.getNumeroOperandos(); i++) {
                n = a1.retrieveOperand(i);
                op = decToHex(n);

                if(op.length() < 2) {
                    cop += "0";
                }

                cop += op;
                cop += " ";
            }
        } else if(a1.getMnemonico() == "DC.B" || a1.getMnemonico() == "FCC") { // IF NO OPERANDS THEN
            cop = "00";
        }
        //FOR DC.W or FCC
        else if((a1.getMnemonico() == "DC.W" || a1.getMnemonico() == "FCC") && a1.getNumeroOperandos() > 0) {
            for(int i = 0; i < a1.getNumeroOperandos(); i++) {
                n = a1.retrieveOperand(i);
                op = decToHex(n);
                //ADD THEM 0s
                for(int k = op.length(); k < 4; k++) {
                    cop += "0";
                    if(k % 2 == 0) {
                        cop += " ";
                    }
                }

                cop += op;
                cop += " ";
            }
        } else if(a1.getMnemonico() == "DC.W" || a1.getMnemonico() == "FCC") {
            cop = "00 00";
        }
        //FOR BSZ or ZMB
        else if(a1.getMnemonico() == "BSZ" || a1.getMnemonico() == "ZMB") {
            cont = atoi(a1.getOperandos().c_str());
            for(int i = 0; i < cont; i++) {
                cop += "00 ";
            }
        }
        //FOR FILL
        else if(a1.getMnemonico() == "FILL") {
            bytesReserved = atoi(a1.getOperandos().c_str());
            reps = a1.retrieveOperand(1);
            n = a1.retrieveOperand(0);
            op = decToHex(n);

            for(int i = 0; i < reps; i++) {
                if(op.length() < 2) {
                    cop += "0";
                }
                cop += op;
                cop += " ";
            }
        }
    }
    //FORMAT
    cop = formatCOP(cop);

    return cop;
}

string relativeCOP(Instruccion &a1, Tabco* t1, int pc) {
    int op, next_cop, rr, n;
    string hex, cop_aux, ope, cop, aux, aux_cop, c = "", count_reg, offset;
    Instruccion a2 = Instruccion();
    a2 = a1;
    if(a1.getMnemonico() == "BNE") {
        ope = a1.getOperandos();
        ope = referenceTabSim(a1.getOperandos()); //IF IT HAS A LABEL AS OPERANDO RETRIEVE THE VALUE
        next_cop = pc;

        if(ope == "-1") {
            op = a1.retrieveOperand(0);
            aux = hex = decToHexSigned(op - next_cop);
        } else {
            if(ope[1] == 'x') {
                ope.erase(0,2);
            }
            rr = std::stoi(ope,0,16);
            aux = hex = decToHexSigned(rr - next_cop);
        }

        for(int i = 0; i < strlen(aux.c_str())- 2; i++) {
            if(aux[i] == 'f' || aux[i] == '0') {

            } else {
                return "Fuera de rango";
            }
        }

        aux.erase(0, strlen(aux.c_str())-2);
        rr =  std::stoi(aux, 0,16);


        if(hex[0] == 'f' && (rr >= 128 && rr <= 255)) {
            //return aux;
        } else if (hex[0] == '0' && (rr >= 0 && rr <= 127)) {
            //return aux;
        } else {
            hex = "Fuera de rango";
            return hex;
        }
    } else if(a1.getMnemonico() == "LBNE") {
        ope = a1.getOperandos();
        ope = referenceTabSim(a1.getOperandos());//IF IT HAS A LABEL AS OPERANDO RETRIEVE THE VALUE
        next_cop = pc;

        if(ope == "-1") {
            op = a1.retrieveOperand(0);
            aux = hex = decToHexSigned(op - next_cop);
        } else {
            if(ope[1] == 'x') {
                ope.erase(0,2);
            }

            rr = std::stoi(ope,0,16);
            aux = hex = decToHexSigned(rr - next_cop);
        }

        if(strlen(aux.c_str()) >= 4) {
            for(int i = 0; i < strlen(aux.c_str())- 4; i++) {
                if(aux[i] == 'f' || aux[i] == '0') {

                } else {
                    return "Fuera de rango";
                }
            }
            aux.erase(0, strlen(aux.c_str())-4);
        }
        rr =  std::stoi(aux, 0,16);

        if(hex[0] == 'f' && (rr >= 32768 && rr <= 65535)) {
            //return aux;
        } else if (hex[0] == '0' && (rr >= 0 && rr <= 32767)) {
            //return aux;
        } else {
            hex = "Fuera de rango";
            return hex;
        }
    } else if(a1.getMnemonico() == "IBNE") {
        ope = a2.retrieveStringOperand(1);
        ope = referenceTabSim(ope); //IF IT HAS A LABEL AS OPERANDO RETRIEVE THE VALUE
        next_cop = pc;

        if(ope == "-1") {
            op = a1.retrieveOperand(1);
            aux = hex = decToHexSigned(op - next_cop);
        } else {
            if(ope[1] == 'x') {
                ope.erase(0,2);
            }
            rr = std::stoi(ope,0,16);
            aux = hex = decToHexSigned(rr - next_cop);
        }

        //CHECK IF ITS NEGATIVE OR POSITIVE -> THEN CALCULATE COP
        count_reg = a1.retrieveStringOperand(0);
        offset = getOffset(aux, count_reg);

        //CHECK IF ITS WITHIN RANGE
        for(int i = 0; i < strlen(aux.c_str())- 2; i++) {
            if(aux[i] == 'f' || aux[i] == '0') {

            } else {
                return "Fuera de rango";
            }
        }

        aux.erase(0, strlen(aux.c_str())-2);
        rr =  std::stoi(aux, 0,16);

        //CHECK IF IT IS WITHIN RANGE
        if(rr >= 0 && rr <= 255) {
            //return aux;
        } else {
            hex = "Fuera de rango";
            return hex;
        }
    }

    for(int i = aux.length(); i < t1->getLengthCop() - t1->getlongInstr() * 2; i++) {
        aux += "0";
        if(i % 2 == 0 && i != 0) {
            aux += " ";
        }
    }

    c = t1->getCodOperacion();
    n = (t1->getLengthCop() - t1->getlongInstr()) * 2;
    cop = c.substr(0, n+1);
    if(a1.getMnemonico() == "IBNE") {
        cop += offset;
    }
    cop += aux;

    //FORMAT
    return formatCOP(cop);
}

string indexedCOP(Instruccion &a1, Tabco* t1 ) {
    string rr, nn, ope, reg, binCOP, cop, c, aux, bin_xb, z, s;
    int op, n, bin;
    Tabco t2 = Tabco();

    //CALCULATE RR
    reg = a1.retrieveStringOperand(1);
    if(reg[strlen(reg.c_str())-1] == ']') {
        reg = reg.substr(0,strlen(reg.c_str())-1);
    }

    if(reg == "X" || reg == "x") {
        rr = "00";
    } else if(reg == "Y" || reg == "y") {
        rr = "01";
    } else if(reg == "SP" || reg == "sp") {
        rr = "10";
    } else if(reg == "PC" || reg == "pc") {
        rr = "11";
    } else {
        return "ERROR";
    }

    t2 = *t1;
    if(t1->getModoDirec() == "IDX") {
        ope = a1.retrieveStringOperand(0);

        if(ope == "") {
            nn = "00000";
        } else {
            op = a1.retrieveOperand(0);
            //GET NN
            nn = std::bitset<sizeof(op)*8>(op).to_string();
            nn.erase(0, strlen(nn.c_str())-5);
        }

        binCOP = "%" + rr + "0" + nn;
        op = convertNumberDec(binCOP, 1);
        aux = decToHexSigned(op);
        aux.erase(0, strlen(aux.c_str())-2);

        c = t1->getCodOperacion();
        n = (t1->getLengthCop() - t1->getlongInstr()) * 2; //SIZE OF COD OP
        cop = c.substr(0, n+1);
        cop += aux;
    }
    else if(t1->getModoDirec() == "IDX1") {
        z = "0";
        ope = a1.retrieveStringOperand(0);

        if(ope == "") {
            return "Error";
        } else {
            op = a1.retrieveOperand(0);
            if(op < 0) {
                s = "1";
            } else {
                s = "0";
            }
            //GET NN
            nn = std::bitset<sizeof(op)*8>(op).to_string();
            nn.erase(0, strlen(nn.c_str())-8);
            nn = "%" + nn;
        }

        bin_xb = "%111" + rr + "0" + z + s;
        //ztamaño
        bin = convertNumberDec(bin_xb,1);
        binCOP = decToHexSigned(bin);

        op = convertNumberDec(nn, 1);
        aux = decToHexSigned(op);

        aux.erase(0, strlen(aux.c_str())-2);
        binCOP.erase(0, strlen(binCOP.c_str())-2);

        c = t1->getCodOperacion();
        n = (t1->getLengthCop() - t1->getlongInstr()) * 2; //SIZE OF COD OP
        cop = c.substr(0, n+1);
        cop = cop + binCOP + " " + aux;
    }
    else if(t1->getModoDirec() == "IDX2") {
        z = "1";
        ope = a1.retrieveStringOperand(0);

        if(ope == "") {
            return "Error";
        } else {
            op = a1.retrieveOperand(0);
            if(op < 0) {
                s = "1";
            } else {
                s = "0";
            }
            //GET NN
            nn = decToHexSigned(op);
            nn.erase(0, strlen(nn.c_str())-4);
        }

        bin_xb = "%111" + rr + "0" + z + s;
        //ztamaño
        bin = convertNumberDec(bin_xb,1);
        binCOP = decToHexSigned(bin);

        binCOP.erase(0, strlen(binCOP.c_str())-2);

        c = t1->getCodOperacion();
        n = (t1->getLengthCop() - t1->getlongInstr()) * 2; //SIZE OF COD OP
        cop = c.substr(0, n+1);
        cop = cop + binCOP + " " + nn;
    }
    else if(t1->getModoDirec() == "[IDX2]") {
        bin_xb = "%111" + rr + "011";
        bin = convertNumberDec(bin_xb,1);
        binCOP = decToHexSigned(bin);
        binCOP.erase(0,strlen(binCOP.c_str())-2);

        op = a1.retrieveOperand(0);

        ope = decToHexSigned(op);
        ope.erase(0,strlen(ope.c_str())-4);

        c = t1->getCodOperacion();
        n = (t1->getLengthCop() - t1->getlongInstr()) * 2; //SIZE OF COD OP
        cop = c.substr(0, n+1);
        cop = cop + binCOP + " " + ope;
    }
    else if(t1->getModoDirec() == "[D,IDX]"){
        bin_xb = "%111" + rr + "111";
        bin = convertNumberDec(bin_xb,1);
        binCOP = decToHexSigned(bin);
        binCOP.erase(0,strlen(binCOP.c_str())-2);

        c = t1->getCodOperacion();
        n = (t1->getLengthCop() - t1->getlongInstr()) * 2; //SIZE OF COD OP
        cop = c.substr(0, n+1);
        cop = cop + binCOP;
    }
    return formatCOP(cop);
}

string getOffset(string aux, string count_reg) {
    string offset;

    if(aux[0] == 'f') {
        if(count_reg == "A") {
            offset = "B0";
        } else if(count_reg == "B") {
            offset = "B1";
        } else if(count_reg == "D") {
            offset = "B4";
        } else if(count_reg == "X") {
            offset = "B5";
        } else if(count_reg == "Y") {
            offset = "B6";
        } else if(count_reg == "SP") {
            offset = "B7";
        } else {
            return "Error de registro";
        }
    } else {
        if(count_reg == "A") {
            offset = "A0";
        } else if(count_reg == "B") {
            offset = "A1";
        } else if(count_reg == "D") {
            offset = "A4";
        } else if(count_reg == "X") {
            offset = "A5";
        } else if(count_reg == "Y") {
            offset = "A6";
        } else if(count_reg == "SP") {
            offset = "A7";
        } else {
            return "Error de registro";
        }
    }
    return offset;
}

int modifyPC(Instruccion& a1, Tabco* t1, int pc) {
    int num = 0xF, bytesReserved;
    int reps,n;
    std:: string cod = "", operandos = "";
    Contloc c1 = Contloc();
    c1.setInstruc(&a1);

    num = 0x0;
    operandos = t1->getCodOperacion();

    if(a1.getMnemonico() == "ORG" && a1.getNumeroOperandos() == 1) {
        num = convertNumberDec(a1.getOperandos(), a1.getNumeroOperandos());
        pc = num;

    }

    else if(a1.validateMnemonico(a1.getMnemonico(), tablaMnemonicos)) {
        pc = pc + t1->getLengthCop();
    }

    else if((a1.getMnemonico() == "DC.B" || a1.getMnemonico() == "FCC") && a1.getNumeroOperandos() > 0) {
        pc = pc + a1.getNumeroOperandos();
    } else if(a1.getMnemonico() == "DC.B" || a1.getMnemonico() == "FCC") {
        pc++;
    }

    else if((a1.getMnemonico() == "DC.W" || a1.getMnemonico() == "FCC") && a1.getNumeroOperandos() > 1) {
        pc = pc + (a1.getNumeroOperandos());
    } else if(a1.getMnemonico() == "DC.W" || a1.getMnemonico() == "FCC") {
        pc = pc + 2;
    }

    else if(a1.getMnemonico() == "BSZ" || a1.getMnemonico() == "ZMB") {
        bytesReserved = atoi(a1.getOperandos().c_str());
        pc = pc + bytesReserved;
    }

    else if(a1.getMnemonico() == "FILL") {
        bytesReserved = atoi(a1.getOperandos().c_str());
        reps = a1.retrieveOperand(1);
        n = a1.retrieveOperand(0);
        pc += reps;
    }

    return pc;
}

bool operandSize_isValid(string operandos, double numBytes, int numOperandos) {
    int number, limit;
    limit = pow(2.0, 8.0*numBytes);

    if(operandos[0] == '#') {
        operandos.erase(0, operandos.find('#')+ 1);
    }

    if(numOperandos == 1) {
        //HEXADECIMAL
        if(operandos[0] == '$') {
            operandos.erase(0, operandos.find('$')+ 1);
            number = std::stoi(operandos, 0,16);
            if(limit-1 >= number && limit*-1 <= number ) {
                return true;
            }
        }
        //OCTAL
        else if(operandos[0] == '@') {
            operandos.erase(0, operandos.find('@')+ 1);
            number = stoi(operandos, 0,8);
            if(limit-1 >= number && limit*-1 <= number ) {
                return true;
            }
        }
        //BINARIO
        else if(operandos[0] == '%') {
            operandos.erase(0, operandos.find('%')+ 1);
            number = stoi(operandos, 0,2);
            if(limit-1 >= number && limit*-1 <= number ) {
                return true;
            }
        }
        //DECIMAL
        else {
            number = stoi(operandos, 0, 10);
            if(limit-1 >= number && limit*-1 <= number ) {
                return true;
            }
        }
    }
    return false;
}

Tabco* addressMode(Instruccion& instruc) {
    string tabcoFile = "TABCOP.txt";
    ifstream fin;
    Tabco* t1 = new Tabco();
    Tabco t2;

    fin.open(tabcoFile.c_str());

    if(!fin.is_open()) {
        cout << "Error opening tabco file" << endl;
    } else {
        while(!fin.eof()) {
            fin >> *t1;
            t2 = *t1;

            if(t1->getMnemonico() == instruc.getMnemonico()) {
                while(t1->getMnemonico() == instruc.getMnemonico()) {

                    if(instruc.getNumeroOperandos() <= 1) {
                        //VALIDATE INH MODO DE DIRECCIONAMIENTO
                        if (t1->getlongInstr() == 0 && instruc.getNumeroOperandos() == 0) {
                            fin.close();
                            return t1;
                        }
                        //VALIDATE IMM MODO DE DIRECCIONAMIENTO
                        if(t1->getModoDirec() == "IMM") {
                            if(instruc.getOperandos()[0] == '#') {
                                if(t1->getModoDirec() == "IMM" && operandSize_isValid(instruc.getOperandos(),t1->getlongInstr(),instruc.getNumeroOperandos())) {
                                    fin.close();
                                    return t1;
                                }
                            }
                        }
                        //VALIDATE REL MODO DE DIRECCIONAMIENTO
                        if(t1->getModoDirec() == "REL") {
                            fin.close();
                            return t1;
                        }
                        //VALIDATES IF OPERANDO IS ETIQUETA
                        if(checkETQ_OP(instruc.getOperandos())) {
                            return t1;
                        }
                        //ELSE
                        if (operandSize_isValid(instruc.getOperandos(),t1->getlongInstr(),instruc.getNumeroOperandos())) {
                            fin.close();
                            return t1;
                        }
                    } else {
                        double numBytes;
                        if(t1->getModoDirec() == "REL" && instruc.getMnemonico() == "IBNE") {
                            fin.close();
                            return t1;
                        }
                        string ope = instruc.getOperandos();
                        int lengthOp = strlen(ope.c_str());
                        //[IDX]
                        if(ope[0] == '[' && ope[lengthOp-1] == ']') {
                            int op = instruc.retrieveOperand(0);
                            string opi = instruc.retrieveStringOperand(0);

                            if(instruc.getNumeroOperandos() == 2 && t1->getModoDirec() == "[D,IDX]" && opi[1] == 'D') {
                                return t1;
                            }
                            if(instruc.getNumeroOperandos() == 2 && t1->getModoDirec() == "[IDX2]") {
                                if(op >= 0 && op <= 65535) {
                                    return t1;
                                }
                            }
                        }//OTHER IDX WITH NOT []
                        else {
                            //-16 a 15
                            if(instruc.getNumeroOperandos() == 2 && t1->getModoDirec() == "IDX") {
                                string op = instruc.retrieveStringOperand(0);
                                numBytes = double(t1->getlongInstr()) * 0.50;
                                if(op == "") {
                                    op = "0";
                                }
                                if(operandSize_isValid(op,numBytes, 1)) {
                                    fin.close();
                                    return t1;
                                }
                            }
                            //-256 a 255
                            if(instruc.getNumeroOperandos() == 2 && t1->getModoDirec() == "IDX1") {
                                string op = instruc.retrieveStringOperand(0);
                                numBytes = double(t1->getlongInstr()-1);
                                if(operandSize_isValid(op,numBytes, 1)) {
                                    fin.close();
                                    return t1;
                                }
                            }
                            if(instruc.getNumeroOperandos() == 2 && t1->getModoDirec() == "IDX2") {
                                string op = instruc.retrieveStringOperand(0);
                                numBytes = double(t1->getlongInstr());
                                if(operandSize_isValid(op,numBytes, 1)) {
                                    fin.close();
                                    return t1;
                                }
                            }
                        }
                    }
                    fin >> *t1;
                    t2 = *t1;
                }
                fin.close();
            }
        }
        fin.close();
        t1->setModoDirecc("Invalid");
        return t1;
    }
}

void writeTablaSimbolos(Instruccion& a1, int pc) {
    Contloc c1 = Contloc();
    string num;
    string outFile = "TABSIM.txt";
    ofstream fout;

    fout.open(outFile.c_str(), std::ios::app);

    if(!fout.is_open()) {
        cout << "Error opening file" << endl;
    } else {
        if(a1.validateMnemonico(a1.getMnemonico(), tablaMnemonicos)) {
            fout << a1.getEtiqueta() << " -> " << decToHex(pc) << endl;
        } else if(c1.validarDirectiva(a1.getMnemonico(),tablaDirectivas)) {
            num = decToHex(a1.retrieveOperand(0));
            fout << a1.getEtiqueta() <<  " -> 0x0" << num << endl;
        }
    }
    fout.close();
}

string referenceTabSim(string operando) {
    string etq, value, del;

    string inFile = "TABSIM.txt";
    ifstream fin;

    if(operando[0] == ' ') {
        operando = operando.substr(1, strlen(operando.c_str()));
    }

    fin.open(inFile.c_str());
    //START READING THE P2 ARCHIVE
    if(!fin.is_open()) {
        cout << "Error opening file" << endl;
    } else {
        while(!fin.eof()) {
            getline(fin, etq, ' ');
            getline(fin, del, ' ');
            getline(fin, value);

            if(etq == operando) {
                return value;
            }
        }
    }
    fin.close();
    return "-1";
}

bool checkETQ_OP(string op) {
    Instruccion a1 = Instruccion();
    string inFile = "P5.asm";
    ifstream fin;

    fin.open(inFile.c_str());
    //START READING THE P2 ARCHIVE
    if(!fin.is_open()) {
        cout << "Error opening file" << endl;
    } else {
        while(!fin.eof()) {
            fin >> a1;

            if(a1.hasEtiqueta() && a1.getEtiqueta() == op) {
                return true;
            }
        }
    }
    fin.close();
    return false;
}

//CONVERT FUNCs
string decToHex(int dec_num) {
    string hexaDeciNum;
    int r;
    string hexdec_num="";
    char hex[]= {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    while(dec_num>0) {
        r = dec_num % 16;
        hexdec_num = hex[r] + hexdec_num;
        dec_num = dec_num/16;
    }
    return hexdec_num;
}

string decToHexSigned(int num) {
    // map for decimal to hexa, 0-9 are
    // straightforward, alphabets a-f used
    // for 10 to 15.
    map<int, char> m;

    char digit = '0';
    char c = 'a';

    for (int i = 0; i <= 15; i++) {
        if (i < 10) {
            m[i] = digit++;
        } else {
            m[i] = c++;
        }
    }

    // string to be returned
    string res = "";

    // check if num is 0 and directly return "0"
    if (!num) {
        return "0";
    }
    // if num>0, use normal technique as
    // discussed in other post
    if (num > 0) {
        while (num) {
            res = m[num % 16] + res;
            num /= 16;
        }
        res = "000" + res;
    }
    // if num<0, we need to use the elaborated
    // trick above, lets see this
    else {
        // store num in a u_int, size of u_it is greater,
        // it will be positive since msb is 0
        uint32_t n = num;

        // use the same remainder technique.
        while (n) {
            res = m[n % 16] + res;
            n /= 16;
        }
    }

    return res;
}

int convertNumberDec(string operandos, int numOperandos) {
    int number;

    if(operandos[0] == '#') {
        operandos.erase(0, operandos.find('#')+ 1);
    }
    if(numOperandos == 1) {
        //HEXADECIMAL
        if(operandos[0] == '$') {
            operandos.erase(0, operandos.find('$')+ 1);
            number = std::stoi(operandos, 0,16);
        }
        //OCTAL
        else if(operandos[0] == '@') {
            operandos.erase(0, operandos.find('@')+ 1);
            number = stoi(operandos, 0,8);
        }
        //BINARIO
        else if(operandos[0] == '%') {
            operandos.erase(0, operandos.find('%')+ 1);
            number = stoi(operandos, 0,2);
        }
        //DECIMAL
        else {
            number = stoi(operandos, 0, 10);
        }
    }
    return number;
}

string convertNumber(string operandos, int numOperandos) {
    std::stringstream ss;
    string res;
    int number = 0;

    //REMOVE IMM SIMBOL IF THERE IS ONE
    if(operandos[0] == '#') {
        operandos.erase(0, operandos.find('#')+ 1);
    }

    if(numOperandos == 1) {
        //HEXADECIMAL
        if(operandos[0] == '$') {
            operandos.erase(0, operandos.find('$')+ 1);
            return operandos;
        }
        //OCTAL
        else if(operandos[0] == '@') {
            operandos.erase(0, operandos.find('@')+ 1);
            number = stoi(operandos);

            ss << std::dec << number;
            res = ss.str();
            ss << std::hex << res;
            res = ss.str();
        }
        //BINARIO
        else if(operandos[0] == '%') {
            operandos.erase(0, operandos.find('%')+ 1);
            number = stoi(operandos, 0,2);
        }
        //DEC TO HEXADECIMAL
        else {
            number = stoi(operandos);
            ss << std::hex << number;
            res = ss.str();
        }
    }
    return res;
}



