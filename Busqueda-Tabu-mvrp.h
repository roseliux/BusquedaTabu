//
//  Busqueda-Tabu-mvrp.h
//  Busqueda-Tabu-mvrp
//
//  Created by Rocela Durazo on 3/31/14.
//  Copyright (c) 2014 Rocela Durazo. All rights reserved.
//

#ifndef Busqueda_Tabu_mvrp_Busqueda_Tabu_mvrp_h
#define Busqueda_Tabu_mvrp_Busqueda_Tabu_mvrp_h

using namespace std;
#include <vector> // vector
#include <time.h> // rand
#include <cmath> // pow, sqrt
#include <deque>

#define TAM 10 //tam lista tabu
#define MEJORES 5
#define LAMDA 10

struct Vecino{
    
    int indice1;
    int indice2;
    float distancia;
};

struct Nodo {
    float x;
    float y;
    
    Nodo(){
        x = 0.0;
        y = 0.0;
    }
};
struct Ruta{
    
    vector<int> ciudades;
    float distancia;
    vector<float> vector_distancia;
    
    Ruta():distancia(0){ }
    Ruta(vector<int> c, float d, vector<float> vd):ciudades(c), distancia(d), vector_distancia(vd){ }
    void Imprimir() {
        cout << "Ruta: (";
        for(int i = 0; i< ciudades.size(); i++){
            if( i == ciudades.size() -1){
                cout << ciudades[i] << ")" << " distancia: " << distancia;
            }
            else{
                cout << ciudades[i] << ", ";
            }
        }
    }
    void Asignar(vector<int> c, float dis, vector<float> vd){
        ciudades = c;
        distancia = dis;
        vector_distancia = vd;
        
    }
    // sobrecargar el operador = para asignar ruta1 = ruta2
    Ruta& operator = (const Ruta& r){
        ciudades = r.ciudades;
        distancia = r.distancia;
        vector_distancia = r.vector_distancia;
        return *this;
    }
};
class BusquedaTabu {
    
private:
    //PROPIEDADES PRIVADAS
    const int capacidad;
    vector<int> demanda; // demanda por cada nodo
    const int num_ciudades;
    const int num_rutas;
    vector<Nodo> coordenadas_ciudades;
    float **matriz_distancia; // matriz donde esta cada distancia
    Ruta MejoresRutas[MEJORES]; // para guardar las 5 mejores rutas globales
    Vecino vecindad[TAM + 1]; // se guardan los vecinos para seleccionar uno
    const int indice_deposito;
    deque<int> lista_tabu; // cola lista tabu donde se tendran los elementos tabu, primeros 2 los elementos a cambiar luego el tipo de movimiento: intercambio (1), corrimiento (2) e inversion de la ruta (3).
    
    
    // METODOS PRIVADAS internas en la clase
    void agregar_lista_tabu(int indice1, int indice2, int tipo_movimiento){
        lista_tabu.push_front(tipo_movimiento);
        lista_tabu.push_front(indice2);
        lista_tabu.push_front(indice1);
        
        while(lista_tabu.size()/3 > TAM){
            lista_tabu.pop_back();
            lista_tabu.pop_back();
            lista_tabu.pop_back();
        }
    }
    int costo_capacidad(vector<int> ciudades); // este costo es para saber cuanto se pasa en las rutas.
    void inicializar_vecindad();
    void ordenar_vecindad(); //ordenar el arreglo vecindad de menor distancia total a mayor
    void agregar_vecindad(int indice1, int indice2, float distancia); // agrega una ruta en el arreglo vecindad
    vector<float> ObtenerVectorDistancia(vector<int>);
    float ObtenerDistanciaTotal(vector<float> vd){
        float dis = 0.0;
        for (int i = 0; i < vd.size(); i++) {
            dis += vd[i];
        }
        return dis;
    }
    float formula_distancia(float a, float b, float c, float d){
        
        return sqrt(pow(c - a, 2) + pow(d - b, 2));
    }
    
