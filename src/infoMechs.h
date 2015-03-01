/* 
 * File:   infoMechs.h
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 *
 */

#ifndef _INFOMECHS_H
#define	_INFOMECHS_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include "funcs.h"

using namespace std;

struct Componente_Mech {
    int codigo; // Código del componente
    string nombre; // Nombre del arma
    componente_t clase;
    bool trasera;
    int localizacion;
    int localizacion2;
    int tipo;
    int calor;
    int danio;
    int disparos_por_turno;
    int distanciaMinima;
    int distanciaCorta;
    int distanciaMedia;
    int distanciaLarga;
    bool operativo;

    //Si el componente es munición
    int codigoArma; // El arma que la utiliza
    int cantidad;
    bool especial;
    int modificador;
};

struct Actuador_Mech {
    int codigo; // Código del actuador
    string nombre; // Nombre del actuador
    int localizacion; // Esto va con los enums que definieron
    bool operativo; // INdica si el actuador esta operativo o no
    int num_impactos; // Número de impactos recibidos por el actuador
};

struct Slot_Mech {
    int clase;
    string nombre;
    int cantidad;
    int codigo;
    int indice_componente;
    int indice_actuador;
    int danio_municion;
};

struct Localizacion_Mech {
    int slots_ocupados;
    Slot_Mech *slots; // El número de slots es de 12
};

struct l_blindaje {
    int BI;
    int TI;
    int PI;
    int BD;
    int TD;
    int PD;
    int TC;
    int CAB;
    int PATD;
    int PATI;
    int PATC;
};

struct l_interna {
    int BI;
    int TI;
    int PI;
    int BD;
    int TD;
    int PD;
    int TC;
    int CAB;
};

typedef struct {
    /*  Variables que están en el archivo de los componentes de cada mech */
    int num_componentes; // Número de componentes del mech
    Componente_Mech *componentes; // Vector en el que almacenaremos los componentes del mech (armas, municiones...)
    int toneladas; // Peso en toneladas del mech
    int potencia; // Potencia del mech
    int radiadoresInternos;
    int radiadores;
    bool MASC;
    int max_calor;
    bool *brazos; // Cada casilla corresponde a una parte del brazo
    int num_armas; // Número de armas del mech
    int num_actuadores; // Número de actuadores del mech
    int tipoRadiadores;
    Actuador_Mech *actuadores;
    Localizacion_Mech *localizaciones;
} defMechStruct;

typedef struct {
    //Para el jugador
    int PM_andar; //>=0
    int PM_correr; //>=0
    int PM_saltar; //>=0
    int Radiadores_operativos_encendidos; //>=0
    int Radiadores_operativos_apagados; //>=0
    int Heridas_MW; //>=0
    bool MW_consciente;
    bool slots_impactados[78]; /* (TRUE-FALSE) indica para cada slot si fue impactado */
    bool local_disparo_arma[8]; /* (TRUE-FALSE) Ejemplo: disparo[CABEZA] => Se dispará un arma de la cabeza */
    int municiones_expulsar; //>0
    municion* expulsada;
} datosMechJugador;

class iMech {
public:

    int numJ; //entero positivo
    bool operativo;
    bool desconectado;
    bool atascadoEnPantano;
    bool enElSuelo;
    hexagono_pos pos_Hexagono;
    int encaramiento_mech; //1 a 6
    int encaramiento_torso; //1 a 6
    int temp_actual; //>0
    bool ardiendo;
    bool con_garrote;
    int tipo_garrote; //0 o 1
    l_blindaje blindaje;
    l_interna puntosEstructuraInterna;
    bool* narc; //Vector de bool,que indica si el mech actual ha colocado un narc al mech cuyo número de jugador es [índice]
    bool* inarc; //Similar al anterior
    defMechStruct* defMechInfo;
    datosMechJugador* dmj;

