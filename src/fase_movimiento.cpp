/* 
 * File:   fase_movimiento.cpp
 * Author: Angel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "fase_movimiento.h"
#include <time.h>

movimiento_t::movimiento_t(infoMapa* inf_mapa, infoMechs * inf_mechs) {
    mapa = inf_mapa;
    mechs = inf_mechs;
    tipo_movimiento = 0;
    destino.fila = 0;
    destino.columna = 0;
    lado = 0;
    MASC = false;
    pasos = 0;
    for (int i = 0; i < 20; ++i) {
        tipo[i] = -1;
        veces[i] = 0;
    }
}

void movimiento_t::salida(string numJ) {
    /* Crear el archivo de salida */
    string cad = "accionJ" + numJ + ".sbt";
    string accion;
    ofstream out(cad.c_str());

    //out.open(cad.c_str());
    /* Linea 1: tipo de movimiento */

    switch (tipo_movimiento) {
        case INMOVIL:
            accion = accion + "Inmovil\n";
            break;
        case ANDAR:
            accion = accion + "Andar\n";
            break;
        case CORRER:
            accion = accion + "Correr\n";
            break;
        case SALTAR:
            accion = accion + "Saltar\n";
            break;
    }

    /* Si el movimiento es INMOVIL, terminar aquiÂ­*/
    if (tipo_movimiento == INMOVIL) {
        out << accion;
        out.close();
        return;
    }

    /* Linea 2: Hexagono de destino */
    accion = accion + destino.stringPos() + "\n";

    /* Linea 3: Lado de destino */
    accion = accion + itoStr(lado) + "\n";
    /* Si el movimiento es SALTAR, terminar aquiÂ­ */
    if (tipo_movimiento == SALTAR) {
        out << accion;
        out.close();
        return;
    }

    /* Linea 4: Usar MASC */
    switch (MASC) {
        case true:
            accion = accion + "True\n";
            break;
        case false:
            accion = accion + "False\n";
            break;
    }

    /* Linea 5: numero de pasos */
    accion = accion + itoStr(pasos) + "\n";

    /* Para cada paso */
    for (int i = 0; i < pasos; ++i) {
        /* Tipo de paso */
        switch (tipo[i]) {
            case MOV_ADELANTE:
                accion = accion + "Adelante\n";
                break;
            case MOV_ATRAS:
                accion = accion + "Atras\n";
                break;
            case MOV_IZQUIERDA:
                accion = accion + "Izquierda\n";
                break;
            case MOV_DERECHA:
                accion = accion + "Derecha\n";
                break;
            case MOV_LEVANTARSE:
                accion = accion + "Levantarse\n";
                break;
            case MOV_TIERRA:
                accion = accion + "Cuerpo a Tierra\n";
                break;
        }
        /* Numero de veces o lado */
        accion = accion + itoStr(veces[i]) + "\n";
    }

    /* Cerrar el archivo */
    //cout <<accion;
    //cin.get();
    out << accion;

    out.close();
}

