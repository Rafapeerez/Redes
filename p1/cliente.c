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
#include <string.h>


int main(){
    /*Identificador del socket y buffer de datos*/
    int socket_cliente;

    /*Info. del servidor*/
    struct sockaddr_in Servidor;
    socklen_t long_servidor;

    /*Se abre el socket cliente*/
    socket_cliente = socket(AF_INET, SOCK_DGRAM, 0); //Se abre el socket
    if(socket_cliente == -1){
        printf("No se puede abrir el socket cliente. \n");
        exit(-1);
    }

    fd_set lectura; //Conjunto de sockets comprobados para leer
    int salida = 0;

    /*Inicio estructura timeval*/
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    /*Variables para fecha y hora*/
 /*   time_t tiempo;
    struct tm *stTm;

    tiempo = time(NULL);
    setlocale(LC_ALL, "");
    stTm = localtime(&tiempo);
*/
    /*Info necesaria con los datos del servidor para poder solicitarle servicio*/
    Servidor.sin_family = AF_INET;
	Servidor.sin_port = htons(2000);
	Servidor.sin_addr.s_addr =  inet_addr("127.0.0.1");
 	long_servidor = sizeof(Servidor);
    
    /*Inicio de conjuntos fd_set*/
    FD_ZERO(&lectura);
    FD_SET(0, &lectura);
    FD_SET(socket_cliente, &lectura);

    char cadena1[256];
    int datos;
    printf("Introduce una de las siguientes opciones dependiendo de lo que desee: \n");
    printf("1. DAY -> Recibes el dia actual. \n");
    printf("2. TIME -> Recibes las hora actual. \n");
    printf("3. DAYTIME -> Recibes ambas cosas. \n");
    scanf("%d", &datos);

    switch (datos){
        case 1:
            strcpy(cadena1, "DAY");
        //break;
        case 2:
            strcpy(cadena1, "TIME");
        break;
        case 3:
            strcpy(cadena1, "DAYTIME");
        break;
        default:
            printf("Parametro seleccionado no disponible. Try again. \n");
        break;
    }
    

    for(int i = 0; (i < 3); i++){
        /*Enviamos al servidor*/
        int enviado = sendto(socket_cliente, (char *)&cadena1, sizeof(cadena1), 0,
                            (struct sockaddr *)&Servidor, long_servidor);
        if(enviado < 0){
            printf("Error al enviar los datos. \n");
        }
        else{
            salida = select(socket_cliente + 1, &lectura, NULL, NULL, &timeout);
            int recibido = recvfrom(socket_cliente, (char*)&cadena1, sizeof(cadena1),
                                    0, (struct sockaddr *)&Servidor, &long_servidor);
            
            if(recibido > 0){
                printf("Leido: %s \n", cadena1);
                i = 4;
            }

            else if (salida == -1){
		        printf("Error en la funcion select(). \n");
	        }
            else if(salida == 0){
                printf("Se ha acabado el tiempo de espera. \n");
            }   
        }
    }    
    	
	close(socket_cliente);
	return 0;

}