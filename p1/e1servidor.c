#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

int main(){

    /*Identificador del socket y buffer de datos*/
    int socket_servidor;
    struct sockaddr_in Servidor;
	
    /*Info. del servidor*/
    struct sockaddr_in Cliente;
	socklen_t long_cliente;

    /*Se abre el socket servidor*/
    socket_servidor = socket (AF_INET, SOCK_DGRAM, 0);
	if (socket_servidor == -1){
		printf ("No se puede abrir socket servidor\n");
		exit (-1);	
	}

	/*Variable para la hora y el tiempo*/
	/*time_t tiempo;
	struct tm *stTm;

	tiempo = time(NULL);
	setlocale(LC_ALL, "");
	stTm = localtime(&tiempo);
*/
	/*Info necesaria para la funcion bind()*/
    Servidor.sin_family = AF_INET;
	Servidor.sin_port = htons(2000);
	Servidor.sin_addr.s_addr = htonl(INADDR_ANY);

    /*Asociamos el puerto con la funcion bind*/
    if(bind(socket_servidor, (struct sockaddr *)&Servidor, sizeof(Servidor)) == -1){
		close (socket_servidor);
		exit (-1);
	}

	/*Del cliente solo necesitamos el tamano de su estructura, el 
	resto de informacion (familia, puerto, ip),nos la proporcionaa 
	el propio metodo recvfrom, cuando recibamos la llamada de un cliente.*/
	long_cliente = sizeof(Cliente);


	while(1){
		char cadena[80];
		int recibido = recvfrom(socket_servidor, (char*) &cadena, sizeof(cadena), 
						0, (struct sockaddr *)&Cliente, &long_cliente);

		if(recibido > 0){
			printf("I have received it. \n");

			time_t tiempo;
			struct tm *stTm;

			tiempo = time(NULL);
			setlocale(LC_ALL, "");
			stTm = localtime(&tiempo);

			if(strcmp(cadena, "DAY") == 0){
				strftime(cadena, 80, "%a, %d de %B \n",stTm);
				printf("Cadena: %s", cadena);
				/*char enviado = sendto(socket_servidor, (char *)&cadena, sizeof(cadena), 0,
								(struct sockaddr *)&Cliente, long_cliente);*/
			}
			else if(strcmp(cadena, "TIME") == 0){
				strftime(cadena, 80, "%H:%M:%S \n",stTm);
				printf("Cadena: %s", cadena);
				/*char enviado = sendto(socket_servidor, (char *)&cadena, sizeof(cadena), 0,
								(struct sockaddr *)&Cliente, long_cliente);*/
			}
			else if(strcmp(cadena, "DAYTIME") == 0){
				strftime(cadena, 80, "%a, %d de %B; %H:%M:%S \n",stTm);
				printf("Cadena: %s", cadena);
				/*char enviado = sendto(socket_servidor, (char *)&cadena, sizeof(cadena), 0,
								(struct sockaddr *)&Cliente, long_cliente);*/
			}
			char enviado = sendto(socket_servidor, (char *)&cadena, sizeof(cadena), 0,
								(struct sockaddr *)&Cliente, long_cliente);
		}
	}
	close(socket_servidor);
	return(0);
    
}