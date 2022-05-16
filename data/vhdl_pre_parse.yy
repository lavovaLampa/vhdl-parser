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
%token LOOP
%token PACKAGE
%token PROCEDURE
%token PROCESS
%token PROTECTED
%token RECORD
%token UNITS

%token BASIC_IDENTIFIER
%token EXTENDED_IDENTIFIER

%start block_list
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
  | subprogram_body
  ;

block_list:
    %empty
  | generic_block block_list
  ;

temp_match.inside:
    generic_block
  | identifier
  | IN
  | ';'
  ;

temp_match:
    %empty
  | temp_match.inside temp_match
  ;

architecture_body:
    ARCHITECTURE identifier temp_match BEGIN temp_match END identifier_opt ';'
  | ARCHITECTURE identifier temp_match BEGIN temp_match END ARCHITECTURE identifier_opt ';'
  ;

block_or_component_configuration:
  FOR temp_match END FOR ';'
  ;

block_statement:
  BLOCK temp_match BEGIN temp_match END BLOCK
  ;

case_statement:
  CASE temp_match END CASE
  ;

component_declaration:
  COMPONENT identifier temp_match END COMPONENT
  ;

configuration_declaration:
    CONFIGURATION identifier temp_match END identifier_opt ';'
  | CONFIGURATION identifier temp_match END CONFIGURATION identifier_opt ';'
  ;

entity_declaration:
    ENTITY identifier temp_match END identifier_opt ';'
  | ENTITY identifier temp_match BEGIN temp_match END identifier_opt ';'
  | ENTITY identifier temp_match END ENTITY identifier_opt ';'
  | ENTITY identifier temp_match BEGIN temp_match END ENTITY identifier_opt ';'
  ;

generate_statement:
    GENERATE temp_match END GENERATE
  | GENERATE temp_match BEGIN temp_match END GENERATE
  ;

if_statement:
  IF temp_match END IF
  ;

loop_statement:
    FOR identifier IN temp_match LOOP temp_match END LOOP
  | LOOP temp_match END LOOP
  ;

package_body:
    PACKAGE BODY temp_match END identifier_opt ';'
  | PACKAGE BODY temp_match END PACKAGE BODY identifier_opt ';'
  ;

package_declaration:
    PACKAGE identifier temp_match END identifier_opt ';'
  | PACKAGE identifier temp_match END PACKAGE identifier_opt ';'
  ;

physical_type_definition:
  UNITS temp_match END UNITS
  ;

process_statement:
  PROCESS temp_match BEGIN temp_match END PROCESS
  ;

protected_type_body:
  PROTECTED BODY temp_match END PROTECTED BODY
  ;

protected_type_declaration:
  PROTECTED temp_match END PROTECTED
  ;

record_type_definition:
  RECORD temp_match END RECORD
  ;

subprogram_body:
    FUNCTION temp_match BEGIN temp_match END FUNCTION identifier_opt ';'
  | FUNCTION temp_match BEGIN temp_match END identifier_opt ';'
  | PROCEDURE temp_match BEGIN temp_match END PROCEDURE identifier_opt ';'
  | PROCEDURE temp_match BEGIN temp_match END identifier_opt ';'
  ;

%%
