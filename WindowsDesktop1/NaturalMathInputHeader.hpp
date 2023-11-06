#pragma once


#include "WindowsHfiles.hpp"
//#include <stdio.h>
//#include <math.h>

//#include <time.h>


// #defines

#define MAX_OPER 1000		// amount paranthesis increase precedence
#define MAX_INP 5			// maximum number of inputs a function can have
#define MAX_POS 3			// the position of the number as seen from bottom 3
	#define POST_FIX 2		// 
	#define IN_FIX   1		//
	#define PRE_FIX  0		//
#define VAR_X 0				// temporary for while only one variable supported

// operator precedence with additional information such as # of inputs and pre, in, post status
#define OPERATOR_PRECEDENCE(name) enum name {	\
	DEF_OPE(NAO, 		0, PRE_FIX),			\
	DEF_OPE(PLUS, 		2, IN_FIX),				\
	DEF_OPE(MINUS, 		2, IN_FIX),				\
	DEF_OPE(MULTIPLY, 	2, IN_FIX),				\
	DEF_OPE(DIVIDE, 	2, IN_FIX),				\
	DEF_OPE(POSITIVE, 	1, PRE_FIX),			\
	DEF_OPE(NEGATIVE, 	1, PRE_FIX),			\
	DEF_OPE(EXPONENT, 	2, IN_FIX),				\
	DEF_OPE(SIN, 		1, PRE_FIX),			\
	DEF_OPE(COS, 		1, PRE_FIX),			\
	DEF_OPE(TAN, 		1, PRE_FIX),			\
	DEF_OPE(LN,			1, PRE_FIX),			\
	DEF_OPE(FACTORIAL, 	1, POST_FIX),			\
	DEF_OPE(DETERMINANT,4, PRE_FIX),			\
};

// the following are various macro abuses because I'm too lazy to order them myself
// vscode does not like this however, and may show some errors
#define DEF_OPE(oper,inp,pos) pabcd ## oper
OPERATOR_PRECEDENCE(primitive_Operation)
#undef DEF_OPE
#define DEF_OPE(oper,inp,pos) oper = (pabcd ## oper * MAX_POS + pos) * MAX_INP + inp
OPERATOR_PRECEDENCE(Operation)

//to find "original" operator
#define ORI_OPER(inp) inp % MAX_OPER 
// to find the paranthesis depth
#define PAR_OPER(inp) inp / MAX_OPER 
// to find the number of inputs of the operation
#define INP_OPER(inp) inp % MAX_OPER % MAX_INP 
// to find the position (post,in,pre) of the operation
#define POS_OPER(inp) inp % MAX_OPER / MAX_INP % MAX_POS 


// enum, struct declarations

enum recFuncType {
	NaT,		//Not a Type
	NUMBER,		//number   (float)
	VARIABLE,	//variable (float *)
	FUNCTION	//function (recFunc *)
};

struct inpStru{
	enum recFuncType type = NaT; // what kind of input
	union inpTypes{
		float num;					// the number
		float * var;				// the memory address of the variable
		struct recFunc * func;		// the memory address of the output of the function
	} inp; // the storage for the input
};

// the main recursive structure.
struct recFunc{
	int operation = NAO; 			// what operation is stored (operation * MAX_INPUT + inputs) + MAX_OPER * Parantheses
	struct recFunc * linked = NULL; // who has the output of this function
	struct inpStru * inps = NULL; 	// the inputs of this function
};

struct Func {
	struct recFunc * nestFunc = NULL;
	int lengthNestFunc = -1;
	float * vars = NULL;
	int lengthVars = -1;
};

struct nmi_point {
	float x = 0;
	float y = 0;
};

// function declarations

// compiles the input string into a function structure
struct Func * compileFunc(int length, char * input, int time, int debug);
// evaluates the function structure at the point x
float evalFunct(struct Func * f1, float * x, int lengthVarial);
float evalFunct(struct Func* f1, float x);

//prints funct
void printFunct(struct recFunc * nestFunc, std::string logFile);
struct recFunc *  findHeadFunct(struct recFunc * nestFunc);

// frees all the data from compileFunc. please call this
void freeFunc(struct Func * f1, int debug);
// graphs in console
void graph(struct Func * f1);




// test times
std::string timeToDate(std::time_t input);