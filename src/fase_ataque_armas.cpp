/* 
 * File:   fase_ataque_armas.cpp
 * Author: Angel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "fase_ataque_armas.h"


ataque_armas_t::ataque_armas_t(infoMapa* inf_mapa, infoMechs * inf_mechs) {
    mapa = inf_mapa;
    mechs = inf_mechs;
    coger_garrote = false;
    objetivo.fila = 0;
    objetivo.columna = 0;
    num_armas = 0;
    for (int i = 0; i < 20; i++) {
        armas_mech[i].localizacion = 0;
        armas_mech[i].slot = 0;
        armas_mech[i].doble_cadencia = false;
        armas_mech[i].loc_municion = -1;
        armas_mech[i].slot_municion = -1;
        armas_mech[i].objetivo.columna = 0;
        armas_mech[i].objetivo.fila = 0;
        armas_mech[i].tipo_objetivo = 0;
    }
}

void ataque_armas_t::salida(string numJ) {
    /* Crear el archivo de salida */
    char cad2[500];
    string cad = "accionJ" + numJ + ".sbt";
    string accion;
    string hexaux;
    ofstream out(cad.c_str());
    /* Linea 1: Coger garrote */
    switch (coger_garrote) {
        case true:
            sprintf(cad2,"%s : Vamos a coger un garrote para pelear cuerpo a cuerpo\n\n ",ctime(&tiempo));
            flog +=cad2;
            accion = accion + "True\n";
            break;
        case false:
            sprintf(cad2,"%s : No vamos a coger un garrote para pelear cuerpo a cuerpo\n\n ",ctime(&tiempo));
            flog +=cad2;
            accion = accion + "False\n";
            break;
    }

    /* Si se coge un garrote, terminar aqui */

    if (coger_garrote == true) {
        out << accion;
            cout<< accion <<endl;
    //cin.get();
        out.close();
        return;
    }

    hexaux = objetivo.stringPos(); 
    hexaux += "\n\n";
    /* Linea 2: Hexagono objetivo primario */
    sprintf(cad2,"%s : El hexagono objetivo primario es %02i%02i\n",ctime(&tiempo),objetivo.columna,objetivo.fila);
    flog +=cad2;

    flog += hexaux;
    accion = accion + objetivo.stringPos() + "\n";

    /* Linea 3: Numero de armas que se van a disparar */
    sprintf(cad2,"%s : Vamos a disparar %i armas\n\n ",ctime(&tiempo),num_armas);
    flog +=cad2;
    accion = accion + itoStr(num_armas) + "\n";
    

    /* Para cada arma a disparar */
    for (int i = 0; i < num_armas; ++i) {
        /* Localizacion del arma */

        switch (armas_mech[i].localizacion) {
            case 0:
                sprintf(cad2,"%s : El arma numero %i a disparar se encuentra en el brazo izquierdo \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "BI\n";
                break;
            case 1:
            case 8:
                sprintf(cad2,"%s : El arma numero %i a disparar se encuentra en el torso izquierdo \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "TI\n";
                break;
            case 2:
                sprintf(cad2,"%s : El arma numero %i a disparar se encuentra en la pierna izquierda \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "PI\n";
                break;
            case 3:
                sprintf(cad2,"%s : El arma numero %i a disparar se encuentra en la pierna derecha \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "PD\n";
                break;
            case 4:
            case 9:
                sprintf(cad2,"%s : El arma numero %i a disparar se encuentra en el torso derecho \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "TD\n";
                break;
            case 5:
                sprintf(cad2,"%s : El arma numero %i a disparar se encuentra en el brazo derecho \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "BD\n";
                break;
            case 6:
            case 10:
                sprintf(cad2,"%s : El arma numero %i a disparar se encuentra en el torso central \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "TC\n";
                break;
            case 7:
                sprintf(cad2,"%s : El arma numero %i a disparar se encuentra en la cabeza \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "CAB\n";
                break;
        }

        /* Slot del arma dentro de la localizacion */
        sprintf(cad2,"%s : El arma numero %i a disparar esta en el slot %i \n\n ",ctime(&tiempo),i,armas_mech[i].slot);
        flog +=cad2;
        accion = accion + itoStr(armas_mech[i].slot) + "\n";

        /* Doble cadencia */
        switch (armas_mech[i].doble_cadencia) {
            case true:
                sprintf(cad2,"%s : El arma numero %i va a disparar con doble cadencia \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "True\n";
                break;
            case false:
                sprintf(cad2,"%s : El arma numero %i va a disparar sin doble cadencia \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "False\n";
                break;
        }

        /* Localizacion de la municion */

        switch (armas_mech[i].loc_municion) {
            case 0:
                sprintf(cad2,"%s : El arma numero %i a disparar tiene la municion en el brazo izquierdo \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "BI\n";
                break;
            case 1:
            case 8:
                sprintf(cad2,"%s : El arma numero %i a disparar tiene la municion en el torso izquierdo \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "TI\n";
                break;
            case 2:
                sprintf(cad2,"%s : El arma numero %i a disparar tiene la municion en la pierna izquierda \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "PI\n";
                break;
            case 3:
                sprintf(cad2,"%s : El arma numero %i a disparar tiene la municion en la pierna derecha \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "PD\n";
                break;
            case 4:
            case 9:
                sprintf(cad2,"%s : El arma numero %i a disparar tiene la municion en el torso derecho \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "TD\n";
                break;
            case 5:
                sprintf(cad2,"%s : El arma numero %i a disparar tiene la municion en el brazo derecho \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "BD\n";
                break;
            case 6:
            case 10:
                sprintf(cad2,"%s : El arma numero %i a disparar tiene la municion en el torso central \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "TC\n";
                break;
            case 7:
                sprintf(cad2,"%s : El arma numero %i a disparar tiene la municion en la cabeza \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "CAB\n";
                break;
            default:
                sprintf(cad2,"%s : El arma numero %i a disparar no tiene municion\n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "-1\n";
        }

        /* Slot de la municion dentro de la localizacion */
        sprintf(cad2,"%s : La municion del arma %i a disparar esta en el slot %i \n\n ",ctime(&tiempo),i,armas_mech[i].slot_municion);
        flog +=cad2;
        accion = accion + itoStr(armas_mech[i].slot_municion) + "\n";

        /* Hexagono objetivo del arma */
        sprintf(cad2,"%s : El hexagono objetivo del arma %i a disparar es %i%i \n\n ",ctime(&tiempo),i,armas_mech[i].objetivo.columna,armas_mech[i].objetivo.fila);
        flog +=cad2;
        accion = accion + armas_mech[i].objetivo.stringPos() + "\n";

        /* Tipo de objetivo */
        switch (armas_mech[i].tipo_objetivo) {
            case MECH:
                sprintf(cad2,"%s : El tipo de objetivo para el arma %i a disparar es un mech \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "Mech\n";
                break;
            case HEXAGONO:
                sprintf(cad2,"%s : El tipo de objetivo para el arma %i a disparar es un hexagono \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "Hexagono\n";

                break;
            case NINGUNO:
                sprintf(cad2,"%s : El tipo de objetivo para el arma %i a disparar es ninguno \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "Ninguno\n";
                break;
        }
    }
    /* Cerrar el archivo */
    out << accion;
    cout<< accion <<endl;
    //cin.get();
    out.close();
}

void ataque_armas_t::posiciones_adyacentes(int f, int c, int adyacentes[][2]) {
    //Arriba
    adyacentes[0][0] = f - 1;
    adyacentes[0][1] = c;
    //Abajo
    adyacentes[3][0] = f + 1;
    adyacentes[3][1] = c;

    if (c % 2 != 0) {
        adyacentes[1][0] = f - 1;
        adyacentes[1][1] = c + 1;

        adyacentes[2][0] = f;
        adyacentes[2][1] = c + 1;

        adyacentes[4][0] = f;
        adyacentes[4][1] = c - 1;

        adyacentes[5][0] = f - 1;
        adyacentes[5][1] = c - 1;
    } else {

        adyacentes[1][0] = f;
        adyacentes[1][1] = c + 1;

        adyacentes[2][0] = f + 1;
        adyacentes[2][1] = c + 1;

        adyacentes[4][0] = f + 1;
        adyacentes[4][1] = c - 1;

        adyacentes[5][0] = f;
        adyacentes[5][1] = c - 1;
    }
}

int ataque_armas_t::angulo_mech(int mech_obj) {
    int alto, ancho, fila_init;

    alto = mapa->filas;
    ancho = mapa->columnas;

    switch (mechs->mechJugador->encaramiento_torso) {
        case 1:
            /*Mirar adelante*/
            for (int i = 1; i < mechs->mechJugador->pos_Hexagono.fila; i++) {
                if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == mechs->mechJugador->pos_Hexagono.columna)
                        && (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == i)) {
                    return FRONTAL;
                }
            }
            /* Mirar las casillas en la diagonal superior izquierda */
            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila - 1)*2 - 2;
            else
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2 - 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna - 1; (i > 0) && ((fila_init / 2) > 0); --i, --fila_init) {
                for (int j = fila_init / 2; j > 0; --j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return FRONTAL;
                }
            }
            /* Mirar las casillas en la diagonal superior derecha */
            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila - 1)*2 - 2;
            else
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2 - 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna + 1; (i < mapa->columnas) && ((fila_init / 2) > 0); ++i, --fila_init) {
                for (int j = fila_init / 2; j > 0; --j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return FRONTAL;
                }
            }
            /* Mirar toda la linea de casillas que tiene detras */

            for (int i = mechs->mechJugador->pos_Hexagono.fila + 1; i < mapa->filas; ++i) {
                if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == mechs->mechJugador->pos_Hexagono.columna) &&
                        (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == i))
                    return TRASERO;
            }
            /* Mirar las casillas en la diagonal inferior izquierda */
            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila + 2)*2;
            else
                fila_init = (mechs->mechJugador->pos_Hexagono.fila + 1)*2 + 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna - 1; (i > 0) && ((fila_init / 2) < mapa->filas); --i, ++fila_init) {
                for (int j = fila_init / 2; j < mapa->columnas; ++j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return TRASERO;
                }
            }
            /* Mirar las casillas en la diagonal inferior derecha */
            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila + 2)*2;
            else
                fila_init = (mechs->mechJugador->pos_Hexagono.fila + 1)*2 + 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna + 1; (i < mapa->columnas) && ((fila_init / 2) < mapa->filas); ++i, ++fila_init) {
                for (int j = fila_init / 2; j < mapa->columnas; ++j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return TRASERO;
                }
            }
            /* Mirar si esta en los laterales */

            if (mechs->iMechVector[mech_obj]->pos_Hexagono.columna > mechs->mechJugador->pos_Hexagono.columna)
                return DERECHO;
            else
                return IZQUIERDO;
            break;
        case 2:
            /* Comprobar el angulo frontal */

            for (int i = 1; i < mechs->mechJugador->pos_Hexagono.fila; ++i) {
                if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == mechs->mechJugador->pos_Hexagono.columna) &&
                        (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == i))
                    return FRONTAL;
            }
            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila + 1)*2;
            else
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2 + 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna + 1; (i < mapa->columnas) && ((fila_init / 2) < mapa->filas); ++i, ++fila_init) {
                for (int j = fila_init / 2; j > 0; --j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return FRONTAL;
                }
            }
            /* Comprobar el angulo trasero */
            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila + 1)*2;
            else
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2 + 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna - 1; (i > 0) && ((fila_init / 2) > 0); --i, --fila_init) {
                for (int j = fila_init / 2; j < mapa->filas; ++j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return TRASERO;
                }
            }
            /* Comprobar los angulos laterales */
            if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna > mechs->mechJugador->pos_Hexagono.columna) ||
                    (mechs->iMechVector[mech_obj]->pos_Hexagono.fila > mechs->mechJugador->pos_Hexagono.fila))
                return DERECHO;
            else
                return IZQUIERDO;
            break;
        case 3:
            /* Comprobar el angulo frontal */
            for (int i = mechs->mechJugador->pos_Hexagono.fila + 1; i < mapa->filas; ++i) {
                if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == mechs->mechJugador->pos_Hexagono.columna) &&
                        (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == i))
                    return FRONTAL;
            }
            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2;
            else
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2 - 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna + 1; (i < mapa->columnas) && ((fila_init / 2) > 0); ++i, --fila_init) {
                for (int j = fila_init / 2; j < mapa->filas; ++j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return FRONTAL;
                }
            }
            /* Comprobar el angulo trasero */
            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2;
            else
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2 - 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna - 1; (i > 0) && ((fila_init / 2) < mapa->filas); --i, ++fila_init) {
                for (int j = fila_init / 2; j > 0; --j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return TRASERO;
                }
            }
            /* Comprobar los angulos laterales */
            if (mechs->iMechVector[mech_obj]->pos_Hexagono.fila < mechs->mechJugador->pos_Hexagono.fila)
                return IZQUIERDO;
            else
                return DERECHO;
            break;
        case 4:
            /* angulo frontal */
            for (int i = mechs->mechJugador->pos_Hexagono.fila + 1; i < mapa->filas; ++i) {
                if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == mechs->mechJugador->pos_Hexagono.columna) &&
                        (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == i))
                    return FRONTAL;
            }

            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila + 1)*2;
            else
                fila_init = (mechs->mechJugador->pos_Hexagono.fila)*2 + 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna - 1; (i > 0) && ((fila_init / 2) < mapa->filas); --i, ++fila_init) {
                for (int j = fila_init / 2; j < mapa->columnas; ++j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return FRONTAL;
                }
            }

            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila + 1)*2;
            else
                fila_init = (mechs->mechJugador->pos_Hexagono.fila)*2 + 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna + 1; (i < mapa->columnas) && ((fila_init / 2) < mapa->filas); ++i, ++fila_init) {
                for (int j = fila_init / 2; j < mapa->columnas; ++j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return FRONTAL;
                }
            }

            /* angulo trasero */
            for (int i = 1; i < mechs->mechJugador->pos_Hexagono.fila; ++i) {
                if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == mechs->mechJugador->pos_Hexagono.columna) &&
                        (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == i))
                    return TRASERO;
            }

            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila - 1)*2;
            else
                fila_init = (mechs->mechJugador->pos_Hexagono.fila - 2)*2 + 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna - 1; (i > 0) && ((fila_init / 2) > 0); --i, --fila_init) {
                for (int j = fila_init / 2; j > 0; --j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return TRASERO;
                }
            }

            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila - 1)*2;
            else
                fila_init = (mechs->mechJugador->pos_Hexagono.fila - 2)*2 + 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna + 1; (i < mapa->columnas) && ((fila_init / 2) > 0); ++i, --fila_init) {
                for (int j = fila_init / 2; j > 0; --j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return TRASERO;
                }
            }
            /* angulos laterales */
            if (mechs->iMechVector[mech_obj]->pos_Hexagono.columna > mechs->mechJugador->pos_Hexagono.columna)
                return IZQUIERDO;
            else
                return DERECHO;
            break;
        case 5:
            /* angulo frontal */
            for (int i = mechs->mechJugador->pos_Hexagono.fila + 1; i < mapa->filas; ++i) {
                if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == mechs->mechJugador->pos_Hexagono.columna) &&
                        (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == i))
                    return FRONTAL;
            }

            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila - 1)*2 - 2;
            else
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2 - 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna - 1; (i > 0) && ((fila_init / 2) > 0); --i, --fila_init) {
                for (int j = fila_init / 2; j < mapa->filas; ++j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return FRONTAL;
                }
            }
            /* angulo trasero */
            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila - 1)*2 - 2;
            else
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2 - 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna + 1; (i < mapa->columnas) && ((fila_init / 2) < mapa->filas); ++i, ++fila_init) {
                for (int j = fila_init / 2; j > 0; --j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return TRASERO;
                }
            }
            /* angulos laterales */
            if (mechs->iMechVector[mech_obj]->pos_Hexagono.columna > mechs->mechJugador->pos_Hexagono.columna)
                return IZQUIERDO;
            else
                return DERECHO;
            break;
        case 6:
            /* angulo frontal */
            for (int i = 1; i < mechs->mechJugador->pos_Hexagono.fila; ++i) {
                if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == mechs->mechJugador->pos_Hexagono.columna) &&
                        (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == i))
                    return FRONTAL;
            }

            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.fila + 1)*2;
            else
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2 + 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna - 1; (i > 0) && ((fila_init / 2) < mapa->filas); --i, ++fila_init) {
                for (int j = fila_init / 2; j > 0; --j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return FRONTAL;
                }
            }
            /* angulo trasero */
            if ((mechs->mechJugador->pos_Hexagono.columna % 2) == 0)
                fila_init = (mechs->mechJugador->pos_Hexagono.columna + 1)*2;
            else
                fila_init = mechs->mechJugador->pos_Hexagono.fila * 2 + 1;
            for (int i = mechs->mechJugador->pos_Hexagono.columna + 1; (i < mapa->columnas) && ((fila_init / 2) > 0); ++i, --fila_init) {
                for (int j = fila_init / 2; j < mapa->columnas; ++j) {
                    if ((mechs->iMechVector[mech_obj]->pos_Hexagono.columna == i) &&
                            (mechs->iMechVector[mech_obj]->pos_Hexagono.fila == j))
                        return TRASERO;
                }
            }
            /* angulos laterales */
            if (mechs->iMechVector[mech_obj]->pos_Hexagono.columna > mechs->mechJugador->pos_Hexagono.columna)
                return DERECHO;
            else
                return IZQUIERDO;
            break;
    }
    return 0; /* No se alcanza nunca */


}

