//COMPILACION: gcc -std=c99 -o EjecutorLab5 -lpthread EjecutorLab5.c
//EJECUCION: ./EjecutorLab5 VectoresPrueba.txt
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h> 
#include <time.h> 
#include <math.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

//funcion para concatenar caracteres al final de un string
void stchcat(char *cadena, char chr);
//funcion que ejecuta el programa Lab5 y le entrega los parametros correspondientes
int ejecutarLab5();

//se maneja un contador global para la captura de los parametros que seran 
//enviados al programa Lab5
int contador;
//se maneja un string para la captura de los parametros que seran 
char *stringAux;
//se definen los enteros gloabales que almacenaran temporalmente la informacion
//enviada al programa Lab5
int global_numero_hilos;
int global_cantidad_tiempo;
int global_numero_cuentas;
int global_valor_inicial;
int global_repeticiones;


int main( int argc, char *argv[] ) {

	if(argc != 2)
	{	//No ingresa ningun argumento o ingresa demasiados
		printf("No se han ingresado los argumentos o ingreso demasiados\n");
		return EXIT_FAILURE;
	}

	//de define la variable que almacenara el nombre del archivo que contiene los vectores	
	char* nombreArchivo = argv[1];

	//se definen variables auxiliares para obtener la informacion de los vectores
	char caracterFila;
	char caracteresFila[100];

	//se define el archivo y se procede a abrir
	FILE *archivoVectores;
	archivoVectores = fopen(nombreArchivo,"r");

	//se maneja excepcion
	if(archivoVectores == NULL){
		printf("Error en la apertura del archivo con los vectores\n");
		return EXIT_FAILURE;
	}

	//se realiza conteo del numero de lineas del archovo de vectores
	int numeroLineas = 0;
	while(feof(archivoVectores) == 0){
		fgets(caracteresFila,100,archivoVectores);
 		numeroLineas = numeroLineas + 1;
	}

	//se controla que el archivo de vectores no este vacio
	if(numeroLineas == 1){
		printf("Error en el archivo con los vectores, se encuentra vacio\n");
		return EXIT_FAILURE;
	}
	numeroLineas = numeroLineas - 1;

	//se posiciona el cursor nuevamente al inicio del archivo
	rewind(archivoVectores);
	
	//se define un arreglo para cada uno de los parametros que debe recibir el programa Lab5
	//al igual que un arreglo para las repeciciones que debe tener cada vector
	int numero_hilos[numeroLineas];
	int cantidad_tiempo[numeroLineas];
	int numero_cuentas[numeroLineas];
	int valor_inicial[numeroLineas];
	int repeticiones[numeroLineas];

	//se definen algunas variables auxiliares y se les asignan valores
	int contadorParametros = 0;
	contador = 0;
	stringAux = malloc(strlen("") + 2);
	strcpy(stringAux, "");
	
	//se recorre caracter a caracter el archivo para obtener los parametros de cada vector
	while(!feof(archivoVectores)){
		caracterFila = fgetc(archivoVectores);
		
		if(caracterFila != ' ')
		{
			if(caracterFila == '\n'){
				repeticiones[contadorParametros] = atoi(stringAux);
				contador = 0;
				strcpy(stringAux, "");
				contadorParametros = contadorParametros + 1;
			}else{
					stchcat(stringAux, caracterFila);
			}
		}else
		{
			//dependiendo del valor de contador se asigna el valor a cada uno de los parametros a entregar
			if(contador == 0){
				numero_hilos[contadorParametros] = atoi(stringAux);
				strcpy(stringAux, "");
			}else if(contador == 1){
				cantidad_tiempo[contadorParametros] = atoi(stringAux);
				strcpy(stringAux, "");
			}else if(contador == 2){
				numero_cuentas[contadorParametros] = atoi(stringAux);
				strcpy(stringAux, "");
			}else if(contador == 3){
				valor_inicial[contadorParametros] = atoi(stringAux);
				strcpy(stringAux, "");
			}else if(contador == 4){
				repeticiones[contadorParametros] = atoi(stringAux);
				strcpy(stringAux, "");
			}
			contador = contador + 1;
		}
	}
	//se cierra el archivo que contiene la informacion de los vectores
	fclose(archivoVectores);
	
	//se recorre cada uno de los arreglos que contienen los parametros a entregar
	//y se asignan esos valores a las variables globales para asi ejecutar el programa Lab5
	for(int i = 0; i < contadorParametros; i++){
		global_numero_hilos = numero_hilos[i];
		global_cantidad_tiempo = cantidad_tiempo[i];
		global_numero_cuentas = numero_cuentas[i];
		global_valor_inicial = valor_inicial[i];
		global_repeticiones = repeticiones[i];
		ejecutarLab5(i);
	}

	//se devuelve el proceso exitoso
	return EXIT_SUCCESS;
}

