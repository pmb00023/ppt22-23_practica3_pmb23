/*******************************************************
 * Protocolos de Transporte
 * Grado en Ingenier?a Telem?tica
 * Dpto. Ingen?er?a de Telecomunicaci?n
 * Univerisdad de Ja?n
 *
 *******************************************************
 * Pr?ctica 1.
 * Fichero: cliente.c
 * Versi?n: 3.1
 * Fecha: 10/2020
 * Descripci?n:
 * 	Cliente sencillo TCP para IPv4 e IPv6
 * Autor: Juan Carlos Cuevas Mart?nez
 *
 ******************************************************
 * Alumno 1:
 * Alumno 2:
 *
 ******************************************************/
#include <stdio.h>		// Biblioteca est?ndar de entrada y salida
#include <ws2tcpip.h>	// Necesaria para las funciones IPv6
#include <conio.h>		// Biblioteca de entrada salida b?sica
#include <locale.h>		// Para establecer el idioma de la codificaci?n de texto, n?meros, etc.
#include "protocol.h"	//Declarar constantes y funciones de la pr?ctica

#pragma comment(lib, "Ws2_32.lib")//Inserta en la vinculaci?n (linking) la biblioteca Ws2_32.lib


int main(int* argc, char* argv[])
{
	SOCKET sockfd;
	struct sockaddr* server_in = NULL;
	struct sockaddr_in server_in4;
	struct sockaddr_in6 server_in6;
	int address_size = sizeof(server_in4);
	char buffer_in[1024], buffer_out[1024], input[1024];
	int recibidos = 0, enviados = 0;
	int estado;
	char option;
	int ipversion = AF_INET;//IPv4 por defecto
	char ipdest[256];
	char default_ip4[16] = "127.0.0.1";
	char default_ip6[64] = "::1";
	int cont = 0, cont2 = 0;
	char dest[32];

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	//Inicializaci?n de idioma
	setlocale(LC_ALL, "es-ES");


	//Inicializaci?n Windows sockets - SOLO WINDOWS
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0) {
		return(0);
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return(0);
	}
	//Fin: Inicializaci?n Windows sockets

	printf("**************\r\nCLIENTE TCP SENCILLO SOBRE IPv4 o IPv6\r\n*************\r\n");

	do {
		printf("CLIENTE> ?Qu? versi?n de IP desea usar? 6 para IPv6, 4 para IPv4 [por defecto] ");
		gets_s(ipdest, sizeof(ipdest));

		if (strcmp(ipdest, "6") == 0) {
			//Si se introduce 6 se emplear? IPv6
			ipversion = AF_INET6;

		}
		else { //Distinto de 6 se elige la versi?n IPv4
			ipversion = AF_INET;
		}

		sockfd = socket(ipversion, SOCK_STREAM, 0);
		if (sockfd == INVALID_SOCKET) {
			printf("CLIENTE> ERROR\r\n");
			exit(-1);
		}
		else {
			printf("CLIENTE> Introduzca la IP destino (pulsar enter para IP por defecto): ");
			gets_s(ipdest, sizeof(ipdest));

			//Direcci?n por defecto seg?n la familia
			if (strcmp(ipdest, "") == 0 && ipversion == AF_INET)
				strcpy_s(ipdest, sizeof(ipdest), default_ip4);

			if (strcmp(ipdest, "") == 0 && ipversion == AF_INET6)
				strcpy_s(ipdest, sizeof(ipdest), default_ip6);

			if (ipversion == AF_INET) {
				server_in4.sin_family = AF_INET;
				server_in4.sin_port = htons(TCP_SERVICE_PORT);
				inet_pton(ipversion, ipdest, &server_in4.sin_addr.s_addr);
				server_in = (struct sockaddr*) & server_in4;
				address_size = sizeof(server_in4);
			}

			if (ipversion == AF_INET6) {
				memset(&server_in6, 0, sizeof(server_in6));
				server_in6.sin6_family = AF_INET6;
				server_in6.sin6_port = htons(TCP_SERVICE_PORT);
				inet_pton(ipversion, ipdest, &server_in6.sin6_addr);
				server_in = (struct sockaddr*) & server_in6;
				address_size = sizeof(server_in6);
			}

			//Cada nueva conexi?n establece el estado incial en
			estado = BIENVENIDA;

			if (connect(sockfd, server_in, address_size) == 0) {
				printf("CLIENTE> CONEXION ESTABLECIDA CON %s:%d\r\n", ipdest, TCP_SERVICE_PORT);

				//Inicio de la m?quina de estados
				do {
					switch (estado) {
					case BIENVENIDA:
						printf("Bienvenido al cliente SMTP \r\n");
						break;
					case HELO:
						// establece la conexion de aplicacion 
						strcpy_s(input, sizeof(input), "");
						printf("CLIENTE> Introduzca el usuario (enter para salir): ");
						gets_s(input, sizeof(input));
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", SD, CRLF);
							estado = S_QUIT;
						}
						else {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s %s%s", HL, input, CRLF);
						}
						break;
					case MAIL:
						printf("CLIENTE> Introduzca el remitente (enter para salir): ");
						gets_s(input, sizeof(input));
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", SD, CRLF);
							estado = S_QUIT;
						}
						else
							sprintf_s(buffer_out, sizeof(buffer_out), "%s %s%s", ML, input, CRLF);
						cont++;
						break;
					case RCPT:
						printf("CLIENTE> Introduzca el destinatario (enter o QUIT para salir): ");
						gets_s(dest,sizeof(dest));
						printf(dest);
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", SD, CRLF);
							estado = S_QUIT;
						}
						else {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s %s%s", RPT, input, CRLF);
							cont2++;
						}
						break;

					
					case DATA:
						printf("CLIENTE> Introduzca el asunto del mail (enter o QUIT para salir): ");
						gets_s(input, sizeof(input));
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", SD, CRLF);
							estado = S_QUIT;
						}
						else {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s %s%s", SB, input, CRLF);
							cont2++;
						}
						
						break;
					}

					
					if (estado != BIENVENIDA) {
						enviados = send(sockfd, buffer_out, (int)strlen(buffer_out), 0);
						if (enviados == SOCKET_ERROR) {
							estado = S_QUIT;
							continue;// La sentencia continue hace que la ejecuci?n dentro de un
									 // bucle salte hasta la comprobaci?n del mismo.
						}
					}

					recibidos = recv(sockfd, buffer_in, 512, 0);
					if (recibidos <= 0) {
						DWORD error = GetLastError();
						if (recibidos < 0) {
							printf("CLIENTE> Error %d en la recepci?n de datos\r\n", error);
							estado = S_QUIT;
						}
						else {
							printf("CLIENTE> Conexi?n con el servidor cerrada\r\n");
							estado = S_QUIT;
						}
					}
					else {
						buffer_in[recibidos] = 0x00;
						printf(buffer_in);
						if (estado == BIENVENIDA) {
							if (strncmp(buffer_in, OK1, 3) == 0){
							estado++;
							}
						}
						if (estado == HELO) {
							
							if (strncmp(buffer_in, OK2, 3) == 0) {
								estado++;
								
							}
						}
						if (estado == MAIL) {
							if (cont < 2) {
								cont++;
							}
							else {
								estado++;
							}
						}
						if (estado == RCPT) {
							if (cont2 < 1) {
								cont++;
							}
							else {
								estado++;
							}

						}
						/*if (estado != HELO && strncmp(buffer_in, OK, 2) == 0){
							estado++;
						*///}
						//Si la autenticaci?n no es correcta se vuelve al estado S_USER
						/*if (estado == S_PASS && strncmp(buffer_in, OK, 2) != 0) {
							estado = HELO;
						}*/
						

					}

				} while (estado != S_QUIT);
			}
			else {
				int error_code = GetLastError();
				printf("CLIENTE> ERROR AL CONECTAR CON %s:%d\r\n", ipdest, TCP_SERVICE_PORT);
			}
			closesocket(sockfd);

		}
		printf("-----------------------\r\n\r\nCLIENTE> Volver a conectar (S/N)\r\n");
		option = _getche();

	} while (option != 'n' && option != 'N');

	return(0);
}
