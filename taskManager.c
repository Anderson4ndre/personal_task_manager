#include <stdio.h>
#include <stdlib.h>
#define STRING_SIZE 128
#define CATEGORY_LIMIT 5

void formatFile(FILE*);
void clearScreen(void);
int linesNum(FILE*);
void menu(void);
void tasks(FILE *, FILE *, int);
void addTask(FILE *, char *);
void deleteTask(FILE *, int);
long int fileSize(FILE *);

int main(void){
    int choice = -1;
    int page = 1;
    char s[STRING_SIZE];
    FILE* categoriesNames = fopen("./categorias/index.txt","r+");
    FILE* taskFile;
    int pageLines = linesNum(categoriesNames);
    do{
        clearScreen();
        printf("\n");
        tasks(taskFile, categoriesNames, page);
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
            case 3:
                fprintf(stdout, "Digite a página: ");
                scanf("%d", &page);
                //fclose(taskFile);
                break;
            default:
                printf("Comando não reconhecido\n");
        }
    }while(1);
    
}

void clearScreen(void){
    printf("\e[1;1H\e[2J");
}

int linesNum(FILE *file){
    int lines = 0;
    char s[STRING_SIZE];
    while(fgets(s, sizeof(s), file) != NULL){
        lines++;
    };
    rewind(file);
    return lines;
}

//Under maintanace
void tasks(FILE *taskFile, FILE *categories, int page){
    char s[STRING_SIZE];
    int c;
    for(int i = 1; i < page; i++){
        fgets(s, sizeof(s), categories);
    }
    s[0] = '\0'; //Limpa s[0] para entra no próximo for 

    while((c = fgetc(categories)) != ' '){
        fputc(c, stdout);
    }
    for(int i = 0; s[i] != '\n'; i++ )
    {
        c = fgetc(categories);
        s[i] = c;
        if(c == '|'){
            s[i] = '\0';
        }
    }
    taskFile = fopen(s, "r+");
    printf("\n%s", s); //DEBUG
    fputs("\n\n", stdout);
    for(int i = 1; fgets(s, sizeof(s), taskFile); i++){
        printf("%d-%s", i, s);
    }
    
    printf("\n\n");
    rewind(taskFile);
    rewind(categories);
}

void menu(void){
    printf("ADD(1) COMPLETAR(2) CATEGORIA(3) SAIR(0)\n");
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