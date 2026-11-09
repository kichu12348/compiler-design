#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

struct Var
{
    char name[100];
    int known;
    int val;
} tab[50];

int cnt = 0;

int isNum(char s[])
{
    return isdigit(s[0]) || (s[0] == '-' && isdigit(s[1]));
}

int find(char s[])
{
    for (int i = 0; i < cnt; i++)
    {
        if (strcmp(s, tab[i].name) == 0)
            return i;
    }

    return -1;
}

void setVar(char name[], int known, int val)
{
    int i = find(name);

    if (i < 0)
    {
        i = cnt++;
        strcpy(tab[i].name, name);
    }

    tab[i].known = known;
    tab[i].val = val;
}

int resolve(char s[], int *val)
{
    if (isNum(s))
    {
        *val = atoi(s);
        return 1;
    }

    int i = find(s);

    if (i >= 0 && tab[i].known)
    {
        *val = tab[i].val;
        sprintf(s, "%d", *val);
        return 1;
    }
    return 0;
}

int main()
{
    int n;
    char line[100], lhs[30], a[30], b[30], op;

    printf("Enter no of statements: ");
    scanf("%d", &n);

    char stmts[50][100];

    printf("Enter statements: \n");

    for (int i = 0; i < n; i++)
    {
        scanf(" %[^\n]", line);
        strcpy(stmts[i], line);
    }

    printf("\n\n");

    for (int i = 0; i < n; i++)
    {
        int va, vb;

        if (sscanf(stmts[i], "%s = %s %c %s", lhs, a, &op, b) == 4)
        {
            int ca = resolve(a, &va);
            int cb = resolve(b, &vb);

            if (ca && cb)
            {
                int r = 0;

                switch (op)
                {
                case '+':
                    r = va + vb;
                    break;
                case '-':
                    r = va - vb;
                    break;
                case '*':
                    r = va * vb;
                    break;
                case '/':
                    r = vb ? va / vb : 0;
                    break;
                }

                printf("%s = %d\n", lhs, r);
                setVar(lhs, 1, r);
            }
            else
            {
                printf("%s = %s %c %s\n", lhs, a, op, b);
                setVar(lhs, 0, 0);
            }
        }

        else if (sscanf(stmts[i], "%s = %s", lhs, a) == 2)
        {

            int ca = resolve(a, &va);

            printf("%s = %s\n", lhs, a);
            setVar(lhs, ca, va);
        }
    }

    return 0;
}