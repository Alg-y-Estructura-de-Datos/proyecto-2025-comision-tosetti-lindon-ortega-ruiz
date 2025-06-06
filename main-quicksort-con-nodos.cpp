#include <iostream> // cout, cin
#include <sstream> // Permite trabajar con las lineas de csv como strings (stringstream, getline)
#include <fstream> // Permite leer y escribir archivo externos (ifstream)
#include <iomanip> // Permite manipular cout para que no trunque automaticamente los float
#include <algorithm> // Swap --> Intercambiar los valores de dos variables
#include "HashMap\HashMap.h" // CAMBIAR POR HASHMAP LIST PARA USAR MEJOR MANEJO DE COLISIONES
#include "Lista\Lista.h"
#include "Cola\Cola.h"
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

Nodo<ciudad_monto>* obtenerNodo(Lista<ciudad_monto>& lista, int pos) { // Para trabajar con punteros, obtenemos el nodo
    Nodo<ciudad_monto>* aux = lista.getInicio(); // el auxiliar es el primer nodo de la lista
    int i = 0;
    while (aux != nullptr && i < pos) { // busco la posicion
        aux = aux->getSiguiente();
        i++;
    }
    return aux; // devuelve el nodo correspondiente a la posicion
}

// funcion de particion para simplificar el quicksort, divide en mayores y menores que el pivote 
int partition(Lista<ciudad_monto>& lista, int bot, int top) { //
    Nodo<ciudad_monto>* pivotNodo = obtenerNodo(lista, top); // el primer pivot es el nodo del final
    float pivot = pivotNodo->getDato().total; //obtengo el total del pivot
    int i = bot - 1; //valor que usaré para dividir la lista

    for (int j = bot; j < top; j++) { //recorro la particion de principio a fin
        Nodo<ciudad_monto>* nodoJ = obtenerNodo(lista, j);
        if (nodoJ->getDato().total >= pivot) { // si el valor del nodo es igual o mayor al pivot, intercambio los nodos con el nodo de la posicion i
            i++;
            Nodo<ciudad_monto>* nodoI = obtenerNodo(lista, i);
            ciudad_monto temp = nodoI->getDato();
            nodoI->setDato(nodoJ->getDato());
            nodoJ->setDato(temp); 
        }
    }
    Nodo<ciudad_monto>* nodoI1 = obtenerNodo(lista, i + 1);
    ciudad_monto temp = nodoI1->getDato();
    nodoI1->setDato(pivotNodo->getDato());
    pivotNodo->setDato(temp);
    return i + 1;
}

// Implementación de Quicksort para Lista Enlazada
void quicksort(Lista<ciudad_monto>& lista, int bot, int top) {
    if (bot < top) {
        int pi = partition(lista, bot, top);
        quicksort(lista, bot, pi - 1);
        quicksort(lista, pi + 1, top);
    }
}

