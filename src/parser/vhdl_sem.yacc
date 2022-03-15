%token ABS
%token ACCESS
%token AFTER
%token ALIAS
%token ALL
%token AND
%token ARCHITECTURE
%token ARRAY
%token ASSERT
%token ATTRIBUTE
%token BEGIN
%token BLOCK
%token BODY
%token BUFFER
%token BUS
%token CASE
%token COMPONENT
%token CONFIGURATION
%token CONSTANT
%token DISCONNECT
%token DOWNTO
%token ELSE
%token ELSIF
%token END
%token ENTITY
%token EXIT
%token FILE
%token FOR
%token FUNCTION
%token GENERATE
%token GENERIC
%token GROUP
%token GUARDED
%token IF
%token IMPURE
%token IN
%token INERTIAL
%token INOUT
%token IS
%token LABEL
%token LIBRARY
%token LINKAGE
%token LITERAL
%token LOOP
%token MAP
%token MOD
%token NAND
%token NEW
%token NEXT
%token NOR
%token NOT
%token ID_NULL
%token OF
%token ON
%token OPEN
%token OR
%token OTHERS
%token OUT
%token PACKAGE
%token PORT
%token POSTPONED
%token PROCEDURE
%token PROCESS
%token PROTECTED
%token PURE
%token RANGE
%token RECORD
%token REGISTER
%token REJECT
%token REM
%token REPORT
%token RETURN
%token ROL
%token ROR
%token SELECT
%token SEVERITY
%token SHARED
%token SIGNAL
%token SLA
%token SLL
%token SRA
%token SRL
%token SUBTYPE
%token THEN
%token TO
%token TRANSPORT
%token TYPE
%token UNAFFECTED
%token UNITS
%token UNTIL
%token USE
%token VARIABLE
%token WAIT
%token WHEN
%token WHILE
%token WITH
%token XNOR
%token XOR

%token ARROW                "=>"
%token DOUBLE_STAR          "**"
%token VARIABLE_ASSIGNMENT  ":="
%token INEQUALITY           "/="
%token GT_OR_EQUAL          ">="
%token LT_OR_EQUAL          "<="
%token BOX                  "<>"

%token BASED_LITERAL
%token BASIC_IDENTIFIER
%token BIT_STRING_LITERAL
%token CHARACTER_LITERAL
%token DECIMAL_LITERAL
%token EXTENDED_IDENTIFIER
%token STRING_LITERAL

%start design_file

%glr-parser
%language "c++"

%define parse.error detailed

%%
signal_name:
    // SEMANTIC: "name" must be a signal name
    name
  ;

variable_name:
    // SEMANTIC: "name" must be a VARIABLE name
    name
  ;

file_name:
    // SEMANTIC: "name" must be a FILE name
    name
  ;

function_name:
    // SEMANTIC: "name" must be a FUNCTION name
    name
  ;

architecture_name:
    // SEMANTIC: "name" must be an ARCHITECTURE name
    name
  ;

block_statement_label:
    // SEMANTIC: "label" must be a BLOCK statement label
    label
  ;

generate_statement_label:
    // SEMANTIC: "label" must be a GENERATE statement label
    label
  ;

generic_name:
    // SEMANTIC: "name" must be a GENERIC name
    name
  ;

port_name:
    // SEMANTIC: "name" must be a PORT name
    name
  ;

parameter_name:
    // SEMANTIC: "name" must be a PARAMETER name
    name
  ;

procedure_name:
    // SEMANTIC: "name" must be a PROCEDURE name
    name
  ;

abstract_literal:
    DECIMAL_LITERAL
  | BASED_LITERAL
  ;

access_type_definition:
    ACCESS subtype_indication
  ;

actual_designator:
    expression
  | signal_name
  | variable_name
  | file_name
  | OPEN
  ;

actual_part:
    actual_designator
  | function_name '(' actual_designator ')'
  | type_mark '(' actual_designator ')'
  ;

adding_operator:
    '+'
  | '-'
  | '&'
  ;

aggregate:
    '(' element_association ')'
  | '(' aggregate ',' element_association ')'
  ;

alias_declaration:
    ALIAS alias_designator alias_declaration.opt_subtype IS name alias_declaration.opt_signature
  ;

alias_declaration.opt_subtype:
    %empty
  | ':' subtype_indication
  ;

alias_declaration.opt_signature:
    %empty
  | signature
  ;

alias_designator:
    identifier
  | CHARACTER_LITERAL
  | operator_symbol
  ;

allocator:
    NEW subtype_indication
  | NEW qualified_expression
  ;

architecture_body:
    // SEMANTIC: "name" must be an ENTITY name
    ARCHITECTURE identifier OF name IS
      architecture_declarative_part
    BEGIN
      architecture_statement_part
    // SEMANTIC: "simple_name_opt" must be an ARCHITECTURE name
    // SEMANTIC: "simple_name_opt" == "identifier"
    END architecture_body.architecture_opt simple_name_opt ';'
  ;
  
architecture_body.architecture_opt:
    %empty
  | ARCHITECTURE
  ;

architecture_declarative_part:
    %empty
  | architecture_declarative_part block_declarative_item
  ;

architecture_statement_part:
    %empty
  | architecture_statement_part concurrent_statement
  ;

array_type_definition:
    unconstrained_array_definition
  | constrained_array_definition
  ;

