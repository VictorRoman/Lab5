//COMPILACION: gcc -std=c99 -o Lab5 -lpthread Lab5.c
//EJECUCION: ./Lab5 2 10 3 11
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//Prototipo de funciones
void transferencias(int cuentas[]);

//Variables globales
int numHilos;	//Guarda el numero de hilos pasado por el parametro
int tiempo;	//Guarda la cantidad de tiempo a correr pasado por el parametro
int numCuentas;	//Guarda el numero de cuentas pasado por el parametro
int valorIni;	//Guarda el valor inicial pasado por el parametro
pthread_mutex_t mutex;	//mutex para sincronizar la operacion sobre los datos


int main(int argc, char* argv[])
{
	
	int args[4];	//para guardar los numeros de los argumentos
	if(argc < 5 || argc > 5)
	{	//No ingresa ningun argumento o ingresa demasiados
		printf("No se han ingresado los argumentos o ingreso demasiados\n");
		return EXIT_FAILURE;
	}

	//Se ingresan los argumentos
	int i;
	for(i = 1; i < 5; i++)
	{	//ciclo para separar los numeros de los argumentos
		char* c = argv[i];
		int j;
		int numero = 0;
		int pesoNum = 10;	//se multiplica para aumentar la cifra de acuerdo a su peso
		
		for(j = 0; c[j] != '\0'; j++)
		{	//ciclo para pasar de caracter a entero
			char l = c[j] - 48;
			
			if(l >= 0 && l <= 9)
			{	//verifica que si sea numero
				numero = (numero * pesoNum) + l;
			}
			else
			{
				printf("Debe ingresar solo numeros\n");
			}
		}
		args[i - 1] = numero;	//guarda el numero en el arreglo
	}
	numHilos = args[0];	//guardo el numero de hilos
	tiempo = args[1];	//guardo el tiempo de ejecucion de los hilos
	numCuentas = args[2];	//guardo el numero de cuentas
	valorIni = args[3];	//guardo el valor inicial de las cuentas

	int cuentas[numCuentas];//creo el numero de cuentas
	for(i = 0; i < numCuentas; i++)
	{	//ciclo para guardar el valor inicial
		cuentas[i] = (int)valorIni;
	}

	pthread_t hilo[numHilos];	//Creo la cantidad de hilos

	pthread_mutex_init(&mutex, NULL);	//inicio el mutex

	for(i = 0; i < numHilos; i++)
	{	//ciclo para ejecutar el metodo transacciones de todos los hilos
		pthread_create(&hilo[i], NULL, (void*)transferencias, cuentas);
	}

	pthread_mutex_destroy(&mutex);	//destruye el mutex

	for(i = 0; i < numHilos; i++)
	{	//ciclo para esperar a todos los hilos
		pthread_join(hilo[i], NULL);
	}

	float tamanio = sizeof(cuentas);	//calculamos el tamaño en bytes

	
	int suma = 0;	//variable para obtener la suma de las cuentas
	for(i = 0; i < numCuentas; i++)
	{	//cilo para realizar la suma de las cuentas y mostrar la cantidad en cada cuenta
		int j = (int)cuentas[i];
		printf("%d\n", j);
		suma = suma + j;
	}
	printf("%d\n", suma);	//muestra la suma en todas las cuentas
	return 1;
}

void transferencias(int cuentas[])
{	//metodo para realizar transferencias
	pthread_mutex_lock(&mutex);//bloquea el mutex para una entrada sincronizada
	int i;
	for(i = 0; i < ((tiempo * 1000) / 1000); i++)
	{	//ciclo para que cada el hilo se ejecute el tiempo pasado por parametro
		srand((time(NULL) * pthread_self()));	//genera un numero aleatorio para la cuenta
		int numero1 = rand() % (numCuentas);	//guarda el numero aleatorio
		int numero2;
		do
		{
			srand((time(NULL) * pthread_self()));	//genera otro numero aleatorio para la cuenta
			numero2 = rand() % (numCuentas);	//guarda el otro numero
		}while(numero1 == numero2);			//verifica que no salga el mismo numero
		srand((time(NULL) * pthread_self()));		//genera otro numero aleatorio para el valor transferido
		int cantidad = rand() % (valorIni);		//guarda el numero
		if((cuentas[numero1] - cantidad) >= 0)
		{	//verifica que la cuenta no quede menor que cero
			//se hace la transferencia
			cuentas[numero1] = cuentas[numero1] - cantidad;
			cuentas[numero2] = cuentas[numero2] + cantidad;
		}
		else
		{	//con este mensaje se verifica que los hilos no traten de acceder a la misma cuenta siempre
			//printf("No se puede realizar la transferencia de la cuenta %d a la cuenta %d ya que no se cuenta con los fondos suficientes\n", 					//numero1, numero2);
		}
	sleep(1);	//duerme un segundo
	}
	pthread_mutex_unlock(&mutex);	//desbloquea el mutex
}
