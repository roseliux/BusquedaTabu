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
    int num_rutas = 3;
    //int num_ceros = num_rutas - 1;
    
    // LEER NUMERO DE CIUDADES POR EL ARCHIVO DE TXT
    ifstream read;
    read.open("coordinates.txt");
    read>>num_ciudades;
    
    // ***********************************************
    vector<Nodo> nodo(num_ciudades);
    
    // LEER LAS COORDENADAS X Y Y
    
    int ciudad;
    for(int i=0; i <num_ciudades;i++ ){
        read>>ciudad>>nodo[i].x>>nodo[i].y;
        //ciudades.insert(ciudades., i);
    }
    read.close();
    // ***********************************************
    // IMPRIMIR NODO
    //    cout << num_ciudades << endl;
    //    for (int i = 0; i < num_ciudades; i++) {
    //        cout << nodo[i].x << ", " << nodo[i].y<<endl;
    //    }
    // ***********************************************
    
    BusquedaTabu T(nodo, 15, num_rutas, 4); // @parametros: coordenadas, numero de ciudades, numero de caminos
    //T.ImprimirCoordenadas(); // Las coordenadas se estan recibiendo bien!!!
    cout << endl;
    //T.Inicializar();
    //T.ImprimirRuta();
    cout << endl;
    T.CalcularMatrizDistancia();
    //T.ImprimirMatrizDistancia();
    T.PermutarRuta(100000,0);
    
    
    
}

