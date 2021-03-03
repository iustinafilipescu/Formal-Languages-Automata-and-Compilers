%{

#include <stdio.h>
#include "f.h"
extern FILE* yyin;
extern char* yytext;
extern int yylineno;

%}


%union { int val; char *nume; float fval; }

%type <val> NR expresie
%type <nume> ID
%type <fval> NRFLOAT

%token VAR CONST PRINT NR RETURN ID BGIN END MAIN IF CLASS PUBLIC PRIVAT INT STRING FOR ELSE RETRN AND OR EQ MARE MIC NEQ MAREEGAL MICEGAL THEN WHILE DO CHAR BOOL FLOAT NRFLOAT FALSE TRUE

%start program
%left '+' '-'
%left '*' '/'

%%

program: clase functii declaratii instructiuni 

clase: clasa ';'
     | clase clasa ';'
     ;

clasa: CLASS ID '{' corpclasa '}'
     ;

corpclasa: PUBLIC varpub PRIVAT varpriv functii
         ;

varpub: declaratii
      ;

varpriv: declaratii
       ;

functii: functie 
       | functii functie 
       ;

functie: ID '(' argumente ')' '-' '>' vartype '{' bloc '}' ';'
       ;

argumente: vartype ID 
         | vartype ID ',' argumente
         ;

declaratii:  declaratie ';'
    	     | declaratii declaratie ';'	
     	;

declaratie: VAR vartype ID {if (verifdecl($3) == -1) declarare($3); else {yyerror(); printf ("Variabila a fost deja declarata \n");}}
   	     | VAR vartype ID '-' '>' NR {if (verifdecl($3) == -1) initializareINT($3, $6); else { yyerror(); printf ("Variabila a fost deja initializata \n");}}
          | VAR vartype ID '-' '>' NRFLOAT
          | VAR vartype ID '[' NR ']' 
          | VAR vartype ID '[' NR ']' '-' '>' '{' ids '}'
          | CONST vartype ID '-' '>' NR {if (verifdecl($3) == -1) init_CONST_INT($3, $6); else { yyerror(); printf ("Variabila a fost deja initializata \n");}}
          | CONST vartype ID '-' '>' NRFLOAT {if (verifdecl($3) == -1) init_CONST_INT($3, $6); else { yyerror(); printf ("Variabila a fost deja initializata \n");}}
          ;

ids: '\'' ID '\''
   | NR
   | NRFLOAT
   | NRFLOAT ',' ids
   | '\'' ID '\'' ',' ids
   | NR ',' ids
   ;

vartype: INT
       | CHAR
       | FLOAT
       | BOOL
       | STRING
       ;
      
instructiuni: MAIN BGIN bloc END  
     	  ;
     
bloc: instructiune 
    | instructiune bloc
    ;

instructiune: ID '-' '>' expresie  ';'{if (verifdecl($1) == -1) {yyerror(); printf ("Variabila nu a fost declarata \n");} else atribuireINT($1, $4); }
            | ID '[' NR ']' '-' '>' expresie ';'
            | ID '[' ID ']' '-' '>' expresie ';'
            | PRINT '(' NR ')' ';'
            | PRINT '(' ID ')' ';'
            | instrIF ';'
            | instrWHILE ';'
            | RETRN ID ';'
            | RETRN NR ';'
            | RETRN NRFLOAT ';'
            | RETRN BOOL ';'
            | ID '-' '>' ID '(' lista_apel ')' ';'
            | instrFOR ';'
            ;

instrFOR: FOR '(' exprFOR1 ',' exprFOR2 ',' exprFOR3 ')' '{' bloc '}'
        ;

exprFOR1: ID '-' '>' operator
        ;

exprFOR2:  expresie  MARE  expresie
        | expresie  MIC  expresie
        | expresie  MAREEGAL  expresie
        | expresie  MICEGAL  expresie
        ;

exprFOR3: ID '-' '>' expresie
        ;

lista_apel: operator
          | lista_apel ',' operator
          ;

operator: ID
        | NR
        ;

instrIF: IF '(' listaexprIF ')' THEN '{' bloc '}' 
       | IF '(' listaexprIF ')' THEN '{' bloc '}' ELSE '{' bloc '}'
       ;

listaexprIF: expresieIF AND listaexprIF
           | expresieIF OR listaexprIF
           | expresieIF
           ;

expresieIF: expresie  EQ  expresie
          | expresie  MARE  expresie
          | expresie  MIC  expresie
          | expresie  MAREEGAL  expresie
          | expresie  NEQ  expresie
          | expresie  MICEGAL  expresie
          ;

instrWHILE: WHILE '(' listaexprWHILE ')' DO '{' bloc '}'
          ;
         
listaexprWHILE: expresieWHILE AND listaexprWHILE
              | expresieWHILE OR listaexprWHILE
              | expresieWHILE 
              ;

expresieWHILE: expresie  EQ  expresie
          | expresie  MARE  expresie
          | expresie  MIC  expresie
          | expresie  MAREEGAL  expresie
          | expresie  NEQ  expresie
          | expresie  MICEGAL  expresie
             ;

expresie: expresie '+' expresie {$$ = $1 + $3;}
	   | expresie '-' expresie {$$ = $1 - $3;}
	   | expresie '*' expresie {$$ = $1 * $3;}
	   | expresie '/' expresie {$$ = $1 / $3;}
        | NR {$$ = $1;}
        | NRFLOAT {$$ = $1;}
        | ID {$$ = $1;}
        | '\'' ID '\'' {$$ = $2;}
        ;

%%

int yyerror(char* s)
{
     printf("Eroare: %s la linia: %d\n", s, yylineno);
}

int main(int argc, char** argv)
{
     FILE *tabel;
     tabel = fopen("tabel_simboluri.txt", "w");
     yyin = fopen(argv[1], "r");
     yyparse();
}