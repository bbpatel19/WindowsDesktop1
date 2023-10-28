// gcc MathInputlib.cpp -lm
// -lm to link the math library for math functions
// gcc NaturalMathInputMain.cpp NaturalMathInputFunc.cpp -lm (-g for gdb)

// time in program. look at memory usage with "memusage -t ./a.out"
// valgrind --leak-check=full --show-leak-kinds=all -s ./a.out


#include "NaturalMathInputHeader.hpp"
// go through rename stuff to use the NMI_ prefix for natural math input
// have to clean up variables
// introduce a safe checker to run before compile to make sure its a semi valid function... (inputs as well)

// I want to try a small rpn style at some point down the line
// some constants??? e pi
// for compile func implement prune constant trees.
// may also reintroduce comments and actual documentation



float quickFunc(int length, char * input) {
	struct Func * f1 = compileFunc(length, input, 0, 0);
	float output = evalFunct(f1, NULL, 0);
	freeFunc(f1, 0);
	return output;
}

int quickTests(int debug) {
	char test0[20] = "1+1\n               ";	float answ0 = 1.0+1.0;
	char test1[20] = "120.234-120.234\n   ";	float answ1 = 120.234-120.234;
	char test2[20] = "1-2*3/4\n           ";	float answ2 = 1.0-2.0*3.0/4.0;
	char test3[20] = "(1+2)*3\n           ";	float answ3 = (1.0+2.0)*3.0;
	char test4[20] = "((1+2)*(3+4))\n     ";	float answ4 = ((1.0+2.0)*(3.0+4.0));
	char test5[20] = "((1+2)*(3+4)+5)\n   ";	float answ5 = ((1.0+2.0)*(3.0+4.0)+5.0);
	char test6[20] = "((1/(2+3))*5)\n     ";	float answ6 = ((1.0/(2.0+3.0))*5.0);
	char test7[20] = "(5*(1/(2+3)))\n     ";	float answ7 = (5.0*(1.0/(2.0+3.0)));
	char test8[20] = "n2+n2\n             ";	float answ8 = -2.0+-2.0;
	char test9[20] = "s1/c1\n             ";	float answ9 = sin(1)/cos(1);
	char testa[20] = "1+nc1.5*5.0\n       ";	float answa = 1+-cos(1.5)*5.0;
	char testb[20] = "1+ns1.5/nc1.5*5.0\n ";	float answb = 1+-sin(1.5)/-cos(1.5)*5.0;
	char testc[20] = "c2!\n               ";	float answc = cos(2*1); // explicit here

	char * test[13] = {test0, test1, test2, test3, test4, test5, test6, test7, test8, test9, testa, testb, testc};
	float answ[13] =  {answ0, answ1, answ2, answ3, answ4, answ5, answ6, answ7, answ8, answ9, answa, answb, answc};
	int i = 0;
	for (i = 0; i < 13; i++) {
		if (0) printf("temptest%d\n", i);
		if (answ[i] != quickFunc(20, test[i])) {
			break;
		}
	}
	if (i < 10 && debug) {
		printf("Error in Test#%d: %s =/= %f\n", i, test[i], answ[i]);
		return i;
	}
	return -1;
}

void pointGet(struct Func * f1, int debug) {
	char * varEval = (char *) calloc(20, sizeof(char));
	printf("You entered a function of %d variables. Please enter a set of values you would like to evaluate\n", f1->lengthVars);
	fgets(varEval, 20, stdin);
	if (debug) printf("You entered \n");
	float number = 0;
	float decimalHit = 0;
	int negative = 1;
	int  varial = 0;
	float * varials = (float *) calloc(f1->lengthVars, sizeof(float));
	for (int i = 0; i < 20; i++) {
		if (debug) printf("%c", varEval[i]);
		if (varEval[i] == '.') decimalHit = 1;
		if (varEval[i] == '-') negative = -1;
		if (varEval[i] >= '0' && varEval[i] <= '9') {
			if (decimalHit) {
				number = number + (varEval[i] - '0') * pow(10, -decimalHit++);
			} else {
				number = number * 10 + (varEval[i] - '0');
			}
			if (debug) printf("number = %lf", number);
		}
		if (varEval[i] == ',') {
			if (debug) printf("%lf read and stored", number * negative);
			varials[varial++] = number * negative;
			decimalHit = 0;
			negative = 1;
			number = 0;
		}
		if (varEval[i] == '\n') {
			if (debug) printf("%lf read and stored", number * negative);
			varials[varial++] = number * negative;
			number = 0;
			break;
		}
	}
	
	printFunct(findHeadFunct(f1->nestFunc));
	printf(" evaluated at (");
	for (int i = 0; i < varial; i++) {
		if (i) printf(", ");
		printf("%f", varials[i]);
	}
	printf(") is %f\n", evalFunct(f1, varials, varial));
	free(varEval);
}

// remember to free your mallocs
char * getInput(int * maxLength) {
	char * iniInput = (char *) calloc(*maxLength, sizeof(char));
	fgets(iniInput, *maxLength, stdin);
	int i = 0;
	for (i = 0; i < *maxLength; i++) {
		if (iniInput[i] == '\n') {
			i++;
			break;
		}
	}

	if (i == *maxLength) {
		printf("Too big input\n");
		free(iniInput);
		return NULL;
	}
	*maxLength = i;
	char * input = (char *) calloc(*maxLength, sizeof(char));
	for (i = 0; i < *maxLength; i++) {
		input[i] = iniInput[i];
	}
	free(iniInput);
	iniInput = NULL;
	return input;
}

int getTypeInput(char * input, int length) {
	// what commands? graph. evaluate, define, 
	if (input[0] == 'g') {
		return 1; // graph corresponding function
	}
	if (input[0] == 'f') {
		return 2; // function definition
	}
	if (input[0] == 'F') {
		return 3; // function evaluation
	}
	return 0;
}
/*
int main(void) {
	//clock_t start_t, end_t, start_1t, end_1t;
	//start_1t = clock();
	quickTests(1);
	char Bigresponse = 'Y';
	//printf("Welcome to Natural Math Input. Please enter an expression\n");

	do {

		printf("Please enter your expression. +,-,/,*,^,n(egative),p(ositive),s(in),c(os),t(an)\n");
		int length = 100; // characters given for user input
		char * input = getInput(&length);
		//free(input); return 0; // uncomment for no user input;
		//end_1t = clock();
		//start_t = clock();
		// the three functions...
		struct Func * f1 = compileFunc(length, input, 0, 1);
		free(input);
		if (f1->lengthVars > 0 ) {
			char response = 'Y';
			do {
				pointGet(f1, 0);
				printf("Would you like to enter another point?(Y/N)\n");
				scanf("%c", &response);
				getchar();
				printf("----------------------------------\n");
			} while (response == 'Y' || response == 'y');
		} else {	
			printf("The expression evaluated is : %f\n", evalFunct(f1, NULL, 0));
		}

		freeFunc(f1, 0);

		printf("Would you like to enter another expression?(Y/N)\n");
		
		scanf("%c", &Bigresponse);
		getchar(); // scanf leaves the '\n' in stdin so fgets doesn't like it
		//graph(f1);
		printf("----------------------------------\n");
	} while (Bigresponse == 'Y' || Bigresponse == 'y');


	//end_t = clock();
	//long unsigned int times = (end_t - start_t) + (end_1t - start_1t);
	//printf("This took %ld clock cycles or %lf milliseconds (not counting time for user input)\n", times, times / (double) CLOCKS_PER_SEC * 1000);
	return 0;
}

*/