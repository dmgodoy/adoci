/* 
 * File:   fase_reaccion.h
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 *
 */

#ifndef FASE_REACCION_H
#define	FASE_REACCION_H
#include "funcs.h"
#include <iostream>
#include <fstream>
#include "infoMapa.h"
#include "infoMechs.h"
#include <ctime>


using namespace std;

extern string flog;
extern time_t tiempo;

/* Datos para la fase de reacción */
class reaccion_t {
public:
    int encaramiento;
    infoMapa* mapa;
    infoMechs * mechs;
    
    reaccion_t(infoMapa* inf_mapa, infoMechs * inf_mechs);
    void reaccion_accion() ;
    void salida(string numJ);
};

#endif	/* FASE_REACCION_H */

