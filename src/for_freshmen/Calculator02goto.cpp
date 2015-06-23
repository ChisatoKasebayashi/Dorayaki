#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int convert(void);
int order(char, char);
int compute(void);

char buf[256][256];

int main(void)
{
	printf("-----élë•ââéZìdëÏ-----\n");
	printf("exit Ç≈èIóπ\n");
	printf("----------------------\n");
	
	while(1) {
		if(convert() != 0) {
			printf("          ERROR\n");
			continue;
		}
		if(compute() != 0) {
			printf("          ERROR\n");
		}
	}
}

int convert(void)
{
	char str[256];
	char stack[256];
	int buf0 = 0;
	int buf1 = 0;
	int pt = 0;
	int i;
	int j;
	
	memset(str, '\0', sizeof(str));
	memset(stack, '\0', sizeof(stack));
	for(i = 0; i < 256; i++) {
		for(j = 0; j < 256; j++) {
			buf[i][j] = '\0';
		}
	}
	
	printf("éÆÇì¸óÕ :");
	fgets(str, sizeof(str), stdin);
	str[strlen(str) - 1] = '\0';
	if(strcmp(str, "exit") == 0) exit(0);
	
	for(i = 0; str[i] != '\0'; i++) {
		if(str[i] == ' ') continue;
		if(isdigit(str[i]) != 0) {
			buf[buf0][buf1] = str[i];
			buf1++;
		}
		if(ispunct(str[i]) != 0) {
			switch(str[i]) {
				case '.':
					buf[buf0][buf1] = str[i];
					buf1++;
					break;
				case '+':
				case '-':
				case '*':
				case '/':
					buf0++;
					buf1 = 0;
					j = pt - 1;
					if(j < 0) {
						stack[pt] = str[i];
						pt++;
					}
					else{
						if(order(str[i], stack[j]) != 0) {
							stack[pt] = str[i];
							pt++;
						}
						else {
							buf[buf0][0] = stack[j];
							stack[j] = str[i];
							buf0++;
						}
					}
					break;
				default :
					return 1;
			}
		}
	}
	if(str[i] == '\0') {
		for(j = pt - 1, buf0++; j >= 0; j--) {
			buf[buf0][0] = stack[j];
			buf0++;
		}
		return 0;
	}
	return 1;
}

int order(char ch1, char ch2) {
	switch(ch1) {
		case '+':
		case '-':
			if((ch2 == '*') || (ch2 == '/')) return 0;
			return 1;
		case '*':
		case '/':
			if((ch2 == '*') || (ch2 == '/')) return 0;
			return 1;
		default :
			return -1;
	}
}

int compute(void)
{
	double number[256];
	int pt = 0;
	int i;
	int j;
	int k;
	
	memset(number, '\0', sizeof(number));
	
	for(i = 0; buf[i][0] != '\0'; i++) {
		if(isdigit(buf[i][0]) != 0) {
			number[pt] = atof(buf[i]);
			pt++;
		}
		else {
			j = pt - 1;
			k = j - 1;
			if(k < 0) return 1;
			switch(buf[i][0]) {
				case '+':
					number[k] = number[k] + number[j];
					pt = j;
					break;
				case '-':
					number[k] = number[k] - number[j];
					pt = j;
					break;
				case '*':
					number[k] = number[k] * number[j];
					pt = j;
					break;
				case '/':
					if(number[j] == 0) {
						return 1;
					}
					number[k] = number[k] / number[j];
					pt = j;
					break;
				default :
					return 1;
			}
		}
	}
	if(buf[i][0] == '\0') {
		printf("          = %lf\n", number[0]);
		return 0;
	}
	return 1;
}