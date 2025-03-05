#include<stdio.h>

int main()
{
	printf("Hello world!\n Introduceti un nr intreg: ");

	int a = 0;
	scanf_s("%d", &a);
	printf("You entered: %d", a);

	return 0;
}