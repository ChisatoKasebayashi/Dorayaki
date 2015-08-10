#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class NUMERON
{
	private:
		int i;
	public:
		void set(int str[]);
		void call(int str[]);
		void print(int str[]);
		int difficulty(int max);
		int bite(int s1[], int s2[]);
		int eat(int s1[], int s2[]);
};

void NUMERON::set(int str[])
{
	srand((unsigned) time(NULL));
	
	str[0] = rand() % 10;
	
	while(1) {
		str[1] = rand() % 10;
		if(str[0] != str[1])
			break;
	}
	
	while(1) {
		str[2] = rand() % 10;
		if(str[0] != str[2] && str[1] != str[2])
			break;
	}
}

void NUMERON::call(int str[])
{
	for(i = 0; i < 3; i++)
		scanf("%1d", &str[i]);
	fflush(stdin);
}

int NUMERON::difficulty(int max)
{
	switch(max) {
		case 1:
			return 100;
		case 2:
			return 30;
		case 3:
			return 20;
		case 4:
			return 10;
		case 5:
			return 1;
		default:
			return 0;
	}
}

int NUMERON::bite(int s1[], int s2[])
{
	i = 0;
	if(s1[0] == s2[1] || s1[0] == s2[2])
		i++;
	if(s1[1] == s2[0] || s1[1] == s2[2])
		i++;
	if(s1[2] == s2[0] || s1[2] == s2[1])
		i++;
	
	return i;
}

int NUMERON::eat(int s1[], int s2[])
{
	i = 0;
	if(s1[0] == s2[0])
		i++;
	if(s1[1] == s2[1])
		i++;
	if(s1[2] == s2[2])
		i++;
	
	return i;
}

void NUMERON::print(int str[])
{
	for(i = 0; i < 3; i++)
		printf("%d", str[i]);
	printf("\n");
}

int main()
{
	printf("------------ヌメロン-----------\n");
	printf("3桁の数字を当ててください\n");
	printf("※同じ数字は入りません\n");
	printf("BITE:桁は違うが数字はあっている数\n");
	printf("EAT: 桁と数字が合っている数\n");
	printf("-------------------------------\n");
	
	while(1) {
		NUMERON numeron;
		int num[] = {0, 0, 0};
		int call[] = {0, 0, 0};
		int bite;
		int eat;
		int rest;
		int max;
		char i;
		
		do{
			printf("\n難易度を選んでください\n");
			printf("1)VERY EASY 2)EASY 3) NORMAL 4)HARD 5)EXTREME =>");
			scanf("%d", &max);
			fflush(stdin);
			max = numeron.difficulty(max);
		}
		while(max == 0);
		
		numeron.set(num);
		for(int count = 0; count < max; count++) {
			rest = max - count;
			printf("\nコールしてください(残り%d回): ", rest);
			numeron.call(call);
			bite = numeron.bite(num, call);
			eat = numeron.eat(num, call);
			printf("%dBITE, %dEAT\n", bite, eat);
			if(eat == 3) {
				printf("\n***YOU WIN!!***\n\n");
				break;
			}
			if(count == max - 1) {
				printf("\n***YOU LOST***\n");
				printf("正解は...");
				numeron.print(num);
				printf("\n");
			}
		}
		
		printf("もう一度プレイしますか? y/n =>");
		scanf("%1c", &i);
		fflush(stdin);
		if(i == 'n')
			break;
	}
	return 0;
}
