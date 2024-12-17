/*
7. Napisati program koji pomoću vezanih listi (stabala) predstavlja strukturu direktorija.
Omogućiti unos novih direktorija i pod-direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij. Točnije program treba preko menija simulirati
korištenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.
*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROOT_DIR_NAME "C:"
#define MAX_DIR_LENGTH 256
#define NO_MEMORY_ERROR -1
#define EMPTY_DIR_ERROR -2
#define NOT_EXISTING_DIR_ERROR -3
#define EMPTY_STACK_ERROR -4

typedef struct dir* Position;
typedef struct dir {
    char name[MAX_DIR_LENGTH];
    Position FirstChild;
    Position NextSibling;
} Dir;

typedef struct stack* StackPosition;
typedef struct stack {
    Position dir;
    StackPosition parent;
} Stack;

Position CurrentDir(StackPosition stackHead);
Position InsertDir(Position firstDir, Position newDir);
int MakeDir(Position currentDir);
int PrintDir(StackPosition stackHead);
int PrintCurrentDirName(StackPosition stackHead);

int ChangeDir(StackPosition currentStack);
int Push(StackPosition stackHead, Position dir);
int Pop(StackPosition stackHead);

int main()
{
    Dir rootDir;
    rootDir.NextSibling = NULL;
    rootDir.FirstChild = NULL;
    strcpy(rootDir.name, ROOT_DIR_NAME);

    Stack _stack;
    StackPosition stackHead = &_stack;
    stackHead->dir = NULL;
    stackHead->parent = NULL;
    Push(stackHead, &rootDir);

    int cmd;

    while (1) {
        printf("Unesite naredbu: 1 - md, 2 - cd dir, 3 - cd .., 4 - dir, 5 - izlaz\n> ");
        scanf(" %d", &cmd);
        switch (cmd) {
        case 1:
            MakeDir(CurrentDir(stackHead));
            break;
        case 2:
            ChangeDir(stackHead);
            break;
        case 3:
            Pop(stackHead);
            break;
        case 4:
            // print dir
            PrintDir(stackHead);
            break;
        case 5:
            // exit
            return 0;
        }
    }
}

Position CurrentDir(StackPosition stackHead) {
    return stackHead->parent->dir;
}

int MakeDir(Position currentDir)
{
    Position newDir = (Position)malloc(sizeof(Dir));
    if (!newDir)
    {
        printf("Greska pri alokaciji memorije za direktorij!\n");
        return NO_MEMORY_ERROR;
    }
    newDir->FirstChild = NULL;
    newDir->NextSibling = NULL;
    printf("Unesite ime foldera: ");
    scanf(" %s", newDir->name);

    currentDir->FirstChild = InsertDir(currentDir->FirstChild, newDir);
    return 0;
}

Position InsertDir(Position firstDir, Position newDir) {
    if (firstDir == NULL) {
        return newDir;
    }
    if (strcmp(newDir->name, firstDir->name) < 0) {
        newDir->NextSibling = firstDir;
        return newDir;
    }
    firstDir->NextSibling = InsertDir(firstDir->NextSibling, newDir);
    return firstDir;
}

int PrintDir(StackPosition stackHead)
{
    Position currentDir = CurrentDir(stackHead);
    if (!currentDir->FirstChild) {
        printf("Direktorij "); PrintCurrentDirName(stackHead); printf(" je prazan!\n");
    }
    else
    {
        printf("Poddirektoriji direktorija "); PrintCurrentDirName(stackHead); printf(" su:\n");
        Position subDir = currentDir->FirstChild;
        while (subDir)
        {
            printf("- %s\n", subDir->name);
            subDir = subDir->NextSibling;
        }
    }
    return 0;
}

int PrintCurrentDirName(StackPosition stackHead)
{

    if (stackHead->parent != NULL)
        PrintCurrentDirName(stackHead->parent);

    if (stackHead->dir != NULL)
        printf("%s\\", stackHead->dir->name);

    return 0;
}

int ChangeDir(StackPosition stackHead)
{
    Position currentDir = CurrentDir(stackHead);
    if (!currentDir->FirstChild) {
        printf("Direktorij %s je prazan!\n", currentDir->name);
        return EMPTY_DIR_ERROR;
    }
    char name[MAX_DIR_LENGTH];
    printf("Unesite ime podfoldera: ");
    scanf(" %s", name);

    Position subDir = currentDir->FirstChild;
    while (subDir)
    {
        if (strcmp(subDir->name, name) == 0) {
            Push(stackHead, subDir);
            return 0;
        }
        subDir = subDir->NextSibling;
    }
    printf("Ne postoji trazeni podfolder!\n");
    return NOT_EXISTING_DIR_ERROR;
}

int Push(StackPosition stackHead, Position dir) {
    StackPosition newStackElem = (StackPosition)malloc(sizeof(Stack));
    if (newStackElem == NULL) {
        printf("Greska pri alokaciji memorije za stack!\n");
        return NO_MEMORY_ERROR;
    }
    newStackElem->dir = dir;
    newStackElem->parent = stackHead->parent;
    stackHead->parent = newStackElem;
    return 0;
}

int Pop(StackPosition stackHead)
{
    Position currentDir = CurrentDir(stackHead);
    if (strcmp(currentDir->name, ROOT_DIR_NAME) == 0) {
        printf("Nalazite se u vrsnom folderu!\n");
        return 0;
    }

    StackPosition tmp = stackHead->parent;
    stackHead->parent = stackHead->parent->parent;
    free(tmp);
    return 0;
}
