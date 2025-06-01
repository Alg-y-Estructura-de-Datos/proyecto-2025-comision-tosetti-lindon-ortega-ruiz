#include <iostream> 
#include <sstream> // Permite trabajar con las lineas de csv como strings (stringstream, getline)
#include <fstream> // Permite leer y escribir archivo externos (ifstream)
#include "HashMap\HashMap.h" 
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


int main() {
    HashMap<unsigned int, Venta> mapaVenta(6000, hashFunc); // Mapa de ventas por ID
    // CARGAR LOS DATOS - A través de una funcion en un archivo cpp
    cout << "Cargando datos..." << endl;

    ifstream archivo(NOMBRE_ARCHIVO); // Abrir el archivo

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo."; 
        return 1;
    } else {
        cout << "Archivo abierto con éxito.";
    }

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
        
        mapaVenta.put(v.id, v);
    }

    archivo.close();

    // PROCESAMIENTO
    // Top 5 ciudades con mayor monto de ventas por pais
    
    // Monto total vendido por producto, discriminado por país.
    // Promedio de ventas por categoría en cada país. 
    // Medio de envío más utilizado por país.
    // Medio de envío más utilizado por categoría.
    // Día con mayor cantidad de ventas (por monto de dinero) en toda la base de datos.
    // Estado de envío más frecuente por país.
    // Producto más vendido en cantidad total (no en dinero, sino en unidades).
    // Producto menos vendido en cantidad total.


    // MENU --> ~ 9 opciones
        // Modificaciones
            // Realizar y corremos el procesamiento
        // Consultas dinámicas
}