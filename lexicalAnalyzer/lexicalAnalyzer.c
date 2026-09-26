#include <stdio.h>
#include <ctype.h>
#include <string.h>

char *keywords[] = {
    "int", "float", "char", "double", "void", "if", "else", "while", "for", "do", "return"};

int kwSize = 11;

int isKeyword(char *word)
{
    for (int i = 0; i < kwSize; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int main()
{
    int i;
    char ch;
    char buff[100];

    FILE *fp = fopen("input.c", "r");

    ch = fgetc(fp);

    while (ch != EOF)
    {

        // case 1: skip white spaces, tabs and newline
        if (ch == ' ' || ch == '\t' || ch == '\n')
        {
            ch = fgetc(fp);
            continue;
        }

        // case 2: comments

        if (ch == '/')
        {
            char next = fgetc(fp);

            // single line comment
            if (next == '/')
            {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF)
                    ;
                ch = fgetc(fp);
                continue;
            }
            // multiline comment
            else if (next == '*')
            {
                char prev = 0;
                while ((ch = fgetc(fp)) != EOF)
                {
                    if (prev == '*' && ch == '/')
                    {
                        ch = fgetc(fp);
                        break;
                    }
                    prev = ch;
                }
                continue;
            }

            // its just a division operator
            else
            {
                printf("OPERATOR: /\n");
                ch = next;
                continue;
            }
        }

        // case 3: keywords and identifiers
        if (isalpha(ch) || ch == '_')
        {
            i = 0;
            while (isalpha(ch) || ch == '_')
            {
                buff[i++] = ch;
                ch = fgetc(fp);
            }
            buff[i] = '\0';

            if (isKeyword(buff))
                printf("KEYWORD: %s\n", buff);

            else
                printf("IDENTIFIER: %s\n", buff);

            continue;
        }

        // case 4: numbers
        if (isdigit(ch))
        {
            i = 0;
            while (isdigit(ch) || ch == '.')
            {
                buff[i++] = ch;
                ch = fgetc(fp);
            }

            buff[i] = '\0';
            printf("NUMBER: %s\n", buff);
            continue;
        }

        // case 5: string literals
        if (ch == '"')
        {
            i = 0;
            buff[i++] = ch;
            ch = fgetc(fp);

            while (ch != '"' && ch != EOF)
            {
                buff[i++] = ch;
                ch = fgetc(fp);
            }

            buff[i++] = '"';
            buff[i] = '\0';
            printf("STRING: %s\n", buff);

            ch = fgetc(fp);
            continue;
        }

        // case 6: operators
        if (ch == '=' || ch == '<' || ch == '>' || ch == '!' ||
            ch == '+' || ch == '-' || ch == '&' || ch == '|')
        {
            char op1 = ch;
            char op2 = fgetc(fp);

            if (op2 == '=' || (op1 == '+' && op2 == '+') ||
                (op1 == '-' && op2 == '-') ||
                (op1 == '&' && op2 == '&') ||
                (op1 == '|' && op2 == '|'))
            {
                printf("OPERATOR: %c%c\n", op1, op2);
                ch = fgetc(fp);
            }
            else
            {
                printf("OPERATOR: %c\n", op1);
                ch = op2;
            }
            continue;
        }

        if (ch == '*' || ch == '%')
        {
            printf("OPERATOR: %c\n", ch);
            ch = fgetc(fp);
            continue;
        }

        // case 7: special symbols
        if (ch == '(' || ch == ')' || ch == '{' || ch == '}' ||
            ch == '[' || ch == ']' || ch == ';' || ch == ',')
        {
            printf("SPECIAL SYM: %c\n", ch);
            ch = fgetc(fp);
            continue;
        }

        // case 8: idk just ignore it and move on
        ch = fgetc(fp);
    }

    fclose(fp);
    return 0;
}