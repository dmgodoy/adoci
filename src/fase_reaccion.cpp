/* 
 * File:   fase_reaccion.cpp
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "fase_reaccion.h"

using namespace std;

reaccion_t::reaccion_t(infoMapa* inf_mapa, infoMechs * inf_mechs) {
    mapa = inf_mapa;
    mechs = inf_mechs;
    encaramiento = 0;
}

void reaccion_t::salida(string numJ) {
    /* Crear el archivo de salida */
    string cad = "accionJ" + numJ + ".sbt";
    string accion = "";
    ofstream out;
    out.open(cad.c_str());
    /* Linea 1: Cambio de encaramiento */
    switch (encaramiento) {
        case ENC_IGUAL:
            accion = accion + "Igual\n";
            break;
        case ENC_IZQUIERDA:
            accion = accion + "Izquierda\n";
            break;
        case ENC_DERECHA:
            accion = accion + "Derecha\n";
            break;
    }
    /* Cerrar el archivo */
    out << accion;
    out.close();
}

int izq_hex(int lado) {
    int l = lado;
    l -= 1;
    if (l == 0)
        l = 6;
    return l;
}

int der_hex(int lado) {
    int l = lado;
    l += 1;
    if (l == 7)
        l = 1;
    return l;
}

void reaccion_t::reaccion_accion() {
    int lado;
    char cad[200];
    int cont_izq = 0, cont_der = 0;
    int obj = mechs->mechJugador->buscar_mech_cercano(mechs->iMechVector, mechs->mechJugador->numJ, mechs->nMechs);
    int fil_mech = mechs->mechJugador->pos_Hexagono.fila,
            col_mech = mechs->mechJugador->pos_Hexagono.columna,
            enc_mech = mechs->mechJugador->encaramiento_torso,
            fil_obj = mechs->iMechVector[obj]->pos_Hexagono.fila,
            col_obj = mechs->iMechVector[obj]->pos_Hexagono.columna;
    mapa->encarar_objetivo( fil_obj, col_obj,fil_mech, col_mech, lado);

    printf("Mi encaramiento es %i, para encarar al enemigo debe ser %i\n",enc_mech,lado);
    sprintf(cad,"%s : El encaramiento del mech es %i, para encarar al enemigo debe ser %i\n\n ",ctime(&tiempo),enc_mech,lado);
    flog +=cad;
    if (enc_mech == lado) {
        sprintf(cad, "%s : Encaramiento igual\n\n",ctime(&tiempo));
        flog+=cad;
        encaramiento = ENC_IGUAL;
        //cout << "Enc. igual" << endl;
        //cin.get();
        return;
    }
    int l = enc_mech;
    while (l != lado) {//izq;
        cont_izq++;
        l = izq_hex(l);
    }
    l = enc_mech;
    while (l != lado) {//der
        cont_der++;
        l = der_hex(l);
    }
    if (cont_izq < cont_der) {
        sprintf(cad, "%s : Encaramiento izquierda\n\n",ctime(&tiempo));
        flog+=cad;
        encaramiento = ENC_IZQUIERDA;
        //cout << "Enc. izq" << endl;
        //cin.get();
    } else {
        sprintf(cad, "%s : Encaramiento derecha\n",ctime(&tiempo));
        flog+=cad;
        encaramiento = ENC_DERECHA;
        //cout << "Enc. der." << endl;
       // cin.get();
    }



}
