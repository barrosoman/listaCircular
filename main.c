#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEMS 40

enum comandos{
    INSERT,
    DELETE,
};

typedef struct no {
    char pal[20];
    int urg;
    struct no *prox;
} Elem;

typedef struct info {
    char pal[32];
    int cmd,
        urg;
} Info_t;

Elem *readPointer,
     *writePointer;

Info_t getLineInfo(char *line);
void readFile(Elem **lista,FILE **arquivos);
void initLista(Elem **lista);
void initElem(Elem **lista);
void readJump(int urg);
void writeToElem(Info_t info);
void readElem(Info_t info, FILE *lidosF);
void exitOperations(Elem **lista, FILE **arquivos);
void printElemFile(FILE *lidosF);

int main() {
    Elem *lista = NULL;
    initLista(&lista);
    writePointer = readPointer = lista;

    FILE *arquivos[] = {
        fopen( "pacotes.dat", "r" ),
        fopen( "lidos.dat", "w" ),
    };

    readFile(&lista, arquivos);

    exitOperations(&lista, arquivos);
}

void readFile(Elem **lista, FILE **arquivos) {
    Info_t lineInfo;

    char line[32];
    FILE *pacotesF = arquivos[0];
    FILE *lidosF = arquivos[1];


    while (1) {
        fgets(line, sizeof(line), pacotesF);
        lineInfo = getLineInfo(line);

        if (strcmp(lineInfo.pal, "NULL") == 0 ) {
            exit(0);
        }

        switch (lineInfo.cmd) {
            case INSERT:
                writeToElem(lineInfo);
                break;
            case DELETE:
                readElem(lineInfo, lidosF);
                break;
            default:
            printf("Comando errado\n");
            exit(1);
        }
    }
}

void initElem(Elem **lista){
  Elem *elem;

  elem = (Elem*) malloc(sizeof(Elem));

  if(*lista == NULL){
    elem->prox = elem;
  }else{
    elem->prox = (*lista)->prox;
    (*lista)->prox = elem;
  }

  *lista = elem;
}

void initLista(Elem **lista) {
    for( int i=0; i<MAX_ELEMS; i++) {
        initElem(lista);
    }
}

Info_t getLineInfo(char *line) {
    Info_t lineInfo;
    sscanf(line, "%d %s %d",
            &lineInfo.cmd,
            lineInfo.pal,
            &lineInfo.urg);

    return lineInfo;
}

void printElemFile(FILE *lidosF) {
    fprintf(lidosF, "%s\n", readPointer->pal);
}

void writeToElem(Info_t info) {
    strcpy(writePointer->pal, info.pal);
    writePointer->urg = info.urg;

    writePointer = writePointer->prox;
}

void readJump(int urg) {
    for (int i=0; i<urg; i++) {
        if (readPointer != writePointer) {
            readPointer = readPointer->prox;
        }
    }
}

void readElem(Info_t info, FILE *lidosF) {
    if (readPointer != writePointer) {
        if (strcmp(info.pal, "PRTY") == 0) {
            readJump(info.urg);
        }
        printElemFile(lidosF);
        readJump(1);
    }
}

void exitOperations(Elem **lista, FILE **arquivos) {
    Elem *aux, *temp;

    for (int i=0; i<sizeof(arquivos); i++) {
        fclose(arquivos[i]);
    }

    aux = *lista;
    do {
        temp = aux;
        aux = aux->prox;
        free(temp);
    } while( aux != *lista ) ;
}
