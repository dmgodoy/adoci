/* 
 * File:   nodoArea.cpp
 * Author: Angel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "nodoArea.h"
#include "node.h"

nodoArea::nodoArea() {
}

nodoArea::nodoArea(int f, int c, float d) {
    fila = f;
    columna = c;
    val = d;
}

nodoArea::nodoArea(const nodoArea& orig) {
    fila = orig.fila;
    columna = orig.columna;
    val = orig.val;
}

nodoArea::~nodoArea() {
}

bool nodoArea::adyacenteNodoArea(const nodoArea & n, infoMapa* mapa)const {
    if (adyacente(fila, columna, n.fila, n.columna) == false)
        return false;

    int toneladas = mapa->info_mechs->mechJugador->defMechInfo->toneladas;
    int coste = mapa->coste_mov_ady(fila, columna, n.fila, n.columna, toneladas);
    if (coste != MOVIMIENTO_INVALIDO)
        return true;

}

void nodoArea::show()const {
    printf("Fila: %i, Columna %i, Distancia: %f\n", fila, columna, distancia);
}

bool nodoArea::operator==(const nodoArea & n)const {
    return fila == n.fila && columna == n.columna;

}

bool nodoArea::pertenece(const vector<nodoArea> & nodos)const {
    for (int i = 0; i < nodos.size(); i++) {
        if (*this == nodos[i])
            return true;
    }
    return false;
}

vector<nodoArea> nodoArea::getAdyacentes(infoMapa * mapa)const {
    vector<nodoArea> adyacentes;
    int f, c;
    for (int i = 1; i <= 6; i++) {
        nodoEnEsaDireccion(fila, columna, i, f, c);
        if (f >= 1 && f < mapa->filas && c >= 1 && c < mapa->columnas)
            adyacentes.push_back(nodoArea(f, c, -1));
    }
    return adyacentes;
}

vector<nodoArea> adyacentes(const vector<nodoArea> & anillo, infoMapa* mapa) {
    vector<nodoArea> tmp;
    vector<nodoArea> nodos_adyacentes;
    for (int i = 0; i < anillo.size(); i++) {
        tmp = anillo[i].getAdyacentes(mapa);
        for (int j = 0; j < tmp.size(); j++)
            if (tmp[j].pertenece(nodos_adyacentes) == false)
                nodos_adyacentes.push_back(tmp[j]);
    }
    return nodos_adyacentes;
}

vector<nodoArea> getAnillo(const vector<nodoArea> & anillo, const vector<nodoArea> & anillo_interior, infoMapa * mapa) {

    vector<nodoArea> anilloExterior;
    vector<nodoArea> ady = adyacentes(anillo, mapa);
    vector<nodoArea> ady_int = adyacentes(anillo_interior, mapa);

    for (int i = 0; i < ady.size(); i++) {
        if (ady[i].pertenece(ady_int) == false && ady[i].pertenece(anillo_interior) == false) {
            anilloExterior.push_back(ady[i]);
        }
    }
    return anilloExterior;
}

void showNodosArea(const std::vector<nodoArea>& nodos) {
    for (int i = 0; i < nodos.size(); i++)
        nodos[i].show();

}

vector<nodeVector> getAnillos(const nodoArea & centro, infoMapa * mapa) {

    vector<nodeVector> matrizAnillos;
    matrizAnillos.resize(9);
    vector<nodoArea> vacio;
    vacio.clear();

    //vector<nodeVector> anillo_centro;
    matrizAnillos[0].push_back(centro);
    matrizAnillos[1] = getAnillo(matrizAnillos[0], vacio, mapa);
    matrizAnillos[2] = getAnillo(matrizAnillos[1], matrizAnillos[0], mapa);
    matrizAnillos[3] = getAnillo(matrizAnillos[2], matrizAnillos[1], mapa);
    matrizAnillos[4] = getAnillo(matrizAnillos[3], matrizAnillos[2], mapa);
    matrizAnillos[5] = getAnillo(matrizAnillos[4], matrizAnillos[3], mapa);
    matrizAnillos[6] = getAnillo(matrizAnillos[5], matrizAnillos[4], mapa);
    matrizAnillos[7] = getAnillo(matrizAnillos[6], matrizAnillos[5], mapa);
    matrizAnillos[8] = getAnillo(matrizAnillos[7], matrizAnillos[6], mapa);

    return matrizAnillos;
}

void nodoArea::evaluaHuir(int f, int c, int enc) {
    float d = distancia_hexagonal(fila, columna, f, c);
    evaluacionHuir = d;
}

void nodoArea::evaluaDefensa(int f_obj, int c_obj, int enc, float dist_seg, infoMapa* mapa) {
    if (dist_seg == -1) {
        evaluacionDefensa = -1;
        return;
    }
    float d = distancia_hexagonal(fila, columna, f_obj, c_obj);
    float puntuaD, puntuaLDV;
    if (d < dist_seg / 2)puntuaD = 3 * (((dist_seg / 2)-(dist_seg / 2 - d)) / (dist_seg / 2)); //de 0 a 3
    else if (d <= dist_seg)puntuaD = 10 * ((dist_seg - (dist_seg * 0.7 - d * 0.7)) / dist_seg); //de 3 a 10
    else if (d > dist_seg)puntuaD = ((6 - (d - dist_seg)) / 6)*10.0; //de 10 a 0
    if (puntuaD < 0)puntuaD = 0;
    int numJ = mapa->info_mechs->mechJugador->numJ;
    if (mapa->linea_vision(numJ, hexagono_pos(fila, columna), 1, hexagono_pos(f_obj, c_obj), 1))
        puntuaLDV = 10;
    else
        puntuaLDV = 0;

    evaluacionDefensa = puntuaLDV * 0.3 + puntuaD * 0.7;
}

void posAtaque(int niveles, vector<nodeVector> & anillosJugador, vector<nodeVector> & anillosObjetivo, int & fil_dest, int & col_dest, infoMapa * mapa) {
    int toneladas = mapa->info_mechs->mechJugador->defMechInfo->toneladas;
    int filAnillo, colAnillo;
    if (niveles > NUM_ANILLOS)
        niveles = NUM_ANILLOS;

    fil_dest = anillosJugador[0][0].fila;
    col_dest = anillosJugador[0][0].columna;
    int fil_mech_obj = anillosObjetivo[0][0].fila;
    int col_mech_obj = anillosObjetivo[0][0].columna;
    int nivel_obj = mapa->mapa[fil_mech_obj][col_mech_obj]->nivel;
    int dif_nivel = 0;
    int min_dif=1000;
    for (int j = 0; j < anillosObjetivo[1].size(); j++) {
        filAnillo = anillosObjetivo[1][j].fila;
        colAnillo = anillosObjetivo[1][j].columna;
        if (mapa->pos_valida(filAnillo, colAnillo, toneladas) && !mapa->mapa[filAnillo][colAnillo]->fuego) {
            dif_nivel = mapa->mapa[filAnillo][colAnillo]->nivel - nivel_obj;
            if (dif_nivel < 0)
                dif_nivel = -dif_nivel;


            if (dif_nivel < min_dif) {
                min_dif=dif_nivel;
                fil_dest = filAnillo;
                col_dest = colAnillo;
                //return;
            }
        }
    }

    //    for (int i = anillosObjetivo.size() - 1; i > 0; i--)
    //        for (int j = 0; j < anillosObjetivo[i].size(); j++) {
    //            filAnillo = anillosObjetivo[i][j].fila;
    //            colAnillo = anillosObjetivo[i][j].columna;
    //            if (mapa->pos_valida(filAnillo, colAnillo, toneladas) && !mapa->mapa[filAnillo][colAnillo]->fuego) {
    //                fil_dest = anillosObjetivo[i][j].fila;
    //                col_dest = anillosObjetivo[i][j].columna;
    //                if (perteneceAnillos(niveles, anillosJugador, fil_dest, col_dest)) {
    //                    f = fil_dest;
    //                    c = col_dest;
    //                }
    //            }
    //        }
    //    if (f != -1 && c != -1) {
    //        fil_dest = f;
    //        col_dest = c;
    //    }
}

bool perteneceAnillos(int niveles, vector<nodeVector> & anillos, int f, int c) {
    if (niveles > anillos.size() - 1)
        niveles = anillos.size() - 1;
    nodoArea n(f, c, -1);
    for (int i = 0; i <= niveles; i++)
        if (n.pertenece(anillos[i]))
            return true;
    return false;
}

/**
 * 
 * @param niveles Niveles de anillos a examinar
 * @param anillos anillos que rodean al mech jugador
 * @param fil_dest Devolvera fila dentro de estos anillos
 * @param col_dest Devolvera columna dentro de estos anillos
 * @param f_obj fila del mech objetivo
 * @param c_obj columna del mech objetivo
 * @param enc encaramiento del mech objetivo
 * @param mapa en el que nos movemos
 */
