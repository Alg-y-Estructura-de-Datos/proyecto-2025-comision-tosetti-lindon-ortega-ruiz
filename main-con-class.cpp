#include <iostream> // std::cout, cin
#include <sstream> // Permite trabajar con las lineas de csv como std::strings (std::stringstream, getline)
#include <fstream> // Permite leer y escribir archivo externos (ifstream)
#include <iomanip> // Permite manipular std::cout para que no trunque automaticamente los float
#include "HashMap\HashMap.h" // CAMBIAR POR HASHMAP LIST PARA USAR MEJOR MANEJO DE COLISIONES
#include "Lista\Lista.h"
#include "Cola\Cola.h"
#define NOMBRE_ARCHIVO ("C:/Users/mairi/source/proyecto-2025-comision-tosetti-lindon-ortega-ruiz/ventas_sudamerica.csv")

struct Venta {
    std::string fecha, pais, ciudad, cliente, producto, categoria, medio_envio, estado_envio;
    int id, cantidad;
    float precio_unitario, monto_total;
};

class Monto_Ciudad
{
private:
    // int id;
    std::string ciudad;
    float monto;
public:
    Monto_Ciudad();
    Monto_Ciudad(/* int new_id, */ std::string new_ciudad, float new_monto);
    // int getId();
    std::string getCiudad();
    float getMonto();
    // void setId(int new_id);
    void setCiudad(std::string new_ciudad);
    void setMonto(float new_monto);
    ~Monto_Ciudad();
};

Monto_Ciudad::Monto_Ciudad()
{
    // id = -1;
    ciudad = "defaultcity";
    monto = -1;
}

Monto_Ciudad::Monto_Ciudad(/* int new_id, */ std::string new_ciudad, float new_monto) {
    // id = new_id;
    ciudad = new_ciudad;
    monto = new_monto;
}

/* int Monto_Ciudad::getId() {
    return id;
} */

std::string Monto_Ciudad::getCiudad() {
    return ciudad;
}

float Monto_Ciudad::getMonto() {
    return monto;
}

/* void Monto_Ciudad::setId(int new_id) {
    id = new_id;
} */
void Monto_Ciudad::setCiudad(std::string new_ciudad) {
    ciudad = new_ciudad;
}
void Monto_Ciudad::setMonto(float new_monto) {
    monto = new_monto;
}

Monto_Ciudad::~Monto_Ciudad()
{
}


unsigned int hashFunc(unsigned int clave) {
    return clave;  // Retorna el valor del ID como el índice hash
}

unsigned int hashString(std::string clave) {
    unsigned int hash = 0;
    for (char c : clave) {
        hash = 31 * hash + c;
    }
    return hash;
}

auto trim = [](std::string s) { //FUNCION BUSCADA EN INTERNET PARA ASEGURAR QUE LOS DATOS NO TENGAN ESPACIOS EXTRAS
    while (!s.empty() && isspace(s.front())) s.erase(s.begin());
    while (!s.empty() && isspace(s.back())) s.pop_back();
    return s;
};

int main() {
    HashMap<unsigned int, Venta> mapaVenta(6000, hashFunc); // Mapa de ventas por ID
    // CARGAR LOS DATOS - A través de una funcion en un archivo cpp
    std::cout << "Cargando datos..." << std::endl;

    std::ifstream archivo(NOMBRE_ARCHIVO); // Abrir el archivo

    std::string linea;
    char delimitador = ',';

    getline(archivo, linea); // Descartar primera linea
    int sizeofmap = 0;

    while (getline(archivo, linea)) // Leer todas las líneas
    {
        Venta v;

        std::stringstream stream(linea); // Convertir la cadena a un stream
        std::string id_venta, cantidad, precio_unitario, monto_total;
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

    std::cout << "Archivo cargado con éxito." << std::endl;
    std::cout << sizeofmap << " lineas cargadas." << std::endl;

    // PROCESAMIENTO
    // Top 5 ciudades con mayor monto de ventas por pais

    // MUY COOL --> Hicimos todo junto B)
    HashMap<std::string, Lista<Monto_Ciudad>> mapaPaises(30, hashString); //12 paises en sudamerica, ocupan el 40% --> bajas colisiones
    // Repite por cada venta en el mapa 
    for (int i = 1; i <= sizeofmap; i++) { //IF básico de conteo --> no cuenta
        std::cout << "-------------------------------" << std::endl;
        std::cout << "Analizando venta, ID: " << i << std::endl;
        Venta v = mapaVenta.get(i);
        bool found = false, nuevopais = false;
        int pos = 0;

        Lista<Monto_Ciudad> ciudades;
        std::cout << "Lista de ciudades creada." << std::endl;

        try {
            ciudades = mapaPaises.get(v.pais);
        }
        catch (...) {
            nuevopais = true;
        }
        
        std::cout << "Valor de nuevopais? 1 = true, 0 = false." << std::endl << nuevopais << std::endl;

        if (nuevopais) {
            // Creo el pais
            Monto_Ciudad cm(v.ciudad, v.monto_total);
            std::cout << "El valor a ingresar a ciudades es: " << cm.getCiudad() << " | " << cm.getMonto() << std::endl;
            ciudades.insertarUltimo(cm);
            std::cout << "Se ha insertado la Monto_Ciudad a la lista de ciudades de " << v.pais << std::endl;
            mapaPaises.put(v.pais, ciudades);
            std::cout << "Se ha insertado el pais " << v.pais << std::endl;
        } else {
            int j = 0;
            // Repite por cada ciudad única en la lista ciudades
            while (j < ciudades.getTamanio() && !found) { // IF básico de conteo + IF true/false --> no cuenta
                // Si la ciudad ya fue guardada en la lista, guarda la posición y termina el loop
                if (v.ciudad == ciudades.getDato(j).getCiudad()) { // IF 2500 ventas * (30 ciudades / 2) = 37500 IF
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
                Monto_Ciudad cm(v.ciudad, v.monto_total);
                ciudades.insertarUltimo(cm);
            // Si encontró la ciudad, suma el valor de la venta actual al monto de esa ciudad
            } else {
                Monto_Ciudad cm(v.ciudad, ciudades.getDato(pos).getMonto() + v.monto_total);
                ciudades.reemplazar(pos, cm);
            }

            mapaPaises.put(v.pais, ciudades);
            std::cout << "Se ha actualizado el pais " << v.pais << std::endl;
        }
    }
    
    std::cout << "Ciudades por pais y sus montos totales: " << std::endl;
    std::cout << "Ingrese nombre del pais" << std::endl;
    std::string pais;
    std::getline(std::cin, pais);
    pais = trim(pais);
    Lista<Monto_Ciudad> l;
    std::cout << "Buscando país " << pais << "..." << std::endl;
    try
    {
        l = mapaPaises.get(pais);
    }
    catch(...)
    {
        std::cout << "No se encontró el país";
        return 1;
    }

    std::cout << "Este país tiene " << mapaPaises.get(pais).getTamanio() << " ciudades." << std::endl;
    
    for (int i = 0; i < mapaPaises.get(pais).getTamanio(); i++) { 
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::endl;
        std::cout << "Ciudad: " << (mapaPaises.get(pais).getDato(i)).getCiudad() << std::endl;
        std::cout << "Monto: " << (mapaPaises.get(pais).getDato(i)).getMonto() << std::endl; 
    } // SIGUE HABIENDO UN ERROR; DEBUGG!!!
    
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