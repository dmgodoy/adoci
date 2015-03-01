/* 
 * File:   funcs.cpp
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "funcs.h"

void leeLinea(istream & in, string & cad, char delim) {
    getline(in, cad, delim);
    if(cad[cad.size()-1]=='\n'){
        cad.erase(cad.end()-1);
    }else if(cad[cad.size()-1]=='\r'){
        cad.erase(cad.end()-1);
    }
}

string itoStr(int num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

float distancia_hexagonal(float fila1, float columna1, float fila2, float columna2) {
    float f1 = fila1, f2 = fila2;
    if ((int) columna1 % 2 == 0) {
        f1 += 0.5;
    }
    if ((int) columna2 % 2 == 0) {
        f2 += 0.5;
    }

    return distancia(f1, columna1, f2, columna2);
}

float distancia(float fil_ini, float col_ini, float fil_obj, float col_obj) {
    return sqrtf((float) (col_ini - col_obj)*(col_ini - col_obj) +
            (float) (fil_ini - fil_obj)*(fil_ini - fil_obj));
}



/*int strtoInt(string cadena){
    stringstream salida;
    
    salida << cadena;
    salida.
    return salida.int();
}*/
