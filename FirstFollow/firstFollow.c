#include <stdio.h>
#include <ctype.h>
#include <string.h>

int n, r, p;

char prods[30][30];
char nl, alPrinted[30], input, result[30];

int isAlreadyPrinted(char w)
{
    for (int i = 0; i < p; i++)
    {
        if (alPrinted[i] == w)
            return 1;
    }
    return 0;
}

void addToRes(char w)
{
    for (int i = 0; i < r; i++)
        if (result[i] == w)
            return;
    result[r++] = w;
}

void first(char w)
{
    if (!isupper(w))
    {
        addToRes(w);
        return;
    }

    for (int i = 0; i < n; i++)
    {
        if (prods[i][0] == w)
        {
            if (prods[i][2] == w)
                continue;

            else if (!isupper(prods[i][2]))
                addToRes(prods[i][2]);
            else
                first(prods[i][2]);
        }
    }
}

void follow(char w)
{
    if (prods[0][0] == w)
        addToRes('$');
    for (int i = 0; i < n; i++)
    {
        for (int j = 2; j < strlen(prods[i]); j++)
        {
            if (prods[i][j] == w)
            {
                if (prods[i][j + 1] == '\0')
                {
                    follow(prods[i][0]);
                }
                else if (prods[i][j + 1] == w)
                    continue;
                else if (!isupper(prods[i][j + 1] && prods[i][j + 1] != '#'))
                    addToRes(prods[i][j]);

                else
                    first(prods[i][j + 1]);
            }
        }
    }
}

int main()
{
    printf("enter no of prods: ");
    scanf("%d", &n);

    printf("enter prods\n");

    for (int i = 0; i < n; i++)
    {
        scanf("%s%c", prods[i], &nl);
    }
    printf("\n");

    p = 0;
    for (int i = 0; i < n; i++)
    {
        input = prods[i][0];

        if (!isAlreadyPrinted(input))
        {
            r = 0;
            first(input);
            printf("First of %c: ", input);
            for (int j = 0; j < r; j++)
                printf("%c ", result[j]);

            alPrinted[p++] = input;
            printf("\n");
        }
    }

    p = 0;
    strcpy(result, "");

    printf("\n\n\n");

    for (int i = 0; i < n; i++)
    {
        input = prods[i][0];
        if (!isAlreadyPrinted(input))
        {
            r = 0;
            follow(input);
            printf("follow of %c: ", input);
            for (int j = 0; j < r; j++)
                printf("%c ", result[j]);
            printf("\n");
            alPrinted[p++] = input;
        }
    }

    return 0;
}