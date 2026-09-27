/*
 * SHIFT REDUCE PARSER
 * -----------------------------------------
 * GRAMMAR:
 *      E -> E+E
 *      E -> E*E
 *      E -> (E)
 *      E -> i          ('i' stands for an identifier, e.g. a variable name)
 *
 * IDEA: A shift-reduce parser is "bottom-up" - instead of starting from
 * the start symbol E and expanding it (like the recursive descent
 * parser does), it starts from the raw input symbols and repeatedly
 * groups them back UP into E, using two actions:
 *
 *   SHIFT  : move the next symbol from the input onto the stack.
 *   REDUCE : if the top of the stack exactly matches the right-hand
 *            side of some production (this matching part is called
 *            the "handle"), pop those symbols off and push the
 *            left-hand side (E) instead.
 *
 * We keep alternating: shift a symbol, then reduce as many times as
 * possible, then shift again, and so on - until there is no more
 * input AND the stack contains just the single symbol E. That means
 * the whole input has successfully been reduced to the start symbol,
 * so the string is ACCEPTED by the grammar.
 *
 * EXAMPLE TRACE for input  i+i*i :
 *   Stack        Input        Action
 *   $            i+i*i$       shift
 *   $i           +i*i$        reduce   i -> E
 *   $E           +i*i$        shift
 *   $E+          i*i$         shift
 *   $E+i         *i$          reduce   i -> E
 *   $E+E         *i$          shift
 *   $E+E*        i$           shift
 *   $E+E*i       $            reduce   i -> E
 *   $E+E*E       $            reduce   E*E -> E
 *   $E+E         $            reduce   E+E -> E
 *   $E           $            ACCEPTED
 *
 * ('$' is just a marker we print to show the bottom of the stack /
 *  end of input - it is not really stored, just printed for clarity.)
 *
 * HOW TO COMPILE AND RUN:
 *   gcc shiftreduce.c -o sr
 *   ./sr
 *   Enter something like:  i+i*i   or   (i+i)*i   or   i
 */

#include <stdio.h>
#include <string.h>

char stack[50];
int top = -1;

char input[50];
int ip = 0;

char rhs[30][30] = {"i", "E+E", "E*E", "(E)"};
int numRules = 4;

void push(char c)
{
    stack[++top] = c;
}

void printStatus(char action[])
{
    printf("$");
    for (int i = 0; i <= top; i++)
        printf("%c", stack[i]);

    printf("\t\t%s$\t\t%s\n", &input[ip], action);
}

int tryReduce()
{
    for (int i = 0; i < numRules; i++)
    {
        int len = strlen(rhs[i]);

        if (top + 1 < len)
            continue;

        int matched = 1;
        for (int j = 0; j < strlen(rhs[i]); j++)
        {
            if (stack[top - len + 1 + j] != rhs[i][j])
            {
                matched = 0;
                break;
            }
        }

        if (matched)
        {
            top = top - len;
            push('E');

            char action[30];
            sprintf(action, "Reduce %s -> E", rhs[i]);
            printStatus(action);
            return 1;
        }
    }
    return 0;
}

int main()
{
    printf("Enter input string: ");
    scanf("%s", input);
    printf("\nstack\t\tinput\t\taction\n");
    printf("-------------------------------------\n");
    while (1)
    {
        while (tryReduce())
            ;

        if (ip == strlen(input))
            break;

        // keep shifting
        push(input[ip]);
        ip++;
        printStatus("Shift");
    }

    if (top == 0 && stack[top] == 'E')
    {
        printf("\nAccepted \n");
    }
    else
        printf("\nRejected\n");

    return 0;
}