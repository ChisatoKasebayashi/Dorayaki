#include<stdio.h>
#include<math.h>
#include<stdlib.h>

void main()
{
	double a,b,s;
	
	char c;
	
	printf("���l�@���Z�q���P�����͂��Ă��������B\n  =�F���͂œ���\n\n");

	printf("���l  ");
	scanf("%lf",&a);
	
	while(1){
	
	printf("���Z�q ");
	scanf(" %c",&c);
	
	if((c =='=')==1){
	
		printf("  �����F%lf\n",a);
		printf(" ������ꍇ��,1����͂��Ă������� ");
		scanf("%lf",&s);
	
		if(s!=1){
			exit(1);
	}else{
		printf("���l ");
		scanf("%lf",&a);
		
		printf("���Z�q ");
		scanf(" %c",&c);
	}
	
	}printf("���l ");
	scanf("%lf",&b);
		
		switch(c){
		case '+':
			a += b;
		
			break;
		case '-':
			a -= b;
			
			break;
		case '*':
			a *= b;
			
			break;
		case '/':
		if(b==0){
			printf("  error\n");
			exit(1);
		}
		else if(b!=0){
			a /= b;
		}break;
	}
	}
}