    iMech(){
        defMechInfo = new defMechStruct;
        narc = 0;
        inarc = 0;
        dmj = 0;
    }

    ~iMech() {
        if (narc)
            delete narc;
        if (inarc)
            delete inarc;
        if (dmj)
            delete dmj;
        delete defMechInfo;
    }

    /* Devuelve TRUE si el mech que se pasa como parámetro tiene
     * munición para el arma cuyo código se pasa como parámetro */
    bool municion_ok(int codigo) {
        int i;
        for (i = 0; i < defMechInfo->num_componentes; ++i) {
            if ((defMechInfo->componentes[i].clase == MUNICION) &&
                    (defMechInfo->componentes[i].codigoArma == codigo) &&
                    (defMechInfo->componentes[i].cantidad > 0) &&
                    (defMechInfo->componentes[i].operativo))
                return true;
        }
        return false;
    }

    bool armas_ok() {
        int i;
        //std::cout<<"Comp:"<<defMechInfo->num_componentes<<std::endl;
        for (i = 0; i < defMechInfo->num_componentes; ++i) {
            if ((defMechInfo->componentes[i].clase == ARMA) &&
                    (defMechInfo->componentes[i].trasera == false) &&
                    (defMechInfo->componentes[i].operativo == true) &&
                    ((defMechInfo->componentes[i].tipo == ENERGIA) || (this->municion_ok(defMechInfo->componentes[i].codigo))))
                return true;
        }
        return false;
    }

    bool actuador_ok(int loc, char* actuador) {
        int i;

        for (i = 0; i < defMechInfo->num_actuadores; ++i) {
            if ((defMechInfo->actuadores[i].localizacion == loc) &&
                    (strstr(defMechInfo->actuadores[i].nombre.c_str(), actuador) != NULL) &&
                    (defMechInfo->actuadores[i].operativo == true))
                return true;
        }
        return false;
    }

    /* Devuelve TRUE si el mech es capaz de ponerse de pie y moverse */
    bool equilibrio_ok() {
        int i;

        for (i = 0; i < defMechInfo->num_componentes; ++i) {
            if (strstr(defMechInfo->componentes[i].nombre.c_str(), "Giroscopio") != NULL) {
                if (defMechInfo->componentes[i].operativo == false) {
                    return false;
                } else {
                    break;
                }
            }
        }
        /* Hay que mirar también los actuadores */
        if ((this->puntosEstructuraInterna.PI == 0) ||
                (this->puntosEstructuraInterna.PD == 0) ||
                ((this->actuador_ok(PI, "Cadera") == false) &&
                (this->actuador_ok(PD, "Cadera") == false)))
            return false;
        return true;
    }

    int buscar_mech_cercano(iMech** mechs, int num_jugador, int num_jugadores) {
        int micol, mifil, /* columna y fila de nuestro mech */
                objetivo = -1; /* número del mech objetivo. Si error, entonces -1 */

        float min_d = 10000, /* Mínima distancia encontrada */
                dist; /* variable auxiliar */

        /* Estrategia: Hallar la distancia en línea recta entre el mech num_jugador
           y el resto de mechs y devolver el número del mech con menor distancia
         */
        micol = pos_Hexagono.columna;
        mifil = pos_Hexagono.fila;
        for (int i = 0; i < num_jugadores - 1; ++i) {
            if (mechs[i]->operativo) {
                dist = distancia_hexagonal(mifil, micol, mechs[i]->pos_Hexagono.fila, mechs[i]->pos_Hexagono.columna);
                if (dist < min_d) {
                    objetivo = i;
                    min_d = dist;
                }
            }
        }
        return objetivo;
    }

