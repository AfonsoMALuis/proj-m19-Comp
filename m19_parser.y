%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                  i;	/* integer value */
  double               d;
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  //cdk::literal_node     *literal;
  
  basic_type           *type;       /* expression type */

  m19::block_node      *block;
  m19::body_node       *body;
  m19::section_node    *section;
  m19::initial_section_node *initial_section;
  m19::final_section_node   *final_section;
};

%token tPRIVATE tPUBLIC
%token tPRINTLN tREAD 
%token tCONTINUE tBREAK tRETURN tDLT tDGT tAND

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING

%nonassoc tIF
%nonassoc ':' ';' '{' '}' '!' '#' '$'


%right '='
%left tOR
%left tAND
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY  tDLT tDGT
%nonassoc '[' ']' '(' ')'


%type <section> section
%type <initial_section> initial_section
%type <final_section> final_section
%type <s> string
%type <node> declaration instruction
%type <sequence> args exprs file innerdecls instructions opt_instructions sections
%type <expression> expr literal
%type <lvalue> lval
%type <body> body
%type <block> block

%type<type> data_type ints doubles strings
%type<node> vardecl fundecl fundef

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%
     
file           : declaration              { compiler->ast($$ = new cdk::sequence_node(LINE, $1)); }
               | file declaration         { compiler->ast($$ = new cdk::sequence_node(LINE, $2, $1)); }
               ;

declaration    : vardecl ';'    { $$ = $1; }
               | fundecl ';'    { $$ = $1; }
               | fundef         { $$ = $1; }
               ;               

vardecl  : data_type tIDENTIFIER                     { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tPRIVATE, nullptr); delete $2; }
         | data_type tIDENTIFIER '=' expr            { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tPRIVATE, $4); delete $2; }
         | data_type tIDENTIFIER '!'                 { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tPUBLIC, nullptr); delete $2; }
         | data_type tIDENTIFIER '?'                 { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tPUBLIC, nullptr); delete $2;}
         | data_type tIDENTIFIER '!' '=' expr        { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tPUBLIC, $5); delete $2;}
         | data_type tIDENTIFIER '?' '=' expr        { $$ = new m19::variable_declaration_node(LINE, $1, *$2, tPUBLIC, $5); delete $2;}
         ;
       
data_type    : ints            { $$ = $1; }
             | doubles         { $$ = $1; }
             | strings         { $$ = $1; }
             ;
             
	/*             
funcall  : tIDENTIFIER '(' ')'                ';'		{ $$ = new m19::function_call_node(LINE, *$1, nullptr); delete $1; }
         | tIDENTIFIER '(' args ')'           ';'               { $$ = new m19::function_call_node(LINE, *$1, $3); delete $1; }
	 | tIDENTIFIER '(' ')' 	    %prec tIF                   { $$ = new m19::function_call_node(LINE, *$1, nullptr); delete $1; }
         | tIDENTIFIER '(' args ')' %prec tIF                   { $$ = new m19::function_call_node(LINE, *$1, $3); delete $1; }
         ;*/
                