assertion:
    ASSERT condition assertion.report_opt assertion.severity_opt
  ;

assertion.report_opt:
    %empty
  | REPORT expression
  ;

assertion.severity_opt:
    %empty
  | SEVERITY expression
  ;

assertion_statement:
    assertion ';'
  ;

association_element:
    formal_part "=>" actual_part
  | actual_part
  ;

association_list:
    association_element
  | association_list ',' association_element
  ;

attribute_declaration:
    ATTRIBUTE identifier ':' type_mark ';'
  ;

attribute_designator:
    // SEMANTIC: "simple_name" must be an ATTRIBUTE name
    simple_name
  ;

attribute_name:
    prefix signature '\'' attribute_designator attribute_name.expression_opt 
  ;

attribute_name.expression_opt:
    %empty
  | '(' expression ')'
  ;

attribute_specification:
    ATTRIBUTE attribute_designator OF entity_specification IS expression ';'
  ;

binding_indication:
    binding_indication.entity_aspect_opt
    binding_indication.generic_map_aspect_opt
    binding_indication.port_map_aspect_opt
  ;

binding_indication.entity_aspect_opt:
    %empty
  | USE entity_aspect
  ;

binding_indication.generic_map_aspect_opt:
    %empty
  | generic_map_aspect
  ;

binding_indication.port_map_aspect_opt:
    %empty
  | port_map_aspect
  ;

block_configuration:
    FOR block_specification
      block_configuration.use_clause_mopt
      block_configuration.configuration_item_mopt
    END FOR ';'
  ;

block_configuration.use_clause_mopt:
    %empty
  | block_configuration.use_clause_mopt use_clause
  ;

block_configuration.configuration_item_mopt:
    %empty
  | block_configuration.configuration_item_mopt configuration_item
  ;

block_declarative_item:
    subprogram_declaration
  | subprogram_body
  | type_declaration
  | subtype_declaration
  | constant_declaration
  | signal_declaration
  | shared_variable_declaration
  | file_declaration
  | alias_declaration
  | component_declaration
  | attribute_declaration
  | attribute_specification
  | configuration_specification
  | disconnection_specification
  | use_clause
  | group_template_declaration
  | group_declaration    
  ;

block_declarative_part:
    %empty
  | block_declarative_part block_declarative_item
  ;

block_header:
    block_header.generic_clause_opt
    block_header.port_clause_opt
  ;

block_header.generic_clause_opt:
    %empty
  | generic_clause block_header.generic_clause_opt.generic_map_aspect_opt
  ;

block_header.generic_clause_opt.generic_map_aspect_opt:
    %empty
  | generic_map_aspect ';'
  ;

block_header.port_clause_opt:
    %empty
  | port_clause block_header.port_clause_opt.port_map_aspect_opt
  ;

block_header.port_clause_opt.port_map_aspect_opt:
    %empty
  | port_map_aspect ';'
  ;

block_specification:
    architecture_name
  | block_statement_label
  | generate_statement_label
  // TODO: Replace with another rule?
  | generate_statement_label '(' index_specification ')'
  ;

block_statement:
    label ':'
      BLOCK block_statement.guard_expression_opt is_opt
        block_header
        block_declarative_part
      BEGIN
        block_statement_part
      // SEMANTIC: "label_opt" must a BLOCK label
      // SEMANTIC: "label_opt" == "label"
      END BLOCK label_opt ';'
  ;

block_statement.guard_expression_opt:
    %empty
  // SEMANTIC: "expression" must be a GUARD (boolean) expression
  | '(' expression ')'
  ;

block_statement_part:
    %empty
  | block_statement_part concurrent_statement
  ;

bus_opt:
    %empty
  | BUS
  ;

case_statement:
    CASE expression IS
      case_statement.case_statement_alternative_mul
    // SEMANTIC: "label_opt" must be a CASE label
    // SEMANTIC: "label_opt" == "label_colon_opt"
    END CASE label_opt ';'
  ;

case_statement.case_statement_alternative_mul:
    case_statement_alternative
  | case_statement.case_statement_alternative_mul case_statement_alternative
  ;

case_statement_alternative:
    WHEN choices "=>"
      sequence_of_statements
  ;

choice:
    simple_expression
  | discrete_range
  // SEMANTIC: "simple_name" must be an ELEMENT name
  // "simple_name" can be parsed as a simple_expression
  /* | simple_name */
  | OTHERS
  ;

choices:
    choice
  | choices '|' choice
  ;

component_configuration:
    FOR component_specification
      component_configuration.binding_indication_opt
      component_configuration.block_configuration_opt
    END FOR ';'
  ;

component_configuration.binding_indication_opt:
    %empty
  | binding_indication ';'
  ;

component_configuration.block_configuration_opt:
    %empty
  | block_configuration
  ;

component_declaration:
    COMPONENT identifier is_opt
      component_declaration.local_generic_clause_opt
      component_declaration.local_port_clause_opt
    // SEMANTIC: "simple_name_opt" must be a COMPONENT name
    // SEMANTIC: "simple_name_opt" == "identifier"
    END COMPONENT simple_name_opt ';'
  ;

component_declaration.local_generic_clause_opt:
    %empty
    // SEMANTIC: "generic_clause" must be a LOCAL generic clause
  | generic_clause
  ;

