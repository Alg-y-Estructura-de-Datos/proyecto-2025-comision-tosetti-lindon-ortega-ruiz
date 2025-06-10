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

struct producto_monto {
    string nombre;
    float total;
};

struct categoria {
    string nombre;
    float promedio;
    float total;
    int ventas;
};

struct medioenvio_cantidad {
    string nombre;
    int ventas;
};

struct estadisticas_pais {
    Lista<ciudad_monto> ciudadesOrdenadasMonto;
    Lista<producto_monto> productosMontoTotal;
    Lista<categoria> categoriasPromedio;
    Lista<medioenvio_cantidad> mediosDeEnvio;
};

unsigned int hashFunc(unsigned int clave) {
    return clave;  // Retorna el valor del ID como el índice hash
}

unsigned int hashString(string clave) {
    unsigned int hash = 0;
    for (char c : clave) {
        hash = 131 * hash + c;
        hash ^= (hash >> 5); // 'bitwise shift' --> mueve los bits para mejorar la distribucion
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

void bubbleSortMediosDeEnvio(HashMap<string, estadisticas_pais> &mapa, Lista<string>& claves) {
    bool swapped = false;
    int n;
    //Se realizan pasadas hasta que no se realicen más intercambios
    for (int i = 0; i < l - 1; i++) {
        if_count++; //cuento el if del for i-loop
        swapped = false;
        //Se comparan todos los elementos de a 2
        for (int j = 0; j < (l - i - 1); j++) {
            if_count++; //cuento el if del for j-loop
            if_count++; //cuento el if de abajo
            if (a[j] > a[j+1]) {

                swapped = true;
                n = a[j];
                a[j] = a[j + 1];
                a[j + 1] = n;
            }
        }
        if (!swapped) {
        if_count++; //cuento cada if
        break;
        }
    }
}

void ordenarTop5CiudadesPorMontoSegunPais(HashMap<string, estadisticas_pais>& mapa, Lista<string>& claves_mapaPaises) {  
    
    for (int i = 0; i < claves_mapaPaises.getTamanio(); i++) {

        string clave = claves_mapaPaises.getDato(i);

        if (!mapa.contieneClave(clave)) {
            cout << "Error: el mapa no contiene la clave: " << clave << endl;
            continue;
        }

        estadisticas_pais estadisticas = mapa.get(clave);

        if (estadisticas.ciudadesOrdenadasMonto.getTamanio() > 1) {
            quicksortListaCM(estadisticas.ciudadesOrdenadasMonto, 0, estadisticas.ciudadesOrdenadasMonto.getTamanio() - 1);
        }

        mapa.put(clave, estadisticas);
    }
}

void printTop5CiudadesPorMontoSegunPais(HashMap<string, estadisticas_pais> &mapaPaises, Lista<string> &claves) {
    
    cout << endl << "------------------------" << endl;
    cout << "Top 5 ciudades por país según monto: " << endl;

    for (int i = 0; i < claves.getTamanio(); i++) { //Print cada pais
        
        cout << "--------------------" << endl;
        cout << "País: " << claves.getDato(i) << endl;
        
        string clave = claves.getDato(i);
        
        if (!mapaPaises.contieneClave(clave)) {
            cout << "Error: el mapa no contiene la clave: " << clave << endl;
            continue;
        }

        Lista<ciudad_monto> paisactual = mapaPaises.get(clave).ciudadesOrdenadasMonto;

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

void printMontoTotalPorProductoSegunPais(HashMap<string, estadisticas_pais> &mapaPaises, Lista<string> &claves) {
    cout << endl << "------------------------" << endl;
    cout << "Monto total por producto por país: " << endl;

    for (int i = 0; i < claves.getTamanio(); i++) { //Print cada pais
        
        cout << "--------------------" << endl;
        cout << "País: " << claves.getDato(i) << endl;
        
        string clave = claves.getDato(i);
        
        if (!mapaPaises.contieneClave(clave)) {
            cout << "Error: el mapa no contiene la clave: " << clave << endl;
            continue;
        }

        Lista<producto_monto> paisactual = mapaPaises.get(clave).productosMontoTotal;

        for (int j = 0; j < paisactual.getTamanio(); j++) { // Imprime todos los productos
            cout << fixed << setprecision(2);
            cout << endl;
            cout << "Producto: " << paisactual.getDato(j).nombre << endl;
            cout << "Monto Total: " << paisactual.getDato(j).total << endl; 
            cout << endl;
        }
    }
}

void calcularPromedioVentasPorCategoriaSegunPais(HashMap<string, estadisticas_pais>&mapa, Lista<string> &clavesPaises) {
    for (int i = 0; i < clavesPaises.getTamanio(); i++) {

        string clave = clavesPaises.getDato(i);

        if (!mapa.contieneClave(clave)) {
            cout << "Error: el mapa no contiene la clave: " << clave << endl;
            continue;
        }

        estadisticas_pais estadisticas = mapa.get(clave);

        for (int j = 0; j < estadisticas.categoriasPromedio.getTamanio(); j++) {
            categoria c = estadisticas.categoriasPromedio.getDato(j);
            c.promedio = c.total / c.ventas;
            estadisticas.categoriasPromedio.reemplazar(j, c);
        }

        mapa.put(clave, estadisticas);
    }
}

void printPromedioVentasPorCategoriaSegunPais(HashMap<string, estadisticas_pais> &mapaPaises, Lista<string> &claves) {
    cout << endl << "------------------------" << endl;
    cout << "Promedio ventas por categorias: " << endl;

    for (int i = 0; i < claves.getTamanio(); i++) { //Print cada pais
        
        cout << "--------------------" << endl;
        cout << "País: " << claves.getDato(i) << endl;
        
        string clave = claves.getDato(i);
        
        if (!mapaPaises.contieneClave(clave)) {
            cout << "Error: el mapa no contiene la clave: " << clave << endl;
            continue;
        }

        Lista<categoria> paisactual = mapaPaises.get(clave).categoriasPromedio;

        for (int j = 0; j < paisactual.getTamanio(); j++) { // Imprime todos los productos
            cout << fixed << setprecision(2);
            cout << endl;
            cout << "Categoria: " << paisactual.getDato(j).nombre << endl;
            cout << "Promedio: " << paisactual.getDato(j).promedio << endl; 
            cout << endl;
        }
    }
}

HashMap<string, estadisticas_pais> getListasPorPais(HashMap<unsigned int, Venta>& mapa, Lista<string>& claves, int size) {
    
    HashMap<string, estadisticas_pais> mapaPaises(31, hashString); //12 paises en sudamerica, ocupan el 40% --> bajas colisiones

    for (int i = 1; i <= size; i++) { //IF básico de conteo --> no cuenta
        Venta v = mapa.get(i);
        // Creo la estructura de listas
        estadisticas_pais estadisticas;
        // creo el objeto ciudad monto (si no existe la ciudad, se insertará; si ya existe se usa para reemplazar el valor del monto)
        ciudad_monto cm = {v.ciudad, v.monto_total};
        producto_monto p = {v.producto, v.monto_total};
        categoria c = {v.categoria, 0, v.monto_total, 1};
        medioenvio_cantidad me = {v.medio_envio, 0};

        if (!mapaPaises.contieneClave(v.pais)) { //Si el pais todavia no existe
            Lista<ciudad_monto> Lciudades; //creo la lista de ciudades
            Lista<producto_monto> Lproductos; // creo la lista de productos
            Lista<categoria> Lcategorias; //creo la lista de categorias
            Lista<medioenvio_cantidad> Lmedioenvio;
            Lciudades.insertarUltimo(cm); //inserto la ciudad actual
            Lproductos.insertarUltimo(p); //inserto el producto actual
            Lcategorias.insertarUltimo(c); //inserto el categoria actual
            Lmedioenvio.insertarUltimo(me); 
            // Asigno las listas al struct
            estadisticas.ciudadesOrdenadasMonto = Lciudades; 
            estadisticas.productosMontoTotal = Lproductos; 
            estadisticas.categoriasPromedio = Lcategorias;
            estadisticas.mediosDeEnvio = Lmedioenvio;
            mapaPaises.put(v.pais, estadisticas); //inserto la lista la hashmap
            claves.insertarUltimo(v.pais); //inserto la clave a la lista de claves
        } else { // Si el pais existe
            Lista<ciudad_monto> Lciudades = mapaPaises.get(v.pais).ciudadesOrdenadasMonto;
            bool foundCiu = false;
            for (int j = 0; j < Lciudades.getTamanio() && !foundCiu; j++) { // reviso todas las ciudades de la lista
                if (Lciudades.getDato(j).ciudad == v.ciudad) {
                    cm.total = Lciudades.getDato(j).total + v.monto_total;
                    Lciudades.reemplazar(j, cm);
                    foundCiu = true; //Indico que se encontró la ciudad y altero el monto
                }
            }
            Lista<producto_monto> Lproductos = mapaPaises.get(v.pais).productosMontoTotal;
            bool foundProd = false;
            for (int j = 0; j < Lproductos.getTamanio() && !foundProd; j++) { // reviso todas las productos de la lista
                if (Lproductos.getDato(j).nombre == v.producto) {
                    p.total = Lproductos.getDato(j).total + v.monto_total;
                    Lproductos.reemplazar(j, p);
                    foundProd = true; //Indico que se encontró el producto y altero el monto
                }
            }
            Lista<categoria> Lcategorias = mapaPaises.get(v.pais).categoriasPromedio;
            bool foundCat = false;
            for (int j = 0; j < Lcategorias.getTamanio() && !foundCat; j++) { // reviso todas las categorias de la lista
                if (Lcategorias.getDato(j).nombre == v.categoria) {
                    c.total = Lcategorias.getDato(j).total + v.monto_total;
                    c.ventas = Lcategorias.getDato(j).ventas + 1;
                    Lcategorias.reemplazar(j, c);
                    foundCat = true; //Indico que se encontró el categoria y altero el monto
                }
            }
            Lista<medioenvio_cantidad> Lmedioenvio = mapaPaises.get(v.pais).mediosDeEnvio;
            bool foundMed = false;
            for (int j = 0; j < Lmedioenvio.getTamanio() && !foundMed; j++) { 
                if (Lmedioenvio.getDato(j).nombre == v.medio_envio) {
                    me.ventas = Lmedioenvio.getDato(j).ventas + 1;
                    Lmedioenvio.reemplazar(j, me);
                    foundMed = true; 
                }
            }
            if (!foundCiu) {
                Lciudades.insertarUltimo(cm); //inserto la ciudad si no existe
            }
            if (!foundProd) {
                Lproductos.insertarUltimo(p); //inserto el producto si no existe
            }
            if (!foundCat) {
                Lcategorias.insertarUltimo(c); //inserto el categoria si no existe
            }
            if (!foundMed) {
                Lmedioenvio.insertarUltimo(me); //inserto el medio si no existe
            }
            estadisticas.ciudadesOrdenadasMonto = Lciudades;
            estadisticas.productosMontoTotal = Lproductos;
            estadisticas.categoriasPromedio = Lcategorias; // Al finalizar esto, los promedios serán 0, pero los totales serán correctos
            estadisticas.mediosDeEnvio = Lmedioenvio;
            mapaPaises.put(v.pais, estadisticas); // Realizo la modificacion en el hashMap
        }
    }
    ordenarTop5CiudadesPorMontoSegunPais(mapaPaises, claves); // Ordeno de menor a mayor las ciudades por montos
    calcularPromedioVentasPorCategoriaSegunPais(mapaPaises, claves); // Arreglo los promedios
    bubbleSortMediosDeEnvio(mapaPaises, claves);
    return mapaPaises;
}

#endif