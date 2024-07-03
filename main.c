#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

int menu(sqlite3 * db){
    int opcion=2;
    printf("ingrese su opcion: \n");
    while(opcion!=0){
        printf(" 1- Registrar un valor \n 2- Consultar la tabla\n 3- Eliminar un registro de la tabla\n 4- Actualizar un registro de la tabla\n 0- Salir\n");
        scanf("%i",&opcion);
        if((-1<opcion)&&(opcion<5)){
                switch (opcion){
                    case 0:
                        sqlite3_close(db);
                        exit(1);
                    break;
                    case 1:
                        insertConstruct(db);
                    break;
                    case 2:
                        selectFrom(db);
                        printf("\n");
                    break;
                    case 3:
                        deleteFrom(db);
                    break;
                    case 4:
                        updateFrom(db);
                    break;
                }
        }
        else printf("opcion invalida, ingrese otro numero\n");
    }
}

int gestionaError( sqlite3 * db ) {
    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    return sqlite3_errcode(db);
}

int callback(void * ptr, int numeroColumnas, char ** valorCelda,char ** nombreColumna) { //funcion usada para manejar la devolucion de sentencias SQL
   int ix;
    for (ix = 0; ix < numeroColumnas; ix++)
    printf("%15s |", valorCelda[ix]);
    printf("\n");
    return 0;
}

void insertConstruct(sqlite3 * db){    //Funcion para construir la sentencia insert
    char charInsert[100];
    char nombre[30],edad[5],dni[12];
    printf("ingrese el nombre\n");
    scanf("%s",nombre);
    printf("ingrese la edad\n");
    scanf("%s",edad);
    printf("ingrese el dni\n");
    scanf("%s",dni);
    sprintf(charInsert,"insert into personas(nombre,edad,dni) values ( '%s', %s, %s);",nombre,edad,dni);
    sqlite3_exec(db,charInsert,NULL,NULL,NULL);
}

void selectFrom(sqlite3 * db){
    sqlite3_exec(db,"select rowid, nombre, edad, dni from personas;",callback,NULL,NULL);
}

void updateFrom(sqlite3 * db){
    int id;
    char charUpdate[100];
    char nombre[30],edad[5],dni[12];
    printf("ingrese el id de la fila a actualizar\n");
    scanf("%i",&id);
    sprintf(charUpdate,"select * from personas where rowid=%i",id);
    sqlite3_exec(db,charUpdate,callback,NULL,NULL);
    printf("ingrese el nuevo nombre\n");
    scanf("%s",nombre);
    printf("ingrese la nueva edad\n");
    scanf("%s",edad);
    printf("ingrese el nuevo dni\n");
    scanf("%s",dni);
    sprintf(charUpdate,"update personas set nombre='%s', edad=%s, dni=%s where rowid=%i",nombre,edad,dni,id);
    sqlite3_exec(db,charUpdate,callback,NULL,NULL);
}

void deleteFrom(sqlite3 *db){
    int id,aux;
    char charDelete[40];
    printf("ingrese el id de la fila a eliminar\n");
    scanf("%i",&id);
    sprintf(charDelete,"select * from personas where rowid=%i",id);
    sqlite3_exec(db,charDelete,callback,NULL,NULL);
    printf("\n Esta seguro que desea eliminar este registro?\n Ingrese 1 para si, ingrese 2 para no\n");
    scanf("%i",&aux);
    if(aux==1){
        sprintf(charDelete,"delete from personas where rowid=%i",id);
        sqlite3_exec(db,charDelete,callback,NULL,NULL);
        printf("La fila ha sido eliminada con exito\n");
    }
}

int main() {
    char *charPtr;
    sqlite3 * db = NULL;
    int rc;
    const char * filenameDatabase = "baseCpp.db";
    rc = sqlite3_open(filenameDatabase,&db);
    if(rc){
        gestionaError(db);
        exit(1);
    }
    menu(db);
    return 0;
}

