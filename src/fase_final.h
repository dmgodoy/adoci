/* 
 * File:   fase_final.h
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 *
 */

#ifndef FASE_FINAL_H
#define	FASE_FINAL_H
#include <iostream>
#include "funcs.h"
#include <fstream>
#include <ctime>
using namespace std;
extern string flog;
extern time_t tiempo;

/* Datos para la fase final */
class final_t {
public:
    int radiadores_off;
    int radiadores_on;
    bool soltar_garrote;
    int expulsar;
    municion *expulsada;

    final_t() {
        radiadores_off = 0;
        radiadores_on = 0;
        soltar_garrote = false;
        expulsar = 0;
    }
    void salida(string numJ);
    void logicaFinal();
};


#endif	/* FASE_FINAL_H */