component_declaration.local_port_clause_opt:
    %empty
    // SEMANTIC: "port_clause" must be a LOCAL port clause
  | port_clause
  ;

component_instantiation_statement:
    label ':'
      instantiated_unit
        component_instantiation_statement.generic_map_aspect_opt
        component_instantiation_statement.port_map_aspect_opt ';'
  ;

component_instantiation_statement.generic_map_aspect_opt:
    %empty
  | generic_map_aspect
  ;

component_instantiation_statement.port_map_aspect_opt:
    %empty
  | port_map_aspect
  ;

component_specification:
    // SEMANTIC: "name" must be a COMPONENT name
    instantiation_list ':' name
  ;

composite_type_definition:
    array_type_definition
  | record_type_definition
  ;

concurrent_assertion_statement:
    label_colon_opt postponed_opt assertion ';'
  ;

label_colon_opt:
    %empty
  | label ':'
  ;

postponed_opt:
    %empty
  | POSTPONED
  ;

concurrent_procedure_call_statement:
    label_colon_opt postponed_opt procedure_call ';'
  ;

concurrent_signal_assignment_statement:
    label_colon_opt postponed_opt conditional_signal_assignment
  | label_colon_opt postponed_opt selected_signal_assignment
  ;

concurrent_statement:
    block_statement
  | process_statement
  | concurrent_procedure_call_statement
  | concurrent_assertion_statement
  | concurrent_signal_assignment_statement
  | component_instantiation_statement
  | generate_statement    
  ;

condition:
    // SEMANTIC: "expression" must be a BOOLEAN expression
    expression
  ;

condition_clause:
    UNTIL condition
  ;

conditional_signal_assignment:
    target "<=" options conditional_waveforms ';'
  ;

conditional_waveforms:
    conditional_waveforms.waveform_condition_mopt
    waveform conditional_waveforms.when_condition_opt
  ;

conditional_waveforms.waveform_condition_mopt:
    %empty
  | conditional_waveforms.waveform_condition_mopt waveform WHEN condition ELSE
  ;

conditional_waveforms.when_condition_opt:
    %empty
  | WHEN condition
  ;

configuration_declaration:
    // SEMANTIC: "name" must be an ENTITY name
    CONFIGURATION identifier OF name IS
      configuration_declarative_part
      block_configuration
    // SEMANTIC: "simple_name_opt" must be a CONFIGURATION name
    // SEMANTIC: "simple_name_opt" == identifier
    END configuration_declaration.configuration_opt simple_name_opt ';'
  ;

configuration_declaration.configuration_opt:
    %empty
  | CONFIGURATION
  ;

configuration_declarative_item:
    use_clause
  | attribute_specification
  | group_declaration
  ;

configuration_declarative_part:
    %empty
  | configuration_declarative_part configuration_declarative_item
  ;

configuration_item:
    block_configuration
  | component_configuration
  ;

configuration_specification:
    FOR component_specification binding_indication ';'
  ;

constant_declaration:
    CONSTANT identifier_list ':' subtype_indication constant_declaration.expression_opt ';'

constant_declaration.expression_opt:
    %empty
  | ":=" expression
  ;

constrained_array_definition:
    // SEMANTIC: "subtype_indication" is an ELEMENT subtype indication
    ARRAY index_constraint OF subtype_indication
  ;

constraint:
    range_constraint
  | index_constraint
  ;

constraint_opt:
    %empty
  | constraint
  ;

context_clause:
    %empty
  | context_clause context_item
  ;

context_item:
    library_clause
  | use_clause
  ;

delay_mechanism:
    TRANSPORT
  | INERTIAL
  | REJECT time_expression INERTIAL
  ;

design_file:
    design_unit
  | design_file design_unit
  ;

design_unit:
    context_clause library_unit
  ;

designator:
    identifier
  | operator_symbol
  ;

designator_opt:
    %empty
  | designator
  ;

direction:
    TO
  | DOWNTO
  ;

disconnection_specification:
    DISCONNECT guarded_signal_specification AFTER time_expression ';'
  ;

discrete_range:
    // SEMANTIC: "subtype_indication" must be a DISCRETE subtype
    subtype_indication
  | range
  ;

element_association:
    expression
  | choices "=>" expression
  ;

element_declaration:
    identifier_list ':' element_subtype_definition ';'
  ;

element_subtype_definition:
    subtype_indication
  ;

entity_aspect:
    // SEMANTIC: "name" must be an ENTITY name
    ENTITY name
    // SEMANTIC: "name" must be an ENTITY name
    // SEMANTIC: "identifier" must be an architecture identifier
  | ENTITY name '(' identifier ')'
    // SEMANTIC: "name" must be a CONFIGURATION name
  | CONFIGURATION name
  | OPEN
  ;

entity_class:
    ENTITY
  | PROCEDURE
  | TYPE
  | SIGNAL
  | LABEL    
  | ARCHITECTURE
  | FUNCTION
  | SUBTYPE
  | VARIABLE
  | LITERAL
  | CONFIGURATION
  | PACKAGE
  | CONSTANT
  | COMPONENT
  | UNITS
  ;

entity_class_entry:
    entity_class
  | entity_class "<>"
  ;

entity_class_entry_list:
    entity_class_entry
  | entity_class_entry_list ',' entity_class_entry
  ;