int movimiento_t::getDestino(int fil_mech, int col_mech, int & fil_dest, int & col_dest, int & lado_dest, int estrategia) {

    int tipo_mov;
    char cad[255];
    int f, c;

    int ind_objetivo = mechs->mechJugador->buscar_objetivo(mechs->iMechVector, mechs->mechJugador->numJ, mechs->nMechs);
    this->fil_enemigo = mechs->iMechVector[ind_objetivo]->pos_Hexagono.fila;
    this->col_enemigo = mechs->iMechVector[ind_objetivo]->pos_Hexagono.columna;
    float d_disparo_enemigo = mechs->iMechVector[ind_objetivo]->dist_media_disparo();
    float d_disparo_jugador = mechs->mechJugador->dist_media_disparo();
    float d_seguridad;
    if (MIN_DIST <= d_disparo_jugador)
        d_seguridad = d_disparo_jugador;
    else
        d_seguridad = MIN_DIST;


    int f_obj = this->fil_enemigo;
    int c_obj = this->col_enemigo;
    float distancia_al_enemigo = distancia_hexagonal(f_obj, c_obj, fil_mech, col_mech);
    printf("Mi distancia de disp. es %f, la del enemigo es %f\n", d_disparo_jugador, d_disparo_enemigo);
    printf("La distancia al enemigo es %f\n", distancia_al_enemigo);

    int enc_obj = mechs->iMechVector[ind_objetivo]->encaramiento_mech;

    int pm_corriendo = mechs->mechJugador->dmj->PM_correr;
    int niveles = pm_corriendo / 4;
    if (niveles < 1)
        niveles = 1;

    int dif_nivel_hex, nivelini, niveldest;
    vector<nodeVector> anillos;



    sprintf(cad, "%s : Posicion mech objetivo: fila: %i,columna: %i,lado: %i \n\n", ctime(&tiempo), f_obj, c_obj, enc_obj);
    flog += cad;

    switch (estrategia) {
        case ATACAR:
            anillos = getAnillos(nodoArea(fil_mech, col_mech, -1), mapa); //Anillos mech
            nodoEnEspalda(f_obj, c_obj, enc_obj, f, c);
            if (mapa->pos_valida(f, c, mapa->info_mechs->mechJugador->defMechInfo->toneladas)) {
                nivelini = mapa->mapa[f][c]->nivel;
                niveldest = mapa->mapa[f_obj][c_obj]->nivel;
                dif_nivel_hex = abs(nivelini - niveldest);
            }

            if (!mapa->pos_valida(f, c, mapa->info_mechs->mechJugador->defMechInfo->toneladas) ||
                    mapa->mapa[f][c]->fuego || dif_nivel_hex > 2) {

                //nodoEnEsaDireccion(f_obj, c_obj, enc_obj, f, c);
                vector<nodeVector> anillosObj = getAnillos(nodoArea(f_obj, c_obj, -1), mapa);
                posAtaque(niveles + 2, anillos, anillosObj, f, c, mapa);

            }


            fil_dest = f;
            col_dest = c;
            mapa->encarar_objetivo(f_obj, c_obj,fil_dest, col_dest, lado_dest);

            tipo_mov = CORRER;
            //Pos mas cercana al mas cercano
            break;
        case DEFENDER:
            anillos = getAnillos(nodoArea(fil_mech, col_mech, -1), mapa);

            if (!mapa->info_mechs->mechJugador->enElSuelo && mapa->info_mechs->mechJugador->dmj->PM_saltar > 1) {
                coberturaSalto(mapa->info_mechs->mechJugador->dmj->PM_saltar, anillos, fil_dest, col_dest, f_obj, c_obj, enc_obj, mapa, d_seguridad);
                tipo_mov = SALTAR;
            } else {
                cobertura(niveles, anillos, fil_dest, col_dest, f_obj, c_obj, enc_obj, mapa, d_seguridad);
                tipo_mov = CORRER;
            }


            ///////////////////////////////////
            mapa->encarar_objetivo(f_obj, c_obj, fil_dest, col_dest, lado_dest);

            break;
        case ESCAPAR:
            anillos = getAnillos(nodoArea(fil_mech, col_mech, -1), mapa);

            if (!mapa->info_mechs->mechJugador->enElSuelo && mapa->info_mechs->mechJugador->dmj->PM_saltar > 1) {
                coberturaSalto(mapa->info_mechs->mechJugador->dmj->PM_saltar, anillos, fil_dest, col_dest, f_obj, c_obj, enc_obj, mapa);
                tipo_mov = SALTAR;
            } else {
                cobertura(niveles + 3, anillos, fil_dest, col_dest, f_obj, c_obj, enc_obj, mapa);
                tipo_mov = CORRER;
            }


            ///////////////////////////////////
            mapa->encarar_objetivo(f_obj, c_obj, fil_dest, col_dest, lado_dest);

            break;
    }

    return tipo_mov;

}

