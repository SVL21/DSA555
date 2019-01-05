
#include <stdio.h>
#include <iostream>

using namespace std;


unsigned int factorial(unsigned int n) {

	int factorial = 1;

	for (int i = 1; i <= n; ++i)
	{
		factorial *= i;
	}

	

		//for (int i = 0; i < n; i++) {

		//	factorial = factorial * n--;

		//}

		//return factorial == 0 ? 1 : factorial;
		
	return factorial;
}

double power(double base, unsigned int n) {

	int power = 1;

	for (int i = 1; i <= n; i++) {
		
		power = power * base;
	
	}

	return power;


}
unsigned int fibonacci(unsigned int n) {

	int num = 1; 
	int num2 = 0;
	int num3 = 1;

	for (int i = 1; i < n; i++) {
		
		num = num2 + num3;

		num2 = num3;

		num3 = num;

	}

	if (n == 0) {
		return 0;
	}
	else
		return num;

}

