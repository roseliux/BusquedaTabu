//
//  main.cpp
//  Busqueda-Tabu-mvrp
//
//  Created by Rocela Durazo on 3/31/14.
//  Copyright (c) 2014 Rocela Durazo. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <time.h>


using namespace std;

#include "Busqueda-Tabu-mvrp.h"

int main()
{
    
    srand (time(NULL));
    
    int num_ciudades;
    int num_rutas;
    int capacidad;
    //int num_ceros = num_rutas - 1;
    
    // LEER NUMERO DE CIUDADES POR EL ARCHIVO DE TXT
    ifstream read;
    read.open("ex1.0.txt");
    read >> num_ciudades;
    read >> num_rutas;
    read >> capacidad;
    // ***********************************************
    vector<Nodo> nodo(num_ciudades);
    
    // LEER LAS COORDENADAS X Y Y
    
    float ciudad;
    for(int i=0; i <num_ciudades;i++ ){
        read >> ciudad >> nodo[i].x >> nodo[i].y;
        //ciudades.insert(ciudades., i);
    }
    
    // ***********************************************
    // IMPRIMIR NODO COORDENADAS
//        cout << num_ciudades << endl;
//        for (int i = 0; i < num_ciudades; i++) {
//            cout << nodo[i].x << ", " << nodo[i].y<<endl;
//        }
    // ***********************************************
    vector<int> demanda(num_ciudades);
    // se leen las demandas y se guardan en vector de enteros
    for(int i=0; i <num_ciudades;i++ ){
        read >> ciudad >> demanda[i];
        //ciudades.insert(ciudades., i);
    }
    int deposito; // cual es el deposito
    read >> deposito;
    // se lee
    read.close();
//    for (int i = 0; i < demanda.size(); i++) {
//        cout << demanda[i] << ' ';
//    }
    deposito -= 1;
    
    BusquedaTabu T(nodo, demanda, num_ciudades, num_rutas, deposito, capacidad); // @parametros: coordenadas, numero de ciudades, numero de caminos, indice deposito
    //T.ImprimirCoordenadas(); // Las coordenadas se estan recibiendo bien!!!
    cout << endl;
    T.CalcularMatrizDistancia();
    //T.ImprimirMatrizDistancia();
    T.PermutarRuta(1000);
}

