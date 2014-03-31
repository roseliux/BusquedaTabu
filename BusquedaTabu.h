//
//  BusquedaTabu.h
//  BusquedaTabu
//
//  Created by Rocela Durazo on 3/31/14.
//  Copyright (c) 2014 Rocela Durazo. All rights reserved.
//

#ifndef BusquedaTabu_BusquedaTabu_h
#define BusquedaTabu_BusquedaTabu_h

using namespace std;
#include <vector> // vector
#include <time.h> // rand
#include <cmath> // pow, sqrt
#include <deque>

#define TAM 5 //tam lista tabu
#define MEJORES 10

struct Nodo {
    float x;
    float y;
    
    Nodo(){
        x = 0;
        y = 0;
    }
};
struct Ruta{
    
    vector<int> ciudades;
    float distancia;
    int indice_deposto; // se obtiene del vector coordenada
    
    Ruta():distancia(0), indice_deposto(0){ }
    Ruta(vector<int> c, float d, int dep):ciudades(c), distancia(d), indice_deposto(dep){ }
    void Imprimir() {
        cout << "Ruta: (";
        for(int i = 0; i< ciudades.size(); i++){
            if( i == ciudades.size() -1){
                cout << ciudades[i] << ")" << " distancia: " << distancia << " deposito: " << indice_deposto;
            }
            else{
                cout << ciudades[i] << ", ";
            }
        }
    }
    void Agregar(vector<int> c, float dis, int dep){
        ciudades = c;
        distancia = dis;
        indice_deposto = dep;
    }
};
class BusquedaTabu {
    
private:
    //PROPIEDADES PRIVADAS
    const int num_ciudades;
    const int num_rutas;
    vector<Nodo> coordenadas_ciudades;
    float **matriz_distancia; // matriz donde esta cada distancia
    Ruta MejoresRutas[MEJORES]; // para guardar las 5 mejores rutas
    const int indice_deposito;
    
    
    // FUNCIONES PRIVADAS internas en la clase
    float formula_distancia(float a, float b, float c, float d){
        
        return sqrt(pow(c - a, 2) + pow(d - b, 2));
    }
    