entity_declaration:
    ENTITY identifier IS
      entity_header
      entity_declarative_part
    entity_declaration.entity_statement_part_opt
    // SEMANTIC: "simple_name_opt" must be an ENTITY name
    // SEMANTIC: "simple_name_opt" == "identifier"
    END entity_declaration.entity_opt simple_name_opt ';'
  ;

entity_declaration.entity_statement_part_opt:
    %empty
  | BEGIN
      entity_statement_part
  ;

entity_declaration.entity_opt:
    %empty
  | ENTITY
  ;

entity_declarative_item:
    subprogram_declaration
  | subprogram_body
  | type_declaration
  | subtype_declaration
  | constant_declaration
  | signal_declaration
  | shared_variable_declaration
  | file_declaration
  | alias_declaration
  | attribute_declaration
  | attribute_specification
  | disconnection_specification
  | use_clause
  | group_template_declaration
  | group_declaration
  ;

entity_declarative_part:
    %empty
  | entity_declarative_part entity_declarative_item
  ;

entity_designator:
    entity_tag
  | entity_tag signature
  ;

entity_header:
    %empty
    // SEMANTIC: "generic_clause" must be a FORMAL generic clause
  | generic_clause
    // SEMANTIC: "port_clause" must be a FORMAL port clause
  | port_clause
    // SEMANTIC: "generic_clause" must be a FORMAL generic clause
    // SEMANTIC: "port_clause" must be a FORMAL port clause
  | generic_clause port_clause
  ;

entity_name_list:
    entity_name_list.entity_designator_mul
  | OTHERS
  | ALL
  ;

entity_name_list.entity_designator_mul:
    entity_designator
  | entity_name_list.entity_designator_mul ',' entity_designator
  ;

entity_specification:
    entity_name_list ':' entity_class
  ;

entity_statement:
    concurrent_assertion_statement
    // SEMANTIC: "procedure_call" must be a PASSIVE PROCEDURE call
    // TODO: Check if this is correct, maybe not statement, only call?
  | concurrent_procedure_call_statement
    // SEMANTIC: "process_statement" must be a PASSIVE PROCESS
  | process_statement
  ;

entity_statement_part:
    %empty
  | entity_statement_part entity_statement
  ;

entity_tag:
    simple_name
  | CHARACTER_LITERAL
  | operator_symbol
  ;

enumeration_literal:
    identifier
  | CHARACTER_LITERAL
  ;

enumeration_type_definition:
    '(' enumeration_type_definition.inner ')'
  ;

enumeration_type_definition.inner:
    enumeration_literal
  | enumeration_type_definition.inner ',' enumeration_literal
  ;

exit_statement:
    // SEMANTIC: "label_opt" must be a LOOP label
    EXIT label_opt exit_statement.when_condition_opt ';'
  ;

exit_statement.when_condition_opt:
    %empty
  | WHEN condition
  ;

expanded_selected_name:
    simple_prefix '.' suffix
  ;

simple_prefix:
    expanded_selected_name
  | simple_name
  ;

expression:
    relation expression.logical_expression
  ;

expression.logical_expression:
    %empty
  | expression.and_relation_mul
  | expression.or_relation_mul
  | expression.xor_relation_mul
  | NAND relation
  | NOR relation
  | expression.xnor_relation_mul
  ;

expression.and_relation_mul:
    AND relation
  | expression.and_relation_mul AND relation
  ;

expression.or_relation_mul:
    OR relation
  | expression.or_relation_mul OR relation
  ;

expression.xor_relation_mul:
    XOR relation
  | expression.xor_relation_mul XOR relation
  ;

expression.xnor_relation_mul:
    XNOR relation
  | expression.xnor_relation_mul XNOR relation
  ;

factor:
    primary
  | primary "**" primary
  | ABS primary
  | NOT primary
  ;

file_declaration:
    FILE identifier_list ':' subtype_indication ';'
  | FILE identifier_list ':' subtype_indication file_open_information ';'
  ;

file_logical_name:
    // SEMANTIC: "expression" must be a STRING EXPRESSION
    expression
  ;

file_open_kind_expression:
    // SEMANTIC: "expression" must be a FILE_OPEN_KIND EXPRESSION
    expression
  ;

file_open_information:
    IS file_logical_name
  | OPEN file_open_kind_expression IS file_logical_name
  ;

file_type_definition:
    FILE OF type_mark
  ;

formal_designator:
    generic_name
  | port_name
  | parameter_name    
  ;

formal_parameter_list:
    // SEMANTIC: "interface_list" is a PARAMETER INTERFACE LIST
    interface_list
  ;

formal_part:
    formal_designator
  | function_name '(' formal_designator ')'
  | type_mark '(' formal_designator ')'
  ;

full_type_declaration:
    TYPE identifier IS type_definition ';'
  ;

function_call:
    function_name
    // SEMANTIC: "association_list" is a PARAMETER association list
  | function_name '(' association_list ')'
  ;

generate_statement:
    label ':'
      generation_scheme GENERATE
        generate_statement.block_declarative_statement_opt
        generate_statement.concurrent_statement_mopt
      // SEMANTIC: "label_opt" must be a GENERATE label
      // SEMANTIC: "label_opt" == "label"
      END GENERATE label_opt ';'
  ;

generate_statement.block_declarative_statement_opt:
    %empty
  | generate_statement.block_declarative_statement_opt.block_declarative_item_mopt BEGIN
  ;