fundecl  : data_type tIDENTIFIER '(' ')'                { $$ = new m19::function_declaration_node(LINE, $1, *$2, tPRIVATE, nullptr); delete $2; }
         | '!'       tIDENTIFIER '(' ')'                { $$ = new m19::function_declaration_node(LINE, *$2, tPRIVATE, nullptr); delete $2; }
         | data_type tIDENTIFIER '!' '(' ')'            { $$ = new m19::function_declaration_node(LINE, $1, *$2, tPUBLIC, nullptr); delete $2; }
         | '!'       tIDENTIFIER '!' '(' ')'            { $$ = new m19::function_declaration_node(LINE, *$2, tPUBLIC, nullptr); delete $2; }
         | data_type tIDENTIFIER '?' '(' ')'            { $$ = new m19::function_declaration_node(LINE, $1, *$2, tPUBLIC, nullptr); delete $2; }
         | '!'       tIDENTIFIER '?' '(' ')'            { $$ = new m19::function_declaration_node(LINE, *$2, tPUBLIC, nullptr); delete $2; }
         | data_type tIDENTIFIER '(' args ')'           { $$ = new m19::function_declaration_node(LINE, $1, *$2, tPRIVATE, $4); delete $2; }
         | '!'       tIDENTIFIER '(' args ')'           { $$ = new m19::function_declaration_node(LINE, *$2, tPRIVATE, $4); delete $2; }
         | data_type tIDENTIFIER '!' '(' args ')'       { $$ = new m19::function_declaration_node(LINE, $1, *$2, tPUBLIC, $5); delete $2; }
         | '!'       tIDENTIFIER '!' '(' args ')'       { $$ = new m19::function_declaration_node(LINE, *$2, tPUBLIC, $5); delete $2; }
         | data_type tIDENTIFIER '?' '(' args ')'       { $$ = new m19::function_declaration_node(LINE, $1, *$2, tPUBLIC, $5); delete $2; }
         | '!'       tIDENTIFIER '?' '(' args ')'       { $$ = new m19::function_declaration_node(LINE, *$2, tPUBLIC, $5); delete $2; }
         ;
    
fundef   : data_type tIDENTIFIER '(' ')' body                           { $$ = new m19::function_definition_node(LINE, $1, *$2, tPRIVATE, nullptr, $5); delete $2; }
         | '!'       tIDENTIFIER '(' ')' body                           { $$ = new m19::function_definition_node(LINE, *$2, tPRIVATE, nullptr, $5); delete $2; }
         | data_type tIDENTIFIER '!' '(' ')' body                       { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC, nullptr, $6); delete $2; }
         | '!'       tIDENTIFIER '!' '(' ')' body                       { $$ = new m19::function_definition_node(LINE, *$2, tPUBLIC, nullptr, $6); delete $2; }
         | data_type tIDENTIFIER '?' '(' ')' body                       { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC, nullptr, $6); delete $2; }
         | '!'       tIDENTIFIER '?' '(' ')' body                       { $$ = new m19::function_definition_node(LINE, *$2, tPUBLIC, nullptr, $6); delete $2; }
         | data_type tIDENTIFIER '(' args ')' body                      { $$ = new m19::function_definition_node(LINE, $1, *$2, tPRIVATE, $4, $6); delete $2; }
         | '!'       tIDENTIFIER '(' args ')' body                      { $$ = new m19::function_definition_node(LINE, *$2, tPRIVATE, $4, $6); delete $2; }
         | data_type tIDENTIFIER '!' '(' args ')' body                  { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC, $5, $7); delete $2; }
         | '!'       tIDENTIFIER '!' '(' args ')' body                  { $$ = new m19::function_definition_node(LINE, *$2, tPUBLIC, $5, $7); delete $2; }
         | data_type tIDENTIFIER '?' '(' args ')' body                  { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC, $5, $7); delete $2; }
         | '!'       tIDENTIFIER '?' '(' args ')' body                  { $$ = new m19::function_definition_node(LINE, *$2, tPUBLIC, $5, $7); delete $2; }
         | data_type tIDENTIFIER '(' ')' '=' literal body               { $$ = new m19::function_definition_node(LINE, $1, *$2, tPRIVATE, nullptr, $7); delete $2; }
         | '!'       tIDENTIFIER '(' ')' '=' literal body               { $$ = new m19::function_definition_node(LINE, *$2, tPRIVATE, nullptr, $7); delete $2; }
         | data_type tIDENTIFIER '!' '(' ')' '=' literal body           { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC, nullptr, $8); delete $2; }
         | '!'       tIDENTIFIER '!' '(' ')' '=' literal body           { $$ = new m19::function_definition_node(LINE, *$2, tPUBLIC, nullptr, $8); delete $2; }
         | data_type tIDENTIFIER '?' '(' ')' '=' literal body           { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC, nullptr, $8); delete $2; }
         | '!'       tIDENTIFIER '?' '(' ')' '=' literal body           { $$ = new m19::function_definition_node(LINE, *$2, tPUBLIC, nullptr, $8); delete $2; }
         | data_type tIDENTIFIER '(' args ')' '=' literal body          { $$ = new m19::function_definition_node(LINE, $1, *$2, tPRIVATE, $4, $8); delete $2; }
         | '!'       tIDENTIFIER '(' args ')' '=' literal body          { $$ = new m19::function_definition_node(LINE, *$2, tPRIVATE, $4, $8); delete $2; }
         | data_type tIDENTIFIER '!' '(' args ')' '=' literal body      { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC, $5, $9); delete $2; }
         | '!'       tIDENTIFIER '!' '(' args ')' '=' literal body      { $$ = new m19::function_definition_node(LINE, *$2, tPUBLIC, $5, $9); delete $2; }
         | data_type tIDENTIFIER '?' '(' args ')' '=' literal body      { $$ = new m19::function_definition_node(LINE, $1, *$2, tPUBLIC, $5, $9); delete $2; }
         | '!'       tIDENTIFIER '?' '(' args ')' '=' literal body      { $$ = new m19::function_definition_node(LINE, *$2, tPUBLIC, $5, $9); delete $2; }
         ;
    