int movimiento_t::estrategiaPorPeso() {
    int estrategia;
    char cad[100];
    int tonJ = mechs->mechJugador->defMechInfo->toneladas;
    int ind_objetivo = mechs->mechJugador->buscar_objetivo(mechs->iMechVector, mechs->mechJugador->numJ, mechs->nMechs);
    int tonEnemigo = mechs->iMechVector[ind_objetivo]->defMechInfo->toneladas;
    printf("Ton Enemigo: %i, ton. j: %i\n", tonEnemigo, tonJ);

    if (tonJ >= 80) {//Asalto
        estrategia = ATACAR;
    } else if (tonJ >= 60) {//Pesados
        //Atacamos siempre a no ser que nos enfrentemos a un mech de asalto
        if (tonEnemigo >= 80) {
            estrategia = DEFENDER;
        } else {
            estrategia = ATACAR;
        }
    } else if (tonJ >= 40) {//Medios
        if (tonEnemigo < tonJ + 15)
            estrategia = ATACAR;
        else
            estrategia = DEFENDER;
    } else if (tonJ >= 0) {//Ligeros
        //Huimos del enemigo si pesa 15 ton o + mas que nosotros.
        if (tonEnemigo < tonJ + 15) {
            estrategia = ATACAR;
            printf("Atacar\n");
        } else {
            estrategia = DEFENDER;
            printf("Defender\n");

        }
    }

    if (estrategia == ATACAR) {
        sprintf(cad, "%s : Estrategia de Ataque. \n\n", ctime(&tiempo));
        flog += cad;

    } else if (estrategia == DEFENDER) {

        sprintf(cad, "%s : Estrategia de Defensa. \n\n", ctime(&tiempo));
        flog += cad;

    }
    return estrategia;

}

int movimiento_t::estrategia_movimiento() {
    //    int estrat;
    //    cin >> estrat;
    //    return estrat;
    char cad[255];
    mechs->mechJugador->dmj->Heridas_MW;
    iMech* mech = mechs->mechJugador;
    /* Si el mech esta en pie, el MechWarrior tiene pocas heridas, el
       nivel de temperatura no es muy alto, los puntos de blindaje de la cabeza, 
       del torso central y de las piernas no son bajos y tenemos armas no traseras
       con las que disparar, entonces la estrategia sera de ataque */

    if ((mechs->mechJugador->dmj->Heridas_MW < 5) && /* Para que haya como mucho una prob. de 5/6 de perder la consciencia */
            (mech->temp_actual < 13) && /* Para evitar la desconexion */
            (mech->blindaje.CAB > 2) &&
            (mech->blindaje.TC > 2) &&
            (mech->blindaje.PI > 1) &&
            (mech->blindaje.PD > 1)) {
        return estrategiaPorPeso();
    } else {
        sprintf(cad, "%s : Estrategia de Huida. \n\n", ctime(&tiempo));
        flog += cad;
        return ESCAPAR;
    }
}

//void movimiento_t::getArea(int n, int f, int c, vector<nodoArea> & pos_vector) {
//    int min_f = f - n,
//            max_f = f + n,
//            min_c = c - n,
//            max_c = c + n;
//    if (min_f <= 0) min_f = 1;
//    if (max_f >= mapa->filas) max_f = mapa->filas - 1;
//    if (min_c <= 0) min_c = 1;
//    if (max_c >= mapa->columnas) max_c = mapa->columnas - 1;
//
//    for (int i = min_f; i <= max_f; i++)
//        for (int j = min_c; j <= max_c; j++)
//            pos_vector.push_back(nodoArea(i, j, distanciaNodos(f, c, i, j)));
//
//    showNodosArea(pos_vector);
//}

bool movimiento_t::check_mov_correr(const vector<node>& nodos) {

    int PM = mechs->mechJugador->dmj->PM_correr;
    for (int i = 0; i < nodos.size(); i++) {
        if (mapa->mapa[nodos[i].fil][nodos[i].col]->nivel <= 1 &&
                mapa->mapa[nodos[i].fil][nodos[i].col]->terreno == AGUA &&
                nodos[i].coste < PM) return false;
    }
    return true;

}

