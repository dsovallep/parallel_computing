/*
   tp85c.c IPC. Memoria compartida. Proceso que lee de memoria compartida el total de
   ventas de las sucursales+ventas recibidos por sucursal.  
   guarda las ventas obtenidas de memoria compartida en archivo secuencial, de texto, con formato.
   incorpora estructura sucvta
   los datos en memoria compartida se guardan:  <vector total de ventas><vector ventas por sucursal>
   prepara proceso consumidor para luego implementar logica de semaforos.
   Implementa semaforos utilizando la libreria myipc.c 
  
  
   para compilar:
            gcc -Wall -o tp85c tp85c.c myipc.c
            
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include "myipc.h"

// prototipo de funciones
// datos globales
volatile int salir=0;

// prototipo de funciones
static void sig_salir(int);

int main(int argc, char *argv[])  {
   int ret=0;
   key_t key; 
   int idmem; 
   int pid=(int) getpid();
   double *pventas;
   struct sucvta *psucvta;
   int i;

   // variables de semaforos
   int sem_ids[3];
   key_t sem_keys[3];
   char *sem_paths[] = {IPC_PATH,IPC_PATH,IPC_PATH};
   char sem_chars[] = {IPC_SKEYs ,IPC_SKEYn ,IPC_SKEYe };
   int sem_modo = SEM_R | SEM_A;
   char *sem_error1[] = {"main():Error obteniendo semaforo s",
                                       "main():Error obteniendo semaforo n",
                                       "main():Error obteniendo semaforo e",};
   
   printf("main():inicio proceso consumidor!\n");
   printf("main():para salir envie señal SIGUSR2 a proceso %d\n",pid);
   // capturar señal SIGUSR2
   if (signal(SIGUSR2,sig_salir) == SIG_ERR) fprintf(stderr,"main():Error capturando señal SIGUSR2\n");
   
   /* Create unique key via call to ftok() */ 
   printf("main():solicito clave ipc\n");
   key = obtener_clave(IPC_PATH, IPC_MKEY); 

   /* Open the queue -create if necessary */ 
   int modo = SHM_R;
   printf("main():obtengo memoria compartida\n");
   if ((idmem = obtener_memoria(key,modo)) != -1) { 
      printf("main():atachar memoria local a memoria compartida %d\n",idmem);
      if ((pventas = (double *) conectar_memoria(idmem,modo)) != (void *) -1) {
         
         // inicializacion de semaforos
         printf("main():solicito claves ipc para semaforos\n");
         obtener_claves(sem_paths,sem_chars,sem_keys,3);
         printf("main():obtengo semaforos\n");
         for(i=0;i<3;i++)
            if ( (sem_ids[i] = obtener_semaforo(sem_keys[i],sem_modo)) == -1) 
               perror(sem_error1[i]);
         
         char archivo[30];
         snprintf(archivo,30,"%d.ventas",pid);
         FILE *fpo=fopen(archivo,"a");
         while(!salir) {
            semWait(sem_ids[SEM_n]);  // semWait(n)
            semWait(sem_ids[SEM_s]);  // semWait(s)
               // seccion critica
               printf("main():leo memoria compartida\n");
               psucvta = (struct sucvta *) (pventas + sizeof(double)*10);
               mostrar_sucventas(fpo,psucvta);
               mostrar_ventas(pventas);
               mostrar_sucventas(stdout,psucvta);
               printf("main():pongo a cero memoria consumida\n");
               memset(psucvta,0,sizeof(struct sucvta)*10);
               // fin seccion critica
            semSignal(sem_ids[SEM_s]);  // semSignal(s)
            semSignal(sem_ids[SEM_e]);  // semSignal(e)
         }
         fclose(fpo);
         printf("main():Desconecto memoria compartida\n");
         desconectar_memoria(pventas);
      } else {
         printf("main():Error atachando memoria compartida\n");
         ret=2;
      }
   } else {
      fprintf(stderr,"main():Error obteniendo memoria compartida\n"); 
      ret=1;
   } 
   printf("main():fin proceso consumidor! retorno=%d\n",ret);
   return ret;
} 

static void sig_salir(int signo) {
   salir=1;
   printf("main():recibi señal de salida\n");
}

