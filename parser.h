#ifndef PARSER_H
#define PARSER_H

#define MAX_STACK 100
#define MAX_INPUT 100

// Stack structure for symbols
typedef struct {
    char items[MAX_STACK];
    int top;
} Stack;

// Structure to track both grammar and stack
typedef struct {
    char grammarRule[MAX_STACK];
    char stackContent[MAX_STACK];
    char remainingInput[MAX_INPUT];
    char action[MAX_STACK];
} ParseStep;

// Function prototypes
void initStack(Stack *s);
void push(Stack *s, char c);
char pop(Stack *s);
char peek(Stack *s);
int isEmpty(Stack *s);
int isOperator(char c);
int isIdentifier(char c);
int getPrecedence(char op);
void parseExpression(char *input, int verbose);
void displayStep(Stack *stack, char *input, int inputPos, char *action);
void displayDualStep(Stack *stack, char *input, int inputPos, char *grammarRule, char *action);
int validateExpression(char *expr);
void printPrecedenceTable(void);
void printGrammar(void);

#endif
