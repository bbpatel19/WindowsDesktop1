#include "NaturalMathInputHeader.hpp"

std::string timeToDate(std::time_t input) {
	struct tm timeInput;
	localtime_s(&timeInput, &input);
	int year = timeInput.tm_year + 1900; // XXXX
	int month = timeInput.tm_mon + 1; // 01-12
	int day = timeInput.tm_mday; // 01-31
	int hour = timeInput.tm_hour; // 00-23
	int min = timeInput.tm_min; // 00-59
	int sec = timeInput.tm_sec; // 00-60
	
	std::string dateFrmt = {};
	
	dateFrmt.append(std::to_string(year));
	dateFrmt.append(1u, '_');
	dateFrmt.append(std::to_string(month));
	dateFrmt.append(1u, '_');
	dateFrmt.append(std::to_string(day));
	dateFrmt.append(1u, '_');
	dateFrmt.append(std::to_string(hour));
	dateFrmt.append(1u, '_');
	dateFrmt.append(std::to_string(min));
	dateFrmt.append(1u, '_');
	dateFrmt.append(std::to_string(sec));
	return dateFrmt;
}

// what operations are accepted and have valid meanings
char isAcceptableOperation(int input) {
	switch(ORI_OPER(input)) {
		case PLUS:
			return '+';
		case MINUS:
			return '-';
		case MULTIPLY:
			return '*';
		case DIVIDE:
			return '/';
		case EXPONENT:
			return '^';
		case SIN:
			return 's';
		case COS:
			return 'c';
		case TAN:
			return 't';
		case NEGATIVE:
			return 'n';
		case POSITIVE:
			return 'p';
		case FACTORIAL:
			return '!';
		case DETERMINANT:
			return 'd';
		case LN:
			return 'l';
		case NAO:
		default:
			return 0;
	}
}

// what operations are accepted and have valid meanings (char input)
enum Operation isAcceptableOperationChar(char input) {
	switch(input) {
		case '+':
			return PLUS;
		case '-':
			return MINUS;
		case '*':
			return MULTIPLY;
		case '/':
			return DIVIDE;
		case '^':
			return EXPONENT;
		case 's':
			return SIN;
		case 'c':
			return COS;
		case 't':
			return TAN;
		case 'p':
			return POSITIVE;
		case 'n':
			return NEGATIVE;
		case '!':
			return FACTORIAL;
		case 'd':
			return DETERMINANT;
		case 'l':
			return LN;
		default:
			return NAO;
	}
}

//------------------------------------------------------------------------------------------------------------------------------------
// primitive definition of the factorial function. all values <=0 are automatically equal to 1. also casts to integer before hand
int nm_Factorial(int input) {
	return (input <= 0) ? 1 : input * nm_Factorial(input - 1);
}

// simple 2x2 determinant 
float nm_determinant4(float a, float b, float c, float d) {
	return a * d - b * c;
}

float nm_ln(float input) {
	return log(input);
}
//------------------------------------------------------------------------------------------------------------------------------------
// prints from the given function down the recursive chain
void printnlogFunct(struct recFunc * nestFunc, std::ofstream * logFile) {
	//if (POS_OPER(nestFunc->operation) == PRE_FIX) printf("%c", isAcceptableOperation(nestFunc->operation));
	if (POS_OPER(nestFunc->operation) == PRE_FIX) *logFile << isAcceptableOperation(nestFunc->operation);
	*logFile << "(";
	//printf("(");
	for (int i = 0; i < INP_OPER(nestFunc->operation); i++) {
		//if (i) (POS_OPER(nestFunc->operation) != IN_FIX) ? printf(", ") : printf(" %c ", isAcceptableOperation(nestFunc->operation));
		if (i) (POS_OPER(nestFunc->operation) != IN_FIX) ? *logFile << ", " : *logFile << ' ' << isAcceptableOperation(nestFunc->operation) << ' ';
		switch(nestFunc->inps[i].type) {
			case NUMBER:
				//printf("%f", nestFunc->inps[i].inp.num);
				*logFile << nestFunc->inps[i].inp.num;
				break;
			case VARIABLE:
				//printf("x"); // can fix logic later
				*logFile << "x";
				break;
			case FUNCTION: // recursion central
				printnlogFunct(nestFunc->inps[i].inp.func, logFile);
				break;
		}
	}
	//printf(")");
	*logFile << ")";
	//if (POS_OPER(nestFunc->operation) == POST_FIX) printf("%c", isAcceptableOperation(nestFunc->operation));
	if (POS_OPER(nestFunc->operation) == POST_FIX) *logFile << isAcceptableOperation(nestFunc->operation);
}