int movimiento_t::tipo_movim(const vector<node>& nodos) {
    char cad[255];
    int tipo_mov;
    if (nodos.back().coste <= mechs->mechJugador->dmj->PM_andar)
        tipo_mov = ANDAR;
    else
        tipo_mov = CORRER;

    if (tipo_mov == CORRER) {
        if (check_mov_correr(nodos)) {
            sprintf(cad, "%s : Tipo de movimiento: Correr. \n\n", ctime(&tiempo));
            flog += cad;
            return CORRER;
        } else {
            sprintf(cad, "%s : Tipo de movimiento: Andar. \n\n", ctime(&tiempo));
            flog += cad;
            return ANDAR;
        }
    }
    return tipo_mov;
}

void movimiento_t::logica_movimiento() {


    char cad[255];
    vector<node> nodosDefault;
    vector<node> nodosWalk;
    vector<node> &nodos = nodosDefault;

    int tipo_mov;
    int PM = mechs->mechJugador->dmj->PM_andar;

    int estrategia = estrategia_movimiento(); //ATACAR o DEFENDER

    int tons = mechs->mechJugador->defMechInfo->toneladas;
    int col_mech = mechs->mechJugador->pos_Hexagono.columna;
    int fil_mech = mechs->mechJugador->pos_Hexagono.fila;
    const bool estabaEnSuelo = mapa->info_mechs->mechJugador->enElSuelo;
    int lado_mech = mechs->mechJugador->encaramiento_mech;
    this->destino.fila = fil_mech;
    this->destino.columna = col_mech;
    this->lado = lado_mech;
    int col_dest, fil_dest, lado_dest;

    pasos = 0;
    //Si esta  en el suelo intentar levantarse

    //ATACAR O DEFENDER

    sprintf(cad, "%s : Posicion mech jugador: fila: %i,columna: %i,lado: %i \n\n", ctime(&tiempo), fil_mech, col_mech, lado_mech);
    flog += cad;

    int tipo_mov_get_destino = getDestino(fil_mech, col_mech, fil_dest, col_dest, lado_dest, estrategia);
    if (fil_dest == -1 && col_dest == -1 && lado_dest == -1) {
        sprintf(cad, "El jugador no tiene PM asi que permanecera inmovil.\n");
        flog += cad;
        tipo_movimiento = INMOVIL;
        //cout << "El jugador no tiene PM asi que permanecera inmovil." << endl;
        //cin.get();
        return;
    } else if (fil_dest == fil_mech && col_dest == col_mech && lado_dest == lado_mech && !estabaEnSuelo) {
        sprintf(cad, "El jugador decide permanecer inmovil.\n");
        flog += cad;
        //cout << "El jugador decide permanecer inmovil.(Destino elegido pos. actual)" << endl;
        //cin.get();
        tipo_movimiento = INMOVIL;
        return;

    }



    sprintf(cad, "%s : Destino final de movimiento: fila: %i,columna: %i,lado: %i \n\n", ctime(&tiempo), fil_dest, col_dest, lado_dest);
    flog += cad;



    if (tipo_mov_get_destino == SALTAR) {
        tipo_movimiento = SALTAR;
        this->destino.fila = fil_dest;
        this->destino.columna = col_dest;
        this->lado = lado_dest;
        //cout << "El jugador decide saltar a la pos " << destino.stringPos() << endl;
        //cin.get();
        return;

    }

    if (tipo_mov_get_destino != SALTAR) {

        node *destino = new node(fil_dest, col_dest, lado_dest, mapa, tons);

        node *inicio = new node(fil_mech, col_mech, lado_mech, destino, mapa, tons);
        aStar(inicio, destino, nodosDefault, mapa, tons);
        delete inicio;
        delete destino;

        destino = new node(fil_dest, col_dest, lado_dest, mapa, tons);
        inicio = new node(fil_mech, col_mech, lado_mech, destino, mapa, tons);

        aStar(inicio, destino, nodosWalk, mapa, tons, WALK);
        delete inicio;
        delete destino;

        if (nodosWalk.back().coste + 1 < nodosDefault.back().coste) {
            tipo_mov = ANDAR;
            nodos = nodosWalk;
            if (nodos[1].coste >= PM) {
                sprintf(cad, "%s : Como no podemos realizar la ruta nos encaramos al enemigo.\n\n", ctime(&tiempo), fil_dest, col_dest, lado_dest);
                flog += cad;
                fil_dest = fil_mech;
                col_dest = col_mech;
                mapa->encarar_objetivo(fil_enemigo, col_enemigo, fil_mech, col_mech, lado_dest);

                destino = new node(fil_dest, col_dest, lado_dest, mapa, tons);
                inicio = new node(fil_mech, col_mech, lado_mech, destino, mapa, tons);

                aStar(inicio, destino, nodos, mapa, tons, WALK);
                delete inicio;
                delete destino;
            }

        } else {
            tipo_mov = tipo_movim(nodosDefault);
            nodos = nodosDefault;
        }
        ///////////////
        if (!estabaEnSuelo && mapa->info_mechs->mechJugador->dmj->PM_saltar > 1) {
            int PS = mapa->info_mechs->mechJugador->dmj->PM_saltar;
            int filS, colS;
            saltoIntermedio(fil_mech, col_mech, fil_dest, col_dest, filS, colS, mapa, PS);

            sprintf(cad, "%s : El mech saltara a la posicion f:%i c:%i\n\n", ctime(&tiempo), filS, colS);
            flog += cad;


            tipo_movimiento = SALTAR;
            mapa->encarar_objetivo(fil_enemigo, col_enemigo, filS, colS, this->lado);
            this->destino.fila = filS;
            this->destino.columna = colS;

            //cout << "El jugador decide saltar a la pos " << this->destino.stringPos() << endl;
            //cin.get();
            return;

        } else {
            sprintf(cad, "%s : El mech realizara la ruta a pie\n\n", ctime(&tiempo));
            flog += cad;
            if (nodos[1].coste >= PM) {

                //SALTAR
                sprintf(cad, "%s : Como no podemos realizar la ruta nos encaramos al enemigo.\n\n", ctime(&tiempo), fil_dest, col_dest, lado_dest);


                fil_dest = fil_mech;
                col_dest = col_mech;
                mapa->encarar_objetivo(fil_enemigo, col_enemigo, fil_mech, col_mech, lado_dest);

                //                node *destino = new node(fil_dest, col_dest, lado_dest, mapa, tons);
                //                node *inicio = new node(fil_mech, col_mech, lado_mech, destino, mapa, tons);
                //                aStar(inicio, destino, nodos, mapa, tons);
                //                delete inicio;
                //                delete destino;

            }

        }

        tipo_movimiento = tipo_mov;
        switch (tipo_mov) {
            case ANDAR:
                PM = mechs->mechJugador->dmj->PM_andar;
                break;
            case CORRER:
                PM = mechs->mechJugador->dmj->PM_correr;
                break;
        }//Aniadir saltar



        if (mapa->info_mechs->mechJugador->enElSuelo) {


            if (PM >= 2 && mapa->info_mechs->mechJugador->equilibrio_ok()) {
                cout << "Intentamos levantarnos" << endl;

                tiempo = time(&tiempo);
                sprintf(cad, "%s : El mech %i esta en el suelo e intentara levantarse\n\n", ctime(&tiempo), mapa->info_mechs->mechJugador->numJ);
                flog += cad;

                tipo[pasos] = MOV_LEVANTARSE;
                veces[pasos] = nodos[0].orientacion;
                pasos++;
                this->destino.fila = nodos[0].fil;
                this->destino.columna = nodos[0].col;
                this->lado = nodos[0].orientacion;


                /* Actualizar los PM que llevamos usados */
                /* Para simplificar, suponemos que tenemos ambos brazos y no estan danhados */
                PM -= 2;
            } else {
                sprintf(cad, "%s : El mech %i no puede levantarse\n\n", ctime(&tiempo), mapa->info_mechs->mechJugador->numJ);
                flog += cad;
                tipo_movimiento = INMOVIL;
                //cout << "El mech no puede levantarse " << endl;
                //cin.get();
                return;

            }
        }
    }

    printf("El camino al destino es el siguiente:\n");
    show(nodos);
    getSecuenciaPasos(nodos, PM);

    sprintf(cad, "%s : Destino alcanzado: fila: %i,columna: %i,lado: %i \n\n", ctime(&tiempo), this->destino.fila, this->destino.columna, this->lado);
    flog += cad;

    //cout << "Fin de logica de mov" << endl;
    //cin.get();

}

