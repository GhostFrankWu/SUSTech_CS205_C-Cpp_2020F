#include<stdio.h>/*3. Write a .C program that asks the user to enter a integer value, a character, and a float value. And then use the printf statement to print them out. A sample run should look like this:*/
int main() {
	printf("Please Enter a Character : ");
	char ch;
	scanf("%c", &ch);
	printf("Please Enter an Integer Value : ");
	int i;
	scanf("%d", &i);
	printf("Please Enter a Float Value : ");
	float f;
	scanf("%f", &f);
	printf("\n\nThe variables you entered were:\nThe Character value that you Entered is : %c\nThe Integer value that you Entered is : %d\nThe Float value that you Entered is : %.3f", ch, i, f);
	getchar(); getchar();
	return 0;
}