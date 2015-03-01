/* 
 * File:   movimiento.cpp
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "node.h"
#include <algorithm>
using namespace std;

node::node() {

}

node::node(const node& orig) {
    fil = orig.fil;
    col = orig.col;
    orientacion = orig.orientacion;
    coste = orig.coste;
    padre = 0;
}

node::node(int fila, int columna, int orient, infoMapa* map, int pesoMech) {
    fil = fila;
    col = columna;
    mapa = map;
    padre = 0;
    inicio = 0;
    destino = 0;
    peso = pesoMech;
    hVal = -1;
    orientacion = orient;
    hijos.clear();
}

node::node(int fila, int columna, int orient, node * destino, infoMapa* map, int pesoMech) {
    fil = fila;
    col = columna;
    mapa = map;
    padre = 0;
    inicio = 0;
    peso = pesoMech;
    hVal = -1;
    hijos.clear();
    orientacion = orient;
    this->destino = destino;
}

node::node(node * padr, int fila, int columna, int orienta, float heur) {
    inicio = padr->inicio;
    destino = padr->destino;
    mapa = padr->mapa;
    peso = padr->peso;
    fil = fila;
    col = columna;
    orientacion = orienta;
    hVal = heur;
    padre = padr;
}

node::node(node * ini, node * dest, infoMapa* map, int pesoMech) {
    inicio = ini;
    destino = dest;
    mapa = map;
    peso = pesoMech;
    hVal = -1;
    padre = 0;
    hijos.clear();
}

void node::showPath() {
    node* nodePointer;
    nodePointer = this;
    printf("\n\nCamino->\n");
    while (nodePointer->padre != 0) {
        nodePointer->show();
        nodePointer = nodePointer->padre;
    }
    nodePointer->show();
    printf("Fin de camino--------------------\n\n");

}

void node::storeRoute(vector<node>& nodos,int mode) {
    node* nodePointer;
    nodePointer = this;

    nodePointer->coste = nodePointer->g(mode);
    nodos.push_back(*nodePointer);

    while (nodePointer->padre != 0) {
        nodePointer = nodePointer->padre;
        nodePointer->coste = nodePointer->g(mode);
        nodos.push_back(*nodePointer);
    }

    reverse(nodos.begin(), nodos.end());

}

float distanciaNodos(int forigen, int corigen, int fdestino, int cdestino) {
    float val = sqrt((forigen - fdestino)*(forigen - fdestino)+
            (corigen - cdestino)*(corigen - cdestino));
    return val;
}

node::~node() {
    for (int i = 0; i < hijos.size(); i++)
        if (hijos[i] != 0)
            delete hijos[i];
}

void node::show()const {
    printf("Fila: %i\nColumna: %i\nOrientacion: %i\nCoste: %f\n", fil, col, orientacion, coste);
}

//void node::expand() {
//    hijos.resize(3);
//    int anterior = orientacion - 1;
//    if (anterior == 0)
//        anterior = 6;
//    int siguiente = orientacion + 1;
//    if (siguiente == 7)
//        siguiente = 1;
//    int filhijo;
//    int colhijo;
//    //printf("Vamos a crear los hijos...\n");
//
//    float distAdest = distanciaNodos(fil, col, destino->fil, destino->col);
//    float valH;
//    //printf("Vamos a crear los hijos...\n");
//    nodoEnEsaDireccion(fil, col, anterior, filhijo, colhijo);
//    if (filhijo > 0 && filhijo < mapa->filas && colhijo > 0 && colhijo < mapa->columnas)
//        valH = distanciaNodos(filhijo, colhijo, destino->fil, destino->col);
//    else valH = distAdest + 1;
//    hijos[0] = new node(this, fil, col, anterior, 1 + valH);
//    //printf("Creado hijo 0\n");
//
//    nodoEnEsaDireccion(fil, col, siguiente, filhijo, colhijo);
//    if (filhijo > 0 && filhijo < mapa->filas && colhijo > 0 && colhijo < mapa->columnas)
//        valH = distanciaNodos(filhijo, colhijo, destino->fil, destino->col);
//    else valH = distAdest + 1;
//    hijos[1] = new node(this, fil, col, siguiente, 1 + valH);
//    //printf("Creado hijo 1\n");
//
//
//    nodoEnEsaDireccion(fil, col, orientacion, filhijo, colhijo);
//    if (filhijo > 0 && filhijo < mapa->filas && colhijo > 0 && colhijo < mapa->columnas)
//        hijos[2] = new node(this, filhijo, colhijo, orientacion, -1);
//    else {
//        hijos[2] = 0;
//        hijos.resize(2);
//    }
//    //printf("Creado hijo 2\n");
//
//    //showInfoHijos();
//
//}

void node::expand(int mode) {
    hijos.resize(4);
    int anterior = orientacion - 1;
    if (anterior == 0)
        anterior = 6;
    int siguiente = orientacion + 1;
    if (siguiente == 7)
        siguiente = 1;
    int filhijo;
    int colhijo;
    //printf("Vamos a crear los hijos...\n");

    //printf("Vamos a crear los hijos...\n");
    //--nodoEnEsaDireccion(fil, col, anterior, filhijo, colhijo);
    hijos[0] = new node(this, fil, col, anterior, -1);
    //printf("Creado hijo 0\n");

    //--nodoEnEsaDireccion(fil, col, siguiente, filhijo, colhijo);
    hijos[1] = new node(this, fil, col, siguiente, -1);
    //printf("Creado hijo 1\n");


    nodoEnEsaDireccion(fil, col, orientacion, filhijo, colhijo);
    if (filhijo > 0 && filhijo < mapa->filas && colhijo > 0 && colhijo < mapa->columnas)
        hijos[2] = new node(this, filhijo, colhijo, orientacion, -1);
    else {
        hijos[2] = 0;
        //hijos.resize(2);
    }
    if (mode == WALK) {
        nodoEnEspalda(fil, col, orientacion, filhijo, colhijo);
        if (filhijo > 0 && filhijo < mapa->filas && colhijo > 0 && colhijo < mapa->columnas)
            hijos[3] = new node(this, filhijo, colhijo, orientacion, -1);
        else {
            hijos[3] = 0;
            //hijos.resize(2);
        }

    } else {
        hijos[3] = 0;
    }
    //printf("Creado hijo 2\n");

    //showInfoHijos();

}

float node::g(int mode) {
    if (padre == 0)
        return 0;
    else
        return costeDesdePadre(mode) + padre->g(mode);
}

float node::costeDesdePadre(int mode) {
    float valor = 0;
    if (padre == 0)
        return 0;
    if (padre->fil == fil && padre->col == col)//es un movimiento de giro en un hexágono
        return mapa->coste_mov_giro(fil, col);
    valor = mapa->coste_mov_ady(padre->fil, padre->col, this->fil, this->col, peso,mode);
    return valor;
}

//void node::getSecuencia(float PM, movimiento_t* mov) {
//    node* nodePointer;
//    node* destinoAlcanzado;
//    destinoAlcanzado = this;
//
//    while (destinoAlcanzado->g() > PM) {
//        destinoAlcanzado = destinoAlcanzado->padre;
//    }
//    mov->destino.fila = destinoAlcanzado->fil;
//    mov->destino.columna = destinoAlcanzado->col;
//    mov->destino.pos = destinoAlcanzado->orientacion;
//    int veces[20];
//    for(int i=0;i<20;i++)
//        veces[i]=0;
//    mov->pasos = 0;//Pasos en los que se divide el movimiento
//    mov->tipo[0] = MOV_ADELANTE; //ADELANTE,ATRÁS número de veces que se realiza cada paso
//
//    ////mov->MASC=; Rellenar fuera
//    ////mov->tipo_movimiento=; Rellenar fuera
//    nodePointer = destinoAlcanzado;
//
//    //Vamos de fin a inicio
//    while (nodePointer->padre != 0) {
//        if(nodePointer->fil==nodePointer->padre->fil &&
//           nodePointer->col==nodePointer->padre->col)
//            veces[mov->pasos]++;//incrementamos las veces del paso actual
//        else
//            mov->pasos++;//Incrementamos el paso actual
//        nodePointer = nodePointer->padre;
//    }
//    mov->pasos++;//El num de pasos será el paso actual +1 (empiezan en )
//    
//    //invertimos el orden de los pasos-> veces y tipos
//    for(int i=0;i<mov->pasos;i++){
//        mov->veces[i]=veces[mov->pasos-1-i];
//    }
//    
//}

float node::h() {
    if (hVal == -1) {
        hVal = distancia_hexagonal(this->fil, this->col, destino->fil, destino->col);
    }
    return hVal;
}

float node::f(int mode) {
    float gV = g(mode);
    float hV = h();
    float fV = gV + hV;
    return fV;
}

node& node::operator=(const node & nodo) {
    this->col = nodo.col;
    this->fil = nodo.fil;
    this->destino = nodo.destino;
    this->hVal = nodo.hVal;
    this->hijos = nodo.hijos;
    this->inicio = nodo.inicio;
    this->mapa = nodo.mapa;
    this->orientacion = nodo.orientacion;
    this->padre = nodo.padre;
    this->peso = nodo.peso;
    coste = nodo.coste;
}

bool node::operator==(const node & nodo) {
    return fil == nodo.fil && col == nodo.col && orientacion == nodo.orientacion;
}

bool node::mismo_hexagono(const node & nodo) const {
    return fil == nodo.fil && col == nodo.col;
}

int buscarMejor(vector<node*> nodos,int mode) {//busca menor f
    float menor = nodos[0]->f(mode);
    float valor;
    int ind = 0;
    for (int i = 0; i < nodos.size(); i++) {
        valor = nodos[i]->f(mode);
        if (valor < menor) {
            menor = valor;
            ind = i;
        }
    }
    //    printf("\nContenido del buffer de nodos:\n");
    //    for (int i = 0; i < nodos.size(); i++) {
    //        nodos[i]->showInfo();
    //        printf("f: %i\n",nodos[i]->f());
    //        printf("g: %i\n",nodos[i]->g());
    //        printf("h: %i\n",nodos[i]->h());
    //    }
    //printf("\nFin de contenido del buffer\n");
    return ind;

}

int contenidoEn(vector<node*> nodos, const node& n) {
    for (int i = 0; i < nodos.size(); i++) {
        if (*nodos[i] == n)
            return i;
    }
    return 0;
}

void aStar(node * inicio, node * destino, vector<node>& nodos, infoMapa *mapa, int tonelaje, int mode) {
    nodos.clear();
    vector<node*> abiertos;
    abiertos.push_back(inicio);
    vector<node*> cerrados;
    bool fin = false;
    int mejor;
    int nInd;
    while (!fin) {
        mejor = buscarMejor(abiertos,mode);

        if (*abiertos[mejor] == *destino) {
            fin = true;
            //Almacenamos ruta en destino
            abiertos[mejor]->storeRoute(nodos,mode);
            //printf("Alcanzado destino:\n-->\n");
            //abiertos[mejor]->showPath();
        } else {
            abiertos[mejor]->expand(mode);
            cerrados.push_back(abiertos[mejor]);
            for (int i = 0; i < abiertos[mejor]->hijos.size(); i++) {
                if (abiertos[mejor]->hijos[i] != 0) {
                    if ((nInd = contenidoEn(abiertos, *abiertos[mejor]->hijos[i])) != 0) {
                        //insertarlo manteniendo info del mejor padre.
                        if (abiertos[nInd]->f(mode) > abiertos[mejor]->hijos[i]->f(mode)) {
                            //abiertos[nInd]->showInfo();
                            //abiertos[mejor]->hijos[i]->showInfo();
                            abiertos[nInd] = abiertos[mejor]->hijos[i];

                        }
                    } else if ((nInd = contenidoEn(cerrados, *abiertos[mejor]->hijos[i])) != 0) {
                        //insertarlo manteniendo info del mejor padre y actualizar info de descendientes
                        if (cerrados[nInd]->f(mode) > abiertos[mejor]->hijos[i]->f(mode)) {
                            //cerrados[nInd]->padre=abiertos[mejor]->hijos[i]->padre;
                            abiertos[mejor]->hijos[i]->hijos = cerrados[nInd]->hijos; //enlazo descendientes
                            cerrados[nInd] = abiertos[mejor]->hijos[i]; //sustituyo
                        }
                    } else {
                        abiertos.push_back(abiertos[mejor]->hijos[i]); //insertarlo como un nuevo nodo en abiertos
                    }
                }
            }
            abiertos.erase(abiertos.begin() + mejor);

        }
        if (abiertos.empty()) {
            fin = true;
            printf("No se ha alcanzado destino f:%i, c:%i ...\nNo se converge a la solucion.\n", destino->fil, destino->col);
            nodos.clear();
            nodos.push_back(*inicio);
            return;
            //cin.get();
            //exit(0);
        }
    }


}


void show(const vector<node> & nodos){
    for(int i=0;i<nodos.size();i++){
        nodos[i].show();
    }
}