generate_statement.block_declarative_statement_opt.block_declarative_item_mopt:
    %empty
  | generate_statement.block_declarative_statement_opt.block_declarative_item_mopt block_declarative_item
  ;

generate_statement.concurrent_statement_mopt:
    %empty
  | generate_statement.concurrent_statement_mopt concurrent_statement
  ;

generation_scheme:
    // SEMANTIC: "parameter_specification" must be a GENERATION parameter specification
    FOR parameter_specification
  | IF condition
  ;

generic_clause:
    GENERIC '(' generic_list ')' ';'
  ;

generic_interface_declaration:
    interface_constant_declaration
  ;

generic_interface_element:
    generic_interface_declaration
  ;

generic_interface_list:
    generic_interface_element
  | generic_interface_list ';' generic_interface_element
  ;

generic_list:
    generic_interface_list
  ;

generic_map_aspect:
    // SEMANTIC: "association_list" is a GENERIC association list
    GENERIC MAP '(' association_list ')'
  ;

group_constituent:
    name
  | CHARACTER_LITERAL
  ;

group_constituent_list:
    group_constituent
  | group_constituent_list ',' group_constituent
  ;

group_declaration:
    // SEMANTIC: "name" must be a GROUP TEMPLATE name
    GROUP identifier ':' name '(' group_constituent_list ')' ';'
  ;

group_template_declaration:
    GROUP identifier IS '(' entity_class_entry_list ')' ';'
  ;

guarded_signal_specification:
    // SEMANTIC: "signal_list" must be a list of GUARDD signals + locally static
    signal_list ':' type_mark
  ;

identifier:
    BASIC_IDENTIFIER
  | EXTENDED_IDENTIFIER
  ;

identifier_list:
    identifier
  | identifier_list ',' identifier
  ;

if_statement:
    IF condition THEN
      sequence_of_statements
    if_statement.elsif_condition_mopt
    if_statement.else_opt
    // SEMANTIC: "label_opt" must be an IF STATEMENT label
    // SEMANTIC: "label_opt" == "label_colon_opt"
    END IF label_opt ';'
  ;

if_statement.elsif_condition_mopt:
    %empty
  | if_statement.elsif_condition_mopt ELSIF condition THEN sequence_of_statements
  ;

if_statement.else_opt:
    %empty
  | ELSE sequence_of_statements
  ;

incomplete_type_declaration:
    TYPE identifier
  ;

index_constraint:
    '(' discrete_range index_constraint.discrete_range_mopt ')'
  ;

index_constraint.discrete_range_mopt:
    %empty
  | index_constraint.discrete_range_mopt ',' discrete_range
  ;

index_specification:
    discrete_range
    // SEMANTIC: "expression" must be a static expression
  | expression
  ;

index_subtype_definition:
    type_mark RANGE "<>"
  ;

index_subtype_definition_mul:
    index_subtype_definition
  | index_subtype_definition_mul ',' index_subtype_definition
  ;

indexed_name:
    prefix '(' indexed_name.expression_mul ')'
  ;

indexed_name.expression_mul:
    expression
  | indexed_name.expression_mul ',' expression
  ;

init_expression_opt:
    %empty
  | ":=" expression
  ;

instantiated_unit:
    // SEMANTIC: "name" must be a COMPONENT name
    name
    // SEMANTIC: "name" must be a COMPONENT name
  | COMPONENT name
    // SEMANTIC: "name" must be an ENTITY name
  | ENTITY name
    // SEMANTIC: "name" must be an ENTITY name
    // SEMANTIC: "identifier" must be an ARCHITECTURE identifier
  | ENTITY name '(' identifier ')'
    // SEMANTIC: "name" must be a CONFIGURATION name
  | CONFIGURATION name
  ;

instantiation_list:
    // SEMANTIC: "instantiation.label_mul" must be a list of COMPONENT INSTANCE labels
    instantiation_list.label_mul
  | OTHERS
  | ALL
  ;

instantiation_list.label_mul:
    // SEMANTIC: "label" must be a COMPONENT INSTANCE label
    label
  | instantiation_list.label_mul ',' label
  ;

interface_constant_declaration:
    CONSTANT identifier_list ':'
      interface_constant_declaration.in_opt subtype_indication
        // SEMANTIC: "init_expression_opt" must be a STATIC expression
        init_expression_opt
  ;

interface_constant_declaration.in_opt:
    %empty
  | IN
  ;

interface_declaration:
    interface_unresolved_declaration
  | interface_constant_declaration
  | interface_signal_declaration
  | interface_variable_declaration
  | interface_file_declaration
  ;

interface_element:
    interface_declaration
  ;

interface_file_declaration:
    FILE identifier_list ':' subtype_indication
  ;

interface_list:
    interface_element
  | interface_list ';' interface_element
  ;

interface_signal_declaration:
    // SEMANTIC: "init_expression_opt" must be a STATIC expression
    SIGNAL identifier_list ':' mode_opt subtype_indication bus_opt init_expression_opt
  ;

// SEMANTIC: Can be signal/constant/variable, must be decode from context?
// TODO: Is this correct?
interface_unresolved_declaration:
    identifier_list ':' mode_opt subtype_indication bus_opt init_expression_opt
  ;

