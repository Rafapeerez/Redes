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
	char cad[100];

    /*Info. del servidor*/
    struct sockaddr_in Cliente;
	socklen_t long_cliente;

    /*Se abre el socket servidor*/
    socket_servidor = socket (AF_INET, SOCK_DGRAM, 0); //abrimos el socket servidor
	if (socket_servidor == -1){
		printf ("No se puede abrir socket servidor\n");
		exit (-1);	
	}

	/*Variable para la hora y el tiempo*/
	char cadena[80];
	time_t tiempo;
	struct tm *stTm;

	tiempo = time(NULL);
	setlocale(LC_ALL, "");
	stTm = localtime(&tiempo);

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
		int recibido = recvfrom(socket_servidor, (char*) cad, sizeof(cad), 
						0, (struct sockaddr *)&Cliente, &long_cliente);

		if(cad == 'DAY' || cad == 'TIME' || cad == 'DAYTIME' ){
			printf("I have received it. \n");

			int enviado = sendto(socket_servidor, (char *)&cad, sizeof(cad), 0,
								(struct sockaddr *)&Cliente, long_cliente);
		}
	}
	close(socket_servidor);
	return(0);
    
}