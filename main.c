// ============================================
// main.c - Operator Precedence Parser Visualizer
//          Input Interface
// ============================================
// Project: Operator Precedence Parser Visualizer
// Authors: Niraj Kumar Maurya (2408390109005)
//          Meraj Ahamad (2408390109004)
// Department of Computer Science and Engineering
// Rajkiya Engineering College, Kannauj
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// Function to display the main menu
void displayMenu(void) {
    printf("\n%s\n", "╔════════════════════════════════════════════════╗");
    printf("║   OPERATOR PRECEDENCE PARSER VISUALIZER       ║\n");
    printf("╠════════════════════════════════════════════════╣\n");
    printf("║  Developed by:                                 ║\n");
    printf("║  Niraj Kumar Maurya (2408390109005)           ║\n");
    printf("║  Meraj Ahamad (2408390109004)                  ║\n");
    printf("╠════════════════════════════════════════════════╣\n");
    printf("║  Department of Computer Science and Engineering║\n");
    printf("║  Rajkiya Engineering College, Kannauj          ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
}

// Function to display the main options
void displayOptions(void) {
    printf("\n%s\n", "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    printf("                     M E N U\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("  [1] Parse a new expression\n");
    printf("  [2] View sample expressions\n");
    printf("  [3] View precedence table\n");
    printf("  [4] Quick parse (no step display)\n");
    printf("  [5] Clear screen\n");
    printf("  [6] Exit\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("Enter your choice: ");
}

// Function to display sample expressions
void displaySamples(void) {
    printf("\n%s\n", "═══════════════════════════════════════════════");
    printf("           S A M P L E   E X P R E S S I O N S\n");
    printf("═══════════════════════════════════════════════\n");
    printf("  Valid Expressions:\n");
    printf("  ───────────────────────────────────────────\n");
    printf("  1. a + b * c\n");
    printf("  2. (a + b) * c\n");
    printf("  3. a * (b + c) / d\n");
    printf("  4. a + b - c * d / e\n");
    printf("  5. ((a + b) * c)\n");
    printf("  6. a * b + c / d\n");
    printf("  7. (a + b) * (c - d)\n");
    printf("\n  Invalid Expressions:\n");
    printf("  ───────────────────────────────────────────\n");
    printf("  8. a + * b        (consecutive operators)\n");
    printf("  9. (a + b         (unmatched parenthesis)\n");
    printf("  10. a b + c       (missing operator)\n");
    printf("  11. + a * b       (starting with operator)\n");
    printf("  12. a +           (ending with operator)\n");
    printf("═══════════════════════════════════════════════\n");
}

// Function to get expression from user
void getExpression(char *buffer, int size) {
    printf("\n📝 Enter arithmetic expression: ");
    fgets(buffer, size, stdin);
    
    // Remove newline character
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
}

int main() {
    char expression[MAX_INPUT];
    int choice;
    int parseResult;
    
    // Display initial banner
    displayMenu();
    
    do {
        displayOptions();
        scanf("%d", &choice);
        getchar(); // Clear newline from input buffer
        
        switch(choice) {
            case 1: // Parse new expression with step display
                getExpression(expression, MAX_INPUT);
                
                if (strlen(expression) == 0) {
                    printf("\n⚠️  Empty expression! Please enter a valid expression.\n");
                    break;
                }
                
                printf("\n🔍 Validating expression...\n");
                if (!validateExpression(expression)) {
                    printf("\n%s\n", "❌ INVALID EXPRESSION SYNTAX!");
                    printf("   Please check:\n");
                    printf("   • Balanced parentheses\n");
                    printf("   • Valid operators (+, -, *, /)\n");
                    printf("   • Proper identifier placement\n");
                    printf("   • No consecutive operators\n");
                    break;
                }
                
                printf("\n✅ Expression syntax is valid!\n");
                printf("Starting parser with step-by-step visualization...\n");
                
                // Create a copy for parsing (since parseExpression modifies input)
                char exprCopy[MAX_INPUT];
                strcpy(exprCopy, expression);
                parseExpression(exprCopy, 1); // Verbose mode
                break;
                
            case 2: // View sample expressions
                displaySamples();
                break;
                
            case 3: // View precedence table
                printPrecedenceTable();
                break;
                
            case 4: // Quick parse (no step display)
                getExpression(expression, MAX_INPUT);
                
                if (strlen(expression) == 0) {
                    printf("\n⚠️  Empty expression! Please enter a valid expression.\n");
                    break;
                }
                
                if (!validateExpression(expression)) {
                    printf("\n❌ Invalid expression syntax!\n");
                    break;
                }
                
                printf("\n⚡ Quick parsing result:\n");
                char quickCopy[MAX_INPUT];
                strcpy(quickCopy, expression);
                parseExpression(quickCopy, 0); // Silent mode
                break;
                
            case 5: // Clear screen
                system("clear"); // Use "cls" on Windows
                displayMenu();
                break;
                
            case 6: // Exit
                printf("\n%s\n", "╔════════════════════════════════════════╗");
                printf("║  Thank you for using the parser!      ║\n");
                printf("║  Goodbye!                             ║\n");
                printf("╚════════════════════════════════════════╝\n");
                break;
                
            default:
                printf("\n⚠️  Invalid choice! Please enter a number between 1-6.\n");
        }
        
    } while(choice != 6);
    
    return 0;
}
