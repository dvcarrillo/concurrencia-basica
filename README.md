# Concurrencia básica
Ejemplos básicos de programación utilizando diversos métodos de concurrencia.

## Semáforos
 - **productor-consumidor.cpp** incluye una hebra que produce datos y los almacena en un vector y otra hebra que los consume, con dos semáforos que controlan su ejecucuión.
 - **fumadores.cpp** se trata de un problema donde un estanquero reparte tres tipos de productos, y cada uno de ellos lo necesita un distinto consumidor, los cuales los consumen al ser expedidos. Por tanto, se trata de un programa con 4 hebras y 4 semáforos que controlan su ejecución. Contando la nueva función que muestra estadísticas, se trataría de 5 hebras y 4 semáforos

## Monitores en Java
 - **fumadores-con-monitores.java** es el mismo problema que el anterior, pero en este caso resuelto usando monitores en Java, utilizando una implementación estilo *Hoare*.

## Message Passing Interface (MPI)
 - **productor-consumidor-MPI.cpp** es el problema del productor consumidor anterior implementado con MPI, es decir, una interfaz de paso de mensajes. Se puede usar openMPI en Linux para compilar con `mpicxx -o ./prodcons ./productor-consumidor-MPI.cpp` y ejecutar con `mpirun -np 10 ./prodcons`.
 - **filosofos-sin-camarero.cpp** simula una cena de filósofos, donde todos realizan el mismo proceso: tomar los tenedores (primero el izquierdo y después el derecho), comer, dejar los tenedores y pensar. El objetivo es conseguir reproducirlo sin que se produzca interbloqueo y utilizando únicamente la interfaz de paso de mensajes.
 - **filosofos-con-camarero.cpp** se trata del problema anterior, pero en este caso implementando un camarero que evitaría el interbloqueo permitiendo sentarse a cierto número de filósofos (4 en este caso) al mismo tiempo. Los filósofos deben pedirle permiso a la hora de sentarse y de levantarse.
