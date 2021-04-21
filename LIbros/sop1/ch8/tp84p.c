/*
   tp84p.c IPC. Memoria compartida. Semaforos. Proceso que guarda en memoria compartida el total de
   ventas de las sucursales+total de ventas recibidos por sucursal. Queda bloqueado a la espera de la señal de salida.
   genera ventas de sucursal y las acumula en total de ventas de sucursal
   muestra los valores generados
   incorpora estructura sucvta
   genera datos cada 10 segundos aprox.
   los datos en memoria compartida se guardan:  <vector total de ventas><vector ventas por sucursal>
   prepara proceso productor para luego implementar logica de semaforos.
   
         
   para compilar:
            gcc -Wall -o tp84p tp84p.c myipc.c
            
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include "myipc.h"

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
   
   printf("main():inicio proceso productor!\n");
   printf("main():para salir envie señal SIGUSR2 a proceso %d\n",pid);
   
   // capturar señal SIGUSR2
   if (signal(SIGUSR2,sig_salir) == SIG_ERR) fprintf(stderr,"main():Error capturando señal SIGUSR2\n");
   
   /* Create unique key via call to ftok() */ 
   printf("main():solicito clave ipc\n");
   key = obtener_clave(IPC_PATH, IPC_MKEY); 

   /* Open the queue -create if necessary */ 
   int modo = SHM_R|SHM_W;
   int tamanio = (sizeof(double)*10)+(sizeof(struct sucvta)*10);
   printf("main():creo memoria compartida de %d bytes\n",tamanio);
   if ((idmem = crear_memoria(key,tamanio,modo)) != -1) { 
      printf("main():atachar memoria local a memoria compartida %d\n",idmem);
      if ((pventas = (double *) conectar_memoria(idmem,modo)) != (void *) -1) {
         printf("main():Inicializo memoria compartida a cero\n");
         memset(pventas,0,tamanio);
         psucvta = (struct sucvta *) (pventas + sizeof(double)*10);
         printf("main():genero total de ventas de sucursales en memoria compartida\n");
         // inicializo totales de ventas
         init(pventas);
         int i;
         struct sucvta *p;
         printf("main():Bloqueo proceso hasta recibir señal de salida\n");
         while(!salir) { 
            p = psucvta;
            for(i=0;i<10;i++,p++) { 
               generar_venta(p);
               *(pventas+p->sucursal) += p->monto; // acumulo en totales
            }
            mostrar_ventas(pventas);
            mostrar_sucventas(stdout,psucvta);
            sleep(10);
         }
         printf("main():Desconecto memoria compartida\n");
         desconectar_memoria(pventas);
         printf("main():Destruyo memoria compartida\n");
         borrar_memoria(idmem);
      } else {
         printf("main():Error atachando memoria compartida\n");
         ret=2;
      }
   } else {
      fprintf(stderr,"main():Error creando memoria compartida\n"); 
      ret=1;
   } 
   printf("main():fin proceso productor! retorno=%d\n",ret);
   return ret;
} 

static void sig_salir(int signo) {
   salir=1;
   printf("main():recibi señal de salida\n");
}

