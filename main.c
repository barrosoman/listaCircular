#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOS 40

typedef struct no {
    char pal[20];
    int urg;
    struct no *prox;
} Elem;

Elem *readPointer;
Elem *writePointer;

int readFile(Elem **lista);
void initLista(Elem **lista);
void insereListaCircular(Elem **lista);
void debugPrint();
void printLista(Elem **lista);


int main() {
    Elem *lista = NULL;
    initLista(&lista);
    writePointer = readPointer = lista;

    readFile(&lista);


    /* debugPrint(); */

    /* for( int i=0; i<10; i++) { */
    /*     insereListaCircular(&lista, i); */
    /* } */


}

int readFile(Elem **lista) {
    char line[40], pal[20];
    int cmd, urg;

    FILE *fp = fopen( "pacotes.dat", "r" );
    FILE *lidos = fopen( "lidos.dat", "w" );

    while ( fgets(line, sizeof(line), fp) ) {
        sscanf(line, "%d %s %d", &cmd, pal, &urg);
        if ( strcmp(pal, "NULL") == 0 ) {
            return 0;
        }

        if (cmd == 0) {
            strcpy(writePointer->pal, pal);
            writePointer->urg = urg;

            writePointer = writePointer->prox;
        } else if (cmd == 1) {
            fputs(pal, lidos);

            readPointer = readPointer->prox;
        } else {
            printf("Comando errado");
            return 1;
        }
        printLista(lista);
    }
    fclose(fp);
    return 0;
}

void insereListaCircular(Elem **lista){
  Elem *no;

  no = (Elem*) malloc(sizeof(Elem));

  if(*lista == NULL){
    no->prox = no;
  }else{
    no->prox = (*lista)->prox;
    (*lista)->prox = no;
  }

  *lista = no;
}

void initLista(Elem **lista) {
    for( int i=0; i<40; i++) {
        insereListaCircular(lista);
    }
}


void debugPrint() {
    Elem *no = readPointer;
    for (int i=0; i<10; i++) {
        printf("%s %d", no->pal, no->urg);
        no = no->prox;
    }
}

void printLista(Elem **lista) {
    Elem *aux = *lista;
    do {
        printf("%s %d\n", aux->pal, aux->urg);
        aux = aux->prox;
    } while( aux != *lista ) ;
    putchar('\n');
}
