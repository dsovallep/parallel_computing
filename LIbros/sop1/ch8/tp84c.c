/*
   tp84c.c IPC. Memoria compartida. Proceso que lee de memoria compartida el total de
   ventas de las sucursales+ventas recibidos por sucursal.  
   guarda las ventas obtenidas de memoria compartida en archivo secuencial, de texto, con formato.
   incorpora estructura sucvta
   los datos en memoria compartida se guardan:  <vector total de ventas><vector ventas por sucursal>
   prepara proceso consumidor para luego implementar logica de semaforos.
  
         
   para compilar:
            gcc -Wall -o tp84c tp84c.c myipc.c
            
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include "myipc.h"

// prototipo de funciones

int main(int argc, char *argv[])  {
   int ret=0;
   key_t key; 
   int idmem; 
   double *pventas;
   struct sucvta *psucvta;

   printf("main():inicio proceso consumidor!\n");
   
   /* Create unique key via call to ftok() */ 
   printf("main():solicito clave ipc\n");
   key = obtener_clave(IPC_PATH, IPC_MKEY); 

   /* Open the queue -create if necessary */ 
   int modo = SHM_R;
   printf("main():obtengo memoria compartida\n");
   if ((idmem = obtener_memoria(key,modo)) != -1) { 
      printf("main():atachar memoria local a memoria compartida %d\n",idmem);
      if ((pventas = (double *) conectar_memoria(idmem,modo)) != (void *) -1) {
         printf("main():leo memoria compartida\n");
         mostrar_ventas(pventas);
         psucvta = (struct sucvta *) (pventas + sizeof(double)*10);
         char archivo[30];
         snprintf(archivo,30,"%d.ventas",(int) getpid());
         FILE *fpo=fopen(archivo,"a");
         mostrar_sucventas(fpo,psucvta);
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