//funcion para concatenar un caracter al final de un string
void stchcat(char *cadena, char chr)
{
   size_t longitud = strlen(cadena);
   *(cadena + longitud) = chr;
   *(cadena + longitud + 1) = '\0';
}

//funcion que ejecutara el programa Lab5 las veces que indique el parametro de repeticiones
int ejecutarLab5(int numFila)
{
	
	//se defiene un valor inicial a la variable que se va a pasar como nombre del archivo
	char parametro[100] = "./Lab5 ";

	//se define un arreglo de caracteres auxiliar y se arma la estructura que debe recibir el programa Lab5
	char buffer[10];
	snprintf(buffer, 10, "%d", global_numero_hilos);
	strcat(parametro, buffer);
	strcat(parametro, " ");
	
	snprintf(buffer, 10, "%d", global_cantidad_tiempo);
	strcat(parametro, buffer);
	strcat(parametro, " ");

	snprintf(buffer, 10, "%d", global_numero_cuentas);
	strcat(parametro, buffer);
	strcat(parametro, " ");

	snprintf(buffer, 10, "%d", global_valor_inicial);
	strcat(parametro, buffer);

	printf("\n---------------------------------------\n");	
	printf("\n%s\n\n", parametro);
	
	//se invoca el programa Lab5 las veces que se indica en el parametro de repeciciones
	for(int i = 0; i < global_repeticiones; i++){
		/* Fichero con el que abriremos /Lab5 */
		FILE *fichero;

		/* Variable auxiliar para leer el fichero */
		char aux[1000];
		int contadorAuxiliar = 0;
		int sumaAuxiliar = 0;
		int totalAuxiliar = 0;

		int numFilaAux = numFila + 1;
		int iAux = i + 1;

		/* Se abre el fichero /Lab5, y se comprueba
		* que se ha podido abrir correctamente */
		fichero = popen (parametro, "r");
		if (fichero == NULL)
		{
			perror ("No se puede abrir /Lab5");
			return -1;
		}

		/* Se lee la primera linea y se hace un bucle, hasta fin de fichero,
	 	* para ir sacando por pantalla los resultados.
	 	*/
		fgets (aux, 1000, fichero);
		while (!feof (fichero))
		{
			if(contadorAuxiliar < global_numero_cuentas){
				contadorAuxiliar = contadorAuxiliar + 1;
				sumaAuxiliar = sumaAuxiliar + atoi(aux);
				printf ("Saldo en cuenta %d: %s", contadorAuxiliar, aux);
			}else{
				totalAuxiliar = atoi(aux);
				printf ("Total en cuentas: %s", aux);
			}
			fgets (aux, 1000, fichero);
		}

		/* Se cierra el fichero */
		pclose (fichero);

		if(sumaAuxiliar == totalAuxiliar){
			printf ("\nPara el vector de la fila %d en la repeticion %d el resultado es correcto\n\n", numFilaAux, iAux);
		}else{
			printf ("\nPara el vector de la fila %d en la repeticion %d el resultado es incorrecto\n", numFilaAux, iAux);
		}
		printf("\n---------------------------------------\n");	
	}
	
}
