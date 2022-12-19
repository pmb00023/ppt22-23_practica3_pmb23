#ifndef protocolostpte_practicas_headerfile
#define protocolostpte_practicas_headerfile
#endif

// COMANDOS DE APLICACION
#define SC "USER"  // SOLICITUD DE CONEXION USER usuario 
#define PW "PASS"  // Password del usuario  PASS password

#define SD  "QUIT"  // Finalizacion de la conexion de aplicacion
#define ECHO "ECHO" // Definicion del comando "ECHO" para el servicio de eco
#define HL "HELO"
#define ML "MAIL FROM:"
// RESPUESTAS A COMANDOS DE APLICACION
#define OK  "OK"
#define ER  "ER"
#define OK1 "220"
#define OK2 "250"

//FIN DE RESPUESTA
#define CRLF "\r\n"

//ESTADOS
#define BIENVENIDA 0
#define HELO 1
#define MAIL 2
#define S_DATA 3
#define S_QUIT 4


//PUERTO DEL SERVICIO
#define TCP_SERVICE_PORT	25

// NOMBRE Y PASSWORD AUTORIZADOS
#define USER		"alumno"
#define PASSWORD	"123456"