void cobertura(int niveles, vector<nodeVector> & anillos, int & fil_dest, int & col_dest,
        int f_obj, int c_obj, int enc, infoMapa * mapa, float dist_seg) {
    if (niveles > NUM_ANILLOS)
        niveles = NUM_ANILLOS;
    int toneladas = mapa->info_mechs->mechJugador->defMechInfo->toneladas;
    int filAnillo, colAnillo;
    fil_dest = anillos[0][0].fila;
    col_dest = anillos[0][0].columna;
    anillos[0][0].evaluaHuir(f_obj, c_obj, enc);
    anillos[0][0].evaluaDefensa(f_obj, c_obj, enc, dist_seg, mapa);


    if (mapa->mapa[fil_dest][col_dest]->fuego) {
        anillos[0][0].evaluacionDefensa = 0;
        anillos[0][0].evaluacionHuir = 0;
    }
    float mayor;
    if (dist_seg == -1) {
        mayor = anillos[0][0].evaluacionHuir;
    } else {
        mayor = anillos[0][0].evaluacionDefensa;
    }
    for (int i = 1; i <= niveles; i++) {
        for (int j = 0; j < anillos[i].size(); j++) {

            anillos[i][j].evaluaHuir(f_obj, c_obj, enc);
        }
    }

    for (int i = 1; i <= niveles; i++) {
        for (int j = 0; j < anillos[i].size(); j++) {
            filAnillo = anillos[i][j].fila;
            colAnillo = anillos[i][j].columna;
            if (dist_seg == -1) {
                if ((anillos[i][j].evaluacionHuir > mayor) &&
                        mapa->pos_valida(filAnillo, colAnillo, toneladas) &&
                        !mapa->mapa[filAnillo][colAnillo]->fuego) {
                    mayor = anillos[i][j].evaluacionHuir;
                    fil_dest = anillos[i][j].fila;
                    col_dest = anillos[i][j].columna;
                }
            } else {

                anillos[i][j].evaluaDefensa(f_obj, c_obj, enc, dist_seg, mapa);
                if ((anillos[i][j].evaluacionDefensa > mayor) &&
                        mapa->pos_valida(filAnillo, colAnillo, toneladas) &&
                        !mapa->mapa[filAnillo][colAnillo]->fuego) {
                    mayor = anillos[i][j].evaluacionDefensa;
                    fil_dest = anillos[i][j].fila;
                    col_dest = anillos[i][j].columna;
                    if (mayor >= 7.5)
                        return;

                }

            }
        }
    }

}