interface_variable_declaration:
    // SEMANTIC: "init_expression_opt" must be a STATIC expression
    VARIABLE identifier_list ':' mode_opt subtype_indication init_expression_opt
  ;

is_opt:
    %empty
  | IS
  ;

iteration_scheme:
    WHILE condition
    // SEMANTIC: "parameter_specification" is a LOOP parameter specification
  | FOR parameter_specification
  ;

label:
    identifier
  ;

label_opt:
    %empty
  | label
  ;

library_clause:
    LIBRARY logical_name_list ';'
  ;

library_unit:
    primary_unit
  | secondary_unit
  ;

literal:
    numeric_literal
  | enumeration_literal
  | STRING_LITERAL
  | BIT_STRING_LITERAL
  | ID_NULL
  ;

logical_name:
    identifier
  ;

logical_name_list:
    logical_name
  | logical_name_list ',' logical_name
  ;

loop_statement:
    loop_statement.iteration_scheme_opt LOOP
      sequence_of_statements
    // SEMANTIC: "label_opt" must be a LOOP label
    // SEMANTIC: "label_opt" == "label_colon_opt"
    END LOOP label_opt ';'
  ;

loop_statement.iteration_scheme_opt:
    %empty
  | iteration_scheme
  ;

mode:
    IN
  | OUT
  | INOUT
  | BUFFER
  | LINKAGE
  ;

mode_opt:
    %empty
  | mode
  ;

multiplying_operator:
    '*'
  | '/'
  | MOD
  | REM
  ;

name:
    attribute_name
  | indexed_name
  | operator_symbol
  | selected_name
  | simple_name
  | slice_name
  ;

next_statement:
    // SEMANTIC: label_opt must be an enclosing LOOP label
    NEXT label_opt next_statement.when_opt ';'
  ;

next_statement.when_opt:
    %empty
  | WHEN condition
  ;

null_statement:
    ID_NULL ';'
  ;

numeric_literal:
    abstract_literal
  | physical_literal
  ;

operator_symbol:
    STRING_LITERAL
  ;

options:
    options.guarded_opt options.delay_mechanism_opt
  ;

options.guarded_opt:
    %empty
  | GUARDED
  ;

options.delay_mechanism_opt:
    %empty
  | delay_mechanism
  ;

package_body:
    PACKAGE BODY simple_name IS
      package_body_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PACKAGE name
    // SEMANTIC: "simple_nam_opt" == "simple_name"
    END package_body.package_body_opt simple_name_opt ';'
  ;

package_body.package_body_opt:
    %empty
  | PACKAGE BODY
  ;

package_body_declarative_item:
    alias_declaration
  | constant_declaration
  | file_declaration
  | group_declaration
  | group_template_declaration
  | shared_variable_declaration
  | subprogram_body
  | subprogram_declaration
  | subtype_declaration
  | type_declaration
  | use_clause
  ;

package_body_declarative_part:
    %empty
  | package_body_declarative_part package_body_declarative_item
  ;

package_declaration:
    PACKAGE identifier IS
      package_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PACKAGE name
    // SEMANTIC: "simple_name_opt" == "identifier"
    END package.package_opt simple_name_opt ';'
  ;

package.package_opt:
    %empty
  | PACKAGE
  ;

package_declarative_item:
    alias_declaration
  | attribute_declaration
  | attribute_specification
  | component_declaration
  | constant_declaration
  | disconnection_specification
  | file_declaration
  | group_declaration
  | group_template_declaration
  | shared_variable_declaration
  | signal_declaration
  | subprogram_declaration
  | subtype_declaration
  | type_declaration
  | use_clause
  ;

package_declarative_part:
    %empty
  | package_declarative_part package_declarative_item
  ;

parameter_specification:
    identifier IN discrete_range
  ;

physical_literal:
    // SEMANTIC: "name" must be a PHYSICAL UNIT name
    name
    // SEMANTIC: "name" must be a PHYSICAL UNIT name
  | abstract_literal name
  ;

physical_type_definition:
    range_constraint
      UNITS
        primary_unit_declaration
        physical_type_definition.secondary_unit_declaration_mopt
      // SEMANTIC: "simple_name_opt" must be a PHYSICAL TYPE name
      // SEMANTIC: "simple_name_opt" should be equal to the type name it's enclosing
      END UNITS simple_name_opt
  ;

physical_type_definition.secondary_unit_declaration_mopt:
    %empty
  | physical_type_definition.secondary_unit_declaration_mopt secondary_unit_declaration
  ;

port_clause:
    PORT '(' port_list ')' ';'
  ;

port_interface_declaration:
    interface_signal_declaration
  ;

port_interface_element:
    port_interface_declaration
  ;

port_interface_list:
    port_interface_element
  | port_interface_list ';' port_interface_element
  ;

port_list:
    port_interface_list
  ;

port_map_aspect:
    // SEMANTIC: "association_list" is a PORT association list
    PORT MAP '(' association_list ')'
  ;

prefix:
    name
  | function_call
  ;

primary:
    name
  | literal
  | aggregate
  | function_call
  | qualified_expression
  | type_conversion
  | allocator
  | '(' expression ')'
  ;

primary_unit:
    entity_declaration
  | configuration_declaration
  | package_declaration
  ;

primary_unit_declaration:
    identifier ';'
  ;

procedure_call:
    procedure_name
    // SEMANTIC: "association_list" is a PARAMETER association list
  | procedure_name '(' association_list ')'
  ;