int ataque_armas_t::objetivoArmas() {
    int auxColumna;
    int auxFila;
    int columnaJugador;
    int filaJugador;
    int mech_mas_debil;
    float score_objetivo[mechs->nMechs - 1]; //Aqui se guardara en cada dimension el Score de cada objetivo para elegir el mejor al que disparar

    int armaduraux = 0;
    int armaduraminima = 100000;
    int objetivo;
    float scoreaux;

    columnaJugador = mechs->mechJugador->pos_Hexagono.columna;
    filaJugador = mechs->mechJugador->pos_Hexagono.fila;

    for (int i = 0; i < mechs->nMechs - 1; i++) {
        score_objetivo[i] = 0;
    }

    for (int i = 0; i < mechs->nMechs - 1; i++) {
        auxColumna = mechs->iMechVector[i]->pos_Hexagono.columna;
        auxFila = mechs->iMechVector[i]->pos_Hexagono.fila;
        if (mechs->iMechVector[i]->operativo == true){ 
            score_objetivo[i] = distancia_hexagonal(filaJugador, columnaJugador, auxFila, auxColumna);//Se pone como Score la distancia del Mech con respecto al nuestro
        }
        
        armaduraux += mechs->iMechVector[i]->blindaje.BD;
        armaduraux += mechs->iMechVector[i]->blindaje.BI;
        armaduraux += mechs->iMechVector[i]->blindaje.CAB;
        armaduraux += mechs->iMechVector[i]->blindaje.PATC;
        armaduraux += mechs->iMechVector[i]->blindaje.PATD;
        armaduraux += mechs->iMechVector[i]->blindaje.PATI;
        armaduraux += mechs->iMechVector[i]->blindaje.PD;
        armaduraux += mechs->iMechVector[i]->blindaje.PI;
        armaduraux += mechs->iMechVector[i]->blindaje.TC;
        armaduraux += mechs->iMechVector[i]->blindaje.TD;
        armaduraux += mechs->iMechVector[i]->blindaje.TI;

        if (armaduraux < armaduraminima) {
            armaduraminima = armaduraux;
            mech_mas_debil = i;
        }
        armaduraux = 0;
    }

    score_objetivo[mech_mas_debil] -= 1; //Si hay dos mechs a igual distancia pero uno esta mas danhado que otro disparamos a este

    scoreaux = score_objetivo[0];
    objetivo = 0;

    for (int i = 1; i < mechs->nMechs-1; i++) {
        if (score_objetivo[i] < scoreaux) {
            scoreaux = score_objetivo[i];
            objetivo = i;
        }
    }
    return objetivo;
}

