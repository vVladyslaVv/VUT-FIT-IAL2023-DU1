/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char topChar;  // Variable to hold the character at the top of the stack

    // Continue until the stack is empty or we encounter a left parenthesis
    while (!Stack_IsEmpty(stack)) {
        // Get the top character from the stack
        Stack_Top(stack, &topChar);
        // Pop the top character from the stack
        Stack_Pop(stack);

        // Break the loop if the top character is a left parenthesis
        if (topChar == '(') {
            break;
        }

        // Append the operator to the postfix expression
        postfixExpression[(*postfixExpressionLength)++] = topChar;
    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char topChar;  // Variable to hold the character at the top of the stack
    
    while (!Stack_IsEmpty(stack)) {
        // Get the top character from the stack
        Stack_Top(stack, &topChar);
        
        // Determine the precedence of the top character and the current character
        int precedenceTopChar = (topChar == '*' || topChar == '/') ? 2 : 1;
        int precedenceCurrentChar = (c == '*' || c == '/') ? 2 : 1;
        
        // If the top character is '(' or has lower precedence than c, break the loop
        if (topChar == '(' || precedenceTopChar < precedenceCurrentChar) {
            break;
        }
        
        // Pop the top character from the stack and append it to the postfix expression
        Stack_Pop(stack);
        postfixExpression[(*postfixExpressionLength)++] = topChar;
    }
    
    // Push the current character onto the stack
    Stack_Push(stack, c);
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	// Allocate memory for the postfix expression
    char *postfixExpression = (char *)malloc(MAX_LEN);
    if (postfixExpression == NULL) {
        return NULL;  // Return NULL in case of allocation failure
    }

    // Initialize the stack
    Stack stack;
    Stack_Init(&stack);

    unsigned postfixExpressionLength = 0;
    int i = 0;
    char c = infixExpression[i];

    // Process each character in the infix expression
    while (c != '=') {
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            // If the character is an operand, append it to the postfix expression
            postfixExpression[postfixExpressionLength++] = c;
        } else if (c == '(') {
            // If the character is an opening parenthesis, push it to the stack
            Stack_Push(&stack, c);
        } else if (c == ')') {
            // If the character is a closing parenthesis, pop and append operators from the stack
            // until an opening parenthesis is encountered
            untilLeftPar(&stack, postfixExpression, &postfixExpressionLength);
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // If the character is an operator, process it using the doOperation function
            doOperation(&stack, c, postfixExpression, &postfixExpressionLength);
        }

        // Move to the next character in the infix expression
        c = infixExpression[++i];
    }

    // Pop and append any remaining operators from the stack
    while (!Stack_IsEmpty(&stack)) {
        char topOperator;
        Stack_Top(&stack, &topOperator);
        postfixExpression[postfixExpressionLength++] = topOperator;
        Stack_Pop(&stack);
    }

    // Append the terminating character '=' to the postfix expression
    postfixExpression[postfixExpressionLength++] = '=';

    // Terminate the postfix expression string with a null character
    postfixExpression[postfixExpressionLength] = '\0';

    // Dispose of the stack
    Stack_Dispose(&stack);

    return postfixExpression;

}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
	// Split the integer into four bytes
    // Start with the least significant byte
    for (int i = 0; i <= 3; i++) {
        char byte = (value >> (i * 8)) & 0xFF;  // Shift and mask to get the i-th byte
        Stack_Push(stack, byte);  // Push the byte onto the stack
    }
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	*value = 0;  // Reset the value to ensure no garbage data
    // Start with the most significant byte (as the order is now reversed due to the stack logic)
    for (int i = 3; i >= 0; i--) {
        char byte;
        Stack_Top(stack, &byte);  // Get the top byte from the stack
        Stack_Pop(stack);  // Pop the top byte from the stack
        *value |= (byte << (i * 8));  // Combine the byte into the value
    }
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	 // Convert the infix expression to postfix notation using the infix2postfix function
    char *postfixExpression = infix2postfix(infixExpression);
    // If the conversion fails (returns NULL), return false
    if (postfixExpression == NULL) {
        return false;  // Conversion failed
    }

    // Create a stack to hold values during evaluation
    Stack valueStack;
    // Initialize the stack
    Stack_Init(&valueStack);

    // Iterate through each character in the postfix expression until the end character = is reached
    for (int i = 0; postfixExpression[i] != '='; i++) {
        char ch = postfixExpression[i];
        // If the character is an alphabetic character, it's a variable
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
            // Look up the variable's value in the provided array and push it onto the stack
            for (int j = 0; j < variableValueCount; j++) {
                if (variableValues[j].name == ch) {
                    expr_value_push(&valueStack, variableValues[j].value);
                    break;  // Exit the loop once the variable is found and processed
                }
            }
        } 
        // If the character is a numeric character, it's a digit
        else if (ch >= '0' && ch <= '9') {
            // Convert the character to an integer and push it onto the stack
            expr_value_push(&valueStack, ch - '0');
        } 
        // If the character is not a variable or a digit, it's an operator
        else {
            // Pop the top two values from the stack to be operands for the operator
            int op2, op1;
            expr_value_pop(&valueStack, &op2);
            expr_value_pop(&valueStack, &op1);
            int result;  // Variable to hold the result of the operation
            // Perform the operation based on the operator character
            switch (ch) {
                case '+': result = op1 + op2; break;
                case '-': result = op1 - op2; break;
                case '*': result = op1 * op2; break;
                case '/': 
                    // Check for division by zero
                    if (op2 == 0) {
                        free(postfixExpression);
                        return false;  // Division by zero is not allowed, return false
                    }
                    result = op1 / op2; 
                    break;
                default: 
                    free(postfixExpression);
                    return false;  // Encountered an invalid operator, return false
            }
            // Push the result of the operation back onto the stack
            expr_value_push(&valueStack, result);
        }
    }

    // The final result of the expression evaluation is now on top of the stack
    expr_value_pop(&valueStack, value);
    // Free the memory allocated for the postfix expression
    free(postfixExpression);
    // Return true to indicate successful evaluation
    return true;
}

/* Konec c204.c */