// sets up log file for printing
void printFunct(struct recFunc* nestFunc, std::string logFile) {
	std::ofstream myfile;
	myfile.open(logFile, std::ofstream::app);
	std::string dateID = timeToDate(std::time(0));
	myfile << "[" << dateID.c_str() << "]";
	myfile << " PrintFunct: ";
	if (myfile.is_open()) {
		printnlogFunct(nestFunc, &myfile);
	}
	myfile << std::endl;
	myfile.close();
	return;
}

// calculates the entire chain from nestFunc down. (all float)
float FunctToFloat(struct recFunc * nestFunc) {
	float x[MAX_INP] = {0};

	for (int i = 0; i < INP_OPER(nestFunc->operation); i++) {
		switch(nestFunc->inps[i].type) {
			case FUNCTION:
				x[i] = FunctToFloat(nestFunc->inps[i].inp.func);
				break;
			case VARIABLE:
				x[i] = *nestFunc->inps[i].inp.var;
				break;
			case NUMBER:
				x[i] = nestFunc->inps[i].inp.num;
				break;
		}
	}

	switch(isAcceptableOperation(nestFunc->operation)) {
		case '+':
			return x[0] + x[1];
		case '-':
			return x[0] - x[1];
		case '*':
			return x[0] * x[1];
		case '/':
			return x[0] / x[1];
		case '^':
			return pow(x[0], x[1]);
		case 's':
			return sin(x[0]);
		case 'c':
			return cos(x[0]);
		case 't':
			return tan(x[0]);
		case 'n':
			return -x[0];
		case 'p':
			return +x[0];
		case '!':
			return nm_Factorial(x[0]);
		case 'd':
			return nm_determinant4(x[0], x[1], x[2], x[3]);
		case 'l':
			return nm_ln(x[0]);
		default:
			return x[0];
	}
}

// find the head Funct of the given nestfunc
struct recFunc *  findHeadFunct(struct recFunc * nestFunc) {
	return (nestFunc->linked) ? findHeadFunct(nestFunc->linked): nestFunc;
}

// given a memory adress and the corresponding func finds the index
int findIndexFunc(struct Func * f1, struct recFunc * nestFunc) {
	for (int i = 0; i < f1->lengthNestFunc; i++) {
		if (&f1->nestFunc[i] == nestFunc) return i;
	}
	return -1;
}

//------------------------------------------------------------------------------------------------------------------------------------

//removes spaces in string input
int rmSpaces(char * input, int length, char * output, int debug) {
	int space = 0;
	int i = 0;
	for(i = 0; i < length; i++) {
		if (input[i] == ' ') {
			continue;
			space++;
		}
		if (output) output[i-space] = input[i];
        if (debug && output) printf("%c", output[i]);
		if (input[i] == '\n') {
			break;
		}
	}
	if (debug) printf("\nwhere %d was reduced to %d\n", length, i + 1 - space);
	return i + 1 - space;
}

//checks formatting (paras)
int chkFormat(char * input, int length, int debug) {
	int paras = 0;
	for(int i = 0; i < length; i++) {
		if (input[i] == '(') paras++;
		if (input[i] == ')') paras--;
	}
	if (debug) printf("%d missing parantheses\n", abs(paras));
	return paras;
}