    int GenerarRandom(int tam){ return rand() % (tam+1); } // generar random servira para las permutaciones, que nosea ni 0 o el ultimo
    Ruta Intercambio(Ruta r,int indice_1, int indice_2);
    bool CerosJuntos(vector<int> r);
    
    
    
public:
    BusquedaTabu(vector<Nodo> c, vector<int> demanda_,int numciudades, int num_rutas_,int indice_deposito_, int capacidad_):coordenadas_ciudades(c), demanda(demanda_), num_ciudades(numciudades), num_rutas(num_rutas_), indice_deposito(indice_deposito_), capacidad(capacidad_){
        
        matriz_distancia = new (std::nothrow) float *[num_ciudades];
        for (int count = 0; count < num_ciudades; count++){
            *(matriz_distancia + count) = new (std::nothrow) float[num_ciudades];
            
        }
        // inicializar matriz distancia
        for (int j = 0; j< numciudades; j++) {
            for (int i = 0; i < numciudades; i++) {
                matriz_distancia[j][i] = 0.00;
            }
        }
        
        
    }
    // FUNCIONES PUBLICAS
    Ruta actualizar_distancia_inversion(Ruta r, int indice1, int indice2);
    Ruta actualizar_distancia_intercambio(Ruta r, int indice1, int indice2);
    void imprimir_vecindad();
    Ruta Inversion(Ruta r, int indice_1, int indice_2);
    Ruta Corrimiento(Ruta r, int indice_1, int indice_2);
    Ruta ActualizarValoresRuta(Ruta);
    void generar_soluciones_vecinos(Ruta, int tipo_movimiento);
    void ImprimirVectorDistancia(vector<float>);
    void OrdenarMejoresRutas();
    void AgregarMejoresRutas(vector<int> c, float dis,vector<float> vd);
    void ImprimirListaTabu(deque<int>);
    void ImprimirMejoresRutas();
    void ImprimirRuta(vector<int>);
    void ImprimirCoordenadas();
    vector<int> Inicializar();
    void ImprimirMatrizDistancia();
    void CalcularMatrizDistancia();
    float PedirDistancia(int de, int a){ return matriz_distancia[de][a];}
    //vector<int> ObtenerRuta(){ return ruta_inicial;}
    void PermutarRuta(int num_permutaciones); //regresara la ruta permutada
    bool SeEncuentraEnLaLista(deque<int> l, int indice1,int indice2, int tipo_movimiento); //determina si se encuentra en la listatabu
    float ObtenerDistanciaRuta(vector<int> r);
    
