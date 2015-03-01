/* 
 * File:   fase_final.cpp
 * Author: Ángel Costela Sanmiguel y David Medina Godoy
 * 
 */

#include "fase_final.h"

void final_t::salida(string numJ) {
    char cad2[400];
    string cad = "accionJ" + numJ + ".sbt";
    string accion;
    ofstream out(cad.c_str());
    /* Linea 1: Número de radiadores a apagar */
    sprintf(cad2,"%s : Vamos a apagar %i radiadores \n\n ",ctime(&tiempo),radiadores_off);
    flog +=cad2;
    accion = accion + itoStr(radiadores_off) + "\n";

    /* Linea 2: Número de radiadores a encender */
    sprintf(cad2,"%s : Vamos a encender %i radiadores \n\n ",ctime(&tiempo),radiadores_on);
    flog +=cad2;
    accion = accion + itoStr(radiadores_on) + "\n";

    /* Linea 3: Soltar garrote */
    switch (soltar_garrote) {
        case true:
            sprintf(cad2,"%s : Vamos a soltar el garrote que tenemos cogido \n\n ",ctime(&tiempo));
            flog +=cad2;
            accion = accion + "True\n";
            break;
        case false:
            sprintf(cad2,"%s : No soltamos ningún garrote \n\n ",ctime(&tiempo));
            flog +=cad2;
            accion = accion + "False\n";
            break;
    }

    /* Linea 4: Número de municiones a expulsar */
    sprintf(cad2,"%s : Vamos a soltar la siguiente cantidad de munición %i \n\n ",ctime(&tiempo),expulsar);
    flog +=cad2;

    accion = accion + itoStr(expulsar) + "\n";

    /* Para cada munición a expulsar */
    for (int i = 0; i < expulsar; ++i) {
        /* Localización de la munición */
        switch (expulsada[i].localizacion) {
            case 0:
                sprintf(cad2,"%s : La munición %i está en el brazo izquierdo \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "BI\n";
                break;
            case 1:
                sprintf(cad2,"%s : La munición %i está en el torso izquierdo \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "TI\n";
                break;
            case 2:
                sprintf(cad2,"%s : La munición %i está en la pierna izquierda \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "PI\n";
                break;
            case 3:
                sprintf(cad2,"%s : La munición %i está en la pierna derecha \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "PD\n";
                break;
            case 4:
                sprintf(cad2,"%s : La munición %i está en el torso derecho \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "TD\n";
                break;
            case 5:
                sprintf(cad2,"%s : La munición %i está en el brazo derecho \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "BD\n";
                break;
            case 6:
                sprintf(cad2,"%s : La munición %i está en el torso central \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "TC\n";
                break;
            case 7:
                sprintf(cad2,"%s : La munición %i está en la cabeza \n\n ",ctime(&tiempo),i);
                flog +=cad2;
                accion = accion + "CAB\n";
                break;
        }

        /* Slot de la munición dentro de la localización */
        sprintf(cad2,"%s : La munición %i está en el slot %i \n\n ",ctime(&tiempo),expulsada[i].slot);
        flog +=cad2;        
        accion = accion + itoStr(expulsada[i].slot) + "\n";
    }
    /* Cerrar el archivo */
    out << accion;
    out.close();
}

void final_t::logicaFinal(){
    radiadores_off=0;
    radiadores_on=0;
    soltar_garrote=0;
    expulsar=0;
    expulsada=0;
}

