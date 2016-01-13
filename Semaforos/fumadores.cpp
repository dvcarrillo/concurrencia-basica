/*****************************************************************************/
// Prácticas de Sistemas Concurrentes y Distribuidos
// Por David Vargas Carrillo, 2015
/*****************************************************************************/

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"
 
using namespace std;
 
////////////////////////////////////////////////////////////////////////////////
// FUNCIÓN FUMAR
// Simula la acción de fumar como un retardo aleatorio de la hebra
 
void fumar() 
{ 
   // calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
   const unsigned miliseg = 100U + (rand() % 1900U) ; 
 
   // retraso bloqueado durante 'miliseg' milisegundos
   usleep (1000U*miliseg); 
}
 
/******************************************************************************/
// Declaración de los semáforos
 
sem_t
    puede_consumir1,
    puede_consumir2,
    puede_consumir3,
    puede_producir,
    control;
     
/******************************************************************************/
// Contabilización de veces que el estanquero produce ingredientes y de las
// veces que se fuma
 
int veces_estanquero;
int veces_fumar; 
 
////////////////////////////////////////////////////////////////////////////////
// FUNCIONES DE LOS FUMADORES
// Encargados de consumir los ingredientes despechados
// Deciden aleatoriamente si fumar o no
 
void * fumador1 (void *)
{
    while (2 > 1)
    {
        sem_wait(&puede_consumir1);
        cout << "El fumador 1 ha recibido cerillas\n";
        sem_post(&puede_producir);
         
        int random = rand() % 2;
 
        if (random == 1) {
            sem_wait (&control);
             
            fumar();
            cout << "El fumador 1 ha fumado un cigarro\n";
            veces_fumar++;
             
            sem_post (&control);
        }
    }
 
    return NULL;
}
 
void * fumador2 (void *)
{
    while (2 > 1)
    {
        sem_wait(&puede_consumir2);
        cout << "El fumador 2 ha recibido tabaco\n";
        sem_post(&puede_producir);
     
        int random = rand() % 2;
 
        if (random == 1) {
            sem_wait (&control);
             
            fumar();
            cout << "El fumador 2 ha fumado un cigarro\n";
            veces_fumar++;
             
            sem_post (&control);
        }
    }
 
    return NULL;
}
 
void * fumador3 (void *)
{
    while (2 > 1)
    {
        sem_wait(&puede_consumir3);
        cout << "El fumador 3 ha recibido papel\n";
        sem_post(&puede_producir);
         
        int random = rand() % 2;
         
        if (random == 1) {
            sem_wait (&control);
             
            fumar();
            cout << "El fumador 3 ha fumado un cigarro\n";
            veces_fumar++;
             
            sem_post (&control);
        }
    }
 
    return NULL;
}
 
////////////////////////////////////////////////////////////////////////////////
// FUNCION DEL ESTANQUERO
// Encargada de despechar ingredientes
 
void * estanquero (void *)
{
    while (2 > 1)
    {
        sem_wait(&puede_producir);
 
        cout << "Despechando ";
 
        int producto = rand() % 3;
 
        switch (producto)
        {
            case 0:
                cout << "cerillas...\n";
                sem_post(&puede_consumir1);
                veces_estanquero++;
            break;
 
            case 1:
                cout << "tabaco...\n";
                sem_post(&puede_consumir2);
                veces_estanquero++;
            break;
 
            case 2:
                cout << "papel...\n";
                sem_post(&puede_consumir3);
                veces_estanquero++;
            break;
 
            default:
                cerr << "...ERROR\n";
                exit(1);
            break;
        }
    }
 
    return NULL;
}
 
////////////////////////////////////////////////////////////////////////////////
// FUNCION CONTADORA
// Muestra cada 2 segundos los contadores de cigarros fumados e ingredientes
// despechados
 
void * contador (void *)
{
    while (2 > 1)
    {
        sleep (2);
        cout << endl << "CONTADORES:" << endl;
        cout << "Ingredientes despechados: " << veces_estanquero << endl;
        cout << "Cigarros fumados: " << veces_fumar << endl << endl;
    }
}
 
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
 
int main()
{
    srand (time (NULL));   
     
    veces_estanquero = 0;
    veces_fumar = 0;
 
    // Creación de hebras
 
    pthread_t
        p_fumador1,
        p_fumador2,
        p_fumador3,
        p_estanquero,
        p_contadores;
 
    // Inicialización de semáforos
 
    sem_init (&puede_consumir1, 0, 0);
    sem_init (&puede_consumir2, 0, 0);
    sem_init (&puede_consumir3, 0, 0);
    sem_init (&puede_producir, 0, 1);
    sem_init (&control, 0, 1);
 
    // Inicialización de las hebras
 
    pthread_create (&p_fumador1, NULL, fumador1, NULL);
    pthread_create (&p_fumador2, NULL, fumador2, NULL);
    pthread_create (&p_fumador3, NULL, fumador3, NULL);
    pthread_create (&p_estanquero, NULL, estanquero, NULL);
    pthread_create (&p_contadores, NULL, contador, NULL);
 
    // Finalización de las hebras
 
    pthread_join (p_fumador1, NULL);
    pthread_join (p_fumador2, NULL);
    pthread_join (p_fumador3, NULL);
    pthread_join (p_estanquero, NULL);
    pthread_join (p_contadores, NULL);
 
    // Finalización de los semáforos
 
    sem_destroy (&puede_consumir1);
    sem_destroy (&puede_consumir2);
    sem_destroy (&puede_consumir3);
    sem_destroy (&puede_producir);
    sem_destroy (&control);
 
    return (0);
}
