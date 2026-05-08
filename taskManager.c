#include <stdio.h>
#include <stdlib.h>
#define STRING_SIZE 128
#define CATEGORY_LIMIT 5

void formatFile(FILE*);
void clearScreen(void);
void menu(void);
void tasks(FILE *, FILE *);
void addTask(FILE *, char *);
void deleteTask(FILE *, int);
long int fileSize(FILE *);
void my_fileReader(FILE *);

int main(void){
    int choice = -1;
    char s[STRING_SIZE];
    FILE* categoriesNames;
    FILE* taskFile;
    do{
        clearScreen();
        printf("\n");
        tasks(taskFile, categoriesNames);
        menu();
        scanf("%d", &choice);
        switch(choice){ //Inserir na posição X
            case 0:
                clearScreen();
                exit(0);
            case 1:
                fprintf(stdout, "Digite a tarefa: ");
                fgets(s, sizeof(STRING_SIZE), stdin);
                addTask(taskFile, s);
                break;
            case 2:
                fprintf(stdout, "Digite o valor: ");
                scanf("%d", &choice);
                deleteTask(taskFile, choice);
                break;
            default:
                printf("Comando não reconhecido\n");
        }
    }while(1);
    
}

void clearScreen(void){
    printf("\e[1;1H\e[2J");
}

void tasks(FILE *taskFile, FILE *categories){
    char s[STRING_SIZE];
    int i = 0;
    char c;
    categories = fopen("./categorias/index.txt", "r");
    taskFile = fopen("./categorias/cat1.txt", "r");
    while((c = fgetc(categories)) != ' '){
        fputc(c, stdout);
    }
    fputs("\n\n", stdout);
    while(fgets(s, sizeof(s), taskFile)){
        printf("%d-%s", ++i, s);
    }
    printf("\n\n");
}

void menu(void){
    printf("ADD(1) COMPLETAR(2) SAIR(0)\n");
    printf("O que deseja fazer? ");
}

void addTask(FILE *taskfile, char *s){
    char checkHead;
    char checkTail;
    taskfile = fopen("./categorias/cat1.txt", "a+");
     //PAIA
    fgets(s, STRING_SIZE, stdin);
    fputs(s, taskfile);
    fclose(taskfile);
}

void deleteTask(FILE* taskFile, int value){
    char s[STRING_SIZE];
    int i = 1;
    FILE *tempFile = fopen("./tempFile.txt", "w");
    taskFile = fopen("./categorias/cat1.txt", "r");
    while(fgets(s, sizeof(s), taskFile)){
        if(i++ != value){
            fputs(s, tempFile);
        }
    }
    fclose(tempFile);
    fclose(taskFile);
    remove("./categorias/cat1.txt");
    rename("tempFile.txt", "./categorias/cat1.txt");
}

long int fileSize(FILE* file){
    fseek(file, 0, SEEK_END);
    return ftell(file);
}