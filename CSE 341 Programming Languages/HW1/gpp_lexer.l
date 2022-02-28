; I could only managed to parse EXPI
; Example: (+ a (- b c))

%{

#define KW_AND 0 
#define KW_OR 1
#define KW_NOT 2 
#define KW_EQUAL 3
#define KW_LESS 4
#define KW_NIL 5
#define KW_LIST 6
#define KW_APPEND 7
#define KW_CONCAT  8
#define KW_SET 9
#define KW_DEFFUN 10 
#define KW_FOR 11
#define KW_IF 12
#define KW_EXIT  13
#define KW_LOAD  14
#define KW_DISP  15
#define KW_TRUE 16
#define KW_FALSE 17
#define OP_PLUS  18
#define OP_MINUS 19
#define OP_DIV 20 
#define OP_DIV2 21
#define OP_MULT 22
#define OP_OP 23 
#define OP_CP 24
#define OP_DBLMULT 25 
#define OP_OC  26
#define OP_CC 27
#define OP_COMMA 28
#define COMMENT 29
#define VALUE 30
#define IDENTIFIER 31

%}

%%
"and"       {return KW_AND;}
"or"        {return KW_OR;}
"not"       {return KW_NOT;}
"equal"     {return KW_EQUAL;}
"less"      {return KW_LESS;}
"nil"       {return KW_NIL;}
"list"      {return KW_LIST;}
"append"    {return KW_APPEND;}
"concat"    {return KW_CONCAT;}
"set"       {return KW_SET;}
"deffun"    {return KW_DEFFUN;}
"for"       {return KW_FOR;}
"if"        {return KW_IF;}
"exit"      {return KW_EXIT;}
"load"      {return KW_LOAD;}
"disp"      {return KW_DISP;}
"true"      {return KW_TRUE;}
"false"     {return KW_FALSE;}
"+"         {return OP_PLUS;}
"-"         {return OP_MINUS;}
"/"         {return OP_DIV;}
[*]         {return OP_MULT;}
"("         {return OP_OP;}
")"         {return OP_CP;}
"**"        {return OP_DBLMULT;}
"\""        {return OP_OC;}
"“"         {return OP_CC;}
","         {return OP_COMMA;}
[";;"][a-zA-z0-9]*     {return COMMENT;}
[1-9][0-9]* {return VALUE;}
[0]         {return VALUE;}
[0-9][.][0-9]* {return VALUE;}
[1-9][0-9]*[.][0-9]* { return VALUE;}
[a-zA-z][a-zA-z0-9]* {return IDENTIFIER;}
" "      {;}
.           {return 999;}
%%      

int yywrap(){}

char* tokenList[] = {"KW_AND", "KW_OR", "KW_NOT", "KW_EQUAL", "KW_LESS", "KW_NIL", "KW_LIST",
"KW_APPEND", "KW_CONCAT", "KW_SET", "KW_DEFFUN", "KW_FOR", "KW_IF",
"KW_EXIT", "KW_LOAD", "KW_DISP", "KW_TRUE", "KW_FALSE", "OP_PLUS", "OP_MINUS", "OP_DIV", "OP_DIV2", "OP_MULT", "OP_OP", "OP_CP",
"OP_DBLMULT", "OP_OC", "OP_CC", "OP_COMMA", "COMMENT", "VALUE", "IDENTIFIER"};

int INPUT();
int EXPI();
int EXPLISTI();

void print(int n){
    printf("%s\n", tokenList[n]);
}

int ctoken;
int ntoken;


int main(){
    INPUT();
    
}

int INPUT(){
    ctoken = yylex();
    if( ctoken != OP_OP){
        EXPI();
        return 0;
    }

    print(OP_OP);
    ctoken = yylex();

    if( (ctoken >= OP_PLUS && ctoken <= OP_DIV) || 
        ctoken == KW_SET || ctoken == KW_DEFFUN ||
        ctoken == KW_IF || ctoken == KW_FOR || ctoken == IDENTIFIER){
            EXPI();
        }

}


int EXPI(){
    switch (ctoken){
        case OP_OP:
            print(OP_OP);
            ctoken = yylex();
            EXPI();
            break;
        case OP_PLUS:
            print(OP_PLUS);
            ctoken = yylex();
            EXPI();
            EXPI();
            if(ctoken != OP_CP){
                printf("SYNTAX ERROR. Expected ')' but found %s.\n", yytext);
                return 1;
            }
            print(OP_CP);
            ctoken = yylex();
            return 0;
            break;
        
        case OP_MINUS:
            print(OP_MINUS);
            ctoken = yylex();
            EXPI();
            EXPI();
            if(ctoken != OP_CP){
                printf("SYNTAX ERROR. Expected ')' but found %s.\n", yytext);
                return 1;
            }
            print(OP_CP);
            break;

        case OP_MULT:
            print(OP_MULT);
            ctoken = yylex();
            EXPI();
            EXPI();
            if(ctoken != OP_CP){
                printf("SYNTAX ERROR. Expected ')' but found %s.\n", yytext);
                return 1;
            }
            print(OP_CP);
            ctoken = yylex();
            return 0;
            break;

        case OP_DIV:
            print(OP_DIV);
            ctoken = yylex();
            EXPI();
            EXPI();
            if(ctoken != OP_CP){
                printf("SYNTAX ERROR. Expected ')' but found %s.\n", yytext);
                return 1;
            }
            print(OP_CP);
            ctoken = yylex();
            return 0;
            break;

        case IDENTIFIER:
            print(IDENTIFIER);
            ctoken = yylex();
            if(ctoken != 999){
                EXPLISTI();
            }
            else{
                return 0;
            }
            break;

        case KW_SET:
            print(KW_SET);
            ctoken = yylex();
            if(ctoken != IDENTIFIER){
                printf("Syntax Error on line %d. Expected identifier but found %s.\n", yylineno, yytext);
                return 0;
            }
            break;

        case KW_IF:
            print(KW_IF);
            break;

        
        case VALUE:
            print(VALUE);
            return 0;
            break;
        
        case OP_CP:
            print(OP_CP);
            return 0;
            break;

        default:
            printf("SYNTAX ERROR. %s cannot be tokenized.\n", yytext);
    }

    

    


}

int EXPLISTI(){

}