    // Destructor - liberar matriz
    ~BusquedaTabu(){
        for( int i = 0 ; i < num_ciudades ; i++ )
        {
            //first we delete each row
            delete [] matriz_distancia[i] ;
        }
        //finally, we delete the array of pointers
        delete [] matriz_distancia;
    }
    
};
void BusquedaTabu::PermutarRuta(int num_permutaciones){
    
    
    vector<int> ruta_permutada = Inicializar();
    vector<float> vector_distancia = ObtenerVectorDistancia(ruta_permutada); // distancia de ciudad a ciudad
    float distancia_total = ObtenerDistanciaTotal(vector_distancia) + costo_capacidad(ruta_permutada);
    AgregarMejoresRutas(ruta_permutada, distancia_total,vector_distancia);
    Ruta ruta(ruta_permutada, distancia_total, vector_distancia);
    
    // Buscar vecindad
    Ruta mejor_vecina;
    int tipo_movimiento = 2; // tipo de movimiento 1(intercambio), 2(corrimiento) o 3(inversion)
    for (int i = 0; i < num_permutaciones; i++) {
        tipo_movimiento = rand() % 2 + 1; // tipo de movimiento 1(intercambio), 2(corrimiento) o 3(inversion)
        generar_soluciones_vecinos(ruta, tipo_movimiento);
        // Seleccionar la mejor solucion vecina
        if (lista_tabu.empty()) {
            // si esta vacia se elije la primera y se agrega a la lista tabu
            if (tipo_movimiento == 1) {
                mejor_vecina = Intercambio(ruta, vecindad[0].indice1, vecindad[0].indice2);
            }
            else if (tipo_movimiento == 2){
                mejor_vecina = Inversion(ruta, vecindad[0].indice1, vecindad[0].indice2);
            }
            agregar_lista_tabu(vecindad[0].indice2, vecindad[0].indice1, tipo_movimiento);
        }
        else{
            for (int vec_ind = 0; vec_ind < TAM + 1; vec_ind++) {
                //cout << vec_ind << endl;
                if (SeEncuentraEnLaLista(lista_tabu, vecindad[vec_ind].indice1, vecindad[vec_ind].indice2, tipo_movimiento) == false) {
                    if (tipo_movimiento == 1) {
                        mejor_vecina = Intercambio(ruta, vecindad[vec_ind].indice1, vecindad[vec_ind].indice2);
                    }
                    else if (tipo_movimiento == 2){
                        mejor_vecina = Inversion(ruta, vecindad[vec_ind].indice1, vecindad[vec_ind].indice2);
                    }
                    agregar_lista_tabu(vecindad[vec_ind].indice2, vecindad[vec_ind].indice1, tipo_movimiento);
                    break;
                }
            }
            
        }
        //cout << i << ',';
//        ImprimirRuta(mejor_vecina.ciudades);
//        cout << mejor_vecina.distancia <<" = " << ObtenerDistanciaRuta(mejor_vecina.ciudades);
//        cout << endl;
        
        mejor_vecina.vector_distancia = ObtenerVectorDistancia(mejor_vecina.ciudades);
//        mejor_vecina.distancia = ObtenerDistanciaTotal(mejor_vecina.vector_distancia);
        mejor_vecina.distancia = ObtenerDistanciaTotal(mejor_vecina.vector_distancia) + costo_capacidad(mejor_vecina.ciudades);
        
        if (costo_capacidad(mejor_vecina.ciudades) < 1) {
            AgregarMejoresRutas(mejor_vecina.ciudades, mejor_vecina.distancia, mejor_vecina.vector_distancia);
        }
        
        ruta.Asignar(mejor_vecina.ciudades, mejor_vecina.distancia, mejor_vecina.vector_distancia);
//        cout << endl;
//        ImprimirRuta(r.ciudades);
//        cout << endl;
//        ImprimirVectorDistancia(r.vector_distancia);
    }
    
    
    cout << "\nLista tabu "<< endl;
    ImprimirListaTabu(lista_tabu);
    
    cout << "\nMejores "<< MEJORES << " Rutas" << endl;
    ImprimirMejoresRutas();
    
    
}
int BusquedaTabu::costo_capacidad(vector<int> ciudades){
    int costo = 0;
    int aux = 0;
    
    for (int i = 0; i < ciudades.size(); i++) {
        int ind = ciudades[i];
        //cout << "demanda[" << ind << "] = " << demanda[ind] << ',';
        aux += demanda[ind];
        if (ciudades[i] == 0 || i == ciudades.size() - 1){
            //cout << aux  << ", ";
            if (aux > capacidad) {
                //cout << " hahahah " << aux - capacidad <<  " adf "<<endl;
                costo = costo + (aux - capacidad);
                
                
            }
            aux = 0;
        }
        //cout << costo <<  endl;
    }
    
    //cout << "costo final " << costo << endl;
    //cout << LAMDA * costo << endl;
    return LAMDA * costo;
}
void BusquedaTabu::generar_soluciones_vecinos(Ruta r, int tipo_moviento){
    
    Ruta vecina(r.ciudades, r.distancia, r.vector_distancia);
    Ruta ruta_vecina;
    float distancia_vecino = 0.00;
    inicializar_vecindad();// inicializar el arreglo vecindad
    
    // generar soluciones buscando en la vecindad guardarlos en vecindad[TAM + 1]
    // si el tipo de movimiento es 1 se realiza el intercambio en toda la vecindad
    for (int j = 0; j < r.ciudades.size(); j++) {
        for (int i = 1+j; i < r.ciudades.size(); i++) {
            if (tipo_moviento == 1) {
                ruta_vecina = Intercambio(r, j, i);
                
            }
            else if(tipo_moviento == 2){
                ruta_vecina = Inversion(r, j, i);
            
            }
            
            if (ruta_vecina.ciudades[0] != 0 && ruta_vecina.ciudades[ruta_vecina.ciudades.size()-1] != 0 && !CerosJuntos(ruta_vecina.ciudades)){
                // se actualiza el vector distancia y distancia total, se mete al arreglo vecindad.
                //ruta_vecina.distancia = actualizar_distancia_inversion(ruta_vecina, j, i).distancia;
                //ruta_vecina.distancia = ObtenerDistanciaRuta(ruta_vecina.ciudades);
                distancia_vecino = ruta_vecina.distancia + costo_capacidad(ruta_vecina.ciudades);
                // guardarla en el array vecindad
                agregar_vecindad(j, i, distancia_vecino);
//                cout << ObtenerDistanciaRuta(ruta_vecina.ciudades) << " = " << ObtenerDistanciaTotal(ObtenerVectorDistancia(ruta_vecina.ciudades)) << " != " << ruta_vecina.distancia<< endl;
//                imprimir_vecindad();
            }
        }
    }
    
    
    //imprimir_vecindad();
    
}
Ruta BusquedaTabu::actualizar_distancia_inversion(Ruta r, int indice1, int indice2){
    
    // PARA VER SI LAS COSAS ESTAN BIEN
//    ImprimirRuta(r.ciudades);
//    cout << endl;
//    cout << "vector distancia " << indice1 << ", " << indice2<< endl;

    float aux;
    
    int indice1_ = indice1, indice2_ = indice2; // copias de indice1 e indice2
    
    // se invierten en el vector distancia
    while (indice1 != indice2) {
        if (abs(indice1 - indice2) == 1) {
            
            aux = r.vector_distancia[indice1];
            r.vector_distancia[indice1] = r.vector_distancia[indice2];
            r.vector_distancia[indice2] = aux;
            indice1 = indice2;
            
        }
        else{
            aux = r.vector_distancia[indice1];
            r.vector_distancia[indice1] = r.vector_distancia[indice2];
            r.vector_distancia[indice2] = aux;
        
            indice1++;
            indice2--;
        }
        
    }
    r.distancia = r.distancia - r.vector_distancia[indice2_];
    // se recorren un espacio desde [indice1, indice2] hacia la derecha
    for (int i = indice2_; i > indice1_; i--) {
        r.vector_distancia[i] = r.vector_distancia[i-1];
        
    }
    r.distancia = r.distancia + r.vector_distancia[indice1_];
    // se restan las distancias que vamos a cambiar
    r.distancia = r.distancia - r.vector_distancia[indice1_] - r.vector_distancia[indice2_ + 1];
    // se calculan solo los que estan a la derecha de indice2 y a la izquierda de ind
    
    if (indice1_ == 0) {
        if (indice2_ != r.ciudades.size()-1) {
            r.vector_distancia[indice1_] = PedirDistancia(r.ciudades[indice1_], 0);
            r.vector_distancia[indice2_ + 1] = PedirDistancia(r.ciudades[indice2_], r.ciudades[indice2_ + 1]);
        }
        else if(indice2_ == r.ciudades.size()-1){
            r.vector_distancia[indice1_] = PedirDistancia(r.ciudades[indice1_], 0);
            r.vector_distancia[indice2_ + 1] = PedirDistancia(r.ciudades[indice2_], 0);
        }
        
    }
    else if(indice1_ != 0){
        if (indice2_ != r.ciudades.size()-1) {
            r.vector_distancia[indice1_] = PedirDistancia(r.ciudades[indice1_], r.ciudades[indice1_ - 1]);
            r.vector_distancia[indice2_] = PedirDistancia(r.ciudades[indice2_], r.ciudades[indice2_ - 1]);
            r.vector_distancia[indice2_ + 1] = PedirDistancia(r.ciudades[indice2_], r.ciudades[indice2_ + 1]);
        }
        else if(indice2_ == r.ciudades.size()-1){
            r.vector_distancia[indice1_] = PedirDistancia(r.ciudades[indice1_], r.ciudades[indice1_ - 1]);
            r.vector_distancia[indice2_] = PedirDistancia(r.ciudades[indice2_], r.ciudades[indice2_ - 1]);
            r.vector_distancia[indice2_ + 1] = PedirDistancia(r.ciudades[indice2_], 0);
        }
    }
    
    // se suman las distancias actualizadas
    r.distancia = r.distancia + r.vector_distancia[indice1_] + r.vector_distancia[indice2_ + 1];
    
    
    /* PARA VER SI LAS COSAS ESTAN BIEN
    ImprimirVectorDistancia(r.vector_distancia);
    cout << ' ' << r.distancia << ' ' << ObtenerDistanciaTotal(r.vector_distancia);
    cout <<endl;
    ImprimirVectorDistancia(ObtenerVectorDistancia(r.ciudades));
    cout << ' ' << ObtenerDistanciaTotal(ObtenerVectorDistancia(r.ciudades));
    cout << endl;
    */
    
    return r;
}
void BusquedaTabu::inicializar_vecindad(){
    
    for (int ind = 0; ind < TAM+1; ind++) {
        vecindad[ind].indice1 = 0;
        vecindad[ind].indice2 = 0;
        vecindad[ind].distancia = -1.0;
    }
}
Ruta BusquedaTabu::Intercambio(Ruta r, int indice_1, int indice_2){
    //cout << indice_1 << ' ' << indice_2 << endl;

    int aux = r.ciudades[indice_1];
    r.ciudades[indice_1] = r.ciudades[indice_2];
    r.ciudades[indice_2] = aux;
    
    // actulizar distancia y vector distancia.
//    r.vector_distancia = ObtenerVectorDistancia(r.ciudades);
//    r.distancia = ObtenerDistanciaTotal(r.vector_distancia);
//    return r;
    // forma mas optimizada
    return actualizar_distancia_intercambio(r, indice_1, indice_2);
}
Ruta BusquedaTabu::Inversion(Ruta r, int indice_1, int indice_2){
    
    int aux;
    // copias de los indices
    int indice1_ = indice_1;
    int indice2_ = indice_2;
    
    if (indice_1 == indice_2) {
        return r;
    }
    
    else if (indice_2 < indice_1) {
        // si el incie 2 es menor al indice 1 se invierten
        aux = indice_1;
        indice_1 = indice_2;
        indice_2 = aux;
    }
    
    while (indice_1 != indice_2) {
        
        if (abs(indice_1 - indice_2) == 1) {
            
            aux = r.ciudades[indice_1];
            r.ciudades[indice_1] = r.ciudades[indice_2];
            r.ciudades[indice_2] = aux;
            return r;
        }
        
        aux = r.ciudades[indice_1];
        r.ciudades[indice_1] = r.ciudades[indice_2];
        r.ciudades[indice_2] = aux;
        
        indice_1++;
        indice_2--;
        
    }
    // actualizar distancia total  y vector distancia. de la forma no optima
//    r.vector_distancia = ObtenerVectorDistancia(r.ciudades);
//    r.distancia = ObtenerDistanciaTotal(r.vector_distancia);
//    return r;
    //ImprimirVectorDistancia(r.vector_distancia);
    // de la forma optima con actualizar distancia inversion
    return actualizar_distancia_inversion(r, indice1_, indice2_);
}
Ruta BusquedaTabu::Corrimiento(Ruta r, int indice_1, int indice_2){
    
    // falta hacer
    return r;
}
void BusquedaTabu::agregar_vecindad(int indice1, int indice2, float distancia){
    
    // agrega una ruta en el arreglo vecindad
    
    for (int ind = 0; ind < TAM+1 ; ind++) {
        if (vecindad[ind].distancia <= 0) {
            vecindad[ind].indice1 = indice1;
            vecindad[ind].indice2 = indice2;
            vecindad[ind].distancia = distancia;
            break;
        }
        // al llenarse que se ordene // que se ordene arreglo vecindad por menor distancia
        if (ind == TAM) {
            ordenar_vecindad();
        }
    }
    
    // si ya esta lleno entonces que se agregue al final y que se ordene
    if (distancia < vecindad[TAM].distancia) {
        vecindad[TAM].indice1 = indice1;
        vecindad[TAM].indice2 = indice2;
        vecindad[TAM].distancia = distancia;
    }
    ordenar_vecindad();

    
    
}
void BusquedaTabu::ordenar_vecindad(){
    
    int indice1_aux, indice2_aux;
    float dis_aux = 0;
    
    for (int i = 0; i < (TAM+1) -1; ++i){
        for (int j = 0; j < (TAM+1) - i - 1; ++j){
            if (vecindad[j].distancia > vecindad[j + 1].distancia){
                // las de aux
                dis_aux = vecindad[j].distancia;
                indice1_aux = vecindad[j].indice1;
                indice2_aux = vecindad[j].indice2;
                
                //las de j
                vecindad[j].distancia = vecindad[j + 1].distancia;
                vecindad[j].indice1 = vecindad[j + 1].indice1;
                vecindad[j].indice2 = vecindad[j + 1].indice2;
                
                //las de j+1
                vecindad[j + 1].distancia = dis_aux;
                vecindad[j + 1].indice1 = indice1_aux;
                vecindad[j + 1].indice2 = indice2_aux;
                
            }
        }
    }
}
Ruta BusquedaTabu::actualizar_distancia_intercambio(Ruta r, int indice1, int indice2){
    
    // se restan las distancias que vamos a cambiar
    r.distancia = r.distancia - r.vector_distancia[indice1] - r.vector_distancia[indice1 + 1]
    - r.vector_distancia[indice2] - r.vector_distancia[indice2 + 1];
    
    // se actualiza el vector distancia, dependiendo de los indices cambiados
    if(indice1 == 0 && indice2 < r.ciudades.size()-1){
        
        r.vector_distancia[indice1] = PedirDistancia(r.ciudades[indice1], 0);
        r.vector_distancia[indice1 + 1] = PedirDistancia(r.ciudades[indice1], r.ciudades[indice1+1]);
        r.vector_distancia[indice2] = PedirDistancia(r.ciudades[indice2], r.ciudades[indice2-1]);
        r.vector_distancia[indice2 + 1] = PedirDistancia(r.ciudades[indice2], r.ciudades[indice2+1]);
        
    }
    else if (indice1 == 0 && indice2 == r.ciudades.size()-1){
        
        r.vector_distancia[indice1] = PedirDistancia(r.ciudades[indice1], 0);
        r.vector_distancia[indice1+1] = PedirDistancia(r.ciudades[indice1], r.ciudades[indice1+1]);
        r.vector_distancia[indice2] = PedirDistancia(r.ciudades[indice2], r.ciudades[indice2-1]);
        r.vector_distancia[indice2+1] = PedirDistancia(r.ciudades[indice2], 0);
        
    }
    else if (indice1 != 0 && indice2 == r.ciudades.size()-1){
        
        r.vector_distancia[indice1] = PedirDistancia(r.ciudades[indice1], r.ciudades[indice1 - 1]);
        r.vector_distancia[indice1+1] = PedirDistancia(r.ciudades[indice1], r.ciudades[indice1 + 1]);
        r.vector_distancia[indice2] = PedirDistancia(r.ciudades[indice2], r.ciudades[indice2-1]);
        r.vector_distancia[indice2+1] = PedirDistancia(r.ciudades[indice2], 0);
    
    }
    else if(indice1 != 0 && indice2 != r.ciudades.size()-1){
    
        r.vector_distancia[indice1] = PedirDistancia(r.ciudades[indice1], r.ciudades[indice1 - 1]);
        r.vector_distancia[indice1+1] = PedirDistancia(r.ciudades[indice1], r.ciudades[indice1 + 1]);
        r.vector_distancia[indice2] = PedirDistancia(r.ciudades[indice2], r.ciudades[indice2 - 1]);
        r.vector_distancia[indice2+1] = PedirDistancia(r.ciudades[indice2], r.ciudades[indice2 + 1]);
    
    }
    
    // se actualiza la distancia total, se suman las nuevas distancias
    r.distancia = r.distancia + r.vector_distancia[indice1] + r.vector_distancia[indice1 + 1]
    + r.vector_distancia[indice2] + r.vector_distancia[indice2 + 1];
    
//    ImprimirRuta(r.ciudades);
//    cout << endl;
//    cout << "vector distancia " << indice1 << ", " << indice2<< endl;
//    ImprimirVectorDistancia(r.vector_distancia);
//    cout << ' ' << r.distancia;
//    cout <<endl;
//    ImprimirVectorDistancia(ObtenerVectorDistancia(r.ciudades));
//    cout << ' ' << ObtenerDistanciaTotal(ObtenerVectorDistancia(r.ciudades));
//    cout << endl;
    
    return r;
}
vector<float> BusquedaTabu::ObtenerVectorDistancia(vector<int> r){
    
    vector<float> distancia;
    distancia.push_back(PedirDistancia(0, r[0]));
    for (int i = 0; i < r.size() - 1 ; i++) {
        distancia.push_back(PedirDistancia(r[i], r[i+1]));
    }
    distancia.push_back(PedirDistancia(r[r.size()-1], 0));
    return distancia;
}
void BusquedaTabu::ImprimirVectorDistancia(vector<float> r){
   
    cout << "(";
    for(int i = 0; i< r.size(); i++){
        if( i == r.size() -1){
            cout << r[i] << ")";;
        }
        else{
            cout << r[i] << ", ";
        }
    }
    
}
void BusquedaTabu::OrdenarMejoresRutas(){
    
    float dis_aux = 0;
    vector<int> ciud_aux;
    vector<float> vectordis_aux;
    
    for (int i = 0; i < MEJORES -1; ++i){
        for (int j = 0; j < MEJORES - i - 1; ++j){
            if (MejoresRutas[j].distancia > MejoresRutas[j + 1].distancia){
                // las de aux
                dis_aux = MejoresRutas[j].distancia;
                ciud_aux = MejoresRutas[j].ciudades;
                vectordis_aux = MejoresRutas[j].vector_distancia;
                
                //las de j
                MejoresRutas[j].distancia = MejoresRutas[j + 1].distancia;
                MejoresRutas[j].ciudades = MejoresRutas[j + 1].ciudades;
                MejoresRutas[j].vector_distancia = MejoresRutas[j + 1].vector_distancia;
                
                //las de j+1
                MejoresRutas[j + 1].distancia = dis_aux;
                MejoresRutas[j + 1].ciudades = ciud_aux;
                MejoresRutas[j + 1].vector_distancia = vectordis_aux;
                
            }
        }
    }
    
}
void BusquedaTabu::AgregarMejoresRutas(vector<int> c, float dis, vector<float> vd){
    
    for (int ind = 0; ind < MEJORES; ind++) {
        if (MejoresRutas[ind].distancia == dis) {
            break;
        }
        else if(MejoresRutas[ind].distancia > dis){
            MejoresRutas[MEJORES-1].Asignar(c, dis, vd);
            OrdenarMejoresRutas();
            break;
        }
    }
    
}
void BusquedaTabu::ImprimirRuta(vector<int> r){
    
    cout << "(";
    for(int i = 0; i< r.size(); i++){
        if( i == r.size() -1){
            cout << r[i] << ")";;
        }
        else{
            cout << r[i] << ", ";
        }
    }
    
}
void BusquedaTabu::ImprimirCoordenadas(){
    
    for (int i = 0; i < num_ciudades; i++) {
        cout << i+1 << "=> " <<coordenadas_ciudades[i].x << ", " << coordenadas_ciudades[i].y<<endl;
    }
}
vector<int> BusquedaTabu::Inicializar(){
    
    // Inicializar ruta
    vector<int> ruta_inicial;
    int num_ceros = num_rutas - 1;
    
    for (int i = 1; i < (num_ciudades); i++) {
        ruta_inicial.push_back(i);
    }
    // agregar los 0
    for (int i = 0; i < num_ceros; i++) {
        ruta_inicial.push_back(0);
    }
    // revolver para que no queden 0 juntos, o al principio o final.
    while (ruta_inicial[0] == 0 || ruta_inicial[ruta_inicial.size()-1] == 0 || CerosJuntos(ruta_inicial)) {
        
        random_shuffle(ruta_inicial.begin(), ruta_inicial.end());
    }
    
    // inicializar coordenadas ruta
    if (indice_deposito != 0) {
        Nodo aux;
        aux.x = coordenadas_ciudades[0].x;
        aux.y = coordenadas_ciudades[0].y;
        coordenadas_ciudades[0].x = coordenadas_ciudades[indice_deposito].x;
        coordenadas_ciudades[0].y = coordenadas_ciudades[indice_deposito].y;
        coordenadas_ciudades[indice_deposito].x = aux.x;
        coordenadas_ciudades[indice_deposito].y = aux.y;
    }
    
    
    //Imprimir ruta inicial
    cout << "Ruta Inicial: ";
    ImprimirRuta(ruta_inicial);
    //cout << "\nVector distancia: " ; ImprimirVectorDistancia(ObtenerVectorDistancia(ruta_inicial));
    cout << " distancia: " << ObtenerDistanciaRuta(ruta_inicial);
    // *******************************
    
    // inicializar matriz_mejores distancia
    for (int ind = 0; ind < MEJORES; ind++) {
        MejoresRutas[ind].Asignar(ruta_inicial, ObtenerDistanciaRuta(ruta_inicial),ObtenerVectorDistancia(ruta_inicial));
    }
    
    
    
    return ruta_inicial;
    
}
void BusquedaTabu::ImprimirMatrizDistancia(){
    
    for (int j = 0; j < num_ciudades; j++){
        for (int i = 0; i < num_ciudades; i++) {
            if(i != num_ciudades - 1){
                cout << matriz_distancia[j][i] << "         ";
            }
            else{
                cout << matriz_distancia[j][i];
            }
        }
        cout << endl;
    }
}
void BusquedaTabu::CalcularMatrizDistancia(){
    
    for (int j = 0; j < num_ciudades; j++){
        for (int i = 0; i < num_ciudades; i++) {
            if(i == j){
                matriz_distancia[j][i] = 0;
            }
            else{
                
                matriz_distancia[j][i] = formula_distancia(coordenadas_ciudades[i].x, coordenadas_ciudades[i].y, coordenadas_ciudades[j].x, coordenadas_ciudades[j].y);
            }
        }
    }
}
bool BusquedaTabu::CerosJuntos(vector<int> r){
    for (int i = 0; i < r.size() -1 ; i++) {
        if (r[i] == 0 && r[i+1] == 0) {
            return true;
        }
    }
    return false;
}
bool BusquedaTabu::SeEncuentraEnLaLista(deque<int> cola, int indice1, int indice2, int tipo_movimiento){
    
    if (cola.empty()) {
        return false;
    }
    
    for (int ind = 0; ind < cola.size() -1 ; ind += 3) {
        if (cola[ind] == indice1 && cola[ind+1] == indice2 && cola[ind+2] == tipo_movimiento) {
            return true;
        }
        else if (cola[ind] == indice2 && cola[ind+1] == indice1 && cola[ind+2] == tipo_movimiento) {
            return true;
            
        }
    }
    return false;
    
}
float BusquedaTabu::ObtenerDistanciaRuta(vector<int> r){
    
    float distancia = PedirDistancia(0, r[0]) + PedirDistancia(r[r.size()-1], 0);

    //cambiar los 0, asi se me facilita y no pongo ifs
    for (int i = 0; i < r.size()-1; i++) {
        distancia += PedirDistancia(r[i], r[i+1]);
        
    }
    return distancia;
}
void BusquedaTabu::ImprimirListaTabu(deque<int> lt){
    
    for (int i=0; i< lt.size(); i++) {
        cout << lt[i] << ' ';
        if ((i+1)%3 == 0 && i != 0) {
            cout << endl;
        }
    }
    
}
void BusquedaTabu::ImprimirMejoresRutas(){
    
    for (int i=0; i< MEJORES; i++) {
        cout << i << ' ';
        MejoresRutas[i].Imprimir();
        //cout << " = " ;
        //ImprimirVectorDistancia(MejoresRutas[i].vector_distancia);
        //cout << ObtenerDistanciaTotal(MejoresRutas[i].vector_distancia);
        cout << " = " << ObtenerDistanciaTotal(MejoresRutas[i].vector_distancia);
        cout << " costo demanda =  " << costo_capacidad(MejoresRutas[i].ciudades);
        cout << endl;
    }
    
}
void BusquedaTabu::imprimir_vecindad(){
    
    cout << "Vecindad: " << endl;
    for (int i=0; i< TAM+1; i++) {
        cout << "vecino " << i << ':' << ' ';
        cout << vecindad[i].indice1 << ' ' << vecindad[i].indice2 << ' ' << vecindad[i].distancia;
        cout << endl;
    }
    
}

#endif