int movimiento_t::getTipoMov(const node & n1, const node & n2) {


    int o1, o2;
    int f, c;
    if (n1.mismo_hexagono(n2)) {
        o1 = n1.orientacion;
        o2 = n2.orientacion;
        if (o2 == o1 + 1 || (o1 == 6 && o2 == 1))
            return MOV_DERECHA;
        else if (o2 == o1 - 1 || (o1 == 1 && o2 == 6))
            return MOV_IZQUIERDA;
        else {
            cout << "Problema en getTipoMov, pulse uan tecla para continuar ...\n";
            return 0;
        }
        //IZQ O DER
    } else {
        nodoEnEsaDireccion(n1.fil, n1.col, n1.orientacion, f, c);
        if (n2.fil == f && n2.col == c)
            return MOV_ADELANTE;
        else return MOV_ATRAS;
        //ADELANTE O ATRAS
    }


}
//
//void movimiento_t::encarar_objetivo(int f, int c, int f_obj, int c_obj, int & lado) {
//
//
//    if (f < f_obj) {//Los de arriba
//        if (c < c_obj) //los de la izq
//            lado = 6;
//        else if (c > c_obj)
//            lado = 2;
//        else lado = 1;
//    } else {//los de abajo
//        if (c < c_obj) //los de la izq
//            lado = 5;
//        else if (c > c_obj)
//            lado = 3;
//        else
//            lado = 4;
//    }
//}

