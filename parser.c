// ============================================
// parser.c - Operator Precedence Parser
// DUAL VISUALIZATION: Grammar Rules + Stack
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

// Global variables for parsing
char inputStr[MAX_INPUT];
int inputPos;
Stack symbolStack;      // Actual stack showing symbols
int stepCount;
int verbose;
int showStack;          // Toggle for stack visualization

// Stack functions
void initStack(Stack *s) { s->top = -1; }

void push(Stack *s, char c) {
    if (s->top < MAX_STACK - 1) s->items[++(s->top)] = c;
}

char pop(Stack *s) {
    return (s->top >= 0) ? s->items[(s->top)--] : '\0';
}

char peek(Stack *s) {
    return (s->top >= 0) ? s->items[s->top] : '\0';
}

int isEmpty(Stack *s) { return (s->top == -1); }

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int isIdentifier(char c) {
    return isalpha(c) || isdigit(c);
}

int getPrecedence(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        default: return 0;
    }
}

// Get current stack content as string
void getStackContent(Stack *s, char *buffer) {
    int idx = 0;
    for (int i = 0; i <= s->top; i++) {
        buffer[idx++] = s->items[i];
    }
    buffer[idx] = '\0';
}

// Display step with stack and grammar rule
void displayDualStep(Stack *stack, char *input, int inputPos, char *grammarRule, char *action) {
    char stackStr[MAX_STACK];
    getStackContent(stack, stackStr);
    
    // Format stack column
    printf("%-25s", stackStr);
    
    // Format input column
    printf("%-20s", &input[inputPos]);
    
    // Format grammar rule column
    printf("%-30s", grammarRule);
    
    // Format action column
    printf("%-10s\n", action);
}

// Original simple display (for quick mode)
void displayStep(Stack *stack, char *input, int inputPos, char *action) {
    printf("%-20s", "");
    for (int i = 0; i <= stack->top; i++) printf("%c", stack->items[i]);
    printf("%-15s", "");
    for (int i = inputPos; input[i] != '\0'; i++) printf("%c", input[i]);
    printf("%-10s%s\n", "", action);
}

// Validation function
int validateExpression(char *expr) {
    int parenCount = 0;
    int lastWasOp = 0;
    
    for (int i = 0; expr[i] != '\0'; i++) {
        char c = expr[i];
        if (c == ' ') continue;
        
        if (c == '(') {
            parenCount++;
            lastWasOp = 0;
        }
        else if (c == ')') {
            parenCount--;
            if (parenCount < 0) return 0;
            lastWasOp = 0;
        }
        else if (isOperator(c)) {
            if (lastWasOp) return 0;
            lastWasOp = 1;
        }
        else if (isIdentifier(c)) {
            lastWasOp = 0;
        }
        else return 0;
    }
    return (parenCount == 0 && !lastWasOp);
}

// ============= GRAMMAR WITH LEFT RECURSION REMOVED =============
// E  → T E'
// E' → + T E' | - T E' | ε
// T  → F T'
// T' → * F T' | / F T' | ε
// F  → id | (E)

// Function declarations
int E();
int E_prime();
int T();
int T_prime();
int F();

char currentChar() {
    return inputStr[inputPos];
}

void advance() {
    if (inputStr[inputPos] != '\0') {
        inputPos++;
    }
    while (inputStr[inputPos] == ' ') {
        inputPos++;
    }
}

// Record parsing step with both grammar rule and stack operation
void recordStep(char *grammarRule, char *action, char stackOp) {
    if (verbose) {
        char stackOpStr[10];
        if (stackOp == 'p') {
            sprintf(stackOpStr, "Pop");
        } else if (stackOp == 's') {
            sprintf(stackOpStr, "Push");
        } else {
            strcpy(stackOpStr, "");
        }
        
        char fullAction[50];
        if (strlen(stackOpStr) > 0) {
            sprintf(fullAction, "%s %s", action, stackOpStr);
        } else {
            strcpy(fullAction, action);
        }
        
        displayDualStep(&symbolStack, inputStr, inputPos, grammarRule, fullAction);
    }
    stepCount++;
}

// F → id | (E)
int F() {
    char grammarRule[50];
    
    if (isIdentifier(currentChar())) {
        sprintf(grammarRule, "F → %c", currentChar());
        recordStep(grammarRule, "Reduce", ' ');
        
        // Push identifier to stack
        push(&symbolStack, currentChar());
        advance();
        return 1;
    }
    else if (currentChar() == '(') {
        sprintf(grammarRule, "F → ( E )");
        recordStep(grammarRule, "Shift", 's');
        push(&symbolStack, '(');
        advance();
        
        if (E()) {
            if (currentChar() == ')') {
                recordStep("F → ( E ) - match", "Match )", 'p');
                pop(&symbolStack); // Pop '('
                push(&symbolStack, 'E'); // Push reduced E
                advance();
                return 1;
            }
        }
    }
    return 0;
}

// T' → * F T' | / F T' | ε
int T_prime() {
    char grammarRule[50];
    
    if (currentChar() == '*') {
        sprintf(grammarRule, "T' → * F T'");
        recordStep(grammarRule, "Shift *", 's');
        push(&symbolStack, '*');
        advance();
        
        if (F()) {
            return T_prime();
        }
        return 0;
    }
    else if (currentChar() == '/') {
        sprintf(grammarRule, "T' → / F T'");
        recordStep(grammarRule, "Shift /", 's');
        push(&symbolStack, '/');
        advance();
        
        if (F()) {
            return T_prime();
        }
        return 0;
    }
    // ε production
    sprintf(grammarRule, "T' → ε");
    recordStep(grammarRule, "Reduce", ' ');
    return 1;
}

