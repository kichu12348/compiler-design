%{
#include<stdio.h>

int wordCount=0;
int lineCount=0;
int charCount=0;
%}

%%

\n {lineCount++; charCount++;}

[^\t\n]+ {wordCount++; charCount+=yyleng;}

. {charCount++;}

%%

int yywrap(){
    return 1;
}

int main(){
    printf("Enter Text then press (Ctrl+D): \n");

    yylex();

    printf("\nLines: %d\n",lineCount);
    printf("Words: %d\n",wordCount);
    printf("Chars: %d\n",charCount);
    return 0;
}