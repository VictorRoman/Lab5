//COMPILACION: gcc -Wall -g Lab5.c -o Lab5 -lpthread
//EJECUCION: ./Lab5 2 10 3 11
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//Prototipo de funciones
void transferencias(void* param);

//Variables globales
int numHilos;//Guarda el numero de hilos pasado por el parametro
int tiempo;//Guarda la cantidad de tiempo a correr pasado por el parametro
int numCuentas;//Guarda el numero de cuentas pasado por el parametro
int valorIni;//Guarda el valor inicial pasado por el parametro

/*Esta estructura que sirve para pasar parametros a el hilo*/
struct parametros_hilo
{
	/*Parametro 1: cuentas*/
	int* cuentas;
	/*Parametro 2: mutex*/
	pthread_mutex_t* mutex;
};

int main(int argc, char* argv[])
{
	int args[4];//para guardar los numeros de los argumentos
	if(argc < 5 || argc > 5)
	{//No ingresa ningun argumento o ingresa demasiados
		printf("No se han ingresado los argumentos o ingreso demasiados\n");
		return EXIT_FAILURE;
	}

	//Se ingresan los argumentos
	int i;
	for(i = 1; i < 5; i++)
	{//ciclo para separar los numeros de los argumentos
		char* c = argv[i];
		int j;
		int numero = 0;
		int pesoNum = 10;//se multiplica para aumentar la cifra de acuerdo a su peso
		for(j = 0; c[j] != '\0'; j++)
		{//ciclo para pasar de caracter a entero
			char l = c[j] - 48;
			if(l >= 0 && l <= 9)
			{//verifica que si sea numero
				numero = (numero * pesoNum) + l;
			}
			else
			{
				printf("Debe ingresar solo numeros\n");
			}
		}
		args[i - 1] = numero;//guarda el numero en el arreglo
	}
	numHilos = args[0];//guardo el numero de hilos
	tiempo = args[1];//guardo el tiempo de ejecucion de los hilos
	numCuentas = args[2];//guardo el numero de cuentas
	valorIni = args[3];//guardo el valor inicial de las cuentas
	
	int cuentas[numCuentas];//creo el numero de cuentas
	for(i = 0; i < numCuentas; i++)
	{//ciclo para guardar el valor inicial
		cuentas[i] = (int)valorIni;
	}

	pthread_t hilo[numHilos];//Creo la cantidad de hilos

	pthread_mutex_t mutex[numCuentas];//mutex para sincronizar la operacion sobre las cuentas

	for(i = 0; i < numCuentas; i++)
	{
		pthread_mutex_init(&mutex[i], NULL);//inicia los mutex
	}

	struct parametros_hilo param;//se crea la estructura y se inicializa
	param.cuentas = cuentas;
	param.mutex = mutex;
	
	for(i = 0; i < numHilos; i++)
	{//ciclo para ejecutar el metodo transacciones de todos los hilos
		pthread_create(&hilo[i], NULL, (void*)transferencias, &param);
	}

	for(i = 0; i < numHilos; i++)
	{//ciclo para esperar a todos los hilos
		pthread_join(hilo[i], NULL);
	}

	for(i = 0; i < numCuentas; i++)
	{
		pthread_mutex_destroy(&mutex[i]);//destruye los mutex
	}

	int suma = 0;//variable para obtener la suma de las cuentas
	for(i = 0; i < numCuentas; i++)
	{//cilo para realizar la suma de las cuentas y mostrar la cantidad en cada cuenta
		int j = (int)cuentas[i];
		printf("cuenta %d %d\n", i, j);
		suma = suma + j;
	}
	printf("suma %d\n", suma);//muestra la suma en todas las cuentas
	return EXIT_SUCCESS;
}

void transferencias(void* param)
{//metodo para realizar transferencias
	struct parametros_hilo* p = (struct parametros_hilo*)param;
	int i;
	for(i = 0; i < ((tiempo * 1000) / 1000); i++)
	{//ciclo para que cada el hilo se ejecute el tiempo pasado por parametro
		int unsigned seed1 = time(NULL) * pthread_self();
		int numero1 = rand_r(&seed1) % numCuentas;//genera un numero aleatorio para la cuenta 1
		int numero2;
		do
		{
			int unsigned seed2 = time(NULL) * pthread_self();
			numero2 = rand_r(&seed2) % numCuentas;//genera otro numero aleatorio para la cuenta 2
		}while(numero1 == numero2);//verifica que no salga el mismo numero

		pthread_mutex_lock(&(p->mutex[numero1]));//bloquea el mutex para una entrada sincronizada en la cuenta 1
		pthread_mutex_lock(&(p->mutex[numero2]));//bloquea el mutex para una entrada sincronizada en la cuenta 2

		int unsigned seed3 = time(NULL) * pthread_self();	
		int cantidad = rand_r(&seed3)  % valorIni;//genera otro numero aleatorio para el valor transferido
		if((p->cuentas[numero1] - cantidad) >= 0)
		{//verifica que la cuenta no quede menor que cero
		//se hace la transferencia
			p->cuentas[numero1] = p->cuentas[numero1] - cantidad;
			p->cuentas[numero2] = p->cuentas[numero2] + cantidad;
		}
		else
		{//con este mensaje se verifica que los hilos no traten de acceder a la misma cuenta siempre
			//printf("No se puede realizar la transferencia de la cuenta %d a la cuenta %d ya que no se cuenta con los fondos suficientes\n", numero1, numero2);
		}
		pthread_mutex_unlock(&(p->mutex[numero2]));//desbloquea el mutex
		pthread_mutex_unlock(&(p->mutex[numero1]));//desbloquea el mutex
	sleep(1);//duerme un segundo
	}
}
