#include <stdio.h>

main()
{
	char ope;
	double a, b, ans;
	
	while (1) {
		fflush(stdin);
		a = 0;
		b = 0;
		ans = 0;
		
		printf("������͂��Ă������� :");
		scanf("%lf %c %lf", &a, &ope, &b);
		
		switch (ope) {
			case '+' :
				ans = a + b;
				break;
			case '-' :
				ans = a - b;
				break;
			case '*' :
				ans = a * b;
				break;
			case '/' :
				if (b == 0) {
					printf("0�Ŋ��邱�Ƃ͂ł��܂���\n");
					continue;
				}
				ans = a / b;
				break;
			default:
				printf("ERROR\n");
				continue;
		}
		
		printf("���� :%lf\n", ans);
	}
}