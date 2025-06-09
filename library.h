#ifndef PROYECT_LIBRARY_
#define PROYECT_LIBRARY_

#include <iostream>
#include <sstream> // Permite trabajar con las lineas de csv como strings (stringstream, getline)
#include <fstream> // Permite leer y escribir archivo externos (ifstream)
#include <iomanip>
#include <math.h>
#include "Lista\Lista.h"
#include "HashMap\HashMap.h" // CAMBIAR POR HASHMAP LIST PARA USAR MEJOR MANEJO DE COLISIONES
#define NOMBRE_ARCHIVO ("C:/Users/mairi/source/proyecto-2025-comision-tosetti-lindon-ortega-ruiz/ventas_sudamerica.csv")
using namespace std;

struct Venta {
    string fecha, pais, ciudad, cliente, producto, categoria, medio_envio, estado_envio;
    int id, cantidad;
    float precio_unitario, monto_total;
};

// Estructura que relaciona cada ciudad con su monto total
struct ciudad_monto {
    string ciudad;
    float total;
};

unsigned int hashFunc(unsigned int clave) {
    return clave;  // Retorna el valor del ID como el índice hash
}

unsigned int hashString(string clave) {
    unsigned int hash = 0;
    for (char c : clave) {
        hash = 31 * hash + c;
    }
    return hash;
}

auto trim = [](string s) { //FUNCION BUSCADA EN INTERNET PARA ASEGURAR QUE LOS DATOS NO TENGAN ESPACIOS EXTRAS
    while (!s.empty() && isspace(s.front())) s.erase(s.begin());
    while (!s.empty() && isspace(s.back())) s.pop_back();
    return s;
};

//Función carga de archivo csv
void loadFile (HashMap<unsigned int, Venta> &mapa, int &size) {
    
    ifstream archivo(NOMBRE_ARCHIVO); // Abrir el archivo

    string linea;
    char delimitador = ',';

    getline(archivo, linea); // Descartar primera linea

    while (getline(archivo, linea)) // Leer todas las líneas
    { 
        Venta v;

        stringstream stream(linea); // Convertir la cadena a un stream
        string id_venta, cantidad, precio_unitario, monto_total;
        // Extraer todos los valores de esa fila
        getline(stream, id_venta, delimitador);
        getline(stream, v.fecha, delimitador);
        getline(stream, v.pais, delimitador);
        getline(stream, v.ciudad, delimitador);
        getline(stream, v.cliente, delimitador);
        getline(stream, v.producto, delimitador);
        getline(stream, v.categoria, delimitador);
        getline(stream, cantidad, delimitador);
        getline(stream, precio_unitario, delimitador);
        getline(stream, monto_total, delimitador);
        getline(stream, v.medio_envio, delimitador);
        getline(stream, v.estado_envio, delimitador);

        // Ingresar los datos numericos a la struct
        v.id = stoi(id_venta);
        v.cantidad = stoi(cantidad);
        v.precio_unitario = stof(precio_unitario);
        v.monto_total = stof(monto_total);
        v.pais = trim(v.pais);
        v.ciudad = trim(v.ciudad);
        mapa.put(v.id, v);
        size++;
    }

    archivo.close();
}


Nodo<ciudad_monto>* obtenerNodo(Lista<ciudad_monto>& lista, int pos) { // Para trabajar con punteros, obtenemos el nodo
    if (pos < 0 || pos >= lista.getTamanio()) return nullptr;
    Nodo<ciudad_monto>* aux = lista.getInicio(); // el auxiliar es el primer nodo de la lista
    int i = 0;
    while (aux != nullptr && i < pos) { // busco la posicion
        aux = aux->getSiguiente();
        i++;
    }
    return aux; // devuelve el nodo correspondiente a la posicion
}

int posListAvg(Lista<ciudad_monto>& lista, int bot, int top) { //Obtengo la posición del promedio de la lista
    if (top - bot <= 0 || bot < 0 || top > lista.getTamanio()) return -1;

    Nodo<ciudad_monto>* nodo = obtenerNodo(lista, bot);
    if (!nodo) return -1;

    float suma = 0;
    for (int i = bot; i <= top; i++) {
        if (!nodo) return -1;
        suma += nodo->getDato().total;
        nodo = nodo->getSiguiente();
    }
    float promedio = suma/(top - bot);

    nodo = obtenerNodo(lista,bot);
    int pos = bot;
    float dif_min = abs(nodo->getDato().total - promedio), dif_actual;
    for (int i = bot; i <= top && nodo; ++i) {
        float dif = fabs(nodo->getDato().total - promedio);
        if (dif < dif_min) {
            dif_min = dif;
            pos = i;
        }
        nodo = nodo->getSiguiente();
    }
    return pos;
}

