#include <iostream>
#include <sstream>
#include <fstream>
#define NOMBRE_ARCHIVO ("C:/Users/mairi/source/PROYECTO_FINAL_AED/ventas_sudamerica.csv")
using namespace std;

int main()
{
    ifstream archivo(NOMBRE_ARCHIVO);
    if (!archivo.is_open()) { cout << "Error al abrir el archivo"; return 1; }
    else {cout << "POR FIN";}

    string linea;
    char delimitador = ',';
    // Descartamos primera linea
    getline(archivo, linea);
    // Leemos todas las lineas
    while (getline(archivo, linea))
    {
        stringstream stream(linea); // Convertir la cadena a un stream
        string id_venta, fecha, pais, ciudad, cliente, producto, categoria, cantidad, precio_unitario, monto_total, medio_envio, estado_envio;
        // Extraer todos los valores de esa fila
        getline(stream, id_venta, delimitador);
        getline(stream, fecha, delimitador);
        getline(stream, pais, delimitador);
        getline(stream, ciudad, delimitador);
        getline(stream, cliente, delimitador);
        getline(stream, producto, delimitador);
        getline(stream, categoria, delimitador);
        getline(stream, cantidad, delimitador);
        getline(stream, precio_unitario, delimitador);
        getline(stream, monto_total, delimitador);
        getline(stream, medio_envio, delimitador);
        getline(stream, estado_envio, delimitador);
        // Imprimir
        cout << "==================" << endl;
        cout << "Id: " << id_venta << endl;
        cout << "Fecha: " << fecha << endl;
        cout << "Pais: " << pais << endl;
        cout << "Ciudad: " << ciudad << endl;
        cout << "Cliente: " << cliente << endl;
        cout << "Producto: " << producto << endl;
        cout << "Categoria: " << categoria << endl;
        cout << "Cantidad: " << cantidad << endl;
        cout << "Precio unitario: " << precio_unitario << endl;
        cout << "Monto total: " << monto_total << endl;
        cout << "Medio envío: " << medio_envio << endl;
        cout << "Estado envío: " << estado_envio << endl;
    }

    archivo.close();
}
