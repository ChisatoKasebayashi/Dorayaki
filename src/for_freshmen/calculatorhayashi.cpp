#include<stdio.h>
#include<math.h>
#include<stdlib.h>
main()
{
	int n;
	
	double a,b;
	
	char c;
	
	while(1){
				
		printf("     ��:");
		

		n=scanf("%lf %c %lf",&a,&c,&b);
		
		if(n!=3){
			printf(" �G���[ \n\n");
			exit(1);
		}
			switch(c){
				case '+':
					printf("   �����A   %.2lf\n\n",a+b);
					break;
				case '-':
					printf("   �����A   %.2lf\n\n",a-b);
					break;
				case'*':
					printf("   �����A   %.2lf\n\n",a*b);
					break;
				case'/':
					if(b==0){
						printf("  �����A �G���[(*��*)\n\n");
					}
					else if(b!=0){
					printf("   �����A   %.2lf\n\n",a/b);
					}break;
				default:
					printf(" \n\n         error \n\n");
			
			}
		}
		
	
}