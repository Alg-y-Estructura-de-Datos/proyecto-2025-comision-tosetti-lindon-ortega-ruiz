#include "library.h"
#include <iostream> // cout, cin
#include <sstream> // Permite trabajar con las lineas de csv como strings (stringstream, getline)
#include <fstream> // Permite leer y escribir archivo externos (ifstream)
#include <iomanip> // Permite manipular cout para que no trunque automaticamente los float
#include "HashMap\HashMap.h" // CAMBIAR POR HASHMAP LIST PARA USAR MEJOR MANEJO DE COLISIONES
#include "Lista\Lista.h"
#include "Cola\Cola.h"
#define NOMBRE_ARCHIVO ("C:/Users/mairi/source/proyecto-2025-comision-tosetti-lindon-ortega-ruiz/ventas_sudamerica.csv")
using namespace std;

void printMenu();

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

    // Monto total vendido por producto, discriminado por país
    
    HashMap<string, Lista<producto>> mapaProductos = getMontoTotalPorProductoSegunPais(mapaVenta, sizeofmap); 
    //Sería aún más optimo si pudieramos usar el mismo hashmap, y que conteniera una estructura con todas las listas o que la funcion aceptara typename T (tipo de dato flexible)

    // Promedio de ventas por categoría en cada país
    HashMap<string, Lista<categoria>> mapaPromedioPorCategorias = getPromedioVentasPorCategoriaSegunPais(mapaVenta, sizeofmap, claves_mapaPaises);

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
    int option;
    printMenu();
    cin >> option;
    
    switch (option)
    {
    case 1:
        printTop5CiudadesPorMontoSegunPais(mapaCiudadesOrdenadas, claves_mapaPaises);
        break;
    case 2:
        printMontoTotalPorProductoSegunPais(mapaProductos, claves_mapaPaises);
        break;
    case 3:
        printPromedioVentasPorCategoriaSegunPais(mapaPromedioPorCategorias, claves_mapaPaises);
    default:
        cout << "Unvalid option. Leaving...";
        return 1;
    }
    return 0;
}

void printMenu() {
    cout << "--------- MENU ----------" << endl;
    cout << "01) Top 5 Ciudades por Monto segun Pais" << endl;
    cout << "02) Montos Totales por Producto segun Pais" << endl;
    cout << "03) Promedio de Ventas por Categoria segun Pais" << endl;
    cout << endl;
}