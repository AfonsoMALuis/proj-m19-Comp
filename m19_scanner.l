%option c++ prefix="m19_scanner_" outfile="m19_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <sstream>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "m19_parser.tab.h"

static std::ostringstream strlit;

inline void replace_char(char* text, char letter) {
    //char* text_cpy = 
    for (int i = 0; text[i]!='\0'; i++){
        if(text[i]==letter){
            text[i]='.';
        }
    }
}

// don't change this
#define yyerror LexerError
%}
SPACE     [ \t]
INT       [[:digit:]]+

LETTER  [A-Za-z]
IDENTIFIER   {LETTER}({LETTER}|[0-9]*|"_")*

%x X_COMMENT

%x X_STRING  X_BAR
%x X_OCTAL   X_HEX_INT
%x X_NEWLINE
%%

        //{ set_debug(1); }

  /* ====================================================================== */
  /* ====[                      Comentarios                              ]==== */
  /* ====================================================================== */
  
  /* {SPACE}*"..."\n        ;  ignore continuation marks */

"//".*\n               ; /* ignore comments */

"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"/*"        yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state();
<X_COMMENT>.|\n        ; /* ignore content */




">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE;
"<<"                   return tDLT;
">>"                   return tDGT;
"&&"                   return tAND;
"||"                   return tOR;

">>@"                  return tBREAK;
"@>>"                  return tCONTINUE;
">@<"                  return tRETURN;

"!!"                   return tPRINTLN;

  /* ====================================================================== */
  /* ====[                Identificadores (nomes)                     ]==== */
  /* ====================================================================== */

{IDENTIFIER}           yylval.s = new std::string(yytext); return tIDENTIFIER;


  /* ====================================================================== */
  /* ====[              Cadeias de caracteres                         ]==== */
  /* ====================================================================== */

"\""                   yy_push_state(X_STRING);
<X_STRING>"\\"         yy_push_state(X_BAR);
<X_STRING>"\""         {
                         yylval.s = new std::string(strlit.str());
                         strlit.str("");
                         yy_pop_state();
                         return tSTRING;
                       }
<X_STRING>.            strlit << *yytext;

<X_BAR>n               strlit << '\n'; yy_pop_state();
<X_BAR>r               strlit << '\r'; yy_pop_state();
<X_BAR>t               strlit << '\t'; yy_pop_state();
<X_BAR>"\""            strlit << '"'; yy_pop_state();
<X_BAR>"\\"            strlit << '\\'; yy_pop_state();

<X_BAR>[A-Fa-f0-9]{1,2} {
                                 strlit << (char)(unsigned char)strtoul(yytext, NULL, 16);
                                 yy_pop_state();
                               }
<X_BAR>.                 strlit << *yytext; yy_pop_state();


   /*
\'                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\'           yy_pop_state(); return tSTRING;
<X_STRING>\\\'         *yylval.s += yytext + 1;
<X_STRING>.            *yylval.s += yytext;
<X_STRING>\n           yyerror("newline in string");
    */

  /* ====================================================================== */
  /* ====[                      Inteiros                              ]==== */
  /* ====================================================================== */

"0x"[A-Fa-f0-9]+         yylval.i = strtoul(yytext, NULL, 16); return tINTEGER;
"0x"[A-Za-z0-9]+         yyerror("Invalid base 16 number.");

[0-9]+                 yylval.i = strtoul(yytext, NULL, 10); return tINTEGER;

  /* ====================================================================== */
  /* ====[                        Reais                               ]==== */
  /* ====================================================================== */

[0-9]+\.[0-9]+           { yylval.d = strtod(yytext, NULL); return tDOUBLE; }
[0-9]+a[0-9]+ { replace_char(yytext, 'a'); yylval.d = strtod(yytext, NULL)*1e-18; return tDOUBLE; }
[0-9]+f[0-9]+ { replace_char(yytext, 'f'); yylval.d = strtod(yytext, NULL)*1e-15; return tDOUBLE; }
[0-9]+p[0-9]+ { replace_char(yytext, 'p'); yylval.d = strtod(yytext, NULL)*1e-12; return tDOUBLE; }
[0-9]+n[0-9]+ { replace_char(yytext, 'n'); yylval.d = strtod(yytext, NULL)*1e-9; return tDOUBLE; }
[0-9]+u[0-9]+ { replace_char(yytext, 'u'); yylval.d = strtod(yytext, NULL)*1e-6; return tDOUBLE; }
[0-9]+m[0-9]+ { replace_char(yytext, 'm'); yylval.d = strtod(yytext, NULL)*1e-3; return tDOUBLE; }
[0-9]+k[0-9]+ { replace_char(yytext, 'k'); yylval.d = strtod(yytext, NULL)*1e3; return tDOUBLE; }
[0-9]+M[0-9]+ { replace_char(yytext, 'M'); yylval.d = strtod(yytext, NULL)*1e6; return tDOUBLE; }
[0-9]+G[0-9]+ { replace_char(yytext, 'G'); yylval.d = strtod(yytext, NULL)*1e9; return tDOUBLE; }
[0-9]+T[0-9]+ { replace_char(yytext, 'T'); yylval.d = strtod(yytext, NULL)*1e12; return tDOUBLE; }
[0-9]+P[0-9]+ { replace_char(yytext, 'P'); yylval.d = strtod(yytext, NULL)*1e15; return tDOUBLE; }
[0-9]+E[0-9]+ { replace_char(yytext, 'E'); yylval.d = strtod(yytext, NULL)*1e18; return tDOUBLE; }




[-@#()!?\[\]<>=+*/%;:${}.]       return *yytext;

[ \t\n\r]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
