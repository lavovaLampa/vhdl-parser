%token ARCHITECTURE
%token BEGIN
%token BLOCK
%token BODY
%token CASE
%token COMPONENT
%token CONFIGURATION
%token END
%token ENTITY
%token FOR
%token FUNCTION
%token GENERATE
%token IF
%token IN
%token IS
%token LOOP
%token PACKAGE
%token PROCEDURE
%token PROCESS
%token PROTECTED
%token RECORD
%token UNITS

%token BASIC_IDENTIFIER
%token EXTENDED_IDENTIFIER

%start match_any
%%

identifier:
    BASIC_IDENTIFIER
  | EXTENDED_IDENTIFIER
  ;

identifier_opt:
    %empty
  | identifier
  ;

generic_block:
    architecture_body
  | block_or_component_configuration
  | block_statement
  | case_statement
  | component_declaration
  | configuration_declaration
  | entity_declaration  
  | generate_statement
  | if_statement
  | loop_statement
  | package_body
  | package_declaration
  | physical_type_definition
  | process_statement
  | protected_type_body
  | protected_type_declaration
  | record_type_definition
  | function_decl_body
  | procedure_decl_body
  ;

temp_match.inside:
    generic_block
  | identifier
  | IN
  | IS
  | ';'
  ;

match_any:
    %empty
  | temp_match.inside match_any
  ;

identifier_mopt:
    %empty
  | identifier identifier_mopt
  ;

temp_no_block_match.inside:
    identifier
  | IN
  | IS
  ;

temp_no_block_match:
    %empty
  | temp_no_block_match.inside temp_no_block_match
  ;

architecture_body:
    ARCHITECTURE identifier match_any BEGIN match_any END identifier_opt ';'
  | ARCHITECTURE identifier match_any BEGIN match_any END ARCHITECTURE identifier_opt ';'
  ;

block_or_component_configuration:
    FOR match_any END FOR ';'
  ;

loop_statement:
    FOR identifier IN temp_no_block_match LOOP match_any END LOOP
  | LOOP match_any END LOOP
  ;

block_statement:
    BLOCK match_any BEGIN match_any END BLOCK
  ;

case_statement:
    CASE match_any END CASE
  ;

component_declaration:
    COMPONENT identifier match_any END COMPONENT
  ;

configuration_declaration:
    CONFIGURATION identifier match_any END identifier_opt ';'
  | CONFIGURATION identifier match_any END CONFIGURATION identifier_opt ';'
  ;

entity_declaration:
    ENTITY identifier match_any BEGIN match_any END identifier_opt ';'
  | ENTITY identifier match_any BEGIN match_any END ENTITY identifier_opt ';'
  | ENTITY identifier match_any END ENTITY identifier_opt ';'
  | ENTITY identifier match_any END identifier_opt ';'
  ;

generate_statement:
    GENERATE match_any END GENERATE
  | GENERATE match_any BEGIN match_any END GENERATE
  ;

if_statement:
    IF match_any END IF
  ;

package_body:
    PACKAGE BODY match_any END identifier_opt ';'
  | PACKAGE BODY match_any END PACKAGE BODY identifier_opt ';'
  ;

package_declaration:
    PACKAGE identifier match_any END identifier_opt ';'
  | PACKAGE identifier match_any END PACKAGE identifier_opt ';'
  ;

physical_type_definition:
    UNITS match_any END UNITS
  ;

process_statement:
    PROCESS match_any BEGIN match_any END PROCESS
  ;

protected_type_body:
    PROTECTED BODY match_any END PROTECTED BODY
  ;

protected_type_declaration:
    PROTECTED match_any END PROTECTED
  ;

record_type_definition:
    RECORD match_any END RECORD
  ;

function_decl_body:
    FUNCTION identifier_mopt ';'
    FUNCTION identifier_mopt IS match_any BEGIN match_any END identifier_opt ';'
  | FUNCTION identifier_mopt IS match_any BEGIN match_any END FUNCTION identifier_opt ';'
  ;

procedure_decl_body:
    PROCEDURE identifier_mopt ';'
    PROCEDURE identifier_mopt IS match_any BEGIN match_any END identifier_opt ';'
  | PROCEDURE identifier_mopt IS match_any BEGIN match_any END PROCEDURE identifier_opt ';'
  ;

%%
