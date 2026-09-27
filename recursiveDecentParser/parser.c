/*
 * RECURSIVE DESCENT PARSER (Lab Exam Program)
 * --------------------------------------------
 * GRAMMAR (classic arithmetic expression grammar):
 *
 *      E  -> E + T | T
 *      T  -> T * F | F
 *      F  -> ( E ) | id
 *
 * A recursive descent parser needs ONE function per non-terminal, and
 * each function must be able to decide what to do by just looking at
 * the CURRENT character. The grammar above is LEFT RECURSIVE
 * (E -> E + T calls E first thing, which would call itself forever),
 * so before writing any code we remove left recursion:
 *
 *      E  -> T E'
 *      E' -> + T E' | epsilon      (epsilon = "do nothing / match nothing")
 *      T  -> F T'
 *      T' -> * F T' | epsilon
 *      F  -> ( E ) | id
 *
 * Now every rule is written top-down:
 *      E()  calls T() then E'()
 *      E'() either consumes "+ T" and calls itself again, or does nothing
 *      T()  calls F() then T'()
 *      T'() either consumes "* F" and calls itself again, or does nothing
 *      F()  either matches "( E )" or a single identifier
 *
 * "id" is simplified to any single lowercase letter or digit (a, b, x,
 * 1, 2, ...) so we don't need a separate lexical analyzer for this.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

char buff[100];
int pos;

int E();
int Eprime();
int T();
int Tprime();
int F();

int E()
{
    if (!T())
        return 0;
    return Eprime();
}

int Eprime()
{
    if (buff[pos] == '+')
    {
        pos++;
        if (!T())
            return 0;
        return Eprime();
    }
    return 1;
}

int T()
{
    if (!F())
        return 0;

    return Tprime();
}

int Tprime()
{
    if (buff[pos] == '*')
    {
        pos++;
        if (!F())
            return 0;
        return Tprime();
    }
    return 1;
}

int F()
{
    if (buff[pos] == '(')
    {
        pos++;
        if (!E())
            return 0;

        if (buff[pos] == ')')
        {
            pos++;
            return 1;
        }
    }

    if (isalnum(buff[pos]))
    {
        pos++;
        return 1;
    }
}

int main()
{
    printf("Enter expression: ");
    scanf("%s", buff);

    pos = 0;
    int result = E();

    if (result)
    {
        printf("Valid Expression\n");
    }

    else
    {
        printf("Invalid Expression\n");
    }
}