// funcion de particion para simplificar el quicksort, divide en mayores y menores que el pivote 
int partition(Lista<ciudad_monto>& lista, int bot, int top) { //
    if (bot > top) return bot;

    int pivotIndex = posListAvg(lista, bot, top);
    if (pivotIndex == -1) return bot;

    Nodo<ciudad_monto>* pivotNodo = obtenerNodo(lista, pivotIndex); // el pivot es el promedio de la seccion
    Nodo<ciudad_monto>* pivotMedio = obtenerNodo(lista, (bot + top)/2); //Obtengo el nodo del medio
    if (!pivotNodo || !pivotMedio) return bot;
    
    float pivot = pivotNodo->getDato().total; //obtengo el total del pivot
    
    //Intercambio la posicion de esos dos pivotes
    ciudad_monto temp = pivotNodo->getDato();
    pivotNodo->setDato(pivotMedio->getDato());
    pivotMedio->setDato(temp);
    
    int i = bot, j = top;

    while (i <= j) {
        Nodo<ciudad_monto>* nodoI = obtenerNodo(lista, i);
        while (nodoI && nodoI->getDato().total < pivot) {
            i++;
            nodoI = obtenerNodo(lista, i);
        }

        Nodo<ciudad_monto>* nodoJ = obtenerNodo(lista, j);
        while (nodoJ && nodoJ->getDato().total > pivot) {
            j--;
            nodoJ = obtenerNodo(lista, j);
        }

        if (i <= j && nodoI && nodoJ) {
            ciudad_monto temp = nodoI->getDato();
            nodoI->setDato(nodoJ->getDato());
            nodoJ->setDato(temp);
            i++;
            j--;
        }
    }
    return j;
}

// Implementación de Quicksort para Lista Enlazada
void quicksortListaCM(Lista<ciudad_monto>& lista, int bot, int top) {
    if (bot < top) {
        int pi = partition(lista, bot, top);
        quicksortListaCM(lista, bot, pi);
        quicksortListaCM(lista, pi + 1, top);
    }
}

void showTop5CiudadesPorMontoSegunPais(HashMap<unsigned int, Venta> &mapa, int &size) {
        
    // Top 5 ciudades con mayor monto de ventas por pais

    // MUY COOL --> Hicimos todo junto B)
    HashMap<string, Lista<ciudad_monto>> mapaPaises(31, hashString); //12 paises en sudamerica, ocupan el 40% --> bajas colisiones
    Lista<string> claves_mapaPaises; // Almaceno las claves de mapaPaises
    // Repite por cada venta en el mapa 
    for (int i = 1; i <= size; i++) { //IF básico de conteo --> no cuenta
        Venta v = mapa.get(i);
        ciudad_monto cm = {v.ciudad, v.monto_total};
        // creo el objeto ciudad monto (si no existe la ciudad, se insertará; si ya existe se usa para reemplazar el valor del monto)
        int pos = 0;

        if (!mapaPaises.contieneClave(v.pais)) { //Si el pais todavia no existe
            Lista<ciudad_monto> ciudades; //creo la lista de ciudades
            ciudades.insertarUltimo(cm); //inserto la ciudad actual
            mapaPaises.put(v.pais, ciudades); //inserto la lista la hashmap
            claves_mapaPaises.insertarUltimo(v.pais); //inserto la clave a la lista de claves
        } else { // Si el pais existe
            Lista<ciudad_monto> ciudades = mapaPaises.get(v.pais);
            bool found = false;
            for (int j = 0; j < ciudades.getTamanio() && !found; j++) { // reviso todas las ciudades de la lista
                if (ciudades.getDato(j).ciudad == v.ciudad) {
                    cm.total = ciudades.getDato(j).total + v.monto_total;
                    ciudades.reemplazar(j, cm);
                    found = true; //Indico que se encontró la ciudad y altero el monto
                }
            }
            if (!found) {
                ciudades.insertarUltimo(cm); //inserto la ciudad si no existe
            }
            mapaPaises.put(v.pais, ciudades);
        }
    }
    
    cout << endl << "------------------------" << endl;
    cout << "Top 5 ciudades por país según monto: " << endl;
    
    for (int i = 0; i < claves_mapaPaises.getTamanio(); i++) {
        cout << "--------------------" << endl;
        cout << "País: " << claves_mapaPaises.getDato(i) << endl;

        string clave = claves_mapaPaises.getDato(i);

        if (!mapaPaises.contieneClave(clave)) {
            cout << "Error: el mapa no contiene la clave: " << clave << endl;
            continue;
        }

        Lista<ciudad_monto> paisactual = mapaPaises.get(clave);

        if (paisactual.getTamanio() > 1) {
            quicksortListaCM(paisactual, 0, paisactual.getTamanio() - 1);
        }
        
        int j = min(4, (paisactual.getTamanio() - 1)); //posicion 4 o tamaño de la lista

        while (j >= 0) { // Imprime en orden descendente
            cout << fixed << setprecision(2);
            cout << endl;
            cout << "Ciudad: " << paisactual.getDato(j).ciudad << endl;
            cout << "Monto: " << paisactual.getDato(j).total << endl; 
            cout << endl;
            j--;
        }
    }
}

#endif