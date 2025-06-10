#ifndef U05_HASH_HASHMAP_HASHMAP_H_
#define U05_HASH_HASHMAP_HASHMAP_H_

#include "HashEntry.h"

template <class K, class T>
class HashMap
{
private:
  HashEntry<K, T> **tabla;
  unsigned int tamanio;

  static unsigned int hashFunc(K clave);

  unsigned int (*hashFuncP)(K clave);

public:
  explicit HashMap(unsigned int k);

  HashMap(unsigned int k, unsigned int (*hashFuncP)(K clave));

  T get(K clave);

  void put(K clave, T valor);

  void remove(K clave);

  ~HashMap();

  bool esVacio();

  void print();

  bool contieneClave(K clave);
};

template <class K, class T>
HashMap<K, T>::HashMap(unsigned int k)
{
  tamanio = k;
  tabla = new HashEntry<K, T> *[tamanio];
  for (int i = 0; i < tamanio; i++)
  {
    tabla[i] = NULL;
  }
  hashFuncP = hashFunc;
}

template <class K, class T>
HashMap<K, T>::HashMap(unsigned int k, unsigned int (*fp)(K))
{
  tamanio = k;
  tabla = new HashEntry<K, T> *[tamanio];
  for (int i = 0; i < tamanio; i++)
  {
    tabla[i] = NULL;
  }
  hashFuncP = fp;
}

template <class K, class T>
HashMap<K, T>::~HashMap()
{
  for (int i = 0; i < tamanio; i++)
  {
    if (tabla[i] != NULL)
    {
      delete tabla[i];
    }
  }
}

template <class K, class T> // MODIFICADO CON LINEAR PROBING
T HashMap<K, T>::get(K clave)
{
    unsigned int pos = hashFuncP(clave) % tamanio;
    unsigned int originalPos = pos;

    // Bucle de búsqueda con probing para localizar la clave
    do {
        if (tabla[pos] == NULL) {
            throw 404;  // La clave no existe en la tabla.
        }
        if (tabla[pos]->getClave() == clave) {
            return tabla[pos]->getValor();
        }
        pos = (pos + 1) % tamanio;
    } while (pos != originalPos);

    throw 409;  // Se recorrió toda la tabla sin encontrar la clave.
}

template <class K, class T> // PUT MODIFICADO
void HashMap<K, T>::put(K clave, T valor)
{
  unsigned int pos = hashFuncP(clave) % tamanio;
  unsigned int originalPos = pos;
  unsigned int intentos = 0; // Contador para evitar ciclos infinitos

  // "LINEAR PROBING"
  while (tabla[pos] != NULL && intentos < tamanio) {
    if (tabla[pos]->getClave() == clave) {
      // si ya existe la clave reemplazar valor
      tabla[pos]->setValor(valor);
      return;
    }

    pos = (pos + 1) % tamanio;
    intentos++; 
  }

  // Si encontramos una celda libre, insertamos la nueva entrada
  if (intentos < tamanio) {
    tabla[pos] = new HashEntry<K, T>(clave, valor);
  } else {
    throw std::runtime_error("HashMap lleno. No se pudo insertar.");
  }
}

template <class K, class T>
void HashMap<K, T>::remove(K clave) {}

template <class K, class T>
bool HashMap<K, T>::esVacio()
{
  for (int i = 0; i < tamanio; i++)
  {
    if (tabla[i] != NULL)
    {
      return false;
    }
  }
  return true;
}

template <class K, class T>
unsigned int HashMap<K, T>::hashFunc(K clave)
{
  return (unsigned int)clave;
}

template <class K, class T>
void HashMap<K, T>::print()
{

  std::cout << "i"
            << " "
            << "Clave"
            << "\t\t"
            << "Valor" << std::endl;
  std::cout << "--------------------" << std::endl;
  for (int i = 0; i < tamanio; i++)
  {
    std::cout << i << " ";
    if (tabla[i] != NULL)
    {
      std::cout << tabla[i]->getClave() << "\t\t";
      std::cout << tabla[i]->getValor();
    }
    std::cout << std::endl;
  }
}

template <class K, class T>
bool HashMap<K, T>::contieneClave(K clave) { //Implementamos una funcion para confirmar que existe la clave
  unsigned int pos = hashFuncP(clave) % tamanio;

  unsigned int originalPos = pos;

  do {
    if (tabla[pos] == NULL) {
      return false; // No está en esta posición ni en ninguna posterior
    }

    if (tabla[pos]->getClave() == clave) {
      return true;
    }

    pos = (pos + 1) % tamanio;

  } while (pos != originalPos); // Volvemos al principio → no se encontró

  return false;
}


#endif // U05_HASH_HASHMAP_HASHMAP_H_
