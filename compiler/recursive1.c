/*
a,a,a,a,a �� ����ϴ� ������ ����

G = ({LIST, ELEMENT}, {a, ,}, P, LIST),
P: LIST-> LIST , ELEMENT | ELEMENT
   ELEMENT-> a
*/

#include<stdio.h>
#include<string.h>
#include<ctype.h>

char input[20];
int i, error;
void LIST();
void LISTprime();
void ELEMENT();


void LIST()
{
	ELEMENT();
	LISTprime();
}
void LISTprime()
{
	if (input[i] == ','){
		i++;
		ELEMENT();
		LISTprime();
	}
}
void ELEMENT() {
	if (input[i] == 'a') {
		i++;
	}
	else {
		error = 1;
	}
}
void main()
{
	i = 0;
	error = 0;
	puts("Production Rules:");
	puts("\t LIST --> LIST,ELEMENT");
	puts("\t LIST --> ELEMENT");
	puts("\t ELEMENT --> a");
	printf("$�� ������ �Է½�Ʈ�� : ");

	gets(input);
	LIST();
	if (strlen(input) == i&&error == 0)
		printf("\nAccept\n");
	else
		printf("\nFAIL\n");
}