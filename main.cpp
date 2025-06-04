#include <iostream> // cout, cin
#include <sstream> // Permite trabajar con las lineas de csv como strings (stringstream, getline)
#include <fstream> // Permite leer y escribir archivo externos (ifstream)
#include <iomanip> // Permite manipular cout para que no trunque automaticamente los float
#include "HashMap\HashMap.h" 
#include "Lista\Lista.h"
#include "Cola\Cola.h"
#define NOMBRE_ARCHIVO ("C:/Users/mairi/source/proyecto-2025-comision-tosetti-lindon-ortega-ruiz/ventas_sudamerica.csv")
using namespace std;

struct Venta {
    string fecha, pais, ciudad, cliente, producto, categoria, medio_envio, estado_envio;
    int id, cantidad;
    float precio_unitario, monto_total;
};

unsigned int hashFunc(unsigned int clave) {
    return clave;  // Retorna el valor del ID como el índice hash
}

unsigned int hashString(const string& clave) {
    unsigned int hash = 0;
    for (char c : clave) hash += (unsigned int)c;
    return hash;
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
        mapaVenta.put(v.id, v);
        sizeofmap++;
    }

    archivo.close();

    cout << "Archivo cargado con éxito." << endl;
    cout << sizeofmap << " lineas cargadas." << endl;

    // PROCESAMIENTO
    // Top 5 ciudades con mayor monto de ventas por pais

        //Identificar ciudades únicas y sumo por ciudad
    struct ciudad_monto {
        string nombre;
        float total;
    };
    
    // POSIBLE MEJORA: USAR ESTO EN LUGAR DE STRUCT?? --> HashMap<unsigned int, float> mapaCiudadTotal(100, hashString);

    Lista<ciudad_monto> ciud_dist;
    for (int i = 1; i < sizeofmap; i++) { //IF básico de conteo --> no cuenta
        Venta v = mapaVenta.get(i);
        int j = 0;
        bool found = false;
        int pos = 0;
        while (j < ciud_dist.getTamanio() && !found) { // IF básico de conteo + IF true/false --> no cuenta
            if (v.ciudad == (ciud_dist.getDato(j)).nombre) { // IF 2500 ventas * (30 ciudades / 2) = 37500 IF
                found = true;
                pos = j;

            } else {
                j++;
            }
        }
        // 2500 vueltas (1 por cada venta)
        //  dentro de cada venta,
        //  1 IF por cada ciudad en ciud_dist
        //  El máximo de ciudades en ciud_dist es 30
        ciudad_monto cm;
        cm.nombre = v.ciudad;
        if (!found) { // IF true/false --> no cuenta
            cm.total = v.monto_total;
            ciud_dist.insertarUltimo(cm);
        } else {
            cm.total = (ciud_dist.getDato(pos)).total;
            cm.total += v.monto_total;
            ciud_dist.reemplazar(pos, cm);
        }
    }
    
    cout << "Ciudades y sus montos totales: " << endl;
    for (int i = 0; i < ciud_dist.getTamanio(); i++) { // IF de conteo --> no cuenta
        cout << fixed << setprecision(2);
        cout << endl;
        cout << "Ciudad: " << (ciud_dist.getDato(i)).nombre << endl; // búsqueda de o(1) --> 1 IF
        cout << "Monto: " << (ciud_dist.getDato(i)).total << endl; // búsqueda de o(1) --> 1 IF
    }

        // Divido en colas según PAIS
    
        // Ordeno según monto_total de la ciudad
    
        // Extraigo las primeras 5 de cada cola


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