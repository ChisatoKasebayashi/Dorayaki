#include <stdio.h>

main()
{
	char ope;
	double a, b, ans;
	
	while (1) {
		fflush(stdin);
		
		printf("Ž®‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢ :");
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
					printf("0‚ÅŠ„‚é‚±‚Æ‚Í‚Å‚«‚Ü‚¹‚ñ\n");
					continue;
				}
				ans = a / b;
				break;
			default:
				printf("ERROR\n");
				continue;
		}
		
		printf("“š‚¦ :%lf\n", ans);
	}
}