bool ataque_armas_t::linea_vision(int num_jugador, const hexagono_pos & origen, int nivel_origen, const hexagono_pos & destino, int nivel_destino) {
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

bool ataque_armas_t::queda_municion(const iMech &mech, int codigo) {
  
    for (int k = 0; k < mech.defMechInfo->num_componentes; ++k) {
        if ((mech.defMechInfo->componentes[k].clase == MUNICION) && (mech.defMechInfo->componentes[k].codigoArma == codigo) && (mech.defMechInfo->componentes[k].cantidad > 0) && (mech.defMechInfo->componentes[k].operativo == true))
            return true;
    }
    return false;
}

bool ataque_armas_t::buscar_municion(const iMech &mech, int cod_arma) {
    int cod_municion = 0;

    for (int i = 0; i < mech.defMechInfo->num_componentes; ++i) { //Si encuentra el codigo parar

        if (mech.defMechInfo->componentes[i].codigoArma == cod_arma)
            cod_municion = mech.defMechInfo->componentes[i].codigo;
    }
    if (cod_municion != 0) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < mech.defMechInfo->localizaciones[i].slots_ocupados; ++j) {
                if ((mech.defMechInfo->localizaciones[i].slots[j].codigo == cod_municion) &&
                        (mech.defMechInfo->localizaciones[i].slots[j].cantidad > 0)) {
                    armas_mech[num_armas].loc_municion = i;
                    armas_mech[num_armas].slot_municion = j;
                    return true;
                }
            }
        }
    }
    return false;
}

