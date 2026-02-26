// ============================================
// parser.h - Operator Precedence Parser Header
// ============================================
// Project: Operator Precedence Parser Visualizer
// Authors: Niraj Kumar Maurya (2408390109005)
//          Meraj Ahamad (2408390109004)
// Department of Computer Science and Engineering
// Rajkiya Engineering College, Kannauj
// ============================================

#ifndef PARSER_H
#define PARSER_H

#define MAX_STACK 100
#define MAX_INPUT 100

// Stack structure
typedef struct {
    char items[MAX_STACK];
    int top;
} Stack;

// Function prototypes
void initStack(Stack *s);
void push(Stack *s, char c);
char pop(Stack *s);
char peek(Stack *s);
int isEmpty(Stack *s);
int getPrecedence(char op);
int isOperator(char c);
int isIdentifier(char c);
int comparePrecedence(char stackTop, char input);
void parseExpression(char *input, int verbose);
void displayStep(Stack *stack, char *input, int inputPos, char *action);
int validateExpression(char *expr);
void printPrecedenceTable(void);

#endif
