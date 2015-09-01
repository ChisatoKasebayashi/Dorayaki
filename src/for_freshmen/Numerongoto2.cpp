#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define LIMIT 30

class NUMERON
{
	private:
		int ans[3];
		int cal[3];
	public:
		NUMERON();
		int bite_num;
		int eat_num;
		int count(int max);
		int call();
		void bite();
		void print();
		void clear();
		void eat();
};

NUMERON::NUMERON()
{
	srand((unsigned) time(NULL));
	ans[0] = rand() % 10;
	do{
		ans[1] = rand() % 10;
	}
	while(ans[0] == ans[1]);
	
	do{
		ans[2] = rand() % 10;
	}
	while(ans[0] == ans[2] || ans[1] == ans[2]);
}

int NUMERON::call()
{
	long st = time(NULL);
	for(int i = 0; !i;){
		if(kbhit()){
			for(; i < 3; i++)
				scanf("%1d", &cal[i]);
		}
		if(time(NULL) >= st + LIMIT)
			return 0;
	}
	NUMERON::clear();
	NUMERON::bite();
	NUMERON::eat();
	return 1;
}

void NUMERON::clear()
{
	int i = getchar();
	if(i != 0x0A){
		while(i != 0x0A)
			i = getchar();
	}
}

int NUMERON::count(int max)
{
	char i[] = {0, 100, 50, 25, 10, 5, 1};
	return (max < 1 || max > 6) ? 0: i[max];
}

void NUMERON::bite()
{
	bite_num = 0;
	if(ans[0] == cal[1] || ans[0] == cal[2])
		bite_num++;
	if(ans[1] == cal[0] || ans[1] == cal[2])
		bite_num++;
	if(ans[2] == cal[0] || ans[2] == cal[1])
		bite_num++;
}

void NUMERON::eat()
{
	eat_num = 0;
	if(ans[0] == cal[0])
		eat_num++;
	if(ans[1] == cal[1])
		eat_num++;
	if(ans[2] == cal[2])
		eat_num++;
}

void NUMERON::print()
{
	for(int i = 0; i < 3; i++)
		printf("%d", ans[i]);
	printf("\n");
}

int main()
{
	printf("------------�k������-----------\n");
	printf("3���̐����𓖂ĂĂ�������\n");
	printf("�����������͓���܂���\n");
	printf("BITE:���͈Ⴄ�������͂����Ă��鐔\n");
	printf("EAT: ���Ɛ����������Ă��鐔\n");
	printf("�������Ԃ�30�b�ł�\n");
	printf("-------------------------------\n");
	char end = 'y';
	
	while(end != 'n'){
		NUMERON numeron;
		int max = 0;
		int rest;
		
		do{
			printf("\n��Փx��I��ł�������\n");
			printf("1)VERY EASY 2)EASY 3) NORMAL 4)HARD 5)VERY HARD 6)EXTREME =>");
			scanf("%1d", &max);
			numeron.clear();
			max = numeron.count(max);
		}
		while(!max);
		
		for(int i = 0; ; i++){
			rest = max - i;
			printf("\n3���̐�������͂��Ă�������(�c��%d��): ", rest);
			if(!numeron.call()){
				printf("\n***TIMEOVER***");
				rest = 0;
			}
			else if(numeron.eat_num == 3){
				printf("\n***YOU WIN!!***\n\n");
				break;
			}
			else{
				printf("%dBITE, %dEAT\n", numeron.bite_num, numeron.eat_num);
			}
			if(rest <= 1) {
				printf("\n***YOU LOST***\n������...");
				numeron.print();
				break;
			}
		}
		
		printf("������x�v���C���܂���? yes/no =>");
		scanf("%c", &end);
		numeron.clear();
	}
	return 0;
}