/*
Predictive parser
2017.11.03 by Kang, Seung - Shik at Kookmin University
*/
#include <stdio.h>
#include <string.h>
#pragma warning(disable: 4996)

#define NONTERMINALS 3
#define TERMINALS 2
#define RULESIZE 3
#define MAX 100

#define INPUT_END_MARK ('a'+TERMINALS)
#define STACK_BOTTOM_MARK '_'

char create_rule[RULESIZE][MAX];
int parsing_table[NONTERMINALS][TERMINALS + 1];
char stack[999];
int top;

void input_data(char *str)
{
	int len;

	printf("Input a,a,a,a,a : ");
	scanf("%s", str);

	len = strlen(str);
	str[len] = INPUT_END_MARK;
	str[len + 1] = 0;
	printf("Input string : %s (%c is an endig mark)\n", str, INPUT_END_MARK);
}

/*
G = ({LIST, ELEMENT}, {a, ,}, P, LIST),
P: LIST-> LIST , ELEMENT | ELEMENT
ELEMENT-> a

left recursion을 제거 하여 LL(1)문법으로 만든 후
문자를 인덱스로 표시하기 위하여 아래와 같이 차례대로 A, B..nonterminal 이름 변경..

0. A-> CB
1. B-> ,CB
2. B-> epsilon
3. C-> a
*/
void load_create_rule()
{
	strcpy(create_rule[0], "CB");
	strcpy(create_rule[1], ",CB");
	strcpy(create_rule[2], "");
	strcpy(create_rule[3], "a");
}

/*
Vn/Vt |	a	,   $(c)
----------------------------------------
A	 |	0	-1	-1
B	 |	-1   1	 2 
C	 |	3   -1  -1
*/
void load_parsing_table()
{
	parsing_table[0][0] = 0;
	parsing_table[1][1] = 1;
	parsing_table[1][2] = 2;
	parsing_table[2][0] = 3;
}

char stack_top()
{
	return stack[top - 1];
}

void push(char ch)
{
	stack[top++] = ch;
}

void pop()
{
	top--;
}

void init_stack()
{
	top = 0;
	push(STACK_BOTTOM_MARK);
	push('A');
}

int is_nonterminal(char ch)
{
	if ('A' <= ch && ch <= 'Z') return 1;
	else return 0;
}

int is_terminal(char ch)
{
	return (is_nonterminal(ch) == 0);
}

void predictive_parsing(int table[NONTERMINALS][TERMINALS + 1], char rule[RULESIZE][MAX], char *input)
{
	char *p = input;
	char *str_p;
	int i, index = 0, len;

	while (1) {
		if (stack_top() == STACK_BOTTOM_MARK) {
			if (*p == INPUT_END_MARK)
				printf("[accept]\n");	// parsing OK
			else printf("[error] -- Input is not empty!\n");
			return;
		}

		if (is_terminal(stack_top())) {	// pop -- terminal symbol
			if (*p == stack_top()) {
				pop();
				p++;
			}
			else {
				printf("[error] -- Stack is not empty!\n");
				return;
			}
		}
		else { // expand -- nonterminal
			if (*p == 'a' || *p == 'c') {
				index = parsing_table[stack_top() - 'A'][*p - 'a'];
			}
			else {
				index = parsing_table[stack_top() - 'A'][1];
			}
			if (index != -1) {
				str_p = rule[index];
				len = strlen(str_p);
				printf("[CFG %d] %c -> %s\n", index, stack_top(), str_p);
				pop();
				for (i = len - 1; i >= 0; i--) {
					push(str_p[i]);
				}
			}
			else { // error
				printf("[error] %c -> %c\n", stack_top(), *p);
				return;
			}
		}
	}
}

int main()
{
	char input[100];

	load_create_rule();
	load_parsing_table();

	input_data(input);
	init_stack();

	predictive_parsing(parsing_table, create_rule, input);
	return 0;
}