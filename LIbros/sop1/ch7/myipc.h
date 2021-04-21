/*
   myipc.h IPC. Cola de mensajes, memoria compartida,  semaforos
   
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h>
#include <ctype.h> 
#include	<fcntl.h>
#include	<unistd.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h> 

#define IPC_PATH       "."    // path general para obtener claves ipc
#define IPC_KEY         'm'   // letra para cola de mensajes
#define IPC_SKEY       's'    // letra para semaforos
#define IPC_MKEY      'h'    // letra para memoria  compartida

// funciones en general para ipc
key_t obtener_clave(char *path,char car);

// funciones para el manejo de colas
int borrar_cola(int qid); 
int crear_cola(key_t key);
int enviar_msg(int qid, void *qbuf, int msglen, long type); 
int obtener_cola(key_t key);
int recibir_msg(int qid, void *qbuf,int msglen, long type); 

// funciones para el manejo de memoria compartida

// funciones para el manejo de semaforos

// otras funciones
void init(double *ventas);

