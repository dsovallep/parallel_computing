/*
   myipc.h IPC. Cola de mensajes, memoria compartida,  semaforos
   
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include "myipc.h"

/* Inicializa el arreglo de 10 sucursales con valores aleatorios */
void init(double *ventas) {
   srandom(time(0));
   int i;
   for(i=0;i<10;i++,ventas++) *ventas =((double) random())/10000000.0;
}

void generar_venta(struct sucvta *sv) {
   sv->sucursal = (int) ((long) random() % 10L);
   int dia = (int) ((long) random() % 31L);
   int mes = (int) ((long) random() % 12L)+1;
   int anio = 2009;
   snprintf(sv->fecha,11,"%2d/%2d/%4d",dia,mes,anio);
   sv->factura = (int) ((long) random())/1000000L;
   sv->monto = ((double) random())/10000000.0;
}

void mostrar_sucventas(FILE *salida,struct sucvta *sv) {
   int n;
   for(n=0;n<10;n++,sv++) fprintf(salida,"%2d%s%10d%12.2lf\n",
                                                            sv->sucursal,sv->fecha,sv->factura,sv->monto);
}

void mostrar_ventas(double *ventas) {
   int suc;
   for(suc=0;suc<10;suc++,ventas++) printf("mostrar_ventas(): sucursal[%d]=%lf\n",suc,*ventas);
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

/* Obtiene n claves ipc a partir de un arreglo path y de un arreglo caracter, ambos propios de la aplicacion y
   conocido por ambos: cliente y servidor
   arrkey tiene nkeys elementos, al igual que path y car */ 
void obtener_claves(char *path[],char car[],key_t arrkey[],int nkeys) {
   int i;
   for(i=0;i<nkeys;i++) {
      arrkey[i] = ftok(path[i], car[i]);
//printf("obtener_claves():arrkey[%d]=%d\n",i,(int) arrkey[i]);      
   }
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
int borrar_semaforo(int semid) {
   return borrar_semaforos(semid,0);
}

int borrar_semaforos(int semid,int nsem) {
   return semctl(semid, nsem, IPC_RMID, 0);
}

// incrementar en 1 a un semaforo
int semSignal(int semid) {
   return semSignalTo(semid,1);
}

// decrementar en 1 a un semaforo
int semWait(int semid) {
   return semSignalTo(semid,-1);
}

// idem semSignalTo() para decrementar un valor de un semaforo
int semSignalWaitTo(int semid,int nro) {
   // nro debe ser negativo
   if ( nro > 0 ) nro = nro * -1;
   return semSignalTo(semid,nro);
}

// incrementar/decrementar en n a un semaforo
int semSignalTo(int semid,int nro) {
   struct sembuf sb[1];
   sb[0].sem_num = 0;
   sb[0].sem_op = nro;
   sb[0].sem_flg = SEM_UNDO | 0; // vuelve atras el cambio si se cae este proceso
   return semop(semid,sb,1);
}

// idem semSignalsTo() para decrementar un valor de un semaforo
int semSignalWaitsTo(int semid,struct sembuf sb[],int nsem) {
   int i;
   for(i=0;i<nsem;i++) if ( sb[i].sem_op > 0 ) sb[i].sem_op = sb[i].sem_op * -1;
   return semSignalsTo(semid,sb,nsem);
}

// incrementar/decrementar en n valores distintos a n semaforos
int semSignalsTo(int semid,struct sembuf sb[],int nsem) {
   return semop(semid,sb,nsem);
}

// idem semSignalsToNum() para decrementar en 1 mismo valor  a n semaforos
int semSignalWaitsToNum(int semid,int nro,int nsem) {
   // nro debe ser negativo
   if ( nro > 0 ) nro = nro * -1;
   return semSignalsToNum(semid,nro,nsem);
}

// incrementar/decrementar en 1 mismo valor  a n semaforos
int semSignalsToNum(int semid,int nro,int nsem) {
   struct sembuf sb[nsem];
   int i;
   for(i=0;i<nsem;i++) {
      sb[i].sem_num = i;
      sb[i].sem_op = nro;
      sb[i].sem_flg = SEM_UNDO | 0; // vuelve atras el cambio si se cae este proceso
   }
   return semop(semid,sb,nsem);
}

// creamos semaforos "de a uno", a pesar de que la api de svr4 permite crear n semaforos en una misma llamada
int crear_semaforo(key_t key,int modo) {
   return crear_semaforos(key,1,modo);
}

int crear_semaforos(key_t key,int nsem,int modo) {
   //return semget(key, nsem, IPC_CREAT|SEM_UNDO|modo); // vuelve atras el semaforo si se cae este proceso
   return semget(key, nsem, 0666|IPC_CREAT); // vuelve atras el semaforo si se cae este proceso
}

// obtenemos semaforos "de a uno", a pesar de que la api de svr4 permite crear n semaforos en una misma llamada
int obtener_semaforo(key_t key,int modo) {
   return semget(key, 0, 0666); // vuelve atras el semaforo si se cae este proceso
}

//=====================================================memoria compartida=====================
int borrar_memoria(int shmid) {
   return shmctl(shmid, IPC_RMID, 0);
}

void *conectar_memoria(int shmid,int modo) {
   return shmat(shmid,0,modo);
}

int desconectar_memoria(void *buffer) {
   return shmdt(buffer);
}

int crear_memoria(key_t key,int size,int modo) {
   return shmget(key, size, IPC_CREAT|modo);
}

/* Obtiene memoria compartida previamente creada */ 
int obtener_memoria(key_t key,int modo) {
   return crear_memoria(key,0,modo);
}

