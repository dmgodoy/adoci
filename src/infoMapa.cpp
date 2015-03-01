/* 
 * File:   infoMapa.cpp
 * Author: Angel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "infoMapa.h"

hexagono_t::hexagono_t(int fil, int col) {
    fila = fil;
    columna = col;
}

infoMapa::infoMapa(infoMechs* mechs) {
    filas = 0;
    columnas = 0;
    mapa = 0;
    info_mechs = mechs;
}

infoMapa::infoMapa(const infoMapa& orig) {
}

infoMapa::~infoMapa() {
    for (int i = 1; i < filas; i++)
        for (int j = 1; j < columnas; j++)
            delete mapa[i][j];
    for (int i = 1; i < filas; i++)
        delete[] mapa[i];
    delete[] mapa;
}

void infoMapa::inicializarMapa() {

    mapa = new hexagono_t**[filas];
    if (!mapa) {
        cerr << "\nError en la reserva de memoria\n";
        exit(1);
    }
    for (int i = 0; i < filas; i++) {
        if (i == 0)
            mapa[i] = 0;
        else {
            mapa[i] = new hexagono_t*[columnas];
            if (!mapa[i]) {
                cerr << "\nError en la reserva de memoria\n";
                exit(1);
            }
        }
    }
    for (int i = 1; i < filas; i++)
        for (int j = 0; j < columnas; j++) {
            if (j == 0)
                mapa[i][j] = 0;
            else
                mapa[i][j] = new hexagono_t(i, j);

        }
}

void infoMapa::leeFich(string fichero) {
    string cad;
    ifstream fich(fichero.c_str()); //Creacion y apertura
    if (!fich) {
        cerr << "\nNo es posible abrir " << fichero << "\n";
        exit(1);
    }
    fich.ignore(1000, '\n'); //Ignoramos numero magico

    fich >> filas;
    filas = filas + 1;
    fich >> columnas;
    columnas = columnas + 1;
    //Para simplificar anhadiremos una fila y una columna de mas, ya que
    //en el mapa las filas y las columnas empiezan en 1 y en la matriz en 0
    if (!mapa)
        inicializarMapa();

    for (int j = 1; j < columnas; j++)
        for (int i = 1; i < filas; i++) {
            fich >> mapa[i][j]->nivel;
            fich >> mapa[i][j]->terreno;
            fich >> mapa[i][j]->objetoTerreno;
            fich >> mapa[i][j]->FCE;
            fich >> cad;
            mapa[i][j]->derrumbado = s2bool(cad);
            fich >> cad;
            mapa[i][j]->fuego = s2bool(cad);
            fich >> cad;
            mapa[i][j]->humo = s2bool(cad);
            fich >> mapa[i][j]->num_garrotes;
            for (int k = 0; k < 6; k++) {
                fich >> cad;
                mapa[i][j]->rioCentroCara[k] = s2bool(cad);
            }
            for (int k = 0; k < 6; k++) {
                fich >> cad;
                mapa[i][j]->carreteraCentroCara[k] = s2bool(cad);
            }
        }

}

void infoMapa::encarar_objetivo(int f, int c, int f_obj, int c_obj, int & lado) {
    if (f < f_obj) {//Los de arriba
        if (c < c_obj) //los de la izq
            lado = 6;
        else if (c > c_obj)
            lado = 2;
        else lado = 1;
    } else {//los de abajo
        if (c < c_obj) //los de la izq
            lado = 5;
        else if (c > c_obj)
            lado = 3;
        else
            lado = 4;
    }
}
bool infoMapa::linea_vision(int num_jugador, const hexagono_pos & origen, int nivel_origen, const hexagono_pos & destino, int nivel_destino) {
    char orden[100];
    string linea;
    ifstream fichero_LDV;

    snprintf(orden, 99, "LDVyC.exe mapaJ%i.sbt %02i%02i %i %02i%02i %i", num_jugador, origen.columna,
            origen.fila, nivel_origen, destino.columna, destino.fila, nivel_destino);

    system(orden);

    do {
        fichero_LDV.open("LDV.sbt");
    } while (fichero_LDV == NULL);

    leeLinea(fichero_LDV, linea, '\n');
    leeLinea(fichero_LDV, linea, '\n');

    fichero_LDV.close();

    if (linea.compare(0,4,"True") == 0)
        return true;
    else
        return false;
}
int infoMapa::direccion_objetivo(hexagono_pos origen, hexagono_pos destino) {
    /* Estrategia: Si la casilla objetivo esta en la misma columna, la direccion sera
       1 (si esta arriba) o 4 (si esta abajo). Si estaa en distinta fila y distinta
       columna, la direccian sera la diagonal mas apropiada
     */
    if (origen.columna == destino.columna) {
        /* El mech objetivo esta en la misma columna que el nuestro */
        if (origen.fila < destino.fila)
            return 4; /* Abajo */
        else
            return 1; /* Arriba */
    } else {
        if ((origen.columna % 2) == 0) {
            /* Columna par */
            if (origen.columna < destino.columna) {
                if (origen.fila >= destino.fila)
                    return 2; /* Diagonal arriba derecha */
                else
                    return 3; /* Diagonal abajo derecha */
            } else {
                if (origen.fila >= destino.fila)
                    return 6; /* Diagonal arriba izquierda */
                else
                    return 5; /* Diagonal abajo izquierda */
            }
        } else {
            /* Columna impar */
            if (origen.columna < destino.columna) {
                if (origen.fila > destino.fila)
                    return 2; /* Diagonal arriba derecha */
                else
                    return 3; /* Diagonal abajo derecha */
            } else {
                if (origen.fila > destino.fila)
                    return 6; /* Diagonal arriba izquierda */
                else
                    return 5; /* Diagonal abajo izquierda */
            }
        }
    }
    return 0;
}

