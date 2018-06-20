/*
a+(a+a*(a+a))*a+(a) �� ����ϴ� ������ ����

G = (E, T, F}, {a ,( ,) ,* ,+}, P, E),
P: E-> E + T
   E-> T
   T-> T * F
   T-> F
   F-> (E)
   F-> a
*/

#include<stdio.h>
#include<string.h>
#include<ctype.h>

char input[20];
int i, error;
void E();
void T();
void Eprime();
void Tprime();
void F();

void E()
{
	T();
	Eprime();
}
void Eprime()
{
	if (input[i] == '+')
	{
		i++;
		T();
		Eprime();
	}
}
void T()
{
	F();
	Tprime();
}
void Tprime()
{
	if (input[i] == '*')
	{
		i++;
		F();
		Tprime();
	}
}
void F()
{
	if (input[i]=='a')i++;
	else if (input[i] == '(')
	{
		i++;
		E();
		if (input[i] == ')')
			i++;

		else error = 1;
	}

	else error = 1;
}
void main()
{
	i = 0;
	error = 0;
	puts("Production Rules:");
	puts("\t E --> E + T");
	puts("\t E --> T");
	puts("\t T --> T * F");
	puts("\t T --> F");
	puts("\t F --> ( E )");
	puts("\t F --> a");
	printf("$�� ������ �Է½�Ʈ�� : ");

	gets(input);
	E();
	if (strlen(input) == i&&error == 0)
		printf("\nAccept\n");
	else
		printf("\nFAIL\n");
}