void coberturaSalto(int niveles, vector<nodeVector> & anillos, int & fil_dest, int & col_dest,
        int f_obj, int c_obj, int enc, infoMapa * mapa, float dist_seg) {
    if (niveles > NUM_ANILLOS)
        niveles = NUM_ANILLOS;
    int fil_anillo, col_anillo;
    int fil_mech = anillos[0][0].fila,
            col_mech = anillos[0][0].columna;

    fil_dest = fil_mech;
    col_dest = col_mech;

    anillos[0][0].evaluaHuir(f_obj, c_obj, enc);
    anillos[0][0].evaluaDefensa(f_obj, c_obj, enc, dist_seg, mapa);

    bool asignado = false;
    if (mapa->mapa[fil_dest][col_dest]->fuego) {
        anillos[0][0].evaluacionHuir = 0;
    }



    float mayor;
    if (dist_seg == -1) {
        mayor = anillos[0][0].evaluacionHuir;
    } else {
        mayor = anillos[0][0].evaluacionDefensa;
    }
    for (int i = 1; i <= niveles; i++) {
        for (int j = 0; j < anillos[i].size(); j++) {

            anillos[i][j].evaluaHuir(f_obj, c_obj, enc);

        }
    }
    for (int i = niveles; i >= 1; i--) {
        for (int j = 0; j < anillos[i].size(); j++) {
            fil_anillo = anillos[i][j].fila;
            col_anillo = anillos[i][j].columna;
            if (dist_seg == -1) {
                if (anillos[i][j].evaluacionHuir > mayor && check_salto(anillos[0][0], anillos[i][j], mapa)) {
                    asignado = true;
                    mayor = anillos[i][j].evaluacionHuir;
                    fil_dest = anillos[i][j].fila;
                    col_dest = anillos[i][j].columna;
                }
                if (niveles > 5 && asignado == true)
                    return;
            } else {


                anillos[i][j].evaluaDefensa(f_obj, c_obj, enc, dist_seg, mapa);

                if ((anillos[i][j].evaluacionDefensa > mayor) &&
                        check_salto(anillos[0][0], anillos[i][j], mapa)) {
                    fil_dest = anillos[i][j].fila;
                    col_dest = anillos[i][j].columna;
                    mayor = anillos[i][j].evaluacionDefensa;
                    if (mayor >= 7.5)
                        return;

                }

            }
        }
    }

}

