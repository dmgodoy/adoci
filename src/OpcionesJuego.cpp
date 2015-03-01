/* 
 * File:   OpcionesJuego.cpp
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "OpcionesJuego.h"

OpcionesJuego::OpcionesJuego() {
}

OpcionesJuego::OpcionesJuego(const OpcionesJuego& orig) {
}

OpcionesJuego::~OpcionesJuego() {
}

void OpcionesJuego::leeFich(string fichero){
    string cad;
    ifstream fich(fichero.c_str());//Creación y apertura
    if(!fich){
        cerr << "\nNo es posible abrir " << fichero <<"\n";
        exit(1);
    }
    leeLinea(fich,cad,'\n');
    //fich.ignore(1000,'\n');//Ignoramos número mágico
    leeLinea (fich,cad,'\n');
    incendios_permitidos=s2bool(cad);

    leeLinea (fich,cad,'\n');
    viento_permitido=s2bool(cad);

    leeLinea (fich,cad,'\n');


    direccion_viento = atoi(cad.c_str());//INT 1 y 6 o -1

    if(direccion_viento!=-1 && (direccion_viento<0 || direccion_viento>6)){
        cerr << "\nValor de dirección del viento incorrecto, debe de estar entre 1 y 6 o ser -1:"<<endl;
        cin.get();
        exit(1);
    }
    leeLinea (fich,cad,'\n');
    ataques_fisicos_permitidos=s2bool(cad);

    leeLinea (fich,cad,'\n');
    fase_calor=s2bool(cad);

    leeLinea (fich,cad,'\n');
    devastar_bosques=s2bool(cad);

    leeLinea (fich,cad,'\n');
    derrumbar_edificios=s2bool(cad);

    leeLinea (fich,cad,'\n');
    chequeos_pilotaje=s2bool(cad);

    leeLinea (fich,cad,'\n');
    chequeo_danho=s2bool(cad);

    leeLinea (fich,cad,'\n');
    chequeo_desconexion=s2bool(cad);

    leeLinea (fich,cad,'\n');
    impactos_criticos=s2bool(cad);

    leeLinea (fich,cad,'\n');
    explosion_municion=s2bool(cad);

    leeLinea (fich,cad,'\n');
    apagar_radiadores=s2bool(cad);

    leeLinea (fich,cad,'\n');
    limitar_tiempo_respuesta=s2bool(cad);

    leeLinea (fich,cad,'\n');
    valor_limite_tiempo = atoi(cad.c_str());//entero positivo o -1
    if(valor_limite_tiempo!=-1 && valor_limite_tiempo<0){
        cerr << "\nValor de tiempo incorrecto, debe ser entero positivo o -1"<<endl;
        exit(1);
    }
}

