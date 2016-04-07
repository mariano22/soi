/*
	El programa ya estaba muy bien estructurado para ser paralelizado facilmente. La funciÃ³n handle_client es la que atiende a los clientes entrantes, para que soporte conexiones mÃºltiples en paralelo, simplemente se ha de lanzar un thread con esta funciÃ³n.
	Hubo que hacerle modificaciones para que se adapte al tipo que requiere la funciÃ³n pthread_create, encargada de lanzar el thread. AsÃ­, el tipo de retorno pasÃ³ a ser void *(aunque no se utilice el retorno para comunicar nada). Y el valor que se le pasa, ahora es un puntero a void. Antes se pasaba el descriptor conn_s como un simple entero a la funciÃ³n, ahora deberÃ¡ pasarse un puntero a void, con el cuÃ¡l se pueda acceder al descriptor. diff
	Antes id, la variable que contaba la cantidad de clientes era una variable global. Se puede seguir manteniendo este diseÃ±o, pero habrÃ¡ que controlar la zona crÃ­tica generada por memoria compartida por medio de semÃ¡foros. Pero como simplemente llevaba la cuenta de los clientes que se iban conectando (y no se disminuÃ­a al desconectarse), hemos decidido pasarle a la funciÃ³n un puntero a una estructura que contiene conn_s (el decriptor necesario para enviar y recibir, que se pasaba en la versiÃ³n anterior) y id (el nÃºmero que identifica al cliente que maneja la funciÃ³n, mientras que la variable que lleva la cuenta irÃ¡ aumentando en el main en el bucle principal).
	AdemÃ¡s, ahora muestra un mensaje anunciando que un cliente se ha desconectado.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>       
#include <sys/types.h>       
#include <arpa/inet.h>      
#include <unistd.h>        
#include <string.h>        
#include <pthread.h>  

#define BUFF_SIZE 1024



struct par {
	int conn_s,id;
};

void *handle_client(void *arg)
{
	int conn_s,id;
	conn_s = ((struct par*)arg) -> conn_s;
	id = ((struct par*)arg) -> id;
	free((struct par*)arg);
  char buffer[BUFF_SIZE],buffer2[BUFF_SIZE];
  int res;
  fprintf(stderr,"New client %d connected\n",id);
  while(1) {
    res=read(conn_s,buffer,BUFF_SIZE);
    if (res<=0)
    {
	  fprintf(stderr,"Client %d disconnected\n",id);
      close(conn_s);
      break;
    }
    buffer[res]='\0';
    sprintf(buffer2,"Response to client %d: %s",id,buffer);
    write(conn_s,buffer2,strlen(buffer2));
  }
	return NULL;
}

int main()
{

  int list_s,conn_s=-1,res;
  struct sockaddr_in servaddr;
  char buffer[BUFF_SIZE],buffer2[BUFF_SIZE];
  if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
    return -1;
  }
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port        = htons(8000);

  if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
    fprintf(stderr, "ECHOSERV: Error calling bind()\n");
    return -1; 
  }

  if ( listen(list_s, 10) < 0 ) {
    fprintf(stderr, "ECHOSERV: Error calling listen()\n");
    return -1;                          
  }

	/* Se utiliza una variable tmp_thread como pthread auxiliar para el buen funcionamiento de pthread_create, aunque los threds futuros sobreescribiran su valor, por lo cual no servirÃ¡ para gestionar threads mÃ¡s antiguos que el inmediato anterior. Pero como no se vuelve a utilizar el identificador de thread, mas que para lanzarlo, es innecesario crear estructuras para que persistan todos. */
	pthread_t tmp_thread;
	int id;
	struct par *arg;

	// Bucle principal: se encarga de recibir y gestionar nuevas conexiones.
  for (id=0;;id++) {
    if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
      fprintf(stderr, "ECHOSERV: Error calling accept()\n");
      return -1;
    }
    arg = (struct par*) malloc(sizeof(struct par));
    arg->conn_s = conn_s;
    arg->id = id;
    pthread_create(&tmp_thread,0,handle_client,arg);
  }
/*
	No hacemos join, ya que el bucle se repite eternamente y no hay necesidad de esperar. En cambio, si es necesario estar activo para recibir nuevas conexiones.
*/
  return 0;
}
