%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int IdentifierValues[64] = {0};
char IdentifierNames[64][20] = {'\0'};
int IdIndex = 0;
/*
void createIdentifier(char[] Ident, int value);
int assignToIdentifier(char[] Ident, int value);
int getIdentifierVal(char[] Ident);
*/
void yyerror(char* s);
%}

%start INPUT
%union {int num; char bin[5]; char* Idt;}
%token <num> IntegerValue
%token <bin> BinaryValue
%token <Idt> Id
%type <num> EXPI INPUT
%type <bin> EXPB


%%

INPUT:  EXPI {printf("Syntax OK.\nResult: %d\n", $1);}
        | EXPB {printf("Syntax OK.\n%s\n", $1);}
        ;

EXPI: '(''+' EXPI EXPI ')'   {$$ = $3 + $4;}
    | '(''-' EXPI EXPI ')'   {$$ = $3 - $4;}
    | '(''*' EXPI EXPI ')'   {$$ = $3 * $4;}
    | '(''/' EXPI EXPI ')'   {$$ = $3 / $4;}
//    | Id                    {$$ = getIdentifierVal($1);}
    | IntegerValue          {$$ = $1;}
//    | '('Id EXPLISTI')'     
//    | '('"set" Id EXPI')'               {$$ = assignToIdentifier($3, $4);}
/*    | '('"deffun" Id IDLIST EXPLISTI')'  ** IDLIST not defined
    | '('"if" EXPB EXPLISTI')'          {if($3 == "true"){$$ = $4}}
    | '('"if" EXPB EXPLISTI EXPLISTI')' {if($3 == "true"){$$ = $4} else{$$ = $5}}
    | '('"while" '('EXPB')' EXPLISTI')' {while($4 == "true"){$$ = $6}}
    | '('"for" '('Id EXPI EXPI')' EXPLISTI')' {for(int i = $5; i < $6; i++){$$ = $8}}*/
//    | '('"defvar" Id EXPI')'            {createIdentifier($3, $4); $$ = 0;}
    ;

EXPB: '('"and" EXPB EXPB')'     {if(strcmp($3, "true") == 0 && strcmp($4, "true")== 0){strcpy($$,"true");} else{strcpy($$,"false");}}
    | '('"or" EXPB EXPB')'      {if(strcmp($3, "true") == 0 || strcmp($4, "true")){strcpy($$,"true");} else{strcpy($$,"false");}}
    | '('"not" EXPB')'          {if(strcmp($3, "true") == 0){strcpy($$,"false");} else{strcpy($$,"true");}}
    | '('"equal" EXPB EXPB')'   {if(strcmp($3,$4) == 0){strcpy($$,"true");} else{strcpy($$,"false");}}
    | '('"equal" EXPI EXPI')'   {if($3 == $4){strcpy($$,"true");} else{strcpy($$,"false");}}
    | BinaryValue               {strcpy($$,$1);}
    ;

/*EXPLISTI: '('"concat" EXPLISTI EXPLISTI')' 
        | '('"append" EXPI EXPLISTI')' 
        | LISTVALUE 
        | null 

LISTVALUE:   '\'''('VALUES')' 
            | '\'''('')' 
          //  | null

VALUES: VALUES IntegerValue 
        | IntegerValue

null: "nil"*/

%%


/*
void createIdentifier(char[] Ident, int value){
    IdentifierNames[IdIndex] = Ident;
    IdentifierValues[IdIndex] = value;
    IdIndex++;
}

int assignToIdentifier(char[] Ident, int value){
    for(int i = 0; i < IdIndex; i++){
        if(strcmp(Ident,IdentifierNames[IdIndex]) == 0){
            IdentifierValues[i] = value;
            return value;
        }
    }
}

int getIdentifierVal(char[] Ident){
    for(int i = 0; i < IdIndex; i++){
        if(strcmp(Ident,IdentifierNames[IdIndex]) == 0){
            return IdentifierValues[i];
        }
    }
}*/
/*
int main(void){
    return yyparse();
}*/

void yyerror(char* s){ printf("SYNTAX_ERROR Expression not recognized\n\n");}


int main(void){ return yyparse(); }