int ataque_armas_t::obtenerLocalizacionMunicion(iMech *&mech, Componente_Mech arma) {
    int codigoMunicion = 0;

    for (int i = 0; i < mech->defMechInfo->num_componentes; i++) {
        if (mech->defMechInfo->componentes[i].codigoArma == arma.codigo)
            codigoMunicion = mech->defMechInfo->componentes[i].codigo;
    }

    if (codigoMunicion != 0) {
        for (int i = 0; i < 8; i++) { // Para cada localizacion
            for (int j = 0; j < mech->defMechInfo->localizaciones[i].slots_ocupados; j++) { // MIramos en cada slot
                if ((mech->defMechInfo->localizaciones[i].slots[j].codigo == codigoMunicion)
                        && (mech->defMechInfo->componentes[mech->defMechInfo->localizaciones[i].slots[j].indice_componente].cantidad > 0))
                    return i;
            }
        }
    }
    return -1;
}

int ataque_armas_t::obtenerSlotArma(iMech *&mech, Componente_Mech arma){
    for (int i = 0; i < mech->defMechInfo->localizaciones[arma.localizacion].slots_ocupados; i++) {
        if (mech->defMechInfo->localizaciones[arma.localizacion].slots[i].codigo == arma.codigo)
            return i;
    }
    return -1;
}

