#include "library.h"

#include <iostream> // cout, cin
#include <sstream> // Permite trabajar con las lineas de csv como strings (stringstream, getline)
#include <fstream> // Permite leer y escribir archivo externos (ifstream)
#include <iomanip> // Permite manipular cout para que no trunque automaticamente los float
#include <unordered_map>
#include "HashMap\HashMap.h" // CAMBIAR POR HASHMAP LIST PARA USAR MEJOR MANEJO DE COLISIONES
#include "Lista\Lista.h"
#include "Cola\Cola.h"
#define NOMBRE_ARCHIVO ("C:/Users/mairi/source/proyecto-2025-comision-tosetti-lindon-ortega-ruiz/ventas_sudamerica.csv")
using namespace std;

int main() {
    int sizeofmap = 0;
    // CARGAR LOS DATOS - A través de una funcion en un archivo cpp
    cout << "Cargando datos..." << endl;
    HashMap<unsigned int, Venta> mapaVenta(5999, hashFunc);

    loadFile (mapaVenta, sizeofmap); // Mapa de ventas por ID
    cout << sizeofmap << " lineas cargadas." << endl;

    //El top 5 de ciudades con mayor monto de ventas por país

    Lista<string> claves_mapaPaises; // Almaceno las claves de mapaPaises (usadas en las siguientes funciones)
    HashMap<string, Lista<ciudad_monto>> mapaCiudadesOrdenadas = getTop5CiudadesPorMontoSegunPais(mapaVenta, sizeofmap, claves_mapaPaises);
    printTop5CiudadesPorMontoSegunPais(mapaCiudadesOrdenadas, claves_mapaPaises);

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