int infoMapa::coste_mov_giro(int f, int c) {
    if (mapa[f][c]->terreno == PANTANO)
        return 2;
    return 1; //adaptar a cada hexagono
}

/* Devuelve el coste del cambio de casilla. Si el movimiento es prohibido
 * devuelve 99999 (MOVIMIENTO_INVALIDO)
 */
int infoMapa::coste_mov_ady(int f_origen, int c_origen, int f_objetivo, int c_objetivo, int toneladas, int modo) {

    int coste = 0,
            col_ini = c_origen,
            fil_ini = f_origen,
            col_obj = c_objetivo,
            fil_obj = f_objetivo,
            dif_nivel;

    /* Coste del tipo de terreno */

    //Si la pos esta ocupada, mov invalido
    for (int i = 0; i < info_mechs->nMechs - 1; i++)
        if (info_mechs->iMechVector[i]->pos_Hexagono.columna == c_objetivo &&
                info_mechs->iMechVector[i]->pos_Hexagono.fila == f_objetivo)
            return MOVIMIENTO_INVALIDO;

    switch (mapa[fil_obj][col_obj]->terreno) {
        case 0://ABIERTO:
            coste++;
            break;
        case 1://PAVIMENTADO:
            coste++;
            break;
        case 2://AGUA:
            if (mapa[fil_obj][col_obj]->nivel >= 0)
                coste++;
            else if (mapa[fil_obj][col_obj]->nivel == -1)
                coste += 2;
            else
                coste += 4;
            break;
        case 3://PANTANO:
            coste += 2;
            break;
    }
    /* Coste de objetos en el terreno */

    switch (mapa[fil_obj][col_obj]->objetoTerreno) {
            //0-escombros, 1-bosque ligero, 2-bosque denso,
            //3-edificio ligero,4-edificio medio, 5-edificio pesado,
            //6-edificio reforzado, 7-bunker, 255-nada
        case 0://ESCOMBROS:
            coste += 2;
            break;
        case 1://B_LIGERO:
            coste += 2;
            break;
        case 2://B_DENSO:
            coste += 3;
            break;
        case 3://E_LIGERO:
        case 4://E_MEDIO:
        case 5://E_PESADO:
        case 6://E_REFORZADO:
        case 7://BUNKER:
            if (mapa[fil_obj][col_obj]->FCE < toneladas)
                return MOVIMIENTO_INVALIDO;
            break;
    }
    /* Coste del cambio de elevacion */
    dif_nivel = abs(mapa[fil_ini][col_ini]->nivel - mapa[fil_obj][col_obj]->nivel);
    if (modo == WALK && dif_nivel != 0)
        return MOVIMIENTO_INVALIDO;
    switch (dif_nivel) {
        case 0:
            break;
        case 1:
            coste++;
            break;
        case 2:
            coste += 2;
            break;
        default:
            return MOVIMIENTO_INVALIDO;
    }
    return coste;
}

bool infoMapa::pos_valida(int f, int c, int peso) {
    if (f < 1 || f >= filas || c < 1 || c >= columnas)
        return false;
    if ((mapa[f][c]->FCE < peso) &&
            (mapa[f][c]->objetoTerreno == E_LIGERO ||
            mapa[f][c]->objetoTerreno == E_MEDIO ||
            mapa[f][c]->objetoTerreno == E_PESADO ||
            mapa[f][c]->objetoTerreno == E_REFORZADO ||
            mapa[f][c]->objetoTerreno == BUNKER))
        return false;
    for (int i = 0; i < info_mechs->nMechs - 1; i++)
        if (info_mechs->iMechVector[i]->pos_Hexagono.fila == f && info_mechs->iMechVector[i]->pos_Hexagono.columna == c)
            return false; //casilla ocupada
    return true;
}
/* IMPORTANTE: Que esta funcion devuelva FALSE si la casilla objetivo se sale del tablero
 * y TRUE si no se sale */

