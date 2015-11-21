/*****************************************************************************/
// Sistemas Concurrentes y Distribuidos
// Práctica 2: Problema de los fumadores
// 
// Por David Vargas Carrillo, noviembre de 2015
/*****************************************************************************/

import monitor.*;
import java.util.Random;

/*****************************************************************************/
// Monitor que resuelve la interacción entre el estanquero y los fumadores

class Estanco extends AbstractMonitor
{
	// Guarda el ingrediente que despacha el estanquero
	// El valor -1 se considerará igual a ningún ingrediente disponible
	public int ingrediente_mostrador;

	// Cola de condición para cada fumador, que decide si el fumador
	// especificado puede o no fumar
	private Condition[] puede_consumir = new Condition[3];

	// Cola de condición para el estanquero, que decide si puede o no producir
	private Condition puede_producir = makeCondition();

	public Estanco ()
	{
		ingrediente_mostrador = -1;

		for (int i = 0; i < 3; i++)
		{
			puede_consumir[i] = makeCondition();
		}
	}

	// Método que invocan los fumadores indicando su ingrediente (número)
	public void obtenerIngrediente (int miIngrediente)
	{
		enter();

			if (ingrediente_mostrador != miIngrediente)
			{
				puede_consumir[miIngrediente].await();
			}

			// Se informa de qué fumador ha fumado el ingrediente

			System.out.println ("Fumador " + (miIngrediente + 1) + " ha consumido un cigarro.");

			// Se pone a nulo el ingrediente en el mostrador
			ingrediente_mostrador = -1;
			puede_producir.signal();

		leave();
	}

	// Método que invoca el estanquero, indicando el siguiente ingrediente que
	// pone
	public void ponerIngrediente (int ingrediente)
	{
		enter();

			ingrediente_mostrador = ingrediente;

			switch (ingrediente_mostrador)
			{
				case 0: System.out.println ("Despachando cerillas...");
						break;
				case 1: System.out.println ("Despachando tabaco...");
						break;
				case 2: System.out.println ("Despachando papel...");
			}

			puede_consumir[ingrediente].signal ();

		leave();
	}

	public void esperarRecogidaIngrediente ()
	{
		enter();

			if (ingrediente_mostrador != -1)
				puede_producir.await ();

		leave();
	}
}

/*****************************************************************************/
// Hebra fumadora, de la cual habrá 3 instancias, una por fumador
// Al primer fumador se le asignará el número de ingrediente 0, al segundo
// el 1, y al tercero el 2

class Fumador implements Runnable
{
	private int miIngrediente;
	private Estanco estanco;
	public Thread thr;

	public Fumador (int p_miIngrediente, Estanco un_estanco)
	{
		miIngrediente = p_miIngrediente;
		estanco = un_estanco;
		thr = new Thread (this);
		System.out.println ("Fumador " + (miIngrediente + 1)  + " creado.");
	}

	public void run ()
	{
		while (true)
		{
			estanco.obtenerIngrediente(miIngrediente);
			aux.dormir_max (2000);
		}
	}
}

/*****************************************************************************/
// Hebra estanquero, la cual produce ingredientes y espera a que se recojan

class Estanquero implements Runnable
{
	public Thread thr;
	private Estanco estanco;

	public Estanquero (Estanco un_estanco)
	{
		estanco = un_estanco;
		thr = new Thread (this);
	}

	public void run ()
	{
		while (true)
		{
			int ingrediente = (int) (Math.random () * 3.0);	// Puede ser 0, 1, 2
			estanco.ponerIngrediente (ingrediente);
			estanco.esperarRecogidaIngrediente ();
		}
	}
}

/*****************************************************************************/
// Clase auxiliar, implementa funciones a las que llaman órdenes de la
// hebra Fumador

class aux
{
	static Random r	= new Random();

	static void dormir_max (int msMax)
	{
		try
		{
			Thread.sleep (r.nextInt(msMax));
		}
		catch (InterruptedException e)
		{
			System.err.println("ERROR: Sleep interumpido en ’aux.dormir_max()’");
		}
	}

}

/*****************************************************************************/
// Clase principal

class MainFumadores
{
	public static void main (String [] args)
	{
		if (args.length > 0)
		{
			System.err.println("No necesito argumentos");
			return;
		}

		// Creación de objetos de las clases Estanquero y Fumador, y
		// del monitor

		Estanco estanco = new Estanco();
		Fumador[] fumadores = new Fumador[3];
		Estanquero estanquero = new Estanquero(estanco);

		for (int i = 0; i < 3; i++)
		{
			fumadores[i] = new Fumador(i, estanco);
		}

		estanquero.thr.start();

		for(int i=0; i < 3; i++)
		{
        	fumadores[i].thr.start();
        }
	}
}
