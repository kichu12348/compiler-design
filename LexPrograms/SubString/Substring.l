%{
#include<stdio.h>    
%}

%%

abc {printf("ABC");}

. {printf("%s",yytext);}

\n {printf("\n");}

%%

int yywrap(){
    return 1;
}

int main(){
    printf("Enter text and then Ctrl+D:\n");
    yylex();
    return 0;
}
