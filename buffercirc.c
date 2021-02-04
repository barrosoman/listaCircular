#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEMS 40

typedef char char32_t[32];

enum comandos{
    INSERT,
    DELETE,
};

/* Estrutura do elemento da lista */
typedef struct no {
    char32_t pal;
    int urg;
    struct no *prox;
} Elem;

/* Estrutura das informações obtidas de pacotes.dat */
typedef struct info {
    char32_t pal;
    int cmd,
        urg;
} Info_t;

/* Ponteiros para inserção e remoção de dados na lista */
Elem *readPointer,
     *writePointer;

/* Funções de obtenção dos dados de um arquivo */
int readFile(Elem **lista,FILE **arquivos);
Info_t getLineInfo(char *line);

/* Função inicialização da lista */
void initLista(Elem **lista);
void initElem(Elem **lista);

/* Funções de manipulação dos dados na lista */
void writeToElem(Info_t info);
void readJump(int urg);
void readElem(Info_t info, FILE *lidosF);
void printElemFile(FILE *lidosF);

/* Função de limpeza do programa (garbage collector) */
void exitOperations(Elem **lista, FILE **arquivos);

int main() {
    Elem *lista = NULL;
    int error_status;

    /* Abre os arquivos necessários */
    FILE *arquivos[] = {
        fopen( "pacotes.dat", "r" ),
        fopen( "lidos.dat", "w" ),
    };

    if (arquivos[0] == NULL) {
        printf("Arquivo pacotes.dat não existe.\n");
        return 1;
    }

    /* Inicia a lista */
    initLista(&lista);

    /* Aponta os pointeiros de inserção e remoção para o
       novo começo da lista */
    writePointer = readPointer = lista;


    /* Faz toda a manipulação do arquivo e recebe 1 se
       ocorreu algum problema */
    error_status = readFile(&lista, arquivos);

    /* Após toda manipulação ocorrer, libera toda memórias
       alocadas */
    exitOperations(&lista, arquivos);

    return error_status;
}


/* Função que manda ler cada linha do arquivo e determina
   o que fazer a partir do primeiro número da linha */
int readFile(Elem **lista, FILE **arquivos) {
    Info_t lineInfo;

    char32_t line;
    FILE *pacotesF = arquivos[0];
    FILE *lidosF = arquivos[1];

    while (1) {
        /* Lê uma linha e analisa os dados dela */
        fgets(line, sizeof(line), pacotesF);
        lineInfo = getLineInfo(line);

        /* Se chegou na linha com a palavra NULL, a função
           finaliza sem erro */
        if (strcmp(lineInfo.pal, "NULL") == 0 ) {
            return 0;
        }

        switch (lineInfo.cmd) {
            /* Quando o número é 0, inserir dados */
            case INSERT:
                writeToElem(lineInfo);
                break;
            /* Quando o número é 1, "remover" dados */
            case DELETE:
                readElem(lineInfo, lidosF);
                break;
            /* Caso contrário, o arquivo está com informações
               errôneas */
            default:
            printf("Comando errado\n");
            return 1;
        }
    }
}



/* Faz um loop para criar 40 elementos de uma lista */
void initLista(Elem **lista) {
    for( int i=0; i<MAX_ELEMS; i++) {
        initElem(lista);
    }
}


/* Aloca memória e arruma para que o último da lista aponte
   para o primeiro */
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


/* Le uma linha e preenche as informações em uma struct,
   retornando essa struct */
Info_t getLineInfo(char *line) {
    Info_t lineInfo;
    sscanf(line, "%d %s %d",
            &lineInfo.cmd,
            lineInfo.pal,
            &lineInfo.urg);

    return lineInfo;
}


/* Pega as informações de uma linha de pacotes.dat e insere
   em um elemento da lista */
void writeToElem(Info_t info) {
    if (writePointer->prox == readPointer) {
        return;
    }
    strcpy(writePointer->pal, info.pal);
    writePointer->urg = info.urg;

    writePointer = writePointer->prox;
}


/* Faz o ato de verificar se a palavra é "PRTY", se sim
   pula "urg" vezes. Após pular ou não,printa a palavra
   do elemento que readPointer aponta para o arquivo lidos.dat */
void readElem(Info_t info, FILE *lidosF) {
    /* if (readPointer->prox != writePointer) { */
    if (readPointer != writePointer) {
        if (strcmp(readPointer->pal, "PRTY") == 0) {
            printElemFile(lidosF);
            readJump(readPointer->urg);
            return;
        }
        printElemFile(lidosF);
        readJump(1);
    }
}


/* Faz o ato de imprimir a palavra no arquivo lidos.dat */
void printElemFile(FILE *lidosF) {
    fprintf(lidosF, "%s\n", readPointer->pal);
}


/* Faz o ato do readPointer pular de elemento urg vezes */
void readJump(int urg) {
    for (int i=0; i<urg; i++) {
        /* if (readPointer->prox != writePointer) { */
        if (readPointer != writePointer) {
            readPointer = readPointer->prox;
        }
    }
}


/* Função que fecha os arquivos e da free() em toda a lista */
void exitOperations(Elem **lista, FILE **arquivos) {
    Elem *aux, *temp;
    int qtdFiles = sizeof(*arquivos) / sizeof(FILE *);

    for (int i=0; i<qtdFiles; i++) {
        fclose(arquivos[i]);
    }

    aux = *lista;
    do {
        temp = aux;
        aux = aux->prox;
        free(temp);
    } while( aux != *lista ) ;
}
