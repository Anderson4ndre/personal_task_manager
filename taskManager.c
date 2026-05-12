#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING_SIZE 128
#define CATEGORY_LIMIT 5

struct taskFilePage{
    FILE *taskFile;
    int page;
    char pageName[25];
    //Maybe a page path?
};


void printPage(struct taskFilePage *, char*); //OK
void clearScreen(void); //OK
//int linesNum(FILE*);
void startPage(FILE *, char *); //OK
void accessPage(struct taskFilePage *, FILE *, char *); //OK
void taskFileMgr(struct taskFilePage *, FILE *, char *);
void addTask(FILE *, char *); //OK
void deleteTask(struct taskFilePage *, FILE *, int, char *); //OK
void addCategorie(FILE *, char *, char *);
void deleteCategorie(FILE **, int, char *);
void renameCategorie(FILE **, int, char *);
int my_strcpy(char *, char *); //OK
int totalLines(FILE *, char *); //OK

int main(void){
    int choice = -1;
    int page;
    char pageName[25]; //Para não guardar no buffer e perder
    struct taskFilePage currentPage;
    FILE *index = fopen("./categorias/index.txt","r+");
    char buffer[STRING_SIZE];
    do{
        clearScreen();
        printf("\n");
        startPage(index, buffer); 
        scanf("%d", &choice); 
        fgetc(stdin); //Retira o \n 
        switch(choice){ 
            case 0: //OK
                clearScreen();
                exit(0);
            case 1: //OK
                fprintf(stdout, "Digite a página: ");
                scanf("%d", &currentPage.page);
                fgetc(stdin);
                if(!(currentPage.page)) break;
                accessPage(&currentPage, index, buffer);
                taskFileMgr(&currentPage, index, buffer);
                break;
            case 2:
                fprintf(stdout, "Digite um nome: ");
                scanf("%s", pageName);
                if(pageName == "0\n") break;
                addCategorie(index, pageName, buffer);
                break;
            case 3:
                printf("Número: ");
                scanf("%d", &page);
                fgetc(stdin);
                if(!page) break;
                deleteCategorie(&index, page, buffer);
                break;
            case 4:
                printf("Número: ");
                scanf("%d", &choice);
                fgetc(stdin);
                if(choice == 0) break;
                printf("Novo nome: ");
                fgets(buffer, STRING_SIZE, stdin);
                // if 0 break
                renameCategorie(&index, choice, buffer);
                break;
            default:
                printf("Comando não reconhecido\n");
        }
    }while(1);
    
}

void clearScreen(void){
    printf("\e[1;1H\e[2J");
}

void printPage(struct taskFilePage *currentTaskPage, char *buffer){
    clearScreen();
    int i = 0;
    printf("\n\n%s\n\n", currentTaskPage->pageName);
    while(fgets(buffer, STRING_SIZE, currentTaskPage->taskFile)){
        fprintf(stdout, "%d - %s", ++i, buffer);
    }
    rewind(currentTaskPage->taskFile);
}

void taskFileMgr(struct taskFilePage *currentPage, FILE *index, char *buffer){
    int selector;
    int option;
    do{
        printPage(currentPage, buffer);
        printf("\nADD(1) COMPLETAR (2) VOLTAR(0)\n");
        printf("Opção: ");
        scanf("%d", &selector);
        fgetc(stdin);
        
        switch(selector){
            case 0:
                fclose((*currentPage).taskFile);
                currentPage->page = 0;
                *currentPage->pageName = '\0';
                break;
            case 1:
                printf("Tarefa: ");
                fgets(buffer, STRING_SIZE, stdin); //PODE DAR BUFFER OVERFLOW
                printf("%s", buffer);
                addTask((*currentPage).taskFile, buffer);
                break;
            case 2:
                printf("Número: ");
                scanf("%d", &option);
                fgetc(stdin);
                deleteTask(currentPage, index, option, buffer);
                break;
            default:
                printf("Comando inválido");
        }
    }while(selector);
}

void accessPage(struct taskFilePage *currentPage, FILE *index, char *buffer){ //Abre o arquivo desejado
    int c = 0;
    char name[50];

    rewind(index);
    //Vai até uma linha antes do nome da página desejada
    for(int i = 0; i < (2 * (*currentPage).page - 2); i++){ //2 * page - 1 mapeia index names
        fgets(buffer, STRING_SIZE, index); 
    }

    //Lê o nome da página
    if(fgets(name, sizeof(name), index)) {
        name[strcspn(name, "\n")] = '\0'; // Remove o \n
        my_strcpy(name, currentPage->pageName);
    }

    if(fgets(buffer, STRING_SIZE, index)) {
        buffer[strcspn(buffer, "\n\r")] = '\0';
    }

    (*currentPage).taskFile = fopen(buffer, "r+");
    rewind(index);
}

