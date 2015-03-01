/* 
 * File:   movimiento.h
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 *
 */

#ifndef MOVIMIENTO_H
#define	MOVIMIENTO_H

#include <vector>
#include "infoMapa.h"
using namespace std;

class node {
public:
    int fil;
    int col;
    int orientacion;
    node();
    node(const node & orig);
    node(node * padr, int fila, int columna, int orienta, float heur);
    node(int fila,int columna,int orienta,infoMapa* map,int pesoMech);
    node(int fila,int columna,int orienta,node * destino,infoMapa* map,int pesoMech);
    node(node * ini, node * dest, infoMapa* map, int pesoMech);
    void show()const;
    void showInfo();
    void showPath();
    void storeRoute(vector<node>& nodos,int mode);
    void storeRoute(node *nodoDestino);
    void showInfoHijos();
    //void getSecuencia(float PM,movimiento_t * mov);
    //node(const node& orig);
    vector <node *> hijos;
    void expand(int mode);
    virtual ~node();
    float f(int mode);
    float g(int mode);
    float h();
    float coste;
    bool operator==(const node & nodo);
    bool mismo_hexagono(const node & nodo)const;

    node& operator=(const node & nodo);
    node * padre;

private:
    node * inicio;
    node * destino;
    infoMapa* mapa;
    float hVal;

    int peso;
    float costeDesdePadre(int mode);


};
float distanciaNodos(int forigen, int corigen, int fdestino, int cdestino);
void show(const vector<node> & nodos);
void aStar(node * inicio, node * destino,vector<node>& nodos, infoMapa *mapa, int tonelaje,int mode = DEFAULT);


#endif	/* MOVIMIENTO_H */