    int buscar_objetivo(iMech** mechs, int num_jugador, int num_jugadores) {

        int micol, mifil, /* columna y fila de nuestro mech */
                objetivo = -1; /* número del mech objetivo. Si error, entonces -1 */

        float min_d = 10000, /* Mínima distancia encontrada */
                dist; /* variable auxiliar */

        /* Estrategia: Hallar la distancia en línea recta entre el mech num_jugador
           y el resto de mechs y devolver el número del mech con menor distancia
         */
        micol = pos_Hexagono.columna;
        mifil = pos_Hexagono.fila;

        for (int i = 0; i < num_jugadores - 1; ++i) {
            if (mechs[i]->operativo) {
                dist = distancia_hexagonal(mifil, micol, mechs[i]->pos_Hexagono.fila, mechs[i]->pos_Hexagono.columna);

                if (dist < min_d) {
                    min_d = dist;
                }
            }
        }
        int max_peso = 0;
        for (int i = 0; i < num_jugadores - 1; ++i) {
            if (mechs[i]->operativo) {
                dist = distancia_hexagonal(mifil, micol, mechs[i]->pos_Hexagono.fila, mechs[i]->pos_Hexagono.columna);
                if ((dist < (min_d + 2)) && (mechs[i]->defMechInfo->toneladas > max_peso)) {
                    max_peso = mechs[i]->defMechInfo->toneladas;
                    objetivo = i;
                }
            }
        }
        return objetivo;

    }

    float dist_media_disparo() {
        int i,
                num_armas = 0;
        float dist = 0;
        for (i = 0; i < defMechInfo->num_componentes; ++i) {
            if ((defMechInfo->componentes[i].clase == ARMA) &&
                    (defMechInfo->componentes[i].trasera == false) &&
                    (defMechInfo->componentes[i].operativo == true)) {
                dist += defMechInfo->componentes[i].distanciaMedia;
                num_armas++;
            }
        }
        if(num_armas!=0)
                dist = dist / num_armas;
        else
            return 0;
        return dist;
    }

    /* Devuelve la mejor distancia para disparar con el arma más potente
     * que tiene el mech */
    int dist_disparo() {
        int i,
                max_dano = 0,
                indice;
        for (i = 0; i < defMechInfo->num_componentes; ++i) {
            if ((defMechInfo->componentes[i].clase == ARMA) &&
                    (defMechInfo->componentes[i].trasera == false) &&
                    (defMechInfo->componentes[i].operativo == true) &&
                    ((defMechInfo->componentes[i].danio * defMechInfo->componentes[i].disparos_por_turno) > max_dano) &&
                    ((defMechInfo->componentes[i].tipo == ENERGIA) || municion_ok(defMechInfo->componentes[i].codigo))) {
                max_dano = defMechInfo->componentes[i].danio * defMechInfo->componentes[i].disparos_por_turno;
                indice = i;
            }
        }
        if (max_dano > 0) {
            if (defMechInfo->componentes[indice].distanciaMinima > 0)
                return defMechInfo->componentes[indice].distanciaMinima;
            else
                return defMechInfo->componentes[indice].distanciaMinima;
        } else
            return 0;
    }


};

class infoMechs {
public:
    infoMechs(int numJugadorActual);
    infoMechs(const infoMechs& orig);
    virtual ~infoMechs();
    void leeDatosDefMech(int numJ, int numM, iMech* mech);
    void leeDatosComponentes(ifstream &in, iMech *mech); //Lee los componentes de cada Mech
    void leeDatosCalor(ifstream &in, iMech *mech);
    void leeDatosNarc(ifstream & in, iMech* mech);
    void leeDatosMech(ifstream & in, iMech* mech); //Lee los datos de una estructura iMech, menos numJ, que habra sido leido previamente.
    void leeDatosPPal(ifstream & in);
    void leeDatosMechVector(ifstream & in, int indice, int numJMech);
    void leeFich(string fichero);
    int nMechs; //Numero de Mechs
    iMech** iMechVector;
    iMech* mechJugador;


private:

    bool s2bool(string cad) {
        if (cad.compare(0, 4, "True") == 0)
            return true;
        else
            return false;
    }

};

#endif	/* _INFOMECHS_H */