void startPage(FILE *index, char *buffer){
    int i = 0;
    while(fgets(buffer, STRING_SIZE, index)){
        fprintf(stdout, "%d - %s", ++i, buffer);
        fgets(buffer, STRING_SIZE, index);
    }
    printf("ACESSAR(1) ADICIONAR(2) REMOVER(3) RENOMEAR(4) SAIR(0)\n");
    printf("O que deseja fazer? ");
    rewind(index);
}

void addTask(FILE *taskfile, char *buffer){ //Dá para usar o taskfile no modo +a ao invés de se mover com fgets
    char temp[STRING_SIZE];
    while((fgets(temp, STRING_SIZE, taskfile)));
    fputs(buffer, taskfile);
    fflush(taskfile); //Pesquisar mais sobre fflush
    rewind(taskfile);
}

//Dá para reestruturar apenas jogando o que está em TempFile de volta para taskFile no modo w
void deleteTask(struct taskFilePage *currentPage, FILE *index, int value, char *buffer){
    int i = 1;
    int c = '\0';
    FILE *tempFile = fopen("./tempFile.txt", "w");
    while(fgets(buffer, STRING_SIZE, (*currentPage).taskFile)){
        if(i++ != value){
            fputs(buffer, tempFile);
        }
    }
    fclose(tempFile);
    fclose(currentPage->taskFile);
    for(int i = 0; i < (2 * currentPage->page - 1); i++){ //2 * page - 1 mapeia index names
        fgets(buffer, STRING_SIZE, index); 
    }

    for(int i = 0; c != '\n'; i++)
    {
        c = fgetc(index);
        buffer[i] = c;
        if(c == '\n'){
            buffer[i] = '\0';
        }
    }
    remove(buffer);
    rename("tempFile.txt", buffer);
    rewind(index);
    currentPage->taskFile = fopen(buffer, "r+");
}

void addCategorie(FILE *index, char *pageName, char *buffer){
    FILE *newCat;
    int lines;
    int tailCheck;
    fseek(index, 0, SEEK_END);
    fputs(pageName, index);
    fprintf(index ,"\n./categorias/%s.txt", pageName);
    fputc('\n', index);
    rewind(index);//OK
    lines = totalLines(index, buffer);
    printf("%d", lines);//DEBUG
    
    for(int i = 0; i < lines - 1;){
        if(fgetc(index) == '\n') i++;
    }

    for(int i = 0;buffer[i] != '\0';i++){
        buffer[i] = fgetc(index);
        if(buffer[i] == '\n'){
            buffer[i] = '\0';
        }
    }

    printf("%s", buffer);//DEBUG
    newCat = fopen(buffer, "w");
    fclose(newCat);
    rewind(index);
}

void deleteCategorie(FILE **index, int page, char *buffer){
    int currentLine = 1;
    int c;
    FILE *tempFile = fopen("./categorias/tempFile.txt", "w");

    //Faz a cópia de index.txt sem as informações da categoria a ser removida
    while ((c = fgetc(*index)) != EOF) {
        if (currentLine == (page * 2 - 1)) {
            if (c == '\n') currentLine++;
            continue; 
        }
        if (currentLine == (page * 2)){
            int i = 0;
            buffer[i] = c; //Cópia o '.'
            i++;
            while ((c = fgetc(*index)) != '\n' && c != EOF) //Copia o endereço do arquivo a ser removido para buffer
            {
                buffer[i] = c;
                i++;
            }
            buffer[i] = '\0';
            currentLine++;
            continue;
        }

        fputc(c, tempFile);

        if (c == '\n') {
            currentLine++;
        }
    }
    fprintf(stdout, "\n%s\n", buffer); //DEBUG
    remove(buffer);
    fclose(tempFile);
    fclose(*index);
    remove("./categorias/index.txt");
    rename("./categorias/tempFile.txt", "./categorias/index.txt");
    
    *index = fopen("./categorias/index.txt", "r+");

}

void renameCategorie(FILE **index, int page, char *buffer){
    int currentLine = 0;
    FILE *tempFile = fopen("./categorias/tempFile.txt", "w");
    char secondBuffer[STRING_SIZE];
    while(fgets(secondBuffer, STRING_SIZE, *index)){
        currentLine++;
        if(currentLine == page * 2 - 1) fputs(buffer, tempFile);
        else fputs(secondBuffer, tempFile);
    };
    fclose(tempFile);
    fclose(*index);
    remove("./categorias/index.txt");
    rename("./categorias/tempFile.txt", "./categorias/index.txt");
    *index = fopen("./categorias/index.txt", "r+");
}

int my_strcpy(char *origin, char *destiny){
    int size = strlen(origin);
    for(int i  = 0; i <= size; i++){
        *(destiny + i) = *(origin + i);
    }
    return 1;
}


int totalLines(FILE *index, char *buffer) {
    int lines = 0;

    if (index == NULL) return 0;

    while (fgets(buffer, STRING_SIZE, index)) {
        if (buffer[0] != '\n' && buffer[0] != '\0') {
            lines++;
        }
    }

    rewind(index);
    return lines;
}