args     : vardecl           { $$ = new cdk::sequence_node(LINE, $1);     }
         | args ',' vardecl  { $$ = new cdk::sequence_node(LINE, $3, $1); }
         ;
         
body    : /* empty */                                { $$ = new m19::body_node(LINE, nullptr); }
        | 		  sections        	     { $$ = new m19::body_node(LINE, $1); }
        | initial_section                   	     { $$ = new m19::body_node(LINE, $1, nullptr); }
        | initial_section sections	             { $$ = new m19::body_node(LINE, $1, $2); }
        | 			    final_section    { $$ = new m19::body_node(LINE, nullptr, $1); }
        | 		  sections  final_section    { $$ = new m19::body_node(LINE, $1, $2); }
        | initial_section 	    final_section    { $$ = new m19::body_node(LINE, $1, nullptr, $2); }
        | initial_section sections  final_section    { $$ = new m19::body_node(LINE, $1, $2, $3); }
        ;
        
sections: section               { $$ = new cdk::sequence_node(LINE, $1); }
        | sections section      { $$ = new cdk::sequence_node(LINE, $2, $1); }
        ;
        
section : block                 { $$ = new m19::section_node(LINE, $1); }
        |'[' ']' block          { $$ = new m19::section_node(LINE, $3); }
        | '(' ')' block         { $$ = new m19::section_node(LINE, $3); }
        |'[' expr ']' block     { $$ = new m19::section_node(LINE, $2, $4); }
        |'(' expr ')' block     { $$ = new m19::section_node(LINE, $2, $4); }
        ;
        
initial_section : tDLT block    { $$ = new m19::initial_section_node(LINE, $2); }
                ;

final_section   : tDGT block    { $$ = new m19::final_section_node(LINE, $2); }
                ;
    
block   : '{' opt_instructions '}'             { $$ = new m19::block_node(LINE, nullptr, $2); }
        | '{' innerdecls opt_instructions '}'  { $$ = new m19::block_node(LINE, $2, $3);     }
        ;
     
innerdecls  :            vardecl ';' { $$ = new cdk::sequence_node(LINE, $1);     }
            | innerdecls vardecl ';' { $$ = new cdk::sequence_node(LINE, $2, $1); }
            ;

opt_instructions: /* empty */  { $$ = new cdk::sequence_node(LINE); }
                | instructions { $$ = $1; }
                ;
                
instructions    : instruction                   { $$ = new cdk::sequence_node(LINE, $1);     }
                | instruction instructions      { std::reverse($2->nodes().begin(), $2->nodes().end()); $$ = new cdk::sequence_node(LINE, $1, $2); std::reverse($$->nodes().begin(), $$->nodes().end()); }
                ;

