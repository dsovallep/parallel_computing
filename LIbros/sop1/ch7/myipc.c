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

#include "myipc.h"

/* Inicializa el arreglo de 10 sucursales con valores aleatorios */
void init(double *ventas) {
   srandom(time(0));
   int i;
   for(i=0;i<10;i++,ventas++) *ventas =((double) random())/10000000.0;
}

//=====================================================cola de mensajes=====================
/* Envia un mensaje de determinado tipo a la cola */ 
int enviar_msg(int qid,void *qbuf, int msgsize, long type)  { 
   memcpy(qbuf,&type,sizeof(long)); //qbuf->mtype = type; 
   //strcpy(qbuf->mtext, text); 
   return msgsnd(qid,qbuf,msgsize, 0); 
}

/* Obtiene una clave ipc a partir de un path y de un caracter, ambos propios de la aplicacion y
   conocido por ambos: cliente y servidor */ 
key_t obtener_clave(char *path,char car) {
      return ftok(path, car); 
}

/* Obtiene un mensaje de la cola de un determinado tipo */ 
int recibir_msg(int qid, void *qbuf, int msgsize,long type) { 
   memcpy(qbuf,&type,sizeof(long)); //qbuf->mtype = type; 
   return msgrcv(qid, qbuf, msgsize, type, 0); 
}

/* Destruye cola de mensajes */ 
int borrar_cola(int qid)  { 
   return msgctl(qid, IPC_RMID, 0); 
} 

/* Crea cola de mensajes */ 
int crear_cola(key_t key) {
   return msgget(key, IPC_CREAT|0660);
}

/* Obtiene cola de mensajes previamente creada */ 
int obtener_cola(key_t key) {
   return msgget(key, 0660);
}

//=====================================================semaforos=====================