procedure_call_statement:
    procedure_call ';'
  ;

process_declarative_item:
    subprogram_declaration
  | subprogram_body
  | type_declaration
  | subtype_declaration
  | constant_declaration
  | variable_declaration
  | file_declaration
  | alias_declaration
  | attribute_declaration
  | attribute_specification
  | use_clause
  | group_template_declaration
  | group_declaration
  ;

process_declarative_part:
    %empty
  | process_declarative_part process_declarative_item
  ;

process_statement:
    label_colon_opt
      process_statement.postponed_opt PROCESS process_statement.sensitivity_list_opt is_opt
        process_declarative_part
      BEGIN
        process_statement_part
      // SEMANTIC: "label_opt" must be a PROCESS label
      // SEMANTIC: "label_opt" == "label_colon_opt"
      END process_statement.postponed_opt PROCESS label_opt ';'
  ;

process_statement.postponed_opt:
    %empty
  | POSTPONED
  ;

process_statement.sensitivity_list_opt:
    %empty
  | '(' sensitivity_list ')'
  ;

process_statement_part:
    %empty
  | process_statement_part sequential_statement
  ;

protected_type_body:
    PROTECTED BODY
      protected_type_body_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PROTECTED TYPE name
    END PROTECTED BODY simple_name_opt
  ;

protected_type_body_declarative_item:
    subprogram_declaration
  | subprogram_body
  | type_declaration
  | subtype_declaration
  | constant_declaration
  | variable_declaration
  | file_declaration
  | alias_declaration
  | attribute_declaration
  | attribute_specification
  | use_clause
  | group_template_declaration
  | group_declaration
  ;

protected_type_body_declarative_part:
    %empty
  | protected_type_body_declarative_part protected_type_body_declarative_item
  ;

protected_type_declaration:
    PROTECTED
      protected_type_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PROTECTED TYPE name
    // SEMANTIC: "simple_name_opt" should be equal to the name in enclosing type definition
    END PROTECTED simple_name_opt
  ;

protected_type_declarative_item:
    subprogram_specification
  | attribute_specification
  | use_clause
  ;

protected_type_declarative_part:
    %empty
  | protected_type_declarative_part protected_type_declarative_item
  ;

protected_type_definition:
    protected_type_declaration
  | protected_type_body
  ;

qualified_expression:
    type_mark '\'' '(' expression ')'
  | type_mark '\'' aggregate
  ;

range:
    // SEMANTIC: "attribute_name" must be a RANGE attribute name
    attribute_name
  | simple_expression direction simple_expression
  ;

range_constraint:
    RANGE range
  ;

record_type_definition:
    RECORD
      record_type_definition.element_declaration_mul
    // SEMANTIC: "simple_name_opt" must be a RECORD TYPE name
    // SEMANTIC: "simple_name_opt" should be equal to the enclosing type name
    END RECORD simple_name_opt
  ;

record_type_definition.element_declaration_mul:
    element_declaration
  | record_type_definition.element_declaration_mul element_declaration
  ;

relation:
    shift_expression
  | shift_expression relational_operator shift_expression
  ;

relational_operator:
    '='
  |  "/="
  | '<'
  | "<="
  | '>'
  | ">="
  ;

report_statement:
    REPORT expression ';'
  | REPORT expression
      SEVERITY expression ';'
  ;

return_statement:
    RETURN ';'
  | RETURN expression ';'
  ;

scalar_type_definition:
    enumeration_type_definition
    // SEMANTIC: Can be an integer or floating-point type definition
    // depending on the range expression type
  | range_constraint
  | physical_type_definition
  ;

secondary_unit:
    architecture_body
  | package_body
  ;

secondary_unit_declaration:
    identifier '=' physical_literal
  ;

selected_name:
    prefix '.' suffix
  ;

selected_signal_assignment:
    WITH expression SELECT
      target "<=" options selected_waveforms ';'
  ;

// TODO: Check if correct
selected_waveforms:
    waveform WHEN choices
  | selected_waveforms ',' waveform WHEN choices
  ;

sensitivity_clause:
    ON sensitivity_list
  ;

sensitivity_list:
    // SEMANTIC: "name" must be a SIGNAL name
    name
  | sensitivity_list ',' name
  ;

sequence_of_statements:
    %empty
  | sequence_of_statements sequential_statement
  ;

sequential_statement:
    // There can be an optional label before each sequential statement
    label_colon_opt sequential_statement_wo_label
  ;

sequential_statement_wo_label:
    wait_statement
  | assertion_statement
  | report_statement
  | signal_assignment_statement
  | variable_assignment_statement
  | procedure_call_statement
  | if_statement
  | case_statement
  | loop_statement
  | next_statement
  | exit_statement
  | return_statement
  | null_statement
  ;

shift_expression:
    simple_expression
  | simple_expression shift_operator simple_expression
  ;

shift_operator:
    SLL
  | SRL
  | SLA
  | SRA
  | ROL
  | ROR
  ;

sign:
    '+'
  | '-'
  ;

signal_assignment_statement:
    target "<=" signal_assignment_statement.delay_mechanism_opt waveform ';'
  ;

signal_assignment_statement.delay_mechanism_opt:
    %empty
  | delay_mechanism
  ;

