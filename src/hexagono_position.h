/* 
 * File:   hexagono_position.h
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 *
 */

#ifndef HEXAGONO_POSITION_H
#define	HEXAGONO_POSITION_H
using namespace std;
#include <iostream>
#include <sstream>
#include <stdlib.h>
class hexagono_pos {
public:
    string pos;
    int fila;
    int columna;
    hexagono_pos(){}
    hexagono_pos(const hexagono_pos & orig){
        fila=orig.fila;
        columna=orig.columna;
        pos=orig.stringPos();
    }

    hexagono_pos(int f,int c){
        fila=f;
        columna=c;
    }

    void ini(string cad) {
        pos=cad;
        columna = atoi(cad.substr(0,2).c_str());
        fila = atoi(cad.substr(2,2).c_str());
    }

    string stringPos()const {
        string cad;
        if (columna < 10)//1 cifra
            cad = cad + "0";
        cad = cad + itos(columna);
        if (fila < 10)//1 cifra
            cad = cad + "0";
        cad = cad + itos(fila);

        return cad;
    }
    bool operator==(const hexagono_pos & orig){
        return (this->columna==orig.columna && this->fila==orig.fila);
    }
    bool operator!=(const hexagono_pos & orig){
        return (!(*this==orig));
    }

private:
    string itos(int num)const {
    stringstream ss;
    ss << num;
    return ss.str();
}
};

#endif	/* HEXAGONO_POSITION_H */