void ataque_armas_t::ataque_arma() {
    
        mechs->iMechVector[0]->pos_Hexagono;

    int filaux, colaux;
    int col_jugador = mechs->mechJugador->pos_Hexagono.columna;
    int fil_jugador = mechs->mechJugador->pos_Hexagono.fila;
    int adyacentes [6][2];
    int localizacion;
    int calordisipado = 0;
    int sumatoriaTemp = 0;
    bool ene_adyacente = false;
    int objetivo_mech, angulo;          

    posiciones_adyacentes(fil_jugador, col_jugador, adyacentes);

    //Consideramos enemigo adyacente si esta en una casilla al lado y el desnivel no es mayor que 1 (Le podemos pegar cuerpo a cuerpo).

    for (int i = 0; i < mechs->nMechs - 1; i++) {
        filaux = mechs->iMechVector[i]->pos_Hexagono.fila;
        colaux = mechs->iMechVector[i]->pos_Hexagono.columna;
        for (int j = 0; j < 6; j++) {
            if (adyacentes[j][0] == filaux && adyacentes[j][1] == colaux)
                if (abs(mapa->mapa[fil_jugador][col_jugador]->nivel - mapa->mapa[filaux][colaux]->nivel) <= 1)
                    ene_adyacente = true;
        }
    }
    //NUEVA estrategia armas
    if (ene_adyacente) {
        objetivo_mech = objetivoArmas();
        coger_garrote = false;
        objetivo.columna = 0;
        objetivo.fila = 0;
        num_armas = 0;

    } else { //Si no vamos a pegar fisicamente usamos armas
        if(mechs->mechJugador->enElSuelo){ //Si esta en el suelo no puede disparar;
           num_armas=0;
           return;
        }
        objetivo_mech = objetivoArmas();
        objetivo.fila = mechs->iMechVector[objetivo_mech]->pos_Hexagono.fila;
        objetivo.columna = mechs->iMechVector[objetivo_mech]->pos_Hexagono.columna;

        angulo = angulo_mech(objetivo_mech);

        filaux = mechs->iMechVector[objetivo_mech]->pos_Hexagono.fila;
        colaux = mechs->iMechVector[objetivo_mech]->pos_Hexagono.columna;
        int sum_nivel_or = 0;
        int sum_nivel_dest = 0;

        if (!mechs->mechJugador->enElSuelo)
            sum_nivel_or = 1;
        if (!mechs->iMechVector[objetivo_mech]->enElSuelo)
            sum_nivel_dest = 1;

        if(mechs->mechJugador->defMechInfo->tipoRadiadores == 0)
           calordisipado = mechs->mechJugador->defMechInfo->radiadores;
        else
           calordisipado = 2*mechs->mechJugador->defMechInfo->radiadores;
        
        sumatoriaTemp = mechs->mechJugador->temp_actual;
  mechs->iMechVector[objetivo_mech]->pos_Hexagono.fila;
  mechs->iMechVector[objetivo_mech]->pos_Hexagono.columna;
        if (linea_vision(mechs->mechJugador->numJ, mechs->mechJugador->pos_Hexagono, sum_nivel_or, mechs->iMechVector[objetivo_mech]->pos_Hexagono, sum_nivel_dest) == true /*&& (mechs->mechJugador->temp_actual <10)*/){
            for (int i = 0; i < mechs->mechJugador->defMechInfo->num_componentes; i++) {
                if ((mechs->mechJugador->defMechInfo->componentes->clase == ARMA) && (mechs->mechJugador->defMechInfo->componentes[i].operativo == true) && (mechs->mechJugador->defMechInfo->componentes[i].distanciaLarga >= mapa->distancia_casillas(mechs->mechJugador->pos_Hexagono, mechs->iMechVector[objetivo_mech]->pos_Hexagono))/*&& ((mechs->mechJugador->defMechInfo->componentes[i].tipo == ENERGIA) || (queda_municion(*mechs->mechJugador, mechs->mechJugador->defMechInfo->componentes[i].codigo)) == true)*/) {
                        if ((
                            ((mechs->mechJugador->defMechInfo->componentes[i].localizacion == 6) ||
                            (mechs->mechJugador->defMechInfo->componentes[i].localizacion == 1) ||
                            (mechs->mechJugador->defMechInfo->componentes[i].localizacion == 4) ||
                            (mechs->mechJugador->defMechInfo->componentes[i].localizacion == 7) ||
                            (mechs->mechJugador->defMechInfo->componentes[i].localizacion == 2) ||
                            (mechs->mechJugador->defMechInfo->componentes[i].localizacion == 3)) &&
                            (mechs->mechJugador->defMechInfo->componentes[i].trasera == false) &&
                            (angulo == FRONTAL)
                            ) ||
                            (
                            (mechs->mechJugador->defMechInfo->componentes[i].localizacion == 0) &&
                            (mechs->mechJugador->defMechInfo->componentes[i].trasera == false) &&
                            ((angulo == FRONTAL) ||
                            (angulo == IZQUIERDO))
                            ) ||
                            (
                            (mechs->mechJugador->defMechInfo->componentes[i].localizacion == 5) &&
                            (mechs->mechJugador->defMechInfo->componentes[i].trasera == false) &&
                            ((angulo == FRONTAL) ||
                            (angulo == DERECHO))
                            ) ||
                            (
                            (mechs->mechJugador->defMechInfo->componentes[i].trasera == true) &&
                            (angulo == TRASERO)
                            )) {
                                if(sumatoriaTemp+mechs->mechJugador->defMechInfo->componentes[i].calor < calordisipado){
                                    sumatoriaTemp += mechs->mechJugador->defMechInfo->componentes[i].calor;
                                    localizacion = mechs->mechJugador->defMechInfo->componentes[i].localizacion;
                                    armas_mech[num_armas].localizacion = localizacion;
                                    bool encontrado = false;
                                    for (int j = 0; j < mechs->mechJugador->defMechInfo->localizaciones[localizacion].slots_ocupados && !encontrado ; ++j) {
                                        if (mechs->mechJugador->defMechInfo->localizaciones[localizacion].slots[j].codigo == mechs->mechJugador->defMechInfo->componentes[i].codigo){
                                            armas_mech[num_armas].slot = j;
                                            encontrado = true;
                                        }
                                    }
                                    armas_mech[num_armas].doble_cadencia = false;
                                    if (mechs->mechJugador->defMechInfo->componentes[i].tipo != 1) {
                                        if(queda_municion(*mechs->mechJugador, mechs->mechJugador->defMechInfo->componentes[i].codigo)){
                                            buscar_municion(*mechs->mechJugador, mechs->mechJugador->defMechInfo->componentes[i].codigo);
                                            armas_mech[num_armas].objetivo.columna = mechs->iMechVector[objetivo_mech]->pos_Hexagono.columna;
                                            armas_mech[num_armas].objetivo.fila = mechs->iMechVector[objetivo_mech]->pos_Hexagono.fila;
                                            armas_mech[num_armas].tipo_objetivo = MECH;
                                            num_armas++;
                                        }
                                        
                                    }else{
                                        armas_mech[num_armas].objetivo.columna = mechs->iMechVector[objetivo_mech]->pos_Hexagono.columna;
                                        armas_mech[num_armas].objetivo.fila = mechs->iMechVector[objetivo_mech]->pos_Hexagono.fila;
                                        armas_mech[num_armas].tipo_objetivo = MECH;
                                        num_armas++;
                                    }  
                                }
                        }
                }
            }       

        }  
    }
    if(num_armas == 0){
        objetivo.fila=0;
        objetivo.columna=0;
    }
        
    cout << "Fin de logica de disparo" << endl;
    //cin.get();


    
}