/* Esta funcion modifica el array objetivo */
bool infoMapa::casilla_objetivo(hexagono_pos casilla, int direccion, hexagono_pos & objetivo) {
    switch (direccion) {
        case 1:
            objetivo.columna = casilla.columna;
            objetivo.fila = casilla.fila - 1;
            break;
        case 2:
            if (casilla.columna % 2 == 0) {
                objetivo.columna = casilla.columna + 1;
                objetivo.fila = casilla.fila;
            } else {
                objetivo.columna = casilla.columna + 1;
                objetivo.fila = casilla.fila - 1;
            }
            break;
        case 3:
            if (casilla.columna % 2 == 0) {
                objetivo.columna = casilla.columna + 1;
                objetivo.fila = casilla.fila + 1;
            } else {
                objetivo.columna = casilla.columna + 1;
                objetivo.fila = casilla.fila;
            }
            break;
        case 4:
            objetivo.columna = casilla.columna;
            objetivo.fila = casilla.fila + 1;
            break;
        case 5:
            if (casilla.columna % 2 == 0) {
                objetivo.columna = casilla.columna - 1;
                objetivo.fila = casilla.fila + 1;
            } else {
                objetivo.columna = casilla.columna - 1;
                objetivo.fila = casilla.fila;
            }
            break;
        case 6:
            if (casilla.columna % 2 == 0) {
                objetivo.columna = casilla.columna - 1;
                objetivo.fila = casilla.fila;
            } else {
                objetivo.columna = casilla.columna - 1;
                objetivo.fila = casilla.fila - 1;
            }
            break;
    }
    if ((objetivo.columna < 1) ||
            (objetivo.fila < 1) ||
            (objetivo.columna >= columnas) ||
            (objetivo.fila >= filas))
        return false;
    else
        return true;
}

int infoMapa::distancia_casillas(hexagono_pos casilla_ini, hexagono_pos casilla_obj) {
    hexagono_pos casilla_aux;

    if (casilla_ini.columna == casilla_obj.columna && casilla_ini.fila == casilla_obj.fila)
        return 0;
    else {
        if (casilla_objetivo(casilla_ini, direccion_objetivo(casilla_ini, casilla_obj), casilla_aux))
            return 1 + distancia_casillas(casilla_aux, casilla_obj);
        else
            return -1;
    }
}

void nodoEnEsaDireccion(int fil, int col, int direccion, int & filaSiguiente, int & colSiguiente) {
    if (direccion == 1 || direccion == 4) {
        if (direccion == 1)
            filaSiguiente = fil - 1;

        else if (direccion == 4)
            filaSiguiente = fil + 1;
        colSiguiente = col;

    } else if (col % 2 == 0) {//col par
        switch (direccion) {
            case 2:
                filaSiguiente = fil;
                colSiguiente = col + 1;
                break;
            case 3:
                filaSiguiente = fil + 1;
                colSiguiente = col + 1;
                break;
            case 5:
                filaSiguiente = fil + 1;
                colSiguiente = col - 1;
                break;
            case 6:
                filaSiguiente = fil;
                colSiguiente = col - 1;
                break;
            default:
                cout << "Orientacin invalida!!" << endl;
                cout << "Orientacion: " << direccion << endl;
                exit(0);
        }

    } else {
        switch (direccion) {
            case 2:
                filaSiguiente = fil - 1;
                colSiguiente = col + 1;
                break;
            case 3:
                filaSiguiente = fil;
                colSiguiente = col + 1;
                break;
            case 5:
                filaSiguiente = fil;
                colSiguiente = col - 1;
                break;
            case 6:
                filaSiguiente = fil - 1;
                colSiguiente = col - 1;
                break;
            default:
                cout << "Orientacion " << direccion << "invalida" << endl;
                exit(0);
        }
    }
}

void nodoEnEspalda(int fil, int col, int direccion, int & filAnterior, int & colAnterior) {
    if (direccion == 1 || direccion == 4) {
        if (direccion == 1)
            filAnterior = fil + 1;

        else if (direccion == 4)
            filAnterior = fil - 1;
        colAnterior = col;

    } else if (col % 2 == 0) {//col par
        switch (direccion) {
            case 2:
                filAnterior = fil + 1;
                colAnterior = col - 1;
                break;
            case 3:
                filAnterior = fil;
                colAnterior = col - 1;
                break;
            case 5:
                filAnterior = fil;
                colAnterior = col + 1;
                break;
            case 6:
                filAnterior = fil + 1;
                colAnterior = col + 1;
                break;
            default:
                cout << "Orientacin invalida!!" << endl;
                cout << "Orientacion: " << direccion << endl;
                exit(0);
        }

    } else {
        switch (direccion) {
            case 2:
                filAnterior = fil;
                colAnterior = col - 1;
                break;
            case 3:
                filAnterior = fil - 1;
                colAnterior = col - 1;
                break;
            case 5:
                filAnterior = fil - 1;
                colAnterior = col + 1;
                break;
            case 6:
                filAnterior = fil;
                colAnterior = col + 1;
                break;
            default:
                cout << "Orientacion invalida" << endl;
                exit(0);
        }
    }
}

bool adyacente(int fila, int columna, int fila2, int columna2) {
    int f, c;
    for (int i = 1; i <= 6; i++) {
        nodoEnEsaDireccion(fila, columna, i, f, c);
        if (fila2 == f && columna2 == c)
            return true;
    }
    return false;
}
