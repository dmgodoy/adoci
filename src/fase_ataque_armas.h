/* 
 * File:   fase_ataque_armas.h
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 *
 */

#ifndef FASE_ATAQUE_ARMAS_H
#define	FASE_ATAQUE_ARMAS_H
#include "hexagono_position.h"
#include "funcs.h"
#include <fstream>
#include "infoMapa.h"
#include "infoMechs.h"
#include "nodoArea.h"
#include <ctime>


using namespace std;
extern string flog;
extern time_t tiempo;

/* Datos para el disparo de un arma */
class arma_fuego {
public:
    int localizacion;
    int slot;
    bool doble_cadencia;
    int loc_municion;
    int slot_municion;
    hexagono_pos objetivo;
    int tipo_objetivo;
   
    arma_fuego() {
        localizacion = 0;
        slot = 0;
        doble_cadencia = false;
        loc_municion = -1;
        slot_municion = -1;
        objetivo.fila = 0;
        objetivo.columna = 0;
        tipo_objetivo = 0;
    }
};

/* Datos para la fase de ataque con armas */
class ataque_armas_t {
public:
    bool coger_garrote;
    hexagono_pos objetivo;
    int num_armas;
    arma_fuego armas_mech[20];
    infoMapa* mapa;
    infoMechs * mechs;
    
    ataque_armas_t(infoMapa* inf_mapa, infoMechs * inf_mechs);
    void posiciones_adyacentes(int f, int c, int adyacentes[][2]);
    int angulo_mech(int mech_obj);
    int objetivoArmas();
    bool queda_municion(const iMech &mech, int codigo);
    bool buscar_municion(const iMech &mech, int cod_arma);
    bool linea_vision(int num_jugador, const hexagono_pos & origen, int nivel_origen, const hexagono_pos & destino, int nivel_destino);
    int obtenerLocalizacionMunicion(iMech *&mech, Componente_Mech arma);
    int obtenerSlotArma(iMech *&mech, Componente_Mech arma);
    void ataque_arma();
    void salida(string numJ);
};


#endif	/* FASE_ATAQUE_ARMAS_H */

