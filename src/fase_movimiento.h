/* 
 * File:   fase_movimiento.h
 * Author: Angel Costela Sanmiguel y David Medina Godoy
 *
 */

#ifndef FASE_MOVIMIENTO_H
#define	FASE_MOVIMIENTO_H
#include <fstream>
#include <iostream>
#include <vector>
#include "node.h"
#include "funcs.h"
#include "infoMapa.h"
#include "infoMechs.h"
#include "nodoArea.h"

using namespace std;

extern string flog;
extern time_t tiempo;

#define MIN_DIST 6
/* Datos para la fase de movimiento */
class movimiento_t {
public:
    int tipo_movimiento;
    hexagono_pos destino;
    int lado;
    bool MASC;
    int pasos;
    int tipo[20];
    int veces[20];
    infoMapa* mapa;
    infoMechs * mechs;

    movimiento_t(infoMapa* inf_mapa, infoMechs * inf_mechs);
    int tipo_movim(const vector<node>& nodos);
    int estrategia_movimiento();
    int getDestino(int fil_mech, int col_mech,int& fil_dest, int & col_dest, int & lado_dest, int estrategia);
    void salida(string numJ);
    void logica_movimiento();
    int getTipoMov(const node & n1, const node & n2);
    void getSecuenciaPasos(const vector<node> & nodos, int PM);
//    void encarar_objetivo(int f, int c, int f_obj, int c_obj, int & lado);
int estrategiaPorPeso();
//    void getArea(int n, int f, int c, vector<nodoArea> & pos_vector);

private:
    bool check_mov_correr(const vector<node>& nodos);
    int fil_enemigo;
    int col_enemigo;


};

#endif	/* FASE_MOVIMIENTO_H */

