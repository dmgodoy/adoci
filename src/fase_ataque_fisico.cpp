/* 
 * File:   fase_ataque_fisico.cpp
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "fase_ataque_fisico.h"

ataque_fisico_t::ataque_fisico_t(infoMapa* inf_mapa, infoMechs* inf_mechs){
    mapa = inf_mapa;
    mechs = inf_mechs;
    num_armas = 0;
}

void ataque_fisico_t::salida(string numJ) {
    char cad2[400];
   string cad = "accionJ" + numJ + ".sbt";
    string accion;
    ofstream out(cad.c_str());

     sprintf(cad2,"%s : Vamos a usar %i armas físicas \n\n ",ctime(&tiempo),num_armas);
     flog +=cad2;
    /* Linea 1: Numero de armas fisicas que se van a usar */
    accion = accion + itoStr(num_armas) + "\n";

    /* Para cada arma */
    for (int i = 0; i < num_armas; ++i) {
        /* Localización del arma física */
        switch (armas[i].localizacion) {
            case BI_A:
                sprintf(cad2,"%s : Se pegará un puñetazo con el brazo izquierdo \n\n ",ctime(&tiempo));
                flog +=cad2;
                accion = accion + "BI\n";
                break;
            case BD_A:
                sprintf(cad2,"%s : Se pegará un puñetazo con el brazo derecho \n\n ",ctime(&tiempo));
                flog +=cad2;
                accion = accion + "BD\n";
                break;
            case PI_A:
                sprintf(cad2,"%s : Se pegará una patada con la pierna izquierda \n\n ",ctime(&tiempo));
                flog +=cad2;
                accion = accion + "PI\n";
                break;
            case PD_A:
                sprintf(cad2,"%s : Se pegará una patada con la pierna derecha \n\n ",ctime(&tiempo));
                flog +=cad2;
                accion = accion + "PD\n";
                break;
            case BIBD_A:
                sprintf(cad2,"%s : Se va a usar un garrote \n\n ",ctime(&tiempo));
                flog +=cad2;
                accion = accion + "BIBD\n";
                break;
        }

        /* Slot del arma fisica */
        
        sprintf(cad2,"%s : El slot del arma %i a usar es %i \n\n ",ctime(&tiempo),i,armas[i].slot);
        flog +=cad2;
        accion = accion + itoStr(armas[i].slot) + "\n";

        /* Hexagono objetivo del arma */
        sprintf(cad2,"%s : El hexágono objetivo para el arma %i es %02i%02i \n\n ",ctime(&tiempo),i,armas[i].objetivo.columna,armas[i].objetivo.fila);
        flog +=cad2;
        accion = accion + armas[i].objetivo.stringPos() + "\n";

        /* Tipo de objetivo */
        switch (armas[i].tipo_objetivo) {
            case MECH:
                sprintf(cad2,"%s : El objetivo para el arma %i es un Mech \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "Mech\n";
                break;
            case HEXAGONO:
                sprintf(cad2,"%s : El objetivo para el arma %i es un Hexágono \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "Hexagono\n";
                break;
            case NINGUNO:
                sprintf(cad2,"%s : El objetivo para el arma %i es nada \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "Ninguno\n";
                break;
        }
    }
    /* Cerrar el archivo */
    out << accion;
    out.close();
}

void ataque_fisico_t::posiciones_adyacentes(int f, int c, int adyacentes[][2]) {
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

int ataque_fisico_t::angulo_torso_enemigo(int mech_obj){
    int col_ene;
    int fil_ene;
    int fil_jug;
    int col_jug;
    int lado_hex_ene;
    int enca_jug;
    
    fil_ene = mechs->iMechVector[mech_obj]->pos_Hexagono.fila;
    col_ene = mechs->iMechVector[mech_obj]->pos_Hexagono.columna;
    fil_jug = mechs->mechJugador->pos_Hexagono.fila;
    col_jug = mechs->mechJugador->pos_Hexagono.columna;
        
    if(col_jug %2 == 0){
        if(col_ene < col_jug){ //lado 5 o 6 depende fila
            if(fil_ene > fil_jug)
                lado_hex_ene = 5;
            else
                lado_hex_ene = 6;
        }else if(col_ene > col_jug){ //lado 2 o 3 depende fila
            if(fil_ene > fil_jug)
                lado_hex_ene = 3;
            else
                lado_hex_ene = 2;
        }else if(col_ene == col_jug){ //lado 1 o 4 depende fila
            if(fil_ene < fil_jug)
                lado_hex_ene = 1;
            else
                lado_hex_ene = 4;
        }
    }else{
        if(col_ene < col_jug){ //lado 5 o 6 depende fila
            if(fil_ene < fil_jug)
                lado_hex_ene = 6;
            else
                lado_hex_ene = 5;
        }else if(col_ene > col_jug){ //lado 2 o 3 depende fila
            if(fil_ene < fil_jug)
                lado_hex_ene = 2;
            else
                lado_hex_ene = 3;
        }else if(col_ene == col_jug){ //lado 1 o 4 depende fila
            if(fil_ene < fil_jug)
                lado_hex_ene = 1;
            else
                lado_hex_ene = 4;
        }    
    }
    enca_jug = mechs->mechJugador->encaramiento_torso;
    
    if(lado_hex_ene == enca_jug){
        return FRONTAL;
    }else if(enca_jug == 1 && lado_hex_ene == 6){
        return IZQUIERDO;
    }else if(enca_jug == 1 && lado_hex_ene == 2){
        return DERECHO;
    }else if(enca_jug == 2 && lado_hex_ene == 1){
        return IZQUIERDO;
    }else if(enca_jug == 2 && lado_hex_ene == 3){
        return DERECHO;
    }else if(enca_jug == 3 && lado_hex_ene == 2){
        return IZQUIERDO;
    }else if(enca_jug == 3 && lado_hex_ene == 4){
        return DERECHO;
    }else if(enca_jug == 4 && lado_hex_ene == 3){
        return IZQUIERDO;
    }else if(enca_jug == 4 && lado_hex_ene == 5){
        return DERECHO;
    }else if(enca_jug == 5 && lado_hex_ene == 4){
        return IZQUIERDO;
    }else if(enca_jug == 5 && lado_hex_ene == 6){
        return DERECHO;
    }else if(enca_jug == 6 && lado_hex_ene == 1){
        return DERECHO;
    }else if(enca_jug == 6 && lado_hex_ene == 5){
        return IZQUIERDO;
    }
}

int ataque_fisico_t::angulo_mech_enemigo(int mech_obj){
    int col_ene;
    int fil_ene;
    int fil_jug;
    int col_jug;
    int lado_hex_ene;
    int enca_jug;
    int prueba;
    
    fil_ene = mechs->iMechVector[mech_obj]->pos_Hexagono.fila;
    col_ene = mechs->iMechVector[mech_obj]->pos_Hexagono.columna;
    fil_jug = mechs->mechJugador->pos_Hexagono.fila;
    col_jug = mechs->mechJugador->pos_Hexagono.columna;
    
    prueba = mechs->iMechVector[mech_obj]->numJ;
    
    if(col_jug %2 == 0){
        if(col_ene < col_jug){ //lado 5 o 6 depende fila
            if(fil_ene > fil_jug)
                lado_hex_ene = 5;
            else
                lado_hex_ene = 6;
        }else if(col_ene > col_jug){ //lado 2 o 3 depende fila
            if(fil_ene > fil_jug)
                lado_hex_ene = 3;
            else
                lado_hex_ene = 2;
        }else if(col_ene == col_jug){ //lado 1 o 4 depende fila
            if(fil_ene < fil_jug)
                lado_hex_ene = 1;
            else
                lado_hex_ene = 4;
        }
    }else{
        if(col_ene < col_jug){ //lado 5 o 6 depende fila
            if(fil_ene < fil_jug)
                lado_hex_ene = 6;
            else
                lado_hex_ene = 5;
        }else if(col_ene > col_jug){ //lado 2 o 3 depende fila
            if(fil_ene < fil_jug)
                lado_hex_ene = 2;
            else
                lado_hex_ene = 3;
        }else if(col_ene == col_jug){ //lado 1 o 4 depende fila
            if(fil_ene < fil_jug)
                lado_hex_ene = 1;
            else
                lado_hex_ene = 4;
        }    
    }
    enca_jug = mechs->mechJugador->encaramiento_mech;
    
    if(lado_hex_ene == enca_jug){
        return FRONTAL;
    }else if(enca_jug == 1 && lado_hex_ene == 6){
        return IZQUIERDO;
    }else if(enca_jug == 1 && lado_hex_ene == 2){
        return DERECHO;
    }else if(enca_jug == 2 && lado_hex_ene == 1){
        return IZQUIERDO;
    }else if(enca_jug == 2 && lado_hex_ene == 3){
        return DERECHO;
    }else if(enca_jug == 3 && lado_hex_ene == 2){
        return IZQUIERDO;
    }else if(enca_jug == 3 && lado_hex_ene == 4){
        return DERECHO;
    }else if(enca_jug == 4 && lado_hex_ene == 3){
        return IZQUIERDO;
    }else if(enca_jug == 4 && lado_hex_ene == 5){
        return DERECHO;
    }else if(enca_jug == 5 && lado_hex_ene == 4){
        return IZQUIERDO;
    }else if(enca_jug == 5 && lado_hex_ene == 6){
        return DERECHO;
    }else if(enca_jug == 6 && lado_hex_ene == 1){
        return DERECHO;
    }else if(enca_jug == 6 && lado_hex_ene == 5){
        return IZQUIERDO;
    }
}

int ataque_fisico_t::objetivoFisico() {
    //int auxColumna;
    //int auxFila;
    int columnaJugador;
    int filaJugador;
    //float score_objetivo[mechs->nMechs - 1]; //Aqui se guardará en cada dimension el Score de cada objetivo para elegir el mejor al que disparar
    int objetivo=-1;
    
            

    columnaJugador = mechs->mechJugador->pos_Hexagono.columna;
    filaJugador = mechs->mechJugador->pos_Hexagono.fila;
    vector<nodoArea> posicionesAdy = nodoArea(filaJugador,columnaJugador,-1).getAdyacentes(mapa);
    vector<nodoArea> posMechsAdy;
    posMechsAdy.clear();
    int colObj;
    int filObj;
    for (int i = 0; i < mechs->nMechs - 1; i++) {
        filObj = mechs->iMechVector[i]->pos_Hexagono.fila;
        colObj = mechs->iMechVector[i]->pos_Hexagono.columna;
        if(nodoArea(filObj,colObj,-1).pertenece(posicionesAdy))
                posMechsAdy.push_back(nodoArea(filObj,colObj,i));
    }

    
    if(posMechsAdy.size()==1)
        return (int) posMechsAdy.back().val;
    else if(posMechsAdy.size()==0)
        return -1;
    else if(posMechsAdy.size()>1){
        //Devolver al que alcancemos
        return (int) posMechsAdy.back().val; //Devolvemos este mismo
        //Deberiamos mirar cual es el mejor
    }

    
 
//    for (int i = 0; i < mechs->nMechs - 1; i++) {
//        auxColumna = mechs->iMechVector[i]->pos_Hexagono.columna;
//        auxFila = mechs->iMechVector[i]->pos_Hexagono.fila;
//        if (mechs->iMechVector[i]->operativo == true) 
//            score_objetivo[i] = distancia_hexagonal(filaJugador, columnaJugador, auxFila, auxColumna);//Se pone como Score la distancia del Mech con respecto al nuestro
//
//    }
//
//    for (int i = 0; i < mechs->nMechs; i++) {
//        if (score_objetivo[i] == 1) {
//            objetivo = i;
//        }
//    }
    return objetivo;
}

void ataque_fisico_t::ataque_fisico(){
    bool brazo_izq = true;
    bool brazo_der = true;
    bool ene_adyacente = false;
    int contador_armas=0;
    int desnivel;
    int objetivo;
    int angulo;
    int angulo_patadas;
 
    if(mechs->mechJugador->enElSuelo){ //Si esta en el suelo no puede pegar;
        num_armas=0;
        return;
    }
    int fil_jugador, col_jugador, filaux, colaux;
    int adyacentes [6][2];
    int filene=0;
    int colene=0;

    fil_jugador = mechs->mechJugador->pos_Hexagono.fila;
    col_jugador = mechs->mechJugador->pos_Hexagono.columna;
    
    posiciones_adyacentes(fil_jugador, col_jugador, adyacentes);

    //Consideramos enemigo adyacente si esta en una casilla al lado y el desnivel no es mayor que 1 (Le podemos pegar cuerpo a cuerpo).
    for (int i = 0; i < mechs->nMechs - 1; i++) {
        filaux = mechs->iMechVector[i]->pos_Hexagono.fila;
        colaux = mechs->iMechVector[i]->pos_Hexagono.columna;
        for (int j = 0; j < 6; j++) {
            if (adyacentes[j][0] == filaux && adyacentes[j][1] == colaux){
                if (abs(mapa->mapa[fil_jugador][col_jugador]->nivel - mapa->mapa[filaux][colaux]->nivel) <= 1){ //Si hay enemigo adyacente y está a buena altura
                    ene_adyacente = true; 
                }
            }
        }
    }
    
    if(ene_adyacente){
        objetivo = objetivoFisico();
        filene = mechs->iMechVector[objetivo]->pos_Hexagono.fila;
        colene = mechs->iMechVector[objetivo]->pos_Hexagono.columna;
        angulo = angulo_torso_enemigo(objetivo);
        if(angulo != TRASERO){
            if(!mechs->mechJugador->defMechInfo->brazos[0]){ //Si no tiene brazos no puede pegar puñetazos
                brazo_izq = false;
                brazo_der = false;
            }else{
                for(int i=1; i<5; i++){
                    if(!mechs->mechJugador->defMechInfo->brazos[i])
                        brazo_izq = false;
                }
                for(int i=5; i<9; i++){
                    if(!mechs->mechJugador->defMechInfo->brazos[i])
                        brazo_der = false;
                }
            }
            if(angulo == FRONTAL){ //Puede pegar con los dos brazos.
                desnivel = mapa->mapa[fil_jugador][col_jugador]->nivel - mapa->mapa[filene][colene]->nivel;
                if((desnivel ==0 || desnivel == -1) && !mechs->iMechVector[objetivo]->enElSuelo){ //Tiene que estar a la altura correcta y no en el suelo
                    if(brazo_der){
                        armas[contador_armas].localizacion = BD;
                        armas[contador_armas].slot = 1000;
                        contador_armas++;    
                    }
                    if(brazo_izq){
                        armas[contador_armas].localizacion = BI;
                        armas[contador_armas].slot = 1000;
                        contador_armas++;    
                    } 
                }
                if(desnivel == 0 || desnivel == 1){
                    angulo_patadas = angulo_mech_enemigo(objetivo);
                    if(angulo_patadas = FRONTAL){
                        if(mechs->iMechVector[objetivo]->enElSuelo){
                            if(desnivel == 0){
                                armas[contador_armas].localizacion = PD;
                                armas[contador_armas].slot = 2000;
                                contador_armas++;
                            }
                        }else{
                            armas[contador_armas].localizacion = PD;
                            armas[contador_armas].slot = 2000;
                            contador_armas++;    
                        }
                        
                    }else if(angulo_patadas == DERECHO){
                        if(mechs->iMechVector[objetivo]->enElSuelo){
                            if(desnivel == 0){
                                armas[contador_armas].localizacion = PD;
                                armas[contador_armas].slot = 2000;
                                contador_armas++;
                            }
                        }else{
                            armas[contador_armas].localizacion = PD;
                            armas[contador_armas].slot = 2000;
                            contador_armas++;    
                        }
                    }else if(angulo_patadas == IZQUIERDO){
                        if(mechs->iMechVector[objetivo]->enElSuelo){
                            if(desnivel == 0){
                                armas[contador_armas].localizacion = PI;
                                armas[contador_armas].slot = 2000;
                                contador_armas++;
                            }
                        }else{
                            armas[contador_armas].localizacion = PI;
                            armas[contador_armas].slot = 2000;
                            contador_armas++;    
                        }
                    }
                }
            }else if(angulo == DERECHO){
                if((desnivel ==0 || desnivel == -1) && !mechs->iMechVector[objetivo]->enElSuelo){
                    if(brazo_der){
                        armas[contador_armas].localizacion = BD;
                        armas[contador_armas].slot = 1000;
                        contador_armas++;
                    }                  
                }
                if(desnivel == 0 || desnivel == 1){
                    angulo_patadas = angulo_mech_enemigo(objetivo);
                    if(angulo_patadas = FRONTAL){
                       if(mechs->iMechVector[objetivo]->enElSuelo){
                            if(desnivel == 0){
                                armas[contador_armas].localizacion = PD;
                                armas[contador_armas].slot = 2000;
                                contador_armas++;
                            }
                        }else{
                            armas[contador_armas].localizacion = PD;
                            armas[contador_armas].slot = 2000;
                            contador_armas++;    
                        }
                    }else if(angulo_patadas == DERECHO){
                        if(mechs->iMechVector[objetivo]->enElSuelo){
                            if(desnivel == 0){
                                armas[contador_armas].localizacion = PD;
                                armas[contador_armas].slot = 2000;
                                contador_armas++;
                            }
                        }else{
                            armas[contador_armas].localizacion = PD;
                            armas[contador_armas].slot = 2000;
                            contador_armas++;    
                        }
                    }else if(angulo_patadas == IZQUIERDO){
                        if(mechs->iMechVector[objetivo]->enElSuelo){
                            if(desnivel == 0){
                                armas[contador_armas].localizacion = PI;
                                armas[contador_armas].slot = 2000;
                                contador_armas++;
                            }
                        }else{
                            armas[contador_armas].localizacion = PI;
                            armas[contador_armas].slot = 2000;
                            contador_armas++;    
                        }
                    }
                }
            }else if(angulo == IZQUIERDO){
                if((desnivel ==0 || desnivel == -1) && !mechs->iMechVector[objetivo]->enElSuelo){
                    if(brazo_izq){
                        armas[contador_armas].localizacion = BI;
                        armas[contador_armas].slot = 1000;
                        contador_armas++;   
                    }
                }                
                if(desnivel == 0 || desnivel == 1){
                    angulo_patadas = angulo_mech_enemigo(objetivo);
                    if(angulo_patadas = FRONTAL){
                       if(mechs->iMechVector[objetivo]->enElSuelo){
                            if(desnivel == 0){
                                armas[contador_armas].localizacion = PI;
                                armas[contador_armas].slot = 2000;
                                contador_armas++;
                            }
                        }else{
                            armas[contador_armas].localizacion = PI;
                            armas[contador_armas].slot = 2000;
                            contador_armas++;    
                        }
                    }else if(angulo_patadas == DERECHO){
                        if(mechs->iMechVector[objetivo]->enElSuelo){
                            if(desnivel == 0){
                                armas[contador_armas].localizacion = PD;
                                armas[contador_armas].slot = 2000;
                                contador_armas++;
                            }
                        }else{
                            armas[contador_armas].localizacion = PD;
                            armas[contador_armas].slot = 2000;
                            contador_armas++;    
                        }
                    }else if(angulo_patadas == IZQUIERDO){
                        if(mechs->iMechVector[objetivo]->enElSuelo){
                            if(desnivel == 0){
                                armas[contador_armas].localizacion = PI;
                                armas[contador_armas].slot = 2000;
                                contador_armas++;
                            }
                        }else{
                            armas[contador_armas].localizacion = PI;
                            armas[contador_armas].slot = 2000;
                            contador_armas++;    
                        }
                    }
                }
            }
            num_armas = contador_armas;
            for (int i=0; i<num_armas; i++){
                armas[i].tipo_objetivo = MECH;
                armas[i].objetivo.fila = filene;
                armas[i].objetivo.columna = colene;
            }
        }
        else{
            num_armas=0;
            return;
        }
    }else{
        num_armas=0;
        return;
    }
    cout << "Fin de logica de cuerpo a cuerpo" << endl;
    //cin.get();
    
}