instruction     : '[' expr ']' '#' instruction                          { $$ = new m19::if_node(LINE, $2, $5); }
                | '[' expr ']' '?' instruction %prec tIF                { $$ = new m19::if_node(LINE, $2, $5); }
                | '[' expr ']' '?' instruction ':' instruction   	{ $$ = new m19::if_else_node(LINE, $2, $5, $7); }
                | '['       ';' exprs ';' exprs ']' instruction        	{ $$ = new m19::for_node(LINE, nullptr, $3, $5, $7); }
		| '['       ';' exprs ';'       ']' instruction         { $$ = new m19::for_node(LINE, nullptr, $3, nullptr, $6); }
		| '['       ';'       ';' exprs ']' instruction         { $$ = new m19::for_node(LINE, nullptr, nullptr, $4, $6); }
		| '['       ';'       ';'       ']' instruction         { $$ = new m19::for_node(LINE, nullptr, nullptr, nullptr, $5); }
		| '[' args  ';'       ';' exprs ']' instruction         { $$ = new m19::for_node(LINE, $2, nullptr, $5, $7); }
		| '[' args  ';' exprs ';'       ']' instruction         { $$ = new m19::for_node(LINE, $2, $4, nullptr, $7); }
		| '[' args  ';'       ';'       ']' instruction         { $$ = new m19::for_node(LINE, $2, nullptr, nullptr, $6); }
                | '[' exprs ';'       ';' exprs ']' instruction         { $$ = new m19::for_node(LINE, $2, nullptr, $5, $7); }
		| '[' exprs ';' exprs ';'       ']' instruction         { $$ = new m19::for_node(LINE, $2, $4, nullptr, $7); }
		| '[' exprs ';'       ';'       ']' instruction         { $$ = new m19::for_node(LINE, $2, nullptr, nullptr, $6); }
		| '[' args  ';' exprs ';' exprs ']' instruction         { $$ = new m19::for_node(LINE, $2, $4, $6, $8); }
                | '[' exprs ';' exprs ';' exprs ']' instruction         { $$ = new m19::for_node(LINE, $2, $4, $6, $8); }
                | expr ';'                                              { $$ = new m19::evaluation_node(LINE, $1); }
                | expr '!'                                              { $$ = new m19::print_node(LINE, $1, false); }
                | expr tPRINTLN                                         { $$ = new m19::print_node(LINE, $1, true); }
                | tBREAK                                                { $$ = new m19::break_node(LINE);  }
                | tCONTINUE                                             { $$ = new m19::continue_node(LINE); }
                | tRETURN                                               { $$ = new m19::return_node(LINE); }
                | block                                                 { $$ = $1; }
                ;

