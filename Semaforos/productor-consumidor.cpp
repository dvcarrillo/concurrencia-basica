/*****************************************************************************/
// Prácticas de Sistemas Concurrentes y Distribuidos
// Por David Vargas Carrillo, 2015
/*****************************************************************************/

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

////////////////////////////////////////////////////////////////////////////////
// CONSTANTES

const unsigned num_items = 40, tam_vector = 10;

////////////////////////////////////////////////////////////////////////////////
// VARIABLES GLOBALES

// Vector de datos

int * datos_almacenados;

// Indica la primera posición libre del vector

int primera_libre;

// Semáforos

sem_t puede_producir;
sem_t puede_consumir;

////////////////////////////////////////////////////////////////////////////////
// FUNCIONES PRODUCTORAS Y CONSUMIDORAS

// Función a ejecutar cuando un dato es producido

unsigned producir_dato()
{
    static int contador = 0 ;
    cout << "dato producido: " << contador << endl << flush ;
    return contador++ ;
}

// Funcion a ejecutar cuando un dato es leído

void consumir_dato( int dato )
{
    cout << "dato recibido: " << dato << endl ;
}

// Función productora de datos

void * productor( void * )
{
    int dato;

    for( unsigned i = 0 ; i < num_items ; i = i + 10 )
    {
        // Espera por si aun no puede escribir
        sem_wait(&puede_producir);

        for (int j = 0; j < tam_vector; j++)
        {
            // Secuencia de órdenes de escritura
            dato = producir_dato() ;
            datos_almacenados[primera_libre] = dato;
            primera_libre++;
        }
        // Suma un valor para que pueda leer la otra función
        sem_post(&puede_consumir);
    }
    return NULL ;

}

// Función lectora de datos

void * consumidor( void * )
{
    int dato;

    for( unsigned i = 0 ; i < num_items ; i = i + 10 )
    {
        // Espera por si aun no puede leer
        sem_wait(&puede_consumir);

        for (int j = 0; j < tam_vector; j++)
        {
            primera_libre--;
            dato = datos_almacenados[primera_libre];
            consumir_dato( dato ) ;
        }
        // Suma un valor para que pueda escribir la otra función
        sem_post(&puede_producir);
    }
    return NULL ;

}

////////////////////////////////////////////////////////////////////////////////

int main()
{
    primera_libre = 0;
    datos_almacenados = new int [tam_vector];

    // Se crean dos hebras: una para el productor y otra para el consumidor, lo
    // que permitirá mantener ocupadas las CPUs disponibles el máximo de tiempo
    // También es conveniente crear un vector que almacene los datos producidos

    // Declaración de hebras
    pthread_t p_productor, p_consumidor;

	// Inicialización de semáforos
    sem_init (&puede_producir, 0, 1);
    sem_init (&puede_consumir, 0, 0);
	
	// Inicialización de hebras
    pthread_create(&p_productor, NULL, productor, NULL);
    pthread_create(&p_consumidor, NULL, consumidor, NULL);

	// Finalización de hebras
    pthread_join(p_productor, NULL);
    pthread_join(p_consumidor, NULL);

	// Finalización de los semáforos
    sem_destroy (&puede_producir);
    sem_destroy (&puede_consumir);
    
    delete [] datos_almacenados;

   return 0 ;
}
