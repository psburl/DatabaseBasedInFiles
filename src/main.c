#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.c"
#ifndef utils
#include "utils.c"
#define utils
#endif

#define HEADERFILE  "../data/header.dat"
#define PATHFILE  "../data/register.dat"
#define MAXSTRINGLEN 32

void BuildFields();
Header* ReadHeader();
void InsertRegisters();
void InsertRegister(Header* header, Node* values);
int IsPositiveResponse(char* response);
void SelectAll();
void Menu();

int main(void){
	
	Menu();
}

void Menu(){

	int header_size = GetFileSize(HEADERFILE);
	if(header_size == 0){
		printf("Não foi encontrado nenhum arquivo base.\n");
		printf("Deseja criar uma nova base de dados ?(S/N): ");
		char* input = NewString(1);
		scanf("%s", input);
		if(IsPositiveResponse(input) == 0){
			printf("Processo finalizado!\n");
			return;
		}

		BuildFields();
	}

	printf("Insira o comando relativo ao quê deseja executar..\n");
	printf("[\"insert\", \"selectall\", \"exit\"]\n");

	while(1){

		printf(">");

		char insertCommand[6] = {'i','n','s','e','r','t'};
		char selectAllCommand[9] = {'s','e','l','e','c','t','a','l','l'};
		char exit[4] = {'e', 'x', 'i', 't'};

		char* input = NewString(MAXSTRINGLEN);

		scanf("%s", input);
		
		if(strcmp(input, insertCommand) == 0)
			InsertRegisters();

		else if(strcmp(input, selectAllCommand) == 0)
			SelectAll();

		else if(strcmp(input, exit) == 0)
			return;

		else 
			printf("O comando não existe.\n");
	}
}

void BuildFields(){

	Node *fields = NewNode();

	printf("Criando nova tabela...\n");

	int shouldGetData = 1;
	int countFields = 0;
	while(shouldGetData){

		char* column = NewString(MAXSTRINGLEN);
		printf("Entre com o nome da coluna: ");
		scanf("%s", column);
		Push(fields, column);
		countFields++;
		printf("Deseja inserir mais algum campo ? (S/N): ");
		char* response = NewString(MAXSTRINGLEN);
		scanf("%s", response);
		shouldGetData = IsPositiveResponse(response);
	}

	FILE* file = fopen(HEADERFILE, "w+");
	fwrite(&countFields, sizeof(int), 1, file);

	Node* cursor = fields;

	while(cursor->next != NULL){
		int length = cursor->length;
		fwrite(&length, sizeof(int), 1, file);
		fwrite(cursor->value, length, 1, file);
		cursor = cursor->next;
	}

	fclose(file);
}

Header* ReadHeader(){

	Header* header = NewHeader();

	FILE* file = fopen(HEADERFILE, "r");

	Node* fields = NewNode();

	header->quantity =  ReadInt(file);

	int counter = 0;
	while(counter < header->quantity){

		int fieldLen = ReadInt(file);
		char* field = ReadChar(file, fieldLen);
		Push(fields, field);
		counter++;
	}

	Register* registers = NewRegister(); 
	while(feof(file) == 0){

		int offset = ReadInt(file);
		int length = ReadInt(file);
		PushReg(registers, offset, length);
	}
	header->registers = registers;
	header->fields = fields;
	fclose(file);
	return header;
}

void InsertRegisters(){

	Header* header = ReadHeader();

	Node* values = NewNode();

	int shouldGetData = 1;
	int countInserts = 0;
	while(shouldGetData){
		
		InsertRegister(header, values);
		countInserts++;
		printf("Deseja inserir mais algum registro ? (S/N): ");
		char* response = NewString(1);
		scanf("%s", response);
		shouldGetData = IsPositiveResponse(response);
	}

	Node* cursor = values;

	FILE* regFile = fopen(PATHFILE, "a");
	FILE* header_file = fopen(HEADERFILE, "a");

	while(countInserts){
		int i;
		for(i =0 ; i < header->quantity; i++){
			char* value = cursor->value;
			int length = GetCharSize(value);
			cursor->length = length;
			long offset = ftell(regFile);
			fwrite(value, length, 1, regFile);

			fwrite(&offset, sizeof(int), 1, header_file);
			fwrite(&length, sizeof(int), 1, header_file);
			
			cursor = cursor->next;
		}
		countInserts--;
	}

	fclose(regFile);
	fclose(header_file);
}

void InsertRegister(Header* header, Node* values){

	Node* header_cursor = header->fields;

	while(header_cursor->next != NULL){
		char* field = header_cursor->value;
		printf("Entre com o valor do campo \"%s\": ", field);
		char* value = NewString(MAXSTRINGLEN);
		scanf("%s", value);
		Push(values, value);
		header_cursor = header_cursor->next;
	}
}

void SelectAll(){

	Header* header = ReadHeader();

	Node* header_cursor = header->fields;
	while(header_cursor->next != NULL){
		int length = header_cursor->length;
		PrintField(header_cursor->value,length);
		header_cursor = header_cursor->next;
	}
	printf("\n");

	int fileSize = GetFileSize(PATHFILE);
	if(fileSize==0){
		printf("\n\n Não há nenhum registro");
		return;
	}
	
	FILE* file = fopen(PATHFILE, "r+");

	Register* cursor = 	header->registers;
	while(cursor->next->next != NULL){
		int i;
		for(i =0 ; i < header->quantity; i++){
			int offset = cursor->offset;
			fseek(file, offset, SEEK_SET);
			int length = cursor->length;
			char* value = ReadChar(file, length);
			PrintField(value, length);
			cursor = cursor->next;
		}
		printf("\n");
	}

	fclose(file);
}

int IsPositiveResponse(char* response){
    return response[0] == 's' || response[0] == 'S';
}