//remember to free your mallocs.
void prepInputFunc(char * input, int lengthInput, struct Func * f1, int debug) {
	int operations = 0;
	int variables = 0;
	for (int i = 0; i < lengthInput; i++) {
		if (isAcceptableOperationChar(input[i])) operations++;
		if (input[i] == 'x') variables++; // can come back to fix later
	}

	if (operations == 0) { 
		operations++; // by default one
	}
	f1->nestFunc = (struct recFunc * ) calloc(operations, sizeof(struct recFunc));
	if (debug) printf("mallocing %d operations\n", operations);
	if (f1->nestFunc) f1->lengthNestFunc = operations;

	if (variables) {
		f1->vars = (float *) calloc(variables, sizeof(float));
		if (debug) printf("mallocing %d variables\n", variables);
		if (f1->vars) f1->lengthVars = variables;
	}
}

//------------------------------------------------------------------------------------------------------------------------------------
// updates number based on if a decimal has been hit and the given character input
void hlpAssignRunningNumber(struct inpStru * readInp, char input, int * decimalHit, int debug) {
	if (readInp->type != NUMBER) {
		readInp->type = NUMBER;
		readInp->inp.var = NULL;
	} // 0 it out, possible loss of data
	if (*decimalHit) {
		readInp->inp.num += (input - '0') * pow(10, -(*decimalHit)++);
	} else {
		readInp->inp.num = 10 * readInp->inp.num + (input - '0');
	}
	if (debug) printf("number = %f	", readInp->inp.num);
}

// assigns the character to the operation
void hlpAssignCharOper(struct Func * f1, int currRela, int operation, int debug) {
	if (f1->nestFunc[currRela].inps != NULL) printf("error");
	f1->nestFunc[currRela].operation = operation;
	f1->nestFunc[currRela].inps = (struct inpStru * )
	calloc(INP_OPER(f1->nestFunc[currRela].operation), sizeof(struct inpStru));
	//if (debug) printf("mallocing %d inps	", INP_OPER(f1->nestFunc[currRela].operation));
	if (debug) printf("nestFunc[%d] = %c	", currRela, isAcceptableOperation(operation));
}

// assigns the apropriate input to nestFunc[currRela].inp1 or inp2 ... returns a 1 if a complete functino is formed
int hlpAssignFunc(struct Func * f1, int currRela, struct inpStru * inpStru, int debug) {
	if (currRela >= 0) { // make sure it is a valid index

		// find which inp to assign
		int inp12 = 0;
		while(f1->nestFunc[currRela].inps[inp12].type != NaT) {
			inp12++;
			if (inp12 >= INP_OPER(f1->nestFunc[currRela].operation)) {
				return 0; // no valid inps left to assign
			}
		}

		// find what kind of inp it is and do accordingly
		if (inpStru->type == VARIABLE) {
			f1->nestFunc[currRela].inps[inp12].type = VARIABLE;
			f1->nestFunc[currRela].inps[inp12].inp.var = inpStru->inp.var;
			if (debug) printf("x is nestFunc[%d].inp%d	", currRela, inp12);
			inpStru->type = NaT;
			inpStru->inp.var = NULL;
		} else if (inpStru->type == FUNCTION) {
			inpStru->inp.func = findHeadFunct(inpStru->inp.func);
			f1->nestFunc[currRela].inps[inp12].type = FUNCTION;
			f1->nestFunc[currRela].inps[inp12].inp.func = inpStru->inp.func;
			if (debug) printf("nestFunc[%d] is nestFunc[%d].inp%d	", findIndexFunc(f1, inpStru->inp.func), currRela, inp12);
			inpStru->inp.func->linked = &f1->nestFunc[currRela];
			inpStru->type = NaT;
			inpStru->inp.var = NULL;
		} else if (inpStru->type == NUMBER) {
			f1->nestFunc[currRela].inps[inp12].type = NUMBER;
			f1->nestFunc[currRela].inps[inp12].inp.num = inpStru->inp.num;
			if (debug) printf("number is nestFunc[%d].inp%d	", currRela, inp12);
			inpStru->type = NaT;
			inpStru->inp.var = NULL;
		}

		if (f1->nestFunc[currRela].inps[INP_OPER(f1->nestFunc[currRela].operation) - 1].type != NaT) {
			inpStru->type = FUNCTION;
			inpStru->inp.func = &f1->nestFunc[currRela];
			if (debug) printf("nestFunc[%d] now completed	", currRela);
			return 1;
		}
	}
	return 0;
}

