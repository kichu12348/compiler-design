/* Intermediate Code Generation
 *
 * ALGORITHM
 * ---------
 * This program converts an assignment containing an infix expression into
 * postfix notation and then generates three-address code (TAC).
 * For example, a=b+c*d produces the postfix expression bcd*+ and code such as:
 *     t1 = c*d
 *     t2 = b+t1
 *     a = t2
 *
 * 1. Read the complete assignment as a string.  The expected form is:
 *        variable=infix_expression
 *    For example: result=(a+b)*c
 *
 * 2. Find the assignment operator '='.
 *    strchr(string, character) is a standard library function from <string.h>.
 *    It searches the string from left to right for the first occurrence of the
 *    specified character.  It returns a pointer to that character when found,
 *    or NULL when the character is not found.  Therefore, the pointer returned
 *    by strchr(expr, '=') points to the '=' in the input expression.
 *    Subtracting the address of the first character (expr) from that pointer
 *    gives the number of characters before '=', which is the length of the lhs.
 *
 * 3. Copy the characters after '=' into rhs.  Copy the characters before '='
 *    into lhs and append '\0'.  The null character marks the end of a C string.
 *
 * 4. Convert rhs from infix notation to postfix notation using a stack:
 *    a. Read each character from left to right.
 *    b. Copy operands (letters or digits) directly to the output.
 *    c. Push '(' onto the stack.
 *    d. On ')', pop operators into the output until '(' is reached, then remove
 *       the '('.
 *    e. For an operator, pop operators with greater than or equal precedence,
 *       append them to the output, and push the current operator.
 *    f. After all input is read, pop the remaining operators into the output.
 *
 * 5. Generate TAC from the postfix expression using another stack:
 *    a. Push every operand onto the stack.
 *    b. When an operator is found, pop the right operand, then the left operand.
 *    c. Create a temporary name (t1, t2, ...), print an instruction using the
 *       two operands, and push the temporary name back onto the stack.
 *    d. After processing the postfix expression, the remaining stack item is
 *       the complete result.  Assign it to lhs.
 *
 * FUNCTIONS
 * ---------
 * int prec(char ch): Returns the precedence of an operator.
 * void postfix(char inp[], char out[]): Converts infix input to postfix output.
 * void getTAC(char post[], char lhs[]): Generates TAC from postfix input.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

char stack[50];
int top = -1;

void push(char c)
{
    stack[++top] = c;
}

char pop()
{
    char c = stack[top];
    top--;
    return c;
}

int prec(char ch)
{
    if (ch == '*' || ch == '/')
        return 2;
    if (ch == '+' || ch == '-')
        return 1;

    return 0;
}

void postfix(char inp[], char out[])
{
    int k = 0;

    int len = strlen(inp);

    for (int i = 0; i < len; i++)
    {
        char ch = inp[i];
        if (isalnum(ch))
        {
            out[k++] = ch;
        }
        else if (ch == '(')
        {
            push(ch);
        }
        else if (ch == ')')
        {
            while (top >= 0 && stack[top] != '(')
            {
                out[k] = pop();
                k++;
            }
            pop(); // pop out the ( from stack
        }
        else
        {
            while (top >= 0 && prec(stack[top]) >= prec(ch))
            {
                out[k++] = pop();
            }
            push(ch);
        }
    }

    while (top >= 0)
    {
        out[k] = pop();
        k++;
    }
    out[k] = '\0';
}

void getTAC(char post[], char lhs[])
{
    int len = strlen(post);

    char stack[50][50];
    int top = -1;
    int t = 1;

    for (int i = 0; i < len; i++)
    {
        char ch = post[i];
        if (isalnum(ch))
        {
            stack[++top][0] = ch;
            stack[top][1] = '\0';
        }
        else
        {
            char b[10], a[10], temp[10];

            // Pop the right operand first
            strcpy(b, stack[top--]);

            // Then pop the left operand
            strcpy(a, stack[top--]);

            // write the name of temp variable in temp[10]
            sprintf(temp, "t%d", t++);

            // Print the three-address code instruction
            printf("%s = %s%c%s\n", temp, a, ch, b);

            // Push the temporary variable back onto the stack
            strcpy(stack[++top], temp);
        }
    }
    printf("%s = %s\n", lhs, stack[top]);
}

int main()
{
    char expr[100], post[100], rhs[100], lhs[10];

    printf("Enter expression: ");
    scanf("%s", expr);

    // Find the assignment operator '=' in the input expression
    char *eq = strchr(expr, '=');

    // Calculate the length of the left-hand side (lhs) by
    // subtracting the address of the first character from the address of '='
    // This gives the number of characters before '='
    // for example, in "result=(a+b)*c", the length of lhs is 6 (the length of "result")
    // so eq will point to the '=' character, and expr will point to the first character of the string
    // i.e. 'r', so eq - expr will give 6, which is the length of lhs
    // Copy the characters after '=' into rhs and the characters before '=' into lhs
    // Append '\0' to lhs to mark the end of the string
    // eq + 1 points to the first character after '=', which is the start of the rhs expression, here it is ( in (a+b)*c
    int n = eq - expr;

    strcpy(rhs, eq + 1);
    strncpy(lhs, expr, n);
    lhs[n] = '\0';

    postfix(rhs, post);

    printf("\nPOSTFIX: %s \n\nTHREE ADDRESS CODE:\n", post);
    getTAC(post, lhs);

    return 0;
}