#include <stdio.h>
#include <string.h>

struct var
{
   int valint;
   char* valchar;
   char name[256];
   int init; //1 -> initializata
   int constt; //1 -> constanta
};

/* pentru tabelul de simboluri */

struct variabila
{
	char nume[256];
	char tip[256];
	int init;
	int constt;
	int val;
	float floatval;
	char stringval[256];
	char charval[256];
	int vec_lg;
	int vec_val[256];
	float vec_floatval[256];
	char scope[256];
};

struct variabila a[256];
struct var vars[256];
int nrvar = 0;


void initializareINT(char x[], int v)
{	
	strcpy(vars[nrvar].name, x);
    strcpy(a[nrvar].nume, x);
    strcpy(a[nrvar].tip, "int");
	vars[nrvar].init = 1;
    a[nrvar].init = 1;
    vars[nrvar].constt = 0;
    a[nrvar].constt = 0;
	vars[nrvar].valint = v;
    a[nrvar].val = v;
	nrvar++;
}

void init_CONST_INT(char x[], int v)
{	
	strcpy(vars[nrvar].name, x);
    strcpy(a[nrvar].nume, x);
    strcpy(a[nrvar].tip, "int");
	vars[nrvar].init = 1;
    a[nrvar].init = 1;
    vars[nrvar].constt = 1;
    a[nrvar].constt = 1;
	vars[nrvar].valint = v;
    a[nrvar].val = v;
	nrvar++;
}

void initializareCHAR(char x[], char* v)
{
    strcpy(vars[nrvar].name, x);
    strcpy(a[nrvar].nume, x);
    strcpy(a[nrvar].tip, "char");
    vars[nrvar].init = 1;
    a[nrvar].init = 1;
    vars[nrvar].constt = 0;
    a[nrvar].constt = 0;
    strcpy(vars[nrvar].valchar, v);
    strcpy(a[nrvar].charval, v);
    nrvar++;
}

void init_CONST_CHAR(char x[], char* v)
{
    strcpy(vars[nrvar].name, x);
    strcpy(a[nrvar].nume, x);
    strcpy(a[nrvar].tip, "char");
    vars[nrvar].init = 1;
    a[nrvar].init = 1;
    vars[nrvar].constt = 1;
    a[nrvar].constt = 1;
    strcpy(vars[nrvar].valchar, v);
    strcpy(a[nrvar].charval, v);
    nrvar++;
}

void declarare(char x[])
{
	strcpy(vars[nrvar].name,x);
    strcpy(a[nrvar].nume,x);
	vars[nrvar].init = 0;
    a[nrvar].init = 0;
    vars[nrvar].constt = 0;
    a[nrvar].constt = 0;
    strcpy(a[nrvar].scope, "main");
	nrvar++;
}

void declarare_CONST(char x[])
{
	strcpy (vars[nrvar].name,x);
	vars[nrvar].init = 0;
    vars[nrvar].constt = 1;
	nrvar++;
}

int verifdecl(char x[])
{
    int i;
    for (i = 0; i <= nrvar; i++)
         if (strcmp(x, vars[i].name) == 0) 
         {
             return i;
         }
    return -1;
}

void atribuireINT(char x[], int v)
{
    int p = verifdecl(x);
    vars[p].valint = v;
    a[p].val = v;
    vars[p].init = 1;
    a[p].init = 1;
}

void atribuireCHAR(char x[],char* v)
{
    int p = verifdecl(x);
    strcpy(vars[p].valchar, v);
    strcpy(a[p].charval, v);
    vars[p].init = 1;
    a[p].init = 1;
}

int verifinit(char x[])
{
    int i;
    for (i = 0; i <= nrvar; i++)
         if (strcmp(x, vars[i].name) == 0) 
       	     if (vars[i].init == 0)
        		 return -1;
        	 else 
                 return 1;
    return -1;
}

int verifconst(char x[])
{
    int i;
    for (i = 0; i <= nrvar; i++)
         if (vars[i].constt == 1)
             if (vars[i].init == 0)
                 return -1;
             else
                 return 1;
    return -1;
}

void tabel_var(FILE* file)
{
    int i;
    char* inf, aux;
    FILE* tabel = fopen(file, "w");
    for (i = 0; i < nrvar; i++)
    {
        strcpy(inf, "Nume: "); strcat(inf, a[i].nume);
        strcat(inf, " | Tip: "); strcat(inf, a[i].tip);
        strcat(inf, " | Scope: "); strcat(inf, a[i].scope);

        if (a[i].constt == 1)
			strcat(inf, " | Const ");

        if (a[i].init)
		{
            strcat(inf, " | Value: ");
			if (strcmp(a[i].tip, "int") == 0 || strcmp(a[i].tip, "bool") == 0)
			{
				if (a[i].vec_lg != 0)
					strcat(inf, " Vector");
				else
				{
                    sprintf(aux, "%d", a[i].val);
                    strcat(inf, aux);
                }
			}

			if (strcmp(a[i].tip, "float") == 0)
			{
				if (a[i].vec_lg != 0)
					strcat(inf, " Vector");
				else
			    {
                    sprintf(aux, "%f", a[i].floatval);
                    strcat(inf, aux);
                }
			}

			if (strcmp(a[i].tip, "string") == 0)
			{
				if (a[i].vec_lg != 0)
					printf(" VECTOR");
				else
					strcat(inf, a[i].stringval);
			}

            if (strcmp(a[i].tip, "char") == 0)
			{
				if (a[i].vec_lg != 0)
					printf(" Vector");
				else
				{
                    sprintf(aux, "%c", a[i].charval);
                    strcat(inf, aux);
                }
			}
		}
        else
            strcat(inf, " | Nedefinita");
        strcat(inf, "\n");
        fputs(inf, file);
    }
}