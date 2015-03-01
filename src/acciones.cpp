/* 
 * File:   acciones.cpp
 * Author: Angel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "acciones.h"
using namespace std;

acciones::acciones(string numJ, infoMapa* mapa, infoMechs* mechs, OpcionesJuego* opts) {
    informacion_mapa = mapa;
    informacion_mechs = mechs;
    informacion_juego = opts;
    numeroJugador = numJ;
    mov = 0;
    reaccion = 0;
    armas = 0;
    fisico = 0;
    final = 0;
}

acciones::acciones(const acciones& orig) {
}

acciones::~acciones() {

    if (mov != 0)
        delete mov;

    if (reaccion != 0)
        delete reaccion;

    if (armas != 0)
        delete armas;

    if (fisico != 0)
        delete fisico;

    if (final != 0)
        delete final;

}

void acciones::salida(string cad) {

    if (cad == "Movimiento") {
        mov = new movimiento_t(informacion_mapa,informacion_mechs);
        mov->logica_movimiento();
        mov->salida(numeroJugador);
    }
    if (cad == "Reaccion") {
        reaccion = new reaccion_t(informacion_mapa,informacion_mechs);
        reaccion->reaccion_accion();
        reaccion->salida(numeroJugador);
    }
    if (cad == "AtaqueArmas") {
        armas = new ataque_armas_t(informacion_mapa, informacion_mechs);
        armas->ataque_arma();
        armas->salida(numeroJugador);
    }
    if (cad == "AtaqueFisico") {
        fisico = new ataque_fisico_t(informacion_mapa,informacion_mechs);
        fisico->ataque_fisico();
        fisico->salida(numeroJugador);
    }
    if (cad == "FinalTurno") {
        final = new final_t;
        final->logicaFinal();
        final->salida(numeroJugador);
    }


}
