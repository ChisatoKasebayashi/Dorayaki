#include<stdio.h>
#include<math.h>
#include<stdlib.h>

void main()
{
	double a,b,s;
	
	char c;
	
	printf("数値　演算子を１つずつ入力してください。\n  =：入力で答え\n\n");

	printf("数値  ");
	scanf("%lf",&a);
	
	while(1){
	
	printf("演算子 ");
	scanf(" %c",&c);
	
	if((c =='=')==1){
	
		printf("  答え：%lf\n",a);
		printf(" 続ける場合は,1を入力してください ");
		scanf("%lf",&s);
	
		if(s!=1){
			exit(1);
	}else{
		printf("数値 ");
		scanf("%lf",&a);
		
		printf("演算子 ");
		scanf(" %c",&c);
	}
	
	}printf("数値 ");
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