    int GenerarRandom(int tam){ return rand() % (tam+1); } // generar random servira para las permutaciones, que nosea ni 0 o el ultimo
    vector<int> Intercambio(vector<int> r,int indice_1, int indice_2);
    
    
    
public:
    BusquedaTabu(vector<Nodo> c, int numciudades, int num_rutas_,int indice_deposito_):coordenadas_ciudades(c), num_ciudades(numciudades), num_rutas(num_rutas_), indice_deposito(indice_deposito_){
        
        matriz_distancia = new (std::nothrow) float *[num_ciudades];
        for (int count = 0; count < num_ciudades; count++){
            *(matriz_distancia + count) = new (std::nothrow) float[num_ciudades];
            
        }
        
        
    }
    // FUNCIONES PUBLICAS
    void OrdenarMejoresRutas();
    void AgregarMejoresRutas(vector<int> c, float dis, int dep);
    void ImprimirListaTabu(deque<int>);
    void ImprimirMejoresRutas();
    void ImprimirRuta(vector<int>);
    void ImprimirCoordenadas();
    vector<int> Inicializar();
    int IndiceDeposito(){ return rand() % num_ciudades + 1; } // regresa un deposito random
    void ImprimirMatrizDistancia();
    void CalcularMatrizDistancia();
    float PedirDistancia(int de, int a){ return matriz_distancia[de][a];}
    //vector<int> ObtenerRuta(){ return ruta_inicial;}
    vector<int> PermutarRuta(int num_permutaciones, int deposito); //regresara la ruta permutada
    bool SeEncuentraEnLaLista(deque<int> l, int indice1,int indice2, int tipo_movimiento); //determina si se encuentra en la listatabu
    float ObtenerDistanciaRuta(vector<int> r, int dep);
    vector<int> RepararRuta(vector<int> r); // regresa una ruta sin 0 al principo o final
    
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
vector<int> BusquedaTabu::PermutarRuta(int num_permutaciones, int deposito){
    
    // Lista donde se tendran los elementos tabu
    deque<int> lista_tabu; // primeros 2 los elementos a cambiar luego el tipo de movimiento: intercambio (1), corrimiento (2) e inversion de la ruta (3).
    vector<int> ruta_permutada = Inicializar();
    // hacer el shuffle
    for (int i = 0; i < 10; i++) {
        random_shuffle(ruta_permutada.begin(), ruta_permutada.end());
    }
    cout << "\nRuta Permutada: ";
    ImprimirRuta(ruta_permutada);
    cout << " distancia: " << ObtenerDistanciaRuta(ruta_permutada, deposito);
    cout << endl;
    
    int indice1, indice2; // indices a permutar
    int tipo_movimiento = 1; // tipo de movimiento ... esto despues se cambiara a random por mientras solo 1
    
    // primero preparar la ruta que los ceros no esten pegados, ni al principio o final.
    while(ruta_permutada[0] == 0 || ruta_permutada[ruta_permutada.size()-1] == 0){//|| juntos){
        ruta_permutada = RepararRuta(ruta_permutada);
        
    }
    cout << "Ruta Reparada: ";
    ImprimirRuta(ruta_permutada);
    cout << " distancia: " << ObtenerDistanciaRuta(ruta_permutada, deposito);
    cout << endl;
    
    /*********** Aqui se llena mejores_caminos y la lista tabu que no este vacia**********/
    // inicializar MejoresRutas, lista_tabu.
    indice1 = GenerarRandom(ruta_permutada.size()-1);
    indice2 = GenerarRandom(ruta_permutada.size()-1);
    
    while (indice1 == indice2) {
        indice2 = GenerarRandom(ruta_permutada.size()-1);
    }
    
    lista_tabu.push_back(indice1);
    lista_tabu.push_back(indice2);
    lista_tabu.push_back(tipo_movimiento);
    
    ruta_permutada = Intercambio(ruta_permutada, indice1, indice2);
    
    for (int ind = 0; ind < MEJORES; ind++) {
        MejoresRutas[ind].Agregar(ruta_permutada, ObtenerDistanciaRuta(ruta_permutada,deposito), deposito);
    }
    Ruta Ruta1(ruta_permutada, ObtenerDistanciaRuta(ruta_permutada, deposito), deposito);
    
    // Se empieza a permutar
    for (int perm = 0; perm < num_permutaciones; perm++) {
        // Verificar si se puede hacer esa permutacion, si no esta en la lista tabu.
        indice1 = GenerarRandom(ruta_permutada.size()-1);
        indice2 = GenerarRandom(ruta_permutada.size()-1);
        // que no se repitan los indices
        while (indice1 == indice2) {
            indice2 = GenerarRandom(ruta_permutada.size()-1);
        }
        tipo_movimiento = 1;
        
        vector<int> ruta2 = Intercambio(ruta_permutada, indice1, indice2);
        while(ruta2[0] == 0 || ruta2[ruta2.size()-1] == 0){//|| juntos){
            ruta2 = RepararRuta(ruta_permutada);
            
        }
        Ruta Ruta2(ruta2, ObtenerDistanciaRuta(ruta2, deposito), deposito);
        
        
        if (Ruta2.distancia < Ruta1.distancia || !SeEncuentraEnLaLista(lista_tabu, indice1, indice2, tipo_movimiento)) {
            // si es menor se acepta el cambio
            ruta_permutada = ruta2;
            
            // y se mete a la lista tabu
            lista_tabu.push_front(tipo_movimiento);
            lista_tabu.push_front(indice2);
            lista_tabu.push_front(indice1);
            
            if(lista_tabu.size()/3 == TAM+1){
                lista_tabu.pop_back();
                lista_tabu.pop_back();
                lista_tabu.pop_back();
            }
            
            AgregarMejoresRutas(ruta_permutada, Ruta2.distancia, Ruta2.indice_deposto);
            
        }
        
        
    }
    ImprimirListaTabu(lista_tabu);
    cout << endl;
    ImprimirMejoresRutas();
    cout << "Tam lista tabu: " << lista_tabu.size()/3 << ' ';
    return ruta_permutada;
}
void BusquedaTabu::OrdenarMejoresRutas(){
    
    float dis_aux = 0;
    vector<int> ciud_aux;
    int dep_aux;
    
    for (int i = 0; i < MEJORES -1; ++i){
        for (int j = 0; j < MEJORES - i - 1; ++j){
            if (MejoresRutas[j].distancia > MejoresRutas[j + 1].distancia){
                // las de aux
                dis_aux = MejoresRutas[j].distancia;
                ciud_aux = MejoresRutas[j].ciudades;
                dep_aux = MejoresRutas[j].indice_deposto;
                //las de j
                MejoresRutas[j].distancia = MejoresRutas[j + 1].distancia;
                MejoresRutas[j].ciudades = MejoresRutas[j + 1].ciudades;
                MejoresRutas[j].indice_deposto = MejoresRutas[j + 1].indice_deposto;
                //las de j+1
                MejoresRutas[j + 1].distancia = dis_aux;
                MejoresRutas[j + 1].ciudades = ciud_aux;
                MejoresRutas[j + 1].indice_deposto = dep_aux;
            }
        }
    }
    
}
void BusquedaTabu::AgregarMejoresRutas(vector<int> c, float dis, int dep){
    for (int ind = 0; ind < MEJORES; ind++) {
        if(MejoresRutas[ind].distancia > dis && MejoresRutas[ind].distancia != dis){
            MejoresRutas[MEJORES-1].Agregar(c, dis, dep);
            OrdenarMejoresRutas();
            return;
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
    
    // inicializar coordenadas ruta
    
    Nodo aux;
    aux.x = coordenadas_ciudades[0].x;
    aux.y = coordenadas_ciudades[0].y;
    coordenadas_ciudades[0].x = coordenadas_ciudades[indice_deposito].x;
    coordenadas_ciudades[0].y = coordenadas_ciudades[indice_deposito].y;
    coordenadas_ciudades[indice_deposito].x = aux.x;
    coordenadas_ciudades[indice_deposito].y = aux.y;
    
    cout << "Ruta Inicial : ";
    ImprimirRuta(ruta_inicial);
    
    // inicializar matriz
    
    
    return ruta_inicial;
    
}
void BusquedaTabu::ImprimirMatrizDistancia(){
    
    for (int j = 0; j < num_ciudades; j++){
        for (int i = 0; i < num_ciudades; i++) {
            if(i != num_ciudades - 1){
                cout << matriz_distancia[j][i] << ", ";
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
vector<int> BusquedaTabu::Intercambio(vector<int> r, int indice_1, int indice_2){
    int aux = r[indice_1];
    
    r[indice_1] = r[indice_2];
    r[indice_2] = aux;
    
    return r;
}
vector<int> BusquedaTabu::RepararRuta(vector<int> r){
    int indice;
    while (r[0] == 0) {
        indice = GenerarRandom(r.size());
        r[0] = r[indice];
        r[indice] = 0;
    }
    while(r[r.size() - 1] == 0){
        indice = GenerarRandom(r.size());
        r[r.size() - 1] = r[indice];
        r[indice] = 0;
    }
    return r;
}
bool BusquedaTabu::SeEncuentraEnLaLista(deque<int> cola, int indice1, int indice2, int tipo_movimiento){
    
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
float BusquedaTabu::ObtenerDistanciaRuta(vector<int> r, int dep){
    
    float distancia = 0.0;
    //cambiar los 0, asi se me facilita y no pongo ifs
    for (int i = 0; i < r.size()-1; i++) {
        if(i == 0){
            distancia += PedirDistancia(0, r[0]);
            distancia += PedirDistancia(r[r.size()-1], 0);
        }
        else{
            distancia += PedirDistancia(r[i], r[i+1]);
        }
        
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
        MejoresRutas[i].Imprimir();
        cout << endl;
    }
    
}


#endif
