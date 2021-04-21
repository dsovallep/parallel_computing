/* Comando para correr codigo C++ con phtread en unix
g++ script1.c -o output -pthread
./output
*/    

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <iostream>

//Definicion de espacios de nombre
using namespace std;

/*Funcion que ejecutara el hilo
tiene que estar definido con apuntado a void 
y el argumento tambien tiene que se apuntador a void
y esta funcion solo puede tener un solo argumento
Si se necesita que la funcion reciba mas de un parametro
se puede hacer mediante una estructura 
*/
void* fnDespliegaX(void* arg){

    // Asigana el valor del parametro en este caso x, que viene desde la creación del hilo
    int tabla = *((int*)arg); // Se le hace un casting a arg de void a int, con su respectivo apuntador

    // Despliega el valor de x
    cout << "La tabla del " << tabla << endl;

    // Tabla de multiplicar de 1 a 100 del valor en x}
    for (int i = 1; i <= 1000; i++)
    {
        cout << tabla << "X" << i << "=" << tabla*i << endl;

    }

    return NULL; // Finaliza la funcion
}


//Funcion principal
int main(int argc,char* argv[]){

    // Variable de resultado
    int resultado = 0;

    // Declaro un objeto hilo
    pthread_t hilo1;

    // Valor inicial de x
    int x = 100;

    /* Creo hilo y valido error
    con la funcion pthread_create se le pasa como parametro la direccion del objeto hilo,
    atributos del hilo en este caso NULL, la funcion que va a correr el hilo y 
    la direccion del parametro, que recibe la funcion que corre el hilo (la variable de entrada).
    esa funcion solo permite un parametro 
    */
    if(pthread_create(&hilo1, NULL, fnDespliegaX, &x)){
        
        //Mensaje y valor de retorno
        cout << "Error al crear hilo" << endl;
        resultado = -1;

    }else{
        /* Hace que el programa no finalice hasta que el hilo finalice
        pthread_join(hilo1,NULL);
        */

       cout << "Teclea algo para finalizar" ;
       getchar();//Finaliza hasta que se ingrese algo por consola

    }

    return 0; // Finaliza el programa
}

