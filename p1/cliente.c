#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <locale.h>


int main(){
    /*Identificador del socket y buffer de datos*/
    int socket_cliente;
    char datos[100];

    /*Info. del servidor*/
    struct sockaddr_in Servidor;
    socklen_t long_servidor;

    /*Se abre el socket cliente*/
    socket_cliente = socket(AF_INET, SOCK_DGRAM, 0); //Se abre el socket
    if(socket_cliente == -1){
        printf("No se puede abrir el socket cliente. \n");
        exit(-1);
    }

    /*Estructura timeval*/
    struct timeval timeout;
    fd_set data; //Conjunro de sockets comprobados para leer
    int salida = 0;
    //char cadena[60];

    /*Inicio estructura */
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    /*Variables para fecha y hora*/
    time_t tiempo;
    struct tm *stTm;
    char cadena[80];

    tiempo = time(NULL);
    setlocale(LC_ALL, "");
    stTm = localtime(&tiempo);

    /*Info necesaria con los datos del servidor para poder solicitarle servicio*/
    Servidor.sin_family = AF_INET;
	Servidor.sin_port = htons(2000);
	Servidor.sin_addr.s_addr =  inet_addr("127.0.0.1");
 	long_servidor = sizeof(Servidor);
    
    /*Inicio de conjuntos fd_set*/
    FD_ZERO(&data);
    FD_SET(0, &data);

    printf("Introduce una de las siguientes opciones dependiendo de lo que desee: \n");
    printf("DAY -> Recibes el dia actual. \n");
    printf("TIME -> Recibes las hora actual. \n");
    printf("DAYTIME -> Recibes ambas cosas. \n");
    scanf("%s", datos);
    

    for(int i = 0; ((i < 3) && (salida < 1)); i++){
        /*Enviamos al servidor*/
        int enviado = sendto(socket_cliente, (char *)&datos, sizeof(datos), 0,
                            (struct sockaddr *)&Servidor, long_servidor);
        if(enviado < 0){
            printf("Error al enviar los datos. \n");
        }

        salida = select(socket_cliente + 1, &data, NULL, NULL, &timeout);
        if(salida <= 0){
            break;
        }
    }    
   	if (salida == -1){
		printf("Error en la funcion select(). \n");
	}
    else if(salida == 0){
        printf("Se ha acabado el tiempo de espera. \n");
    }   
    
    else{
	    /*Esperamos la respuesta del Servidor*/ 
	    char recibido = recvfrom (socket_cliente, (char *)&datos, sizeof(datos), 0,
	                                    (struct sockaddr *) &Servidor, &long_servidor);
    
        if(recibido == 'DAY'){
		    strftime(cadena, 80, "%A, %d de %B de %y", stTm);
		    printf("Hoy es %s", cadena);
	    }
	    if(recibido == 'TIME'){
		    strftime(cadena, 80, "%H:%M:%S", stTm);
		    printf("Son las %s", cadena);
        }
	    if(recibido == 'DAYTIME'){
		    strftime(cadena, 80, "%A, %d de %B de %y; %H:%M:%S", stTm);
		    printf("%s", cadena);
	    }
        else{
            printf("Error al leer el servidor. \n");
        }
    }
			
	close(socket_cliente);
	return 0;

}