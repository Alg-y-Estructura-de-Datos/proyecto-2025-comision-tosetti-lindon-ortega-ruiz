#include "library.h"
#include <iostream> // cout, cin
#include <sstream> // Permite trabajar con las lineas de csv como strings (stringstream, getline)
#include <fstream> // Permite leer y escribir archivo externos (ifstream)
#include <iomanip> // Permite manipular cout para que no trunque automaticamente los float
#include <ctime>
#include "HashMap\HashMap.h" // CAMBIAR POR HASHMAP LIST PARA USAR MEJOR MANEJO DE COLISIONES
#include "Lista\Lista.h"
#include "Cola\Cola.h"
#define NOMBRE_ARCHIVO ("C:/Users/mairi/source/proyecto-2025-comision-tosetti-lindon-ortega-ruiz/ventas_sudamerica.csv")
using namespace std;

void printMenu(int n);

int main(void) {
    int sizeofmap = 0;
    // CARGAR LOS DATOS - A través de una funcion en un archivo cpp
    cout << "Cargando datos..." << endl;
    HashMap<unsigned int, Venta> mapaVenta(5999, hashFunc);

    loadFile (mapaVenta, sizeofmap); // Mapa de ventas por ID
    cout << sizeofmap << " lineas cargadas." << endl;
    
    Lista<string> claves_mapaPaises; // Almaceno las claves de mapaPaises (usadas en las siguientes funciones)

    // Extract data from map --> returns struct
    HashMap<string, estadisticas_pais> mapaPaises = getListasPorPais(mapaVenta, claves_mapaPaises, sizeofmap);
    
    Lista<string> claves_mapaCategorias;
    
    HashMap<string, Lista<medioenvio_cantidad>> mapaCategorias = getListasPorCategoria(mapaVenta, claves_mapaCategorias, sizeofmap);

    Lista<producto_cantidad> listaOrdenadaProductosPorCantidad = getListaOrdenadaProductos(mapaVenta, sizeofmap);

    dia_montos fechaConMasVentas = getDiaConMayorCantidadVentas(mapaVenta, sizeofmap);
    // MENU --> ~ 9 opciones
        // Modificaciones
            // Realizar y corremos el procesamiento
        // Consultas dinámicas
    int option, process;
    printMenu(0);
    cin >> option;
    printMenu(option);
    cin >> process;

    switch (option)
    {
        case 1:
            switch (process)
            {
            case 1:
                printTop5CiudadesPorMontoSegunPais(mapaPaises, claves_mapaPaises);
                break;
            case 2:
                printMontoTotalPorProductoSegunPais(mapaPaises, claves_mapaPaises);
                break;
            case 3:
                printPromedioVentasPorCategoriaSegunPais(mapaPaises, claves_mapaPaises);
                break;
            case 4:
                printMedioEnvioMasUtilizadoPorPais(mapaPaises, claves_mapaPaises);
                break;
            case 5:
                printMedioEnvioMasUtilizadoPorCategoria(mapaCategorias, claves_mapaCategorias);
                break;
            case 6:
                printDiaMayorCantidadVentas(fechaConMasVentas);
                break;
            case 7:
                printEstadoDeEnvioMasFrencuentePorPais(mapaPaises, claves_mapaPaises);
                break;
            case 8:
                printProductoMasVendido(listaOrdenadaProductosPorCantidad);
                break;
            case 9:
                printProductoMenosVendido(listaOrdenadaProductosPorCantidad);
                break;
            default:
                cout << "Unvalid option. Leaving...";
                break;
            }
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
    return 0;
}

void printMenu(int n) {
    switch (n)
    {
    case 0:
        cout << "--------- MENU ----------" << endl;
        cout << "01) Revisar estadísticas" << endl;
        cout << "02) Modificar datos" << endl;
        cout << "03) Realizar una consulta" << endl;
        cout << "Ingrese cualquier otro número para salir." << endl;
        cout << endl;
        break;
    case 1:
        cout << "--------- Estadisticas ----------" << endl;
        cout << "01) Top 5 ciudades por monto segun pais" << endl;
        cout << "02) Montos totales por producto segun pais" << endl;
        cout << "03) Promedio de ventas por categoria segun pais" << endl;
        cout << "04) Medio de envío más utilizado por país." << endl;
        cout << "05) Medio de envío más utilizado por categoría." << endl;
        cout << "06) Día con mayor cantidad de ventas (por monto de dinero) en toda la base de datos." << endl;
        cout << "07) Estado de envío más frecuente por país." << endl;
        cout << "08) Producto más vendido en cantidad total (no en dinero, sino en unidades)." << endl;
        cout << "09) Producto menos vendido en cantidad total." << endl;
        cout << "Ingrese cualquier otro número para volver." << endl;
        cout << endl;
        break;
    case 2:
        cout << "---- Modificacion de datos ----" << endl;
        cout << "01) Agregar una venta" << endl;
        cout << "02) Eliminar una venta" << endl;
        cout << "03) Modificar una venta" << endl;
        cout << "Ingrese cualquier otro número para volver." << endl;
        cout << endl;
        break;
    case 3:
        cout << "------ Consultas Dinamicas -----" << endl;
        cout << "01) Ventas de una ciudad" << endl;
        cout << "02) Ventas en un rango de fechas" << endl;
        cout << "03) Comparar dos paises por monto total" << endl;
        cout << "04) Comparar dos paises por cantidad vendida" << endl;
        cout << "05) Comparar dos paises por medio de envío" << endl;
        cout << "06) Comparar dos productos por cantidad total" << endl;
        cout << "07) Comparar dos products por monto" << endl;
        cout << "08) Buscar productos vendidos en promedio por debajo de un monto" << endl;
        cout << "09) Buscar productos vendidos en promedio por encima de un monto" << endl;
        cout << endl;
    default:
        cout << "Saliendo...";
        break;
    }
}