// T → F T'
int T() {
    char grammarRule[50];
    sprintf(grammarRule, "T → F T'");
    recordStep(grammarRule, "Start T", ' ');
    
    if (F()) {
        return T_prime();
    }
    return 0;
}

// E' → + T E' | - T E' | ε
int E_prime() {
    char grammarRule[50];
    
    if (currentChar() == '+') {
        sprintf(grammarRule, "E' → + T E'");
        recordStep(grammarRule, "Shift +", 's');
        push(&symbolStack, '+');
        advance();
        
        if (T()) {
            return E_prime();
        }
        return 0;
    }
    else if (currentChar() == '-') {
        sprintf(grammarRule, "E' → - T E'");
        recordStep(grammarRule, "Shift -", 's');
        push(&symbolStack, '-');
        advance();
        
        if (T()) {
            return E_prime();
        }
        return 0;
    }
    // ε production
    sprintf(grammarRule, "E' → ε");
    recordStep(grammarRule, "Reduce", ' ');
    return 1;
}

// E → T E'
int E() {
    char grammarRule[50];
    sprintf(grammarRule, "E → T E'");
    recordStep(grammarRule, "Start E", ' ');
    
    if (T()) {
        return E_prime();
    }
    return 0;
}

// ============= MAIN PARSING FUNCTION WITH DUAL VISUALIZATION =============
void parseExpression(char *input, int verboseMode) {
    // Initialize
    verbose = verboseMode;
    initStack(&symbolStack);
    
    // Clean input
    char temp[MAX_INPUT];
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] != ' ') {
            temp[j++] = input[i];
        }
    }
    temp[j] = '\0';
    strcpy(inputStr, temp);
    
    inputPos = 0;
    stepCount = 0;
    
    if (verbose) {
        printf("\n%s\n", "╔════════════════════════════════════════════════════════════════════════════════╗");
        printf("║                     DUAL VISUALIZATION: STACK + GRAMMAR RULES                          ║\n");
        printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
        printf("Expression: %s\n\n", inputStr);
        printf("%-25s%-20s%-30s%-10s\n", "STACK", "INPUT", "GRAMMAR RULE", "ACTION");
        printf("%-25s%-20s%-30s%-10s\n", "-----", "-----", "-----------", "------");
        
        // Initial step
        char initStackStr[MAX_STACK];
        getStackContent(&symbolStack, initStackStr);
        printf("%-25s%-20s%-30s%-10s\n", initStackStr, inputStr, "Initialize", "Start");
    }
    
    // Push initial $ marker
    push(&symbolStack, '$');
    
    // Start parsing
    int result = E();
    
    // Check if entire input consumed
    if (result && inputStr[inputPos] == '\0') {
        // Final reduction to single E
        while (symbolStack.top > 1) {
            char rule[50] = "Final Reduce";
            char action[50] = "Reduce to E";
            if (verbose) {
                char stackStr[MAX_STACK];
                getStackContent(&symbolStack, stackStr);
                printf("%-25s%-20s%-30s%-10s\n", stackStr, "", rule, action);
            }
            pop(&symbolStack);
        }
        
        if (verbose) {
            char finalStack[MAX_STACK];
            getStackContent(&symbolStack, finalStack);
            printf("%-25s%-20s%-30s%-10s\n", finalStack, "", "Accept", "Success");
            printf("\n%s\n", "╔════════════════════════════════════════════════════════════════════════════════╗");
            printf("║                                   ACCEPTED!                                           ║\n");
            printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
            printf("✅ EXPRESSION ACCEPTED!\n");
            printf("Total parsing steps: %d\n", stepCount);
        } else {
            printf("\n✅ EXPRESSION ACCEPTED!\n");
        }
    } else {
        if (verbose) {
            printf("\n%s\n", "╔════════════════════════════════════════════════════════════════════════════════╗");
            printf("║                                   REJECTED!                                           ║\n");
            printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
            printf("❌ ERROR: Invalid expression\n");
            if (inputStr[inputPos] != '\0') {
                printf("   Unexpected character '%c' at position %d\n", 
                       inputStr[inputPos], inputPos);
            }
        } else {
            printf("\n❌ EXPRESSION REJECTED!\n");
        }
    }
}

// Print grammar rules
void printGrammar(void) {
    printf("\n%s\n", "╔════════════════════════════════════════════════════════════════════════════════╗");
    printf("║                              GRAMMAR (Left Recursion Removed)                           ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("  E  → T E'\n");
    printf("  E' → + T E' | - T E' | ε\n");
    printf("  T  → F T'\n");
    printf("  T' → * F T' | / F T' | ε\n");
    printf("  F  → id | (E)\n");
    printf("\n");
    printf("  where 'id' represents identifiers (a-z, A-Z, 0-9)\n");
    printf("\n");
}

// Print precedence table
void printPrecedenceTable(void) {
    printf("\n%s\n", "╔════════════════════════════════════════════════════════════════════════════════╗");
    printf("║                           OPERATOR PRECEDENCE TABLE                                      ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("  Operator | Precedence | Associativity\n");
    printf("  ---------+------------+--------------\n");
    printf("     +     |     1      |  Left-to-Right\n");
    printf("     -     |     1      |  Left-to-Right\n");
    printf("     *     |     2      |  Left-to-Right\n");
    printf("     /     |     2      |  Left-to-Right\n");
    printf("     (     |  Highest   |  - \n");
    printf("     )     |  Special   |  - \n");
    printf("\n");
    printf("  Precedence Rules:\n");
    printf("  • * and / have higher precedence than + and -\n");
    printf("  • All operators are left-associative\n");
    printf("  • Parentheses () override normal precedence\n");
    printf("\n");
}
