#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Prototipo de funciones
void transferencias(int cuentas[]);

//Variables globales
int numHilos;
int tiempo;
int numCuentas;
int valorIni;

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
	transferencias(cuentas);
	for(i = 0; i < numCuentas; i++)
	{
	int j = (int)cuentas[i];
		printf("cuenta %d: %d\n", i, j);
	}
	return EXIT_SUCCESS;
}

void transferencias(int cuentas[])
{//metodo para realizar transferencias
	srand(time(NULL));//genera un numero aleatorio para la cuenta
	int numero1 = rand() % (numCuentas);//guarda el numero aleatorio
	int numero2;
	do
	{
		srand(time(NULL));//genera otro numero aleatorio para la cuenta
		numero2 = rand() % (numCuentas);//guarda el otro numero
	}while(numero1 == numero2);//verifica que no salga el mismo numero
	printf("num1: %d\n", numero1);
	printf("num2: %d\n", numero2);
	srand(time(NULL));//genera otro numero aleatorio para el valor transferido
	int cantidad = rand() % (valorIni);//guarda el numero
	printf("cant: %d\n", cantidad);
	if((cuentas[numero1] - cantidad) >= 0)
	{//verifica que la cuenta no quede menor que cero
	//se hace la transferencia
		cuentas[numero1] = cuentas[numero1] - cantidad;
		cuentas[numero2] = cuentas[numero2] + cantidad;
	}
	else
	{
		printf("No se puede realizar la transferencia ya que no se cuenta con los suficientes fondos");
	}
}