vector<nodoArea> check_linea(const nodoArea & ini, const nodoArea & dest, int numJ) {

    char orden[100];
    ifstream fichero_LDV;

    vector<nodoArea> nodos;
    nodos.push_back(ini);
    snprintf(orden, 99, "LDVyC.exe mapaJ%i.sbt %02i%02i %i %02i%02i %i", numJ, ini.columna,
            ini.fila, 1, dest.columna, dest.fila, 1);

    system(orden);
    stringstream ss;

    do {
        fichero_LDV.open("LDV.sbt");
        //fichero_LDV = fopen("LDV.sbt", "r");
    } while (fichero_LDV == NULL);

    hexagono_pos hex;

    char line[100];
    fichero_LDV.getline(line, 100, '\n');
    string linea(line);

    string cad;
    ss << linea;
    while (ss >> cad) {
        hex.ini(cad);
        nodos.push_back(nodoArea(hex.fila, hex.columna, -1));
    }

    fichero_LDV.close();
    nodos.push_back(dest);


    return nodos;
}

void saltoIntermedio(int fil_ini, int col_ini, int fil_dest, int col_dest, int &fil, int &col, infoMapa* mapa, int PS) {
    int puntosSalto;
    if (PS > NUM_ANILLOS)
        puntosSalto = NUM_ANILLOS;
    else
        puntosSalto = PS;

    fil = -1;
    col = -1;
    int fil_d;
    int col_d;
    int filAnillo, colAnillo;
    float disAnillo;
    nodoArea inicio(fil_ini, col_ini, -1);
    nodoArea destino(fil_dest, col_dest, -1);
    if(check_salto(inicio,destino,mapa)){
        fil=fil_dest;
        col=col_dest;
        return;
    }
        
    float dist = distancia_hexagonal(fil_ini, col_ini, fil_dest, col_dest);
    float distMin=dist;
    vector<nodeVector> anillos_pos = getAnillos(inicio, mapa);
    for (int i = 1; i <= puntosSalto; i++)
        for (int j = 0; j < anillos_pos[i].size(); j++) {
            if (check_salto(inicio, anillos_pos[i][j],mapa)) {
                filAnillo = anillos_pos[i][j].fila;
                colAnillo = anillos_pos[i][j].columna;
                disAnillo = distancia_hexagonal(filAnillo, colAnillo, fil_dest, col_dest);
                if (check_salto(anillos_pos[i][j], destino,mapa)) {
                    fil = filAnillo;
                    col = colAnillo;
                    return;
                } else if(disAnillo < distMin){
                    distMin=disAnillo;
                    fil_d = filAnillo;
                    col_d = colAnillo;
                }
            }
        }
    if (fil == -1 && col == -1) {
        fil = fil_d;
        col = col_d;
    }

}

bool check_mov_salto(const vector<nodoArea>& nodos, int PS, infoMapa * mapa) {

    int nivel, minimo = 100, maximo = -100;
    for (int i = 0; i < nodos.size(); i++) {
        nivel = mapa->mapa[nodos[i].fila][nodos[i].columna]->nivel;
        if (nivel < minimo)
            minimo = nivel;
        if (nivel > maximo)
            maximo = nivel;
    }
    nivel = maximo - minimo;
    if (PS >= nivel)
        return true;

    return false;
}

bool check_salto(const nodoArea & ini, const nodoArea & dest, infoMapa * mapa) {
    int toneladas = mapa->info_mechs->mechJugador->defMechInfo->toneladas;
    if (!mapa->pos_valida(dest.fila, dest.columna, toneladas))
        return false;
    if (mapa->mapa[dest.fila][dest.columna]->fuego)
        return false;
    int numJ = mapa->info_mechs->mechJugador->numJ;
    int PS = mapa->info_mechs->mechJugador->dmj->PM_saltar;

    return check_mov_salto(check_linea(ini, dest, numJ), PS, mapa);

}



