#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std;

// /*****************************************************************************/
// Función que simula la acción de fumar como un retardo aleatorio de la hebra

void fumar() 
{ 
   // calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
   const unsigned miliseg = 100U + (rand() % 1900U) ; 

   // retraso bloqueado durante 'miliseg' milisegundos
   usleep (1000U*miliseg); 
}

/*****************************************************************************/
sem_t
	puede_consumir1,
	puede_consumir2,
	puede_consumir3,
	puede_producir;

/*****************************************************************************/ 

void * fumador1 (void *)
{
	while (2 > 1)
	{
		sem_wait(&puede_consumir1);
		cout << "El fumador 1 ha recibido cerillas\n";
		sem_post(&puede_producir);

		fumar();
		cout << "El fumador 1 ha fumado un cigarro\n";
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

		fumar();
		cout << "El fumador 2 ha fumado un cigarro\n";
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

		fumar();
		cout << "El fumador 3 ha fumado un cigarro\n";
	}

	return NULL;
}

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
			break;

			case 1:
				cout << "tabaco...\n";
				sem_post(&puede_consumir2);
			break;

			case 2:
				cout << "papel...\n";
				sem_post(&puede_consumir3);
			break;

			default:
				cerr << "...ERROR\n";
				exit(1);
			break;
		}
	}

	return NULL;
}

// /*****************************************************************************/

int main()
{
	srand (time (NULL));   

	// Creación de hebras

	pthread_t
		p_fumador1,
		p_fumador2,
		p_fumador3,
		p_estanquero;

	// Inicialización de semáforos

	sem_init (&puede_consumir1, 0, 0);
	sem_init (&puede_consumir2, 0, 0);
	sem_init (&puede_consumir3, 0, 0);
	sem_init (&puede_producir, 0, 1);

	// Inicialización de las hebras

	pthread_create (&p_fumador1, NULL, fumador1, NULL);
	pthread_create (&p_fumador2, NULL, fumador2, NULL);
	pthread_create (&p_fumador3, NULL, fumador3, NULL);
	pthread_create (&p_estanquero, NULL, estanquero, NULL);

	// Finalización de las hebras

	pthread_join (p_fumador1, NULL);
	pthread_join (p_fumador2, NULL);
	pthread_join (p_fumador3, NULL);
	pthread_join (p_estanquero, NULL);

	// Finalización de los semáforos

	sem_destroy (&puede_consumir1);
	sem_destroy (&puede_consumir2);
	sem_destroy (&puede_consumir3);
	sem_destroy (&puede_producir);

	return (0);
}
