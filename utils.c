#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PSize 15

/* -- Signatures-- */

int ReadInt(FILE* file);
char* ReadChar(FILE* file, int size);
int GetFileSize(char* path);
int GetCharSize(char *ptr);
char* NewString(int length);
void PrintField(char* value, int length);

/* -- Readers -- */

char* ReadChar(FILE* file, int size){
    char* value = NewString(size);
    fread(value, size, 1, file);
    return value;
}

int ReadInt(FILE* file){
    int value;
    fread(&value, sizeof(int), 1, file);
    return value;
}

/* -- Initializers -- */

char* NewString(int length){
    return (char*) malloc(sizeof(char)*length);
}

/* -- Dynamic lengths -- */

int GetFileSize(char* path){
	FILE* file;
	if((file = fopen(path, "r"))){
        fseek(file, 0, SEEK_END);
        int length =  ftell(file); 
        fclose(file);
        return length;
    }
    return 0;
}

int GetCharSize(char *ptr){
    int offset = 0;
    while (*(ptr + offset) != '\0')
        ++offset;
    return offset;
}

void PrintField(char* value, int length){
    int precision = (PSize - length)%2;
    int sides = (PSize - length)/2;
    int i;
    for(i=0;i<sides;i++)
        printf(" ");
    for(i=0;i<length;i++)
        printf("%c",value[i]);
    for(i=0;i<sides+precision;i++)
        printf(" ");
    printf("|");
}