signal_declaration:
    SIGNAL identifier_list ':' subtype_indication signal_declaration.signal_kind_opt init_expression_opt ';'
  ;

signal_declaration.signal_kind_opt:
    %empty
  | signal_kind
  ;

signal_kind:
    REGISTER
  | BUS
  ;

signal_list:
    signal_list.name_mul
  | OTHERS
  | ALL
  ;

signal_list.name_mul:
    // SEMANTIC: "name" must be a SIGNAL name
    name
  | signal_list.name_mul ',' name
  ;

signature:
    %empty
  | '[' signature.type_mark_mul ']'
  | '[' RETURN type_mark ']'
  | '[' signature.type_mark_mul RETURN type_mark ']'
  ;

signature.type_mark_mul:
    type_mark
  | signature.type_mark_mul ',' type_mark
  ;

simple_expression:
    simple_expression.sign_opt term simple_expression.adding_operator_mopt
  ;

simple_expression.sign_opt:
    %empty
  | sign
  ;

simple_expression.adding_operator_mopt:
    %empty
  | simple_expression.adding_operator_mopt adding_operator term
  ;

simple_name:
    identifier
  ;

simple_name_opt:
    %empty
  | simple_name
  ;

slice_name:
    prefix '(' discrete_range ')'
  ;

subprogram_body:
    subprogram_specification IS
      subprogram_declarative_part
    BEGIN
      subprogram_statement_part
    END subprogram_body.subprogram_kind_opt designator_opt ';'
  ;

subprogram_body.subprogram_kind_opt:
    %empty
  | subprogram_kind
  ;

subprogram_declaration:
    subprogram_specification ';'
  ;

subprogram_declarative_item:
    subprogram_declaration
  | subprogram_body
  | type_declaration
  | subtype_declaration
  | constant_declaration
  | variable_declaration
  | file_declaration
  | alias_declaration
  | attribute_declaration
  | attribute_specification
  | use_clause
  | group_template_declaration
  | group_declaration
  ;

subprogram_declarative_part:
    %empty
  | subprogram_declarative_part subprogram_declarative_item
  ;

subprogram_kind:
    PROCEDURE
  | FUNCTION
  ;

subprogram_specification:
    PROCEDURE designator
  | PROCEDURE designator '(' formal_parameter_list ')'
  | function_type FUNCTION designator RETURN type_mark
  | function_type FUNCTION designator '(' formal_parameter_list ')' RETURN type_mark
  ;

function_type:
    %empty
  | PURE
  | IMPURE
  ;

subprogram_statement_part:
    %empty
  | subprogram_statement_part sequential_statement
  ;

subtype_declaration:
    SUBTYPE identifier IS subtype_indication ';'
  ;

subtype_indication:
    resolution_function_name_opt type_mark constraint_opt
  ;

// FIXME: Check if this is correct
resolution_function_name_opt:
    // SEMANTIC: "designator_opt" must be a RESOLUTION FUNCTION name
    designator_opt
  ;

suffix:
    simple_name
  | CHARACTER_LITERAL
  | operator_symbol
  | ALL
  ;

target:
    name
  | aggregate
  ;

term:
    factor
  | term multiplying_operator factor
  ;

time_expression:
    // SEMANTIC: "expression" must be a TIME expression
    expression
  ;

timeout_clause:
    FOR time_expression
  ;

type_conversion:
    type_mark '(' expression ')'
  ;

type_declaration:
    full_type_declaration
  | incomplete_type_declaration
  ;

type_definition:
    scalar_type_definition
  | composite_type_definition
  | access_type_definition
  | file_type_definition
  | protected_type_definition
  ;

type_mark:
    // SEMANTIC: "name" must be a TYPE or SUBTYPE name
    name
  ;

unconstrained_array_definition:
    // SEMANTIC: "subtype_indication" is an ELEMENT subtype indication
    ARRAY '(' index_subtype_definition_mul ')' OF subtype_indication
  ;

use_clause:
    USE use_clause.selected_name_mul ';'
  ;

use_clause.selected_name_mul:
    expanded_selected_name
  | use_clause.selected_name_mul ',' expanded_selected_name
  ;

value_expression:
    // SEMANTIC: "expression" must be a VALUE expression
    expression
  ;

variable_assignment_statement:
    target ":=" expression ';'
  ;

variable_declaration:
    plain_variable_declaration
  | shared_variable_declaration
  ;

plain_variable_declaration:
    VARIABLE identifier_list ':' subtype_indication init_expression_opt ';'
  ;

shared_variable_declaration:
    SHARED plain_variable_declaration
  ;

wait_statement:
    WAIT wait_statement.sensitivity_clause_opt wait_statement.condition_clause_opt wait_statement.timeout_clause_opt ';'
  ;

wait_statement.sensitivity_clause_opt:
    %empty
  | sensitivity_clause
  ;

wait_statement.condition_clause_opt:
    %empty
  | condition_clause
  ;

wait_statement.timeout_clause_opt:
    %empty
  | timeout_clause
  ;

waveform:
    waveform.waveform_element_mul
  | UNAFFECTED
  ;

waveform.waveform_element_mul:
    waveform_element
  | waveform.waveform_element_mul ',' waveform_element
  ;

waveform_element:
    value_expression
  | value_expression AFTER time_expression
  | ID_NULL
  | ID_NULL AFTER time_expression
  ;
%%
