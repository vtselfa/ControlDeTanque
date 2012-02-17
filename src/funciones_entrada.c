#include "funciones_entrada.h"

#define BUF_INT_SIZE 16 /*Condiciona el nombre màxim de xifres que tindrà l'enter llegit per la stdin*/

int leerEntero (int *i){
    char buf[BUF_INT_SIZE];
    int longitud;
    fgets(buf, sizeof (buf), stdin); /*Ignore conscientment i sense risc el return*/
		
    if (ferror(stdin)){
        clearerr(stdin);
        return -1; //Error en entrada de datos
    }

   	if (feof(stdin)){
    	clearerr(stdin);
        return -2; //Recibido un EOF
    }

    longitud = strlen (buf);
    if (buf[longitud-1] == '\n') {
    	if (longitud==1){
			return 0; //No ha introducido nada.
        }
		else{
        	char *end;
            buf[longitud-1] = '\0';
            errno = 0;
            *i = strtol (buf, &end, 10);
            if (!errno && !*end){
            	return longitud-1; //Tot ok, retornem el nombre de xifres del nombre
            }
            else{
            	if (errno==ERANGE)
					return -4; //El entero introducido es demasiado grande (o pequeño) para ser representable.
                else
                    return -5; //Carácteres inválidos. Solo se admiten enteros en formato decimal
            }
        }
	}else{
    	scanf ("%*[^\n]"); //Netegem stdin de basura
        getchar(); //Netegem stdin de basura
        return -3; //No puede introducir más de BUF_INT_SIZE-2 caracteres
    }
}


int leerCadena(char *desti, int tamany){
    int longitud;
    fgets(desti, tamany, stdin);
    if (ferror(stdin)){
        clearerr(stdin);
        return -1; //error en la entrada de datos
    }
    if (feof(stdin)){
       	clearerr(stdin);
        return -2; //Recibido EOF
    }

    longitud = strlen (desti);

	if (desti[longitud-1] == '\n') {
            desti[longitud-1] = '\0';
            return longitud-1; //Tot OK
    }
    else {
    	scanf ("%*[^\n]");
        getchar();
        return -3; //La cadena és massa llarga, ha sigut truncada truncat
    }
}

void pausa(){
    char cad[2];
    printf("Presione ENTER para continuar...");
    while (1) {
        fgets(cad, 2, stdin);
        if (ferror(stdin)){
            printf("Se ha producido un error en la entrada de datos. Presione ENTER para continuar...");
            clearerr(stdin);
            continue;
        }
        if (feof(stdin)){
            printf("Se recibido e ignorado un EOF. Presione ENTER para continuar...");
            clearerr(stdin);
            continue;
        }
        if (cad[0] == '\n'){
            printf("\n");
            break;
        }
        else {
            scanf ("%*[^\n]");
            getchar();
            printf("¿Que parte no ha entendido? Presione ENTER para continuar...");
        }
    }
}
