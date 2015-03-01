/* 
 * File:   acciones.h
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 *
 */

#ifndef _ACCIONES_H
#define	_ACCIONES_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <cstdlib>
#include "funcs.h"
#include "infoMechs.h"
#include "infoMapa.h"
#include "OpcionesJuego.h"
#include "fase_ataque_armas.h"
#include "fase_ataque_fisico.h"
#include "fase_final.h"
#include "fase_movimiento.h"
#include "fase_reaccion.h"
#include "node.h"

using namespace std;

extern string flog;

/********* Datos para las distintas fases ***********/

class acciones {
public:
    string numeroJugador;
    movimiento_t* mov;
    reaccion_t* reaccion;
    ataque_armas_t* armas;
    ataque_fisico_t* fisico;
    final_t* final;
    infoMapa* informacion_mapa;
    infoMechs* informacion_mechs;
    OpcionesJuego* informacion_juego;

    acciones(string numJ,infoMapa* mapa,infoMechs* mechs, OpcionesJuego* opts);
    acciones(const acciones& orig);
    virtual ~acciones();
    void salida(string cad);
    string mover(hexagono_pos destino);
    void reaccion_accion();
    void ataque_arma();
    int objetivoArmas();
    int obtenerLocalizacionMunicion(iMech mech, Componente_Mech arma);
    int obtenerSlotArma(iMech mech, Componente_Mech arma);
    int angulo_mech(int mech_obj);
    bool buscar_municion(const iMech &mech, int cod_arma);
    bool queda_municion(const iMech &mech, int codigo);
    //Movimiento
    int estrategia_movimiento();
    void reglas_movimiento();
    void confirmar_mov(int tipo_movimiento, hexagono_pos destino, int lado, int MASC, int tipo, int veces);
    void mov_huir(int num_jugador, int num_jugadores, int PM_INI, int tipo_mov);
    void actualizar_mov_candidatos(int PM, hexagono_pos casilla, bool *candidatos, int toneladas, int tipo_mov);
    int func_direccion_giro (int encaramiento, int dir_objetivo);
    int func_coste_giro (int encaramiento, int dir_objetivo, int dir_giro);
    int lado_final_giro (int enc_ini, int dir_giro, int veces);
    int paso_huir(bool* candidatos, int num_jugador, int mech_objetivo, int primer_mov);
    int obt_valor_obstaculos(hexagono_pos pos);
    int es_mejor(int LV1, int valor1, int LV2, int valor2);
    bool linea_vision(int num_jugador, hexagono_pos origen, int nivel_origen,hexagono_pos destino, int nivel_destino);
    void mov_atacar(int num_jugador, int num_jugadores, int PM, int tipo_mov);
    int casilla_ocupada(int num_jugadores, hexagono_pos casilla);
    int mov_acercarse(int num_jugador, int mech_objetivo, bool *candidatos, int PM);
    int obt_paso_anterior(int direccion);

 

private:
    bool dentroMapa(int fila, int columna);
    void astar_hex(int fila,int columna);
    void posiciones_adyacentes(int f,int c,int adyacentes[][2]);



};




#endif	/* _ACCIONES_H */

