/*****************************************************************************/
// Prácticas de Sistemas Concurrentes y Distribuidos
// Por David Vargas Carrillo, 2016
/*****************************************************************************/

#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>

using namespace std;

void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);

// ---------------------------------------------------------------------

int main (int argc, char* argv[])
{
   int rank, size;

   srand(time(0));
   MPI_Init (&argc, &argv);
   MPI_Comm_rank (MPI_COMM_WORLD, &rank);
   MPI_Comm_size (MPI_COMM_WORLD, &size);

   if (size != 10)
   {
      if (rank == 0)
         cout << "El numero de procesos debe ser 10" << endl << flush ;
      MPI_Finalize( );
      return 0;
   }

   if ((rank % 2) == 0)
      Filosofo(rank, size);    // Los pares son Filosofos
   else
      Tenedor(rank, size);     // Los impares son Tenedores

   MPI_Finalize();
   return 0;
}
// ---------------------------------------------------------------------

void Filosofo (int id, int nprocesos)
{
   int izq = (id + 1) % nprocesos;                  // Tenedor izquierdo
   int der = ((id+nprocesos) - 1) % nprocesos;      // Tenedor derecho

   int peticion = 1;          // Valor que se envía para pedir el tenedor
   int depositar = 0;         // Valor que se envía para depositar el tenedor

   while (true)
   {
      // Para evitar el interbloqueo se propone que un determinado filósofo empiece
      // cogiendo el tenedor derecho antes que el izquierdo

      if (id == 0) {
        // Solicita tenedor derecho
        cout << "Filosofo "<< id << " solicita tenedor derecho " << der << endl << flush;
        MPI_Ssend(&peticion, 1, MPI_INT, der, 0, MPI_COMM_WORLD);
       
        // Solicita tenedor izquierdo
        cout << "Filosofo " << id << " solicita tenedor izquierdo " << izq << endl << flush;
        MPI_Ssend(&peticion, 1, MPI_INT, izq, 0, MPI_COMM_WORLD);
      }

      else {
        // Solicita tenedor izquierdo
        cout << "Filosofo " << id << " solicita tenedor izquierdo " << izq << endl << flush;
        MPI_Ssend(&peticion, 1, MPI_INT, izq, 0, MPI_COMM_WORLD);

        // Solicita tenedor derecho
        cout << "Filosofo "<< id << " solicita tenedor derecho " << der << endl << flush;
        MPI_Ssend(&peticion, 1, MPI_INT, der, 0, MPI_COMM_WORLD);
      }

      cout << "Filosofo " << id << " COMIENDO..." << endl << flush;
      sleep((rand() % 3) + 1);

      // Suelta el tenedor izquierdo
      cout << "Filosofo " << id << " suelta tenedor izquierdo " << izq << endl << flush;
      MPI_Ssend(&depositar, 1, MPI_INT, izq, 0, MPI_COMM_WORLD);

      // Suelta el tenedor derecho
      cout <<"Filosofo "<<id<< " suelta tenedor derecho " << der << endl << flush;
      MPI_Ssend(&depositar, 1, MPI_INT, der, 0, MPI_COMM_WORLD);

      // Piensa (espera bloqueada aleatorio del proceso)
      cout << "Filosofo " << id << " PENSANDO..." << endl << flush;

      // espera bloqueado durante un intervalo de tiempo aleatorio
      // (entre una décima de segundo y un segundo)
      usleep (1000U * (100U + (rand() % 900U)));
 }
}
// ---------------------------------------------------------------------

void Tenedor(int id, int nprocesos)
{
  int buf;
  MPI_Status status;
  int Filo;

  while (true)
  {
    // Espera un peticion desde cualquier filosofo vecino
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    // Recibe la peticion del filosofo
    Filo = status.MPI_SOURCE;
    MPI_Recv(&buf, 1, MPI_INT, Filo, 0, MPI_COMM_WORLD, &status);
    cout << "Tenedor " << id << " recibe peticion de " << Filo << endl << flush;

    // Espera a que el filosofo suelte el tenedor...
    MPI_Recv(&buf, 1, MPI_INT, Filo, 0, MPI_COMM_WORLD, &status);
    cout << "Tenedor " << id << " recibe liberacion de " << Filo << endl << flush;
  }
}
