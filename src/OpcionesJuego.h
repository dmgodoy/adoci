/* 
 * File:   OpcionesJuego.h
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 *
 */

#ifndef _OPCIONESJUEGO_H
#define	_OPCIONESJUEGO_H

#include <fstream>
#include <iostream>
#include <cstdlib>
#include "funcs.h"
using namespace std;

class OpcionesJuego {
public:
    bool incendios_permitidos;
    bool viento_permitido;
    int direccion_viento;//INT 1 y 6 o -1
    bool ataques_fisicos_permitidos;
    bool fase_calor;
    bool devastar_bosques;
    bool derrumbar_edificios;
    bool chequeos_pilotaje;
    bool chequeo_danho;
    bool chequeo_desconexion;
    bool impactos_criticos;
    bool explosion_municion;
    bool apagar_radiadores;
    bool limitar_tiempo_respuesta;
    int valor_limite_tiempo;//entero positivo o -1

    OpcionesJuego();
    OpcionesJuego(const OpcionesJuego& orig);
    virtual ~OpcionesJuego();
    void leeFich(string fichero);
private:
    bool s2bool(string cad){
        if(cad=="True")
            return true;
        else
            return false;
    }

};

#endif	/* _OPCIONESJUEGO_H */

