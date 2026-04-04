#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void displayMenu(void) {
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    OPERATOR PRECEDENCE PARSER VISUALIZER                         ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Developed by:                                                                  ║\n");
    printf("║  Niraj Kumar Maurya (2408390109005)                                            ║\n");
    printf("║  Meraj Ahamad (2408390109004)                                                   ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Department of Computer Science and Engineering                                 ║\n");
    printf("║  Rajkiya Engineering College, Kannauj                                           ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
}

void displayOptions(void) {
    printf("\n%s\n", "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    printf("                             M E N U\n");
    printf("%s\n", "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    printf("  [1] Parse expression (DUAL VISUALIZATION - Stack + Grammar)\n");
    printf("  [2] View sample expressions\n");
    printf("  [3] View grammar rules\n");
    printf("  [4] View precedence table\n");
    printf("  [5] Quick parse (no visualization)\n");
    printf("  [6] Clear screen\n");
    printf("  [7] Exit\n");
    printf("%s\n", "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    printf("Enter your choice: ");
}

void displaySamples(void) {
    printf("\n%s\n", "╔════════════════════════════════════════════════════════════════════════════════╗");
    printf("║                           SAMPLE EXPRESSIONS                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("  ✅ Valid Expressions:\n");
    printf("  ───────────────────────────────────────────────────────────────────────────────\n");
    printf("    1.  a + b * c\n");
    printf("    2.  (a + b) * c\n");
    printf("    3.  a * (b + c) / d\n");
    printf("    4.  a * b * (b - c) + c\n");
    printf("    5.  ((a + b) * c)\n");
    printf("    6.  a * b + c / d - e\n");
    printf("    7.  (a + b) * (c - d)\n");
    printf("\n");
    printf("  ❌ Invalid Expressions (Will be rejected):\n");
    printf("  ───────────────────────────────────────────────────────────────────────────────\n");
    printf("    8.  a + * b        (consecutive operators)\n");
    printf("    9.  (a + b         (unmatched parenthesis)\n");
    printf("    10. a b + c        (missing operator)\n");
    printf("    11. + a * b        (starting with operator)\n");
    printf("    12. a +            (ending with operator)\n");
    printf("\n");
    printf("  💡 Key Insight: Left recursion was removed to fix infinite loop!\n");
    printf("     Original: E → E + T | T  (caused infinite recursion)\n");
    printf("     Fixed:    E → T E'       (right recursive, works perfectly)\n");
    printf("\n");
}

void getExpression(char *buffer, int size) {
    printf("\n📝 Enter arithmetic expression: ");
    fgets(buffer, size, stdin);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
}

int main() {
    char expression[MAX_INPUT];
    int choice;
    
    displayMenu();
    
    do {
        displayOptions();
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1: // Parse with DUAL visualization
                getExpression(expression, MAX_INPUT);
                
                if (strlen(expression) == 0) {
                    printf("\n⚠️  Empty expression!\n");
                    break;
                }
                
                printf("\n🔍 Validating expression...\n");
                if (!validateExpression(expression)) {
                    printf("\n❌ INVALID EXPRESSION SYNTAX!\n");
                    printf("   Please check:\n");
                    printf("   • Balanced parentheses\n");
                    printf("   • Valid operators (+, -, *, /)\n");
                    printf("   • Proper identifier placement\n");
                    printf("   • No consecutive operators\n");
                    break;
                }
                
                printf("\n✅ Expression syntax is valid!\n");
                char exprCopy[MAX_INPUT];
                strcpy(exprCopy, expression);
                parseExpression(exprCopy, 1);
                break;
                
            case 2: // View samples
                displaySamples();
                break;
                
            case 3: // View grammar
                printGrammar();
                break;
                
            case 4: // View precedence table
                printPrecedenceTable();
                break;
                
            case 5: // Quick parse (no visualization)
                getExpression(expression, MAX_INPUT);
                
                if (strlen(expression) == 0) {
                    printf("\n⚠️  Empty expression!\n");
                    break;
                }
                
                if (!validateExpression(expression)) {
                    printf("\n❌ Invalid expression syntax!\n");
                    break;
                }
                
                char quickCopy[MAX_INPUT];
                strcpy(quickCopy, expression);
                parseExpression(quickCopy, 0);
                break;
                
            case 6: // Clear screen
                system("clear");
                displayMenu();
                break;
                
            case 7: // Exit
                printf("\n%s\n", "╔════════════════════════════════════════════════════════════════════════════════╗");
                printf("║                    Thank you for using the parser! Goodbye!                             ║\n");
                printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
                break;
                
            default:
                printf("\n⚠️  Invalid choice! Please enter 1-7.\n");
        }
        
    } while(choice != 7);
    
    return 0;
}
