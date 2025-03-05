#include<stdio.h>

int main()
{
	printf("Introduceti un nr real: ");
	float a = 0.0;
	scanf_s("%f", &a);
	printf("Ati introdus: %5.2f", a);

	return 0;
}