int main() {
    HashMap<unsigned int, Venta> mapaVenta(6000, hashFunc); // Mapa de ventas por ID
    // CARGAR LOS DATOS - A través de una funcion en un archivo cpp
    cout << "Cargando datos..." << endl;

    ifstream archivo(NOMBRE_ARCHIVO); // Abrir el archivo

    string linea;
    char delimitador = ',';

    getline(archivo, linea); // Descartar primera linea
    int sizeofmap = 0;

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
        mapaVenta.put(v.id, v);
        sizeofmap++;
    }

    archivo.close();

    cout << "Archivo cargado con éxito." << endl;
    cout << sizeofmap << " lineas cargadas." << endl;

    // PROCESAMIENTO
    // Top 5 ciudades con mayor monto de ventas por pais

    // MUY COOL --> Hicimos todo junto B)
    HashMap<string, Lista<ciudad_monto>> mapaPaises(30, hashString); //12 paises en sudamerica, ocupan el 40% --> bajas colisiones
    Lista<string> claves_mapaPaises; // Almaceno las claves de mapaPaises
    // Repite por cada venta en el mapa 
    for (int i = 1; i <= sizeofmap; i++) { //IF básico de conteo --> no cuenta
        cout << "-------------------------------" << endl;
        cout << "Analizando venta, ID: " << i << endl;
        Venta v = mapaVenta.get(i);
        bool found = false, nuevopais = false;
        int pos = 0;

        Lista<ciudad_monto> ciudades;
        cout << "Lista de ciudades creada." << endl;

        try {
            ciudades = mapaPaises.get(v.pais);
        }
        catch (...) {
            nuevopais = true;
        }
        
        cout << "Valor de nuevopais? 1 = true, 0 = false." << endl << nuevopais << endl;

        // creo el objeto ciudad monto (si no existe la ciudad, se insertará; si ya existe se usa para reemplazar el valor del monto)
        ciudad_monto cm;
        cout << "Se crea la estructura ciudad_monto." << endl;
        cm.ciudad = v.ciudad;
        cout << "Se asigna la ciudad " << cm.ciudad << " a la ciudad_monto." << endl;

        if (nuevopais) {
            // Creo el pais
            cm.total = v.monto_total;
            cout << "Se ha asignado un valor monto total de " << cm.total << endl;
            cout << "El valor a ingresar a ciudades es: " << cm.ciudad << " | " << cm.total << endl;
            ciudades.insertarUltimo(cm);
            cout << "Se ha insertado la ciudad_monto a la lista de ciudades de " << v.pais << endl;
            mapaPaises.put(v.pais, ciudades);
            cout << "Se ha insertado el pais " << v.pais << endl;
        } else {
            int j = 0;
            // Repite por cada ciudad única en la lista ciudades
            while (j < ciudades.getTamanio() && !found) { // IF básico de conteo + IF true/false --> no cuenta
                // Si la ciudad ya fue guardada en la lista, guarda la posición y termina el loop
                if (v.ciudad == ciudades.getDato(j).ciudad) { // IF 2500 ventas * (30 ciudades / 2) = 37500 IF
                    found = true;
                    pos = j;
                // Si no está, sigue buscando
                } else {
                    j++;
                }
            }
            // 2500 vueltas (1 por cada venta)
            //  dentro de cada venta,
            //  1 IF por cada ciudad en ciudades
            //  El máximo esperado de ciudades en ciudades es 30
            
            // Si no se encuentra la ciudad, la mete en ciudades
            if (!found) { // IF true/false --> no cuenta
                cm.total = v.monto_total;
                ciudades.insertarUltimo(cm);
            // Si encontró la ciudad, suma el valor de la venta actual al monto de esa ciudad
            } else {
                cm.total = ciudades.getDato(pos).total;
                cm.total += v.monto_total;
                ciudades.reemplazar(pos, cm);
            }

            mapaPaises.put(v.pais, ciudades);
            claves_mapaPaises.insertarUltimo(v.pais);
            cout << "Se ha actualizado el pais " << v.pais << endl;
        }
    }
    
    cout << "Ciudades por pais y sus montos totales: " << endl;
    
    for (int i = 0; i < claves_mapaPaises.getTamanio(); i++) {
        cout << "--------------------" << endl;
        cout << "País: " << claves_mapaPaises.getDato(i) << endl;
        Lista<ciudad_monto> paisactual = mapaPaises.get(claves_mapaPaises.getDato(i));

        // IMPLEMENTAR UN QUICKSORT ACAAAA
        quicksort(paisactual, 0, paisactual.getTamanio() - 1);
        
        for (int i = 0; i < paisactual.getTamanio(); i++) { 
            cout << fixed << setprecision(2);
            cout << endl;
            cout << "Ciudad: " << paisactual.getDato(i).ciudad << endl;
            cout << "Monto: " << paisactual.getDato(i).total << endl; 
            cout << endl;
        }
    }

    // Entre 65 y 108 IF para quicksort

    // Monto total vendido por producto, discriminado por país
    
    // Promedio de ventas por categoría en cada país

    // Medio de envío más utilizado por país

    // Medio de envío más utilizado por categoría

    // Día con mayor cantidad de ventas (por monto de dinero) en toda la base de datos

    // Estado de envío más frecuente por país

    // Producto más vendido en cantidad total (no en dinero, sino en unidades)

    // Producto menos vendido en cantidad total


    // MENU --> ~ 9 opciones
        // Modificaciones
            // Realizar y corremos el procesamiento
        // Consultas dinámicas
}