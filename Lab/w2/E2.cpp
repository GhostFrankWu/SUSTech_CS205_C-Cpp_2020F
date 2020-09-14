#include<iostream>
using namespace std;
int main2() {/*2. Write a program that asks the user to enter the number of seconds as an integer value(use type long, or, if available, long long) and  then displays the equivalent time in days, hours, minutes and seconds. Use symbolic constants to represent the number of hours in the day, the number of minutes in an hour, and the number of seconds in a minute. The output should look like this: */
	long int sec = 0;
	cout << "Enter the number of seconds:";
	scanf("%li", &sec);
	int second = sec % 60;
	sec -= second;
	int min = sec / 60;
	int minute = min % 60;
	min -= minute;
	int hou = min / 60;
	int hour = hou % 24;
	hou -= hour;
	int day = hou / 24;
	sec += second;
	printf("%li seconds = %d days, %d hours, %d minutes, %d seconds\n",sec,day,hour,minute,second);
	system("pause>nul");
	return 0;
}