void movimiento_t::getSecuenciaPasos(const vector<node> & nodosPath, int PM) {

    vector<node> nodos = nodosPath;

    while (nodos.back().coste > PM) {
        //        cout << "PM: " << PM << endl;
        //        nodos.back().show();
        nodos.pop_back();
    }
    //Si no tenemos PM para movernos nos quedamos inmoviles
    if (nodos.size() <= 1) {
        if (pasos == 0)
            tipo_movimiento = INMOVIL;
        return;
    }
    /////

    destino.fila = nodos.back().fil;
    destino.columna = nodos.back().col;
    lado = nodos.back().orientacion;

    //    int veces[20];
    //    for(int i=0;i<20;i++)
    //        veces[i]=0;

    //pasos = 0; //Pasos en los que se divide el movimiento

    int dir_actual = getTipoMov(nodos[0], nodos[1]);
    int dir;
    node ultimo_nodo(nodos[1]);
    nodos.erase(nodos.begin());
    nodos.erase(nodos.begin());
    tipo[pasos] = dir_actual; //ADELANTE,ATRAS,IZQ,DERE,LEVANTARSE, CUERPO A TIERRA
    veces[pasos] = 1;
    while (!nodos.empty()) {
        dir = getTipoMov(ultimo_nodo, nodos.front());
        ultimo_nodo = nodos.front();
        nodos.erase(nodos.begin());
        if (dir == dir_actual)
            veces[pasos]++; //incrementamos las veces del paso actual
        else {
            pasos++; //Incrementamos el paso actual
            dir_actual = dir;
            tipo[pasos] = dir_actual;
            veces[pasos] = 1;
        }
    }
    pasos++; //El num de pasos sera el paso actual +1 (empiezan en )

}