expr : tINTEGER                  { $$ = new cdk::integer_node(LINE, $1); }
     | string                    { $$ = new cdk::string_node(LINE, $1); }
     | tDOUBLE                   { $$ = new cdk::double_node(LINE, $1); }
     | '-' expr %prec tUNARY     { $$ = new cdk::neg_node(LINE, $2); }
     | '+' expr %prec tUNARY     { $$ = $2/*new m19::identity_node(LINE, $2)*/; }
     | expr '+' expr	         { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	         { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	         { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	         { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	         { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr	         { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	         { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	         { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr             { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	         { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	         { $$ = new cdk::eq_node(LINE, $1, $3); }
     | '~' expr                  { $$ = new cdk::not_node(LINE, $2); }
     | expr tAND expr            { $$ = new cdk::and_node(LINE, $1, $3); }
     | expr tOR expr             { $$ = new cdk::or_node (LINE, $1, $3); }
     | '@' '=' expr              { $$ = new m19::read_node(LINE); } //FIXME
     | '(' expr ')'              { $$ = $2; }
     | '[' expr ']'              { $$ = $2; }
     | lval                      { $$ = new cdk::rvalue_node(LINE, $1); }
     | lval '?'                  { $$ = new m19::address_of_node(LINE, $1); }
     | lval '=' expr             { $$ = new cdk::assignment_node(LINE, $1, $3); }
     | '0'                       { $$ = nullptr; }
     | tIDENTIFIER '('      ')'  { $$ = new m19::function_call_node(LINE, *$1, nullptr); delete $1; }
     | tIDENTIFIER '(' exprs ')'  { $$ = new m19::function_call_node(LINE, *$1, $3); delete $1; }
     ;
     
     
literal : tINTEGER                  { $$ = new cdk::integer_node(LINE, $1); }
        | string                    { $$ = new cdk::string_node(LINE, $1); }
        | tDOUBLE                   { $$ = new cdk::double_node(LINE, $1); }
        ;
exprs     : expr               { $$ = new cdk::sequence_node(LINE, $1); }
          | exprs ',' expr     { $$ = new cdk::sequence_node(LINE, $3, $1); }
          ;
          
lval      : tIDENTIFIER             { $$ = new cdk::variable_node(LINE, $1); }
          | lval '[' expr ']'       { $$ = new m19::left_index_node(LINE, new cdk::rvalue_node(LINE, $1), $3);}
          ;
     
string : tSTRING               { $$ = $1; }
       | string tSTRING        { $$ = new std::string(*$1 + *$2); delete $1; delete $2; }
       ;

ints  : '#'                   { $$ = new basic_type(4, basic_type::TYPE_INT);    }
      | '<' ints '>'          { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = new basic_type(4, basic_type::TYPE_INT); }
      |tDLT ints tDGT         { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = new basic_type(4, basic_type::TYPE_INT); }
      ;

doubles  : '%'                     { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
         | '<' doubles '>'         { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = new basic_type(8, basic_type::TYPE_DOUBLE); }
         | tDLT doubles tDGT       { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = new basic_type(8, basic_type::TYPE_DOUBLE); }
         ;
    
strings  : '$'                          { $$ = new basic_type(4, basic_type::TYPE_STRING); }
         | '<' strings '>'              { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = new basic_type(4, basic_type::TYPE_STRING); }
         | tDLT strings tDGT            { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = new basic_type(4, basic_type::TYPE_STRING); }
         ;
         

    
    /*
blktion         : '[' expr ']' '#' instruction                          { $$ = new m19::if_node(LINE, $2, $5); }
                | '[' expr ']' '?' instruction                          { $$ = new m19::if_node(LINE, $2, $5); }
                | '[' expr ']' '?' instruction ':' instruction          { $$ = new m19::if_else_node(LINE, $2, $5, $7); }
                | '[' args ';' exprs ';' exprs ']' instruction          { $$ = new m19::for_node(LINE, $2, $4, $6, $8); }
                | '[' exprs ';' exprs ';' exprs ']' instruction         { $$ = new m19::for_node(LINE, $2, $4, $6, $8); }
                ;
    
list : stmt	     { $$ = new cdk::sequence_node(LINE, $1); }
     | list stmt { $$ = new cdk::sequence_node(LINE, $2, $1); }
	 ;

stmt : expr ';'                                             { $$ = new m19::evaluation_node(LINE, $1); }
     | tPRINT expr ';'                                      { $$ = new m19::print_node(LINE, $2); }
     | '@' lval ';'                                       { $$ = new m19::read_node(LINE, $2); }
     | tFOR '(' expr ';' expr ';' expr ')' stmt             { $$ = new m19::for_node(LINE, $3, $5, $7, $9); }
     | tIF '(' expr ')' stmt %prec tIFX                     { $$ = new m19::if_node(LINE, $3, $5); }
     | tIF '(' expr ')' stmt tELSE stmt                     { $$ = new m19::if_else_node(LINE, $3, $5, $7); }
     | '{' list '}'                                         { $$ = $2; }
     ;
    */
    

%%
