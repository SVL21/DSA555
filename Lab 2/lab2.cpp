unsigned int factorial (unsigned int n){

	int rc = 1;

	if (n != 0) {

		rc = n * factorial(n - 1);

	}
	return rc;

}

double power (double base, unsigned int n){

	int rc = 1;

	if (n == 0) {

		return rc;

	}
	else{
		
		rc = base * power(base, n - 1);

	}

	return rc;

}

unsigned int fibonacci (unsigned int n){

	if (n == 0) {
		
		return 0;
	
	}

	else if (n == 1) {
		
		return 1;
	
	}
	else {
	
		return fibonacci(n - 1) + fibonacci(n - 2);

	}
	
}