// helps finds the first operation in the current parantehsis depth that does not have all of its inputs filled.
int hlpAssignLastOper(struct Func * f1, int currRela, int paraIntense) {
	int tempPrevOpe = currRela - 1;
	while (tempPrevOpe >= 0 && f1->nestFunc[tempPrevOpe].inps[INP_OPER(f1->nestFunc[tempPrevOpe].operation) - 1].type != NaT) {
		tempPrevOpe--;
	}
	if (tempPrevOpe < 0 || PAR_OPER(f1->nestFunc[tempPrevOpe].operation) != (paraIntense < 0 ? 0 : paraIntense) / MAX_OPER) {
		tempPrevOpe = -1;
	}
	return tempPrevOpe;
}

// sets f1 with string "input" ... infix notation is awful
int setFuncInputNoPara(char * input, int length, struct Func * f1, int debug) { // this is hell and doesn't make sense.
	int prevOpe = -1; // the previous operation to compare precedence too
	int currRela = 0; // the current operation
	int decimalHit = 0;
	int paraIntense = 0;
	struct inpStru readInp = {NaT};
	
    if (debug) printf("char	currRela	prevRela	notes");
	for (int i = 0; i < length; i++) {
		if (debug) {
			printf("\n");
			(input[i] == '\n') ? printf("\\n") : printf("%c", input[i]);
			printf("	%d		%d		", currRela, prevOpe);
		}
		if (isAcceptableOperationChar(input[i])) {
			// set up the operation's inputs and character
			hlpAssignCharOper(f1, currRela, isAcceptableOperationChar(input[i]) + paraIntense, debug);
			// consider if higher or lower precedence then the preceding operators.
			do {
				prevOpe = hlpAssignLastOper(f1, currRela, paraIntense);
				if (POS_OPER(f1->nestFunc[currRela].operation) != PRE_FIX && (prevOpe < 0 || f1->nestFunc[prevOpe].operation < f1->nestFunc[currRela].operation)) {
					prevOpe = currRela;
				} else if (prevOpe >= 0 && POS_OPER(f1->nestFunc[prevOpe].operation) != POST_FIX){
					// do nothing
				} else {
					readInp.type = NaT;
					readInp.inp.var = NULL;
					break;
				}
			} while (prevOpe >= 0 && hlpAssignFunc(f1, prevOpe, &readInp, debug));
			decimalHit = 0;
			currRela++;
			continue;
		}
		switch(input[i]) {
			case '(':
				paraIntense += MAX_OPER;
				break;
			case ')':
				paraIntense -= MAX_OPER;
				do {
					prevOpe = hlpAssignLastOper(f1, currRela, paraIntense + MAX_OPER);
					if (debug) printf("prevOpe:%d	",prevOpe);
					if (prevOpe < 0) break;
					if (POS_OPER(f1->nestFunc[prevOpe].operation) == POST_FIX) continue;
				} while (hlpAssignFunc(f1, prevOpe, &readInp, debug));
				break;
			case ',':
			{	
				do {
					prevOpe = hlpAssignLastOper(f1, currRela, paraIntense);
					if (debug) printf("prevOpe:%d	",prevOpe);
					if (prevOpe < 0) break;
					if (POS_OPER(f1->nestFunc[prevOpe].operation) == POST_FIX) continue;
				} while (hlpAssignFunc(f1, prevOpe, &readInp, debug));
				
				if (debug) printf("Setting argument");
				prevOpe = hlpAssignLastOper(f1, currRela, paraIntense + MAX_OPER * -1); // one last one for good measure
				hlpAssignFunc(f1, prevOpe, &readInp, debug);

				break;
			}
			case '.':
				decimalHit = 1;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': 
				if (readInp.type == VARIABLE) {
					readInp.inp.var = &f1->vars[VAR_X + (input[i] - '0' + 1)];
				} else {
					hlpAssignRunningNumber(&readInp, input[i], &decimalHit, debug);
				}
				break;
			case 'x':
				readInp.type = VARIABLE;
				readInp.inp.var = &f1->vars[VAR_X];
                break;
		}
	}
	do {
		prevOpe = hlpAssignLastOper(f1, currRela, paraIntense);
		if (prevOpe < 0) break;
		if (POS_OPER(f1->nestFunc[prevOpe].operation) == POST_FIX) continue;
	} while (hlpAssignFunc(f1, prevOpe, &readInp, debug));
    return 1;
}

//------------------------------------------------------------------------------------------------------------------------------------

// main function for user input (if input null asks user...) (contains all other functions...)
struct Func * compileFunc(int lengthFULL, char * inputFULL, int time, int debug) {

	if (chkFormat(inputFULL, lengthFULL, debug)) {
		return NULL;
	}

	if (debug) printf("------------------------\n");
	clock_t start_t, end_t;
	if (time) start_t = clock();

	// gets rid of all spaces in the input
	int length = rmSpaces(inputFULL, lengthFULL, NULL, 0);
	char * input = (char *) calloc(length, sizeof(char));
	rmSpaces(inputFULL, lengthFULL, input, debug);

	//sets up f1 with user function
	struct Func * f1 = (struct Func *) calloc(1, sizeof(struct Func));
	prepInputFunc(input, length, f1, debug);
	setFuncInputNoPara(input, length, f1, debug);
	free(input);
	
	if (debug) printf("\n------------------------\n");

	if (debug) printf("You wanted to find \n");
	//if (debug) printFunct(findHeadFunct(f1->nestFunc));
	if (debug) printf("\n");

	if (time) end_t = clock();
	if (time) printf("The time taken to do this was %ld-%ld=%ld clock cycles\n", end_t,  start_t, end_t - start_t);

	return f1;
}

// is meant to evaluate Funct
float evalFunct(struct Func * f1, float * x, int lengthVarial) {
	if (x != NULL) {
		int length = f1->lengthVars < lengthVarial ? f1->lengthVars : lengthVarial;	
		for (int i = 0; i < length; i++) {
			f1->vars[i] = x[i];
		}
	}
	return FunctToFloat(findHeadFunct(f1->nestFunc));
}

float evalFunct(struct Func* f1, float x) {
	if (f1->lengthVars != 0) {
		f1->vars[0] = x;
	}
	return FunctToFloat(findHeadFunct(f1->nestFunc));
}

//frees all functions
void freeFunc(struct Func * f1, int debug) {
	for(int i = 0; i < f1->lengthNestFunc; i++) {
		free(f1->nestFunc[i].inps);
		if (debug) printf("freed nestFunc[%d].inps	",i);
	}
	free(f1->nestFunc);
	if (debug) printf("freed nestFunc	");
	free(f1->vars);
	if (debug) printf("freed vars	");
	free(f1);
	if (debug) printf("freed f1	\n");
	return;
}

// graphs in console
void graph(struct Func * f1) {
	// -5 to 5 on both axis
	char screen[100 * 100] = {0};

	for (int i = 0; i < 100 * 100; i++) {
		screen[i] = ' ';
	}

	for (int i = 0; i < 100; i++) {
		float x = (i - 50.0) / 50.0 * 5.0;
		float y = evalFunct(f1, &x, 1);
		y = y / -5.0 * 50.0 + 50.0;
		if (y > 0 && y < 100) screen[i + 100 * (int) y] = '.';
	}

	for (int i = 0; i < 100; i++) {
		screen[i + 50 * 100] = '-';
		screen[50 + i * 100] = '|';
	}

	for (int j = 0; j < 100; j++) {
		for (int i = 0; i < 100; i++) {
			printf("%c", screen[i + j * 100]);
		}
		printf("\n");
	}
}

//------------------------------------------------------------------------------------------------------------------------------------