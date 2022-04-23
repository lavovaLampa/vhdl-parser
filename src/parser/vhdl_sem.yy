%require "3.8"
%language "c++"
%skeleton "glr2.cc"
%glr-parser

%define api.token.constructor
%header
%locations
%debug

// Use C++ variant type instead of union
%define api.value.type variant
// Generate detailed error messages
%define parse.error detailed
// Generate assertions to check proper usage of semantic values
%define parse.assert

// Automatically move values in semantic values
%define api.value.automove

%define api.namespace {parser}
%define api.parser.class {Parser}

%define api.location.file "location.hpp"
%define api.location.include {"parser/location.hpp"}

/* %code top
{
  #include "lexer_defs.h"
  #include "lexer.h"
  #include "parser_defs.h"
} */

%code requires
{
#include "lexer/lexer_defs.hpp"
#include "parser/parser_defs.hpp"
#include "lexer/lexer.hpp"
#include <optional>
}

// TOKEN DEFINITIONS

%start design_file

%token ABS            "abs"
%token ACCESS         "access"
%token AFTER          "after"
%token ALIAS          "alias"
%token ALL            "all"
%token AND            "and"
%token ARCHITECTURE   "architecture"
%token ARRAY          "array"
%token ASSERT         "assert"
%token ATTRIBUTE      "attribute"
%token BEGIN          "begin"
%token BLOCK          "block"
%token BODY           "body"
%token BUFFER         "buffer"
%token BUS            "bus"
%token CASE           "case"
%token COMPONENT      "component"
%token CONFIGURATION  "configuration"
%token CONSTANT       "constant"
%token DISCONNECT     "disconnect"
%token DOWNTO         "downto"
%token ELSE           "else"
%token ELSIF          "elsif"
%token END            "end"
%token ENTITY         "entity"
%token EXIT           "exit"
%token FILE           "file"
%token FOR            "for"
%token FUNCTION       "function"
%token GENERATE       "generate"
%token GENERIC        "generic"
%token GROUP          "group"
%token GUARDED        "guarded"
%token IF             "if"
%token IMPURE         "impure"
%token IN             "in"
%token INERTIAL       "inertial"
%token INOUT          "inout"
%token IS             "is"
%token LABEL          "label"
%token LIBRARY        "library"
%token LINKAGE        "linkage"
%token LITERAL        "literal"
%token LOOP           "loop"
%token MAP            "map"
%token MOD            "mod"
%token NAND           "nand"
%token NEW            "new"
%token NEXT           "next"
%token NOR            "nor"
%token NOT            "not"
%token ID_NULL        "id_null"
%token OF             "of"
%token ON             "on"
%token OPEN           "open"
%token OR             "or"
%token OTHERS         "others"
%token OUT            "out"
%token PACKAGE        "package"
%token PORT           "port"
%token POSTPONED      "postponed"
%token PROCEDURE      "procedure"
%token PROCESS        "process"
%token PROTECTED      "protected"
%token PURE           "pure"
%token RANGE          "range"
%token RECORD         "record"
%token REGISTER       "register"
%token REJECT         "reject"
%token REM            "rem"
%token REPORT         "report"
%token RETURN         "return"
%token ROL            "rol"
%token ROR            "ror"
%token SELECT         "select"
%token SEVERITY       "severity"
%token SHARED         "shared"
%token SIGNAL         "signal"
%token SLA            "sla"
%token SLL            "sll"
%token SRA            "sra"
%token SRL            "srl"
%token SUBTYPE        "subtype"
%token THEN           "then"
%token TO             "to"
%token TRANSPORT      "transport"
%token TYPE           "type"
%token UNAFFECTED     "unaffected"
%token UNITS          "units"
%token UNTIL          "until"
%token USE            "use"
%token VARIABLE       "variable"
%token WAIT           "wait"
%token WHEN           "when"
%token WHILE          "while"
%token WITH           "with"
%token XNOR           "xnor"
%token XOR            "xor"

%token ARROW                "=>"
%token DOUBLE_STAR          "**"
%token VARIABLE_ASSIGNMENT  ":="
%token INEQUALITY           "/="
%token GT_OR_EQUAL          ">="
%token LT_OR_EQUAL          "<="
%token BOX                  "<>"

%token <lexer::BasedLiteral>        BASED_LITERAL       "based literal"
%token <lexer::BasicIdentifier>     BASIC_IDENTIFIER    "basic identifier"
%token <lexer::BitStringLiteral>    BIT_STRING_LITERAL  "bit string literal"
%token <lexer::CharacterLiteral>    CHARACTER_LITERAL   "character literal"
%token <lexer::DecimalLiteral>      DECIMAL_LITERAL     "decimal literal"
%token <lexer::ExtendedIdentifier>  EXTENDED_IDENTIFIER "extended identifier"
%token <lexer::StringLiteral>       STRING_LITERAL      "string symbol"

%nterm <parser::AbstractLiteral>                                    abstract_literal
%nterm <parser::AccessTypeDefinition>                               access_type_definition
%nterm <parser::ActualDesignator>                                   actual_designator
%nterm <parser::ActualPart>                                         actual_part
%nterm <parser::AddingOperator>                                     adding_operator
%nterm <parser::Aggregate>                                          aggregate
%nterm <parser::Aggregate>                                          aggregate.element_association_mul
%nterm <parser::AliasDeclaration>                                   alias_declaration
%nterm <std::optional<std::unique_ptr<parser::SubtypeIndication>>>  alias_declaration.opt_subtype
%nterm <parser::AliasDesignator>                                    alias_designator
%nterm <parser::Allocator>                                          allocator
%nterm <parser::ArchitectureBody>                                   architecture_body
%nterm <std::vector<parser::BlockDeclarativeItem>>                  architecture_declarative_part
%nterm <std::vector<parser::ConcurrentStatement>>                   architecture_statement_part
%nterm <parser::ArrayTypeDefinition>                                array_type_definition
%nterm <parser::Assertion>                                          assertion
%nterm <std::unique_ptr<parser::Expression>>                        assertion.report_opt
%nterm <std::unique_ptr<parser::Expression>>                        assertion.severity_opt
%nterm <parser::AssertionStatement>                                 assertion_statement
%nterm <parser::AssociationElement>                                 association_element
%nterm <std::vector<parser::AssociationElement>>                    association_list
%nterm <parser::AttributeDeclaration>                               attribute_declaration
%nterm <parser::Identifier>                                         attribute_designator
%nterm <parser::AttributeName>                                      attribute_name
%nterm <std::unique_ptr<parser::Expression>>                        attribute_name.expression_opt
%nterm <parser::AttributeSpecification>                             attribute_specification
%nterm <parser::BindingIndication>                                  binding_indication
%nterm <std::unique_ptr<parser::EntityAspect>>                      binding_indication.entity_aspect_opt
%nterm <parser::BlockConfiguration>                                 block_configuration
%nterm <std::vector<parser::UseClause>>                             block_configuration.use_clause_mopt
%nterm <std::vector<parser::ConfigurationItem>>                     block_configuration.configuration_item_mopt
%nterm <parser::BlockDeclarativeItem>                               block_declarative_item
%nterm <std::vector<parser::BlockDeclarativeItem>>                  block_declarative_part
%nterm <parser::BlockHeader>                                        block_header
%nterm <parser::BlockHeaderGenericClause>                           block_header.generic_clause
%nterm <parser::BlockHeaderPortClause>                              block_header.port_clause
%nterm <parser::BlockSpecification>                                 block_specification
%nterm <parser::BlockStatement>                                     block_statement
%nterm <std::unique_ptr<parser::Expression>>                        block_statement.guard_expression_opt
%nterm <std::vector<parser::ConcurrentStatement>>                   block_statement_part
%nterm <bool>                                                       bus_opt
%nterm <parser::CaseStatement>                                      case_statement
%nterm <std::vector<parser::CaseAlternative>>                       case_statement.case_statement_alternative_mul
%nterm <parser::CaseAlternative>                                    case_statement_alternative
%nterm <parser::Choice>                                             choice
%nterm <std::vector<parser::Choice>>                                choices
%nterm <parser::ComponentConfiguration>                             component_configuration
%nterm <std::optional<parser::BindingIndication>>                   component_configuration.binding_indication_opt
%nterm <std::optional<parser::BlockConfiguration>>                  component_configuration.block_configuration_opt
%nterm <parser::ComponentDeclaration>                               component_declaration
%nterm <std::optional<parser::GenericClause>>                       component_declaration.local_generic_clause_opt
%nterm <std::optional<parser::PortClause>>                          component_declaration.local_port_clause_opt
%nterm <parser::ComponentInstantiation>                             component_instantiation_statement
%nterm <parser::ComponentSpecification>                             component_specification
%nterm <parser::CompositeTypeDefinition>                            composite_type_definition
%nterm <parser::ConcurrentAssertion>                                concurrent_assertion_statement
%nterm <bool>                                                       postponed_opt
%nterm <parser::ConcurrentSignalAssignment>                         concurrent_procedure_call_statement
%nterm <parser::ConcurrentSelectedSignalAssignment>                 concurrent_signal_assignment_statement
%nterm <parser::ConcurrentStatement>                                concurrent_statement
%nterm <parser::Expression>                                         condition
%nterm <parser::ConditionClause>                                    condition_clause
%nterm <parser::ConditionalSignalAssignment>                        conditional_signal_assignment
%nterm <parser::ConditionalWaveforms>                               conditional_waveforms
%nterm <std::vector<parser::ConditionalWaveform>>                   conditional_waveforms.waveform_condition_mopt
%nterm <std::unique_ptr<parser::Expression>>                        conditional_waveforms.when_condition_opt
%nterm <parser::ConfigurationDeclaration>                           configuration_declaration
%nterm <parser::ConfigurationDeclarativeItem>                       configuration_declarative_item
%nterm <std::vector<parser::ConfigurationDeclarativeItem>>          configuration_declarative_part
%nterm <parser::ConfigurationItem>                                  configuration_item
%nterm <parser::ConfigurationSpecification>                         configuration_specification
%nterm <parser::ConstantDeclaration>                                constant_declaration
%nterm <std::unique_ptr<parser::Expression>>                        constant_declaration.expression_opt
%nterm <parser::ConstrainedArrayDefinition>                         constrained_array_definition
%nterm <parser::Constraint>                                         constraint
%nterm <std::optional<parser::Constraint>>                          constraint_opt
%nterm <std::vector<parser::ContextItem>>                           context_clause
%nterm <parser::ContextItem>                                        context_item
%nterm <parser::DelayMechanism>                                     delay_mechanism
%nterm <std::vector<parser::DesignUnit>>                            design_file
%nterm <parser::DesignUnit>                                         design_unit
%nterm <parser::Designator>                                         designator
%nterm <std::optional<parser::Designator>>                          designator_opt
%nterm <parser::Direction>                                          direction
%nterm <parser::DisconnectionSpecification>                         disconnection_specification
%nterm <parser::DiscreteRange>                                      discrete_range
%nterm <parser::ElementAssociation>                                 element_association
%nterm <parser::ElementDeclaration>                                 element_declaration
%nterm <parser::SubtypeIndication>                                  element_subtype_definition
%nterm <parser::EntityAspect>                                       entity_aspect
%nterm <parser::EntityClass>                                        entity_class
%nterm <parser::EntityClassEntry>                                   entity_class_entry
%nterm <std::vector<parser::EntityClassEntry>>                      entity_class_entry_list
%nterm <parser::EntityDeclaration>                                  entity_declaration
%nterm <std::vector<parser::EntityStatement>>                       entity_declaration.entity_statement_part_opt
%nterm <parser::EntityDeclarativeItem>                              entity_declarative_item
%nterm <std::vector<parser::EntityDeclarativeItem>>                 entity_declarative_part
%nterm <parser::EntityDesignator>                                   entity_designator
%nterm <std::optional<parser::EntityHeader>>                        entity_header
%nterm <parser::EntityNameList>                                     entity_name_list
%nterm <std::vector<parser::EntityDesignator>>                      entity_name_list.entity_designator_mul
%nterm <parser::EntitySpecification>                                entity_specification
%nterm <parser::EntityStatement>                                    entity_statement
%nterm <std::vector<parser::EntityStatement>>                       entity_statement_part
%nterm <parser::EntityTag>                                          entity_tag
%nterm <parser::EnumerationLiteral>                                 enumeration_literal
%nterm <std::vector<parser::EnumerationLiteral>>                    enumeration_type_definition
%nterm <std::vector<parser::EnumerationLiteral>>                    enumeration_type_definition.inner
%nterm <parser::ExitStatement>                                      exit_statement
%nterm <std::optional<parser::Expression>>                          exit_statement.when_condition_opt
%nterm <parser::ExpandedSelectedName>                               expanded_selected_name
%nterm <parser::SimplePrefix>                                       simple_prefix
%nterm <parser::Expression>                                         expression
%nterm <std::vector<parser::Relation>>                              expression.and_relation_mul
%nterm <std::vector<parser::Relation>>                              expression.or_relation_mul
%nterm <std::vector<parser::Relation>>                              expression.xor_relation_mul
%nterm <std::vector<parser::Relation>>                              expression.xnor_relation_mul
%nterm <parser::Factor>                                             factor
%nterm <parser::FileDeclaration>                                    file_declaration
%nterm <parser::Expression>                                         file_logical_name
%nterm <parser::Expression>                                         file_open_kind_expression
%nterm <parser::FileOpenInformation>                                file_open_information
%nterm <parser::FileTypeDefinition>                                 file_type_definition
%nterm <parser::Name>                                               formal_designator
%nterm <std::vector<parser::InterfaceDeclaration>>                  formal_parameter_list
%nterm <parser::FormalPart>                                         formal_part
%nterm <parser::TypeDeclaration>                                    full_type_declaration
%nterm <parser::FunctionCall>                                       function_call
%nterm <parser::GenerateStatement>                                  generate_statement
%nterm <std::vector<parser::BlockDeclarativeItem>>                  generate_statement.block_declarative_statement_opt
%nterm <std::vector<parser::BlockDeclarativeItem>>                  generate_statement.block_declarative_statement_opt.block_declarative_item_mopt
%nterm <std::vector<parser::ConcurrentStatement>>                   generate_statement.concurrent_statement_mopt
%nterm <parser::GenerationScheme>                                   generation_scheme
%nterm <parser::GenericClause>                                      generic_clause
%nterm <std::vector<parser::AssociationElement>>                    generic_map_aspect
%nterm <std::optional<std::vector<parser::AssociationElement>>>     generic_map_aspect_opt
%nterm <parser::GroupConstituent>                                   group_constituent
%nterm <std::vector<parser::GroupConstituent>>                      group_constituent_list
%nterm <parser::GroupDeclaration>                                   group_declaration
%nterm <parser::GroupTemplateDeclaration>                           group_template_declaration
%nterm <parser::GuardedSignalSpecification>                         guarded_signal_specification
%nterm <parser::Identifier>                                         identifier
%nterm <std::vector<parser::Identifier>>                            identifier_list
%nterm <parser::IfStatement>                                        if_statement
%nterm <std::vector<parser::IfStatementPair>>                       if_statement.elsif_condition_mopt
%nterm <std::vector<parser::SequentialStatement>>                   if_statement.else_opt
%nterm <parser::TypeDeclaration>                                    incomplete_type_declaration
%nterm <std::vector<parser::DiscreteRange>>                         index_constraint
%nterm <std::vector<parser::DiscreteRange>>                         index_constraint.discrete_range_mul
%nterm <parser::IndexSpecification>                                 index_specification
%nterm <parser::Name>                                               index_subtype_definition
%nterm <std::vector<parser::Name>>                                  index_subtype_definition_mul
%nterm <parser::IndexedName>                                        indexed_name
%nterm <std::vector<parser::Expression>>                            indexed_name.expression_mul
%nterm <std::unique_ptr<parser::Expression>>                        init_expression_opt
%nterm <parser::InstantiatedUnit>                                   instantiated_unit
%nterm <parser::InstantiationList>                                  instantiation_list
%nterm <std::vector<parser::Identifier>>                            instantiation_list.label_mul
%nterm <parser::InterfaceConstantDeclaration>                       interface_constant_declaration
%nterm <std::vector<parser::InterfaceConstantDeclaration>>          interface_constant_declaration_list
%nterm <parser::InterfaceDeclaration>                               interface_declaration
%nterm <parser::InterfaceDeclaration>                               interface_element
%nterm <parser::InterfaceFileDeclaration>                           interface_file_declaration
%nterm <std::vector<parser::InterfaceDeclaration>>                  interface_list
%nterm <parser::InterfaceSignalDeclaration>                         interface_signal_declaration
%nterm <parser::InterfaceUnresolvedDeclaration>                     interface_unresolved_declaration
%nterm <parser::InterfaceVariableDeclaration>                       interface_variable_declaration
%nterm <parser::IterationScheme>                                    iteration_scheme
%nterm <parser::Identifier>                                         label
%nterm <std::optional<parser::Identifier>>                          label_colon_opt
%nterm <std::optional<parser::Identifier>>                          label_opt
%nterm <std::vector<parser::Identifier>>                            library_clause
%nterm <parser::LibraryUnit>                                        library_unit
%nterm <parser::Literal>                                            literal
%nterm <parser::Identifier>                                         logical_name
%nterm <std::vector<parser::Identifier>>                            logical_name_list
%nterm <parser::LoopStatement>                                      loop_statement
%nterm <std::optional<parser::IterationScheme>>                     loop_statement.iteration_scheme_opt
%nterm <parser::Mode>                                               mode
%nterm <std::optional<parser::Mode>>                                mode_opt
%nterm <parser::MulOp>                                              multiplying_operator
%nterm <parser::Name>                                               name
%nterm <parser::NextStatement>                                      next_statement
%nterm <std::optional<parser::Expression>>                          next_statement.when_opt
%nterm <parser::NullStatement>                                      null_statement
%nterm <parser::NumericLiteral>                                     numeric_literal
%nterm <parser::Options>                                            options
%nterm <bool>                                               options.guarded_opt
%nterm <std::optional<parser::DelayMechanism>>                      options.delay_mechanism_opt 
%nterm <parser::PackageBody>                                        package_body
%nterm <parser::PackageBodyDeclarativeItem>                         package_body_declarative_item
%nterm <std::vector<parser::PackageBodyDeclarativeItem>>            package_body_declarative_part
%nterm <parser::PackageDeclaration>                                 package_declaration
%nterm <parser::PackageDeclarativeItem>                             package_declarative_item
%nterm <std::vector<parser::PackageDeclarativeItem>>                package_declarative_part
%nterm <parser::ParameterSpecification>                             parameter_specification
%nterm <parser::PhysicalLiteral>                                    physical_literal
%nterm <parser::PhysicalTypeDefinition>                             physical_type_definition
%nterm <std::vector<parser::SecondaryUnitDeclaration>>              physical_type_definition.secondary_unit_declaration_mopt
%nterm <parser::PortClause>                                         port_clause
%nterm <parser::InterfaceSignalDeclaration>                         port_interface_declaration
%nterm <parser::InterfaceSignalDeclaration>                         port_interface_element
%nterm <std::vector<parser::InterfaceSignalDeclaration>>            port_interface_list
%nterm <std::vector<parser::InterfaceSignalDeclaration>>            port_list
%nterm <parser::PortMapAspect>                                      port_map_aspect
%nterm <std::optional<parser::PortMapAspect>>                       port_map_aspect_opt
%nterm <parser::Prefix>                                             prefix
%nterm <parser::Primary>                                            primary
%nterm <parser::PrimaryUnit>                                        primary_unit
%nterm <parser::Identifier>                                         primary_unit_declaration
%nterm <parser::ProcedureCall>                                      procedure_call
%nterm <parser::ProcedureCallStatement>                             procedure_call_statement
%nterm <parser::ProcessDeclarativeItem>                             process_declarative_item
%nterm <std::vector<parser::ProcessDeclarativeItem>>                process_declarative_part
%nterm <parser::ProcessStatement>                                   process_statement
%nterm <bool>                                               process_statement.postponed_opt
%nterm <std::optional<std::vector<parser::Name>>>                   process_statement.sensitivity_list_opt
%nterm <std::vector<parser::SequentialStatement>>                   process_statement_part
%nterm <parser::ProtectedTypeBody>                                  protected_type_body
%nterm <parser::ProtectedTypeBodyDeclarativeItem>                   protected_type_body_declarative_item
%nterm <std::vector<parser::ProtectedTypeBodyDeclarativeItem>>      protected_type_body_declarative_part
%nterm <parser::ProtectedTypeDeclaration>                           protected_type_declaration
%nterm <parser::ProtectedTypeDeclarativeItem>                       protected_type_declarative_item
%nterm <std::vector<parser::ProtectedTypeDeclarativeItem>>          protected_type_declarative_part
%nterm <parser::ProtectedTypeDefinition>                            protected_type_definition
%nterm <parser::QualifiedExpression>                                qualified_expression
%nterm <parser::SimpleRange>                                        range
%nterm <parser::SimpleRange>                                        range_constraint
%nterm <parser::RecordTypeDefinition>                               record_type_definition
%nterm <std::vector<parser::ElementDeclaration>>                    record_type_definition.element_declaration_mul
%nterm <parser::Relation>                                           relation
%nterm <parser::RelOp>                                              relational_operator
%nterm <parser::ReportStatement>                                    report_statement
%nterm <std::optional<parser::Designator>>                          resolution_function_name_opt
%nterm <parser::ReturnStatement>                                    return_statement
%nterm <parser::ScalarTypeDefinition>                               scalar_type_definition
%nterm <parser::SecondaryUnit>                                      secondary_unit
%nterm <parser::SecondaryUnitDeclaration>                           secondary_unit_declaration
%nterm <parser::SelectedName>                                       selected_name
%nterm <parser::SelectedSignalAssignment>                           selected_signal_assignment
%nterm <std::vector<parser::SelectedWaveform>>                      selected_waveforms
%nterm <std::vector<parser::Name>>                                  sensitivity_clause
%nterm <std::vector<parser::Name>>                                  sensitivity_list
%nterm <std::vector<parser::SequentialStatement>>                   sequence_of_statements
%nterm <parser::SequentialStatement>                                sequential_statement
%nterm <parser::ShiftExpression>                                    shift_expression
%nterm <parser::ShiftOperator>                                      shift_operator
%nterm <parser::Sign>                                               sign
%nterm <parser::SignalAssignmentStatement>                          signal_assignment_statement
%nterm <std::optional<parser::DelayMechanism>>                      signal_assignment_statement.delay_mechanism_opt
%nterm <parser::SignalDeclaration>                                  signal_declaration
%nterm <std::optional<parser::SignalKind>>                          signal_declaration.signal_kind_opt
%nterm <parser::SignalKind>                                         signal_kind
%nterm <parser::SignalList>                                         signal_list
%nterm <std::vector<parser::Name>>                                  signal_list.name_mul
%nterm <std::optional<parser::Signature>>                           signature
%nterm <std::vector<parser::Name>>                                  signature.type_mark_mul
%nterm <parser::SimpleExpression>                                   simple_expression
%nterm <std::optional<parser::Sign>>                                simple_expression.sign_opt
%nterm <std::vector<parser::TermOpPair>>                            simple_expression.adding_operator_mopt
%nterm <parser::Identifier>                                         simple_name
%nterm <std::optional<parser::Identifier>>                          simple_name_opt
%nterm <parser::SliceName>                                          slice_name
%nterm <parser::SubprogramBody>                                     subprogram_body
%nterm <std::optional<parser::SubprogramKind>>                      subprogram_body.subprogram_kind_opt
%nterm <parser::SubprogramSpecification>                            subprogram_declaration
%nterm <parser::SubprogramDeclarativeItem>                          subprogram_declarative_item
%nterm <std::vector<parser::SubprogramDeclarativeItem>>             subprogram_declarative_part
%nterm <parser::SubprogramKind>                                     subprogram_kind
%nterm <parser::SubprogramSpecification>                            subprogram_specification
%nterm <std::optional<parser::FunctionType>>                        function_type
%nterm <std::vector<parser::SequentialStatement>>                   subprogram_statement_part
%nterm <parser::SubtypeDeclaration>                                 subtype_declaration
%nterm <parser::SubtypeIndication>                                  subtype_indication
%nterm <parser::Suffix>                                             suffix
%nterm <parser::Target>                                             target
%nterm <parser::Term>                                               term
%nterm <parser::Expression>                                         time_expression
%nterm <parser::Expression>                                         timeout_clause
%nterm <parser::TypeConversion>                                     type_conversion
%nterm <parser::TypeDeclaration>                                    type_declaration
%nterm <parser::TypeDefinition>                                     type_definition
%nterm <parser::Name>                                               type_mark
%nterm <parser::UnconstrainedArrayDefinition>                       unconstrained_array_definition
%nterm <parser::UseClause>                                          use_clause
%nterm <std::vector<parser::ExpandedSelectedName>>                  use_clause.selected_name_mul
%nterm <parser::Expression>                                         value_expression
%nterm <parser::VariableAssignmentStatement>                        variable_assignment_statement
%nterm <parser::VariableDeclaration>                                variable_declaration
%nterm <parser::PlainVariableDeclaration>                           plain_variable_declaration
%nterm <parser::SharedVariableDeclaration>                          shared_variable_declaration
%nterm <parser::WaitStatement>                                      wait_statement
%nterm <std::optional<std::vector<parser::Name>>>                   wait_statement.sensitivity_clause_opt
%nterm <std::optional<parser::ConditionClause>>                     wait_statement.condition_clause_opt
%nterm <std::optional<parser::Expression>>                          wait_statement.timeout_clause_opt
%nterm <parser::Waveform>                                           waveform
%nterm <std::vector<parser::WaveformElement>>                       waveform.waveform_element_mul
%nterm <parser::WaveformElement>                                    waveform_element

%%
// FIXME: Handle operator symbols
operator_symbol:
    STRING_LITERAL
  ;

abstract_literal:
    DECIMAL_LITERAL
  | BASED_LITERAL
  ;

access_type_definition:
    ACCESS subtype_indication { $$ = AccessTypeDefinition { {}, $2 }; }
  ;

actual_designator:
    expression
  /**
   *  Name must be:
   *    - variable_name
   *    - signal_name
   *    - file_name
  **/
  | name
  | OPEN                { $$ = Keyword::Open {}; }
  ;

actual_part:
    actual_designator               { $$ = ActualPart { {}, std::nullopt, $1 }; }
    // SEMANTIC: Name must be a function_name or a type_mark
  | name '(' actual_designator ')'  { $$ = ActualPart { {}, $1, $3 }; }
  ;

adding_operator:
    '+'   { $$ = AddingOperator::plus; }
  | '-'   { $$ = AddingOperator::minus; }
  | '&'   { $$ = AddingOperator::ampersand; }
  ;

aggregate:
    '(' aggregate.element_association_mul ')'   { $$ = $2; }
  ;

aggregate.element_association_mul:
    element_association                                         { $$ = Aggregate { $1 }; }
  | aggregate.element_association_mul ',' element_association   { $$ = $1; $$.push_back($3); }
  ;

alias_declaration:
    ALIAS alias_designator alias_declaration.opt_subtype        
      IS name signature                                     { $$ = AliasDeclaration { {}, $2, $3, $5, $6 }; }
  ;

alias_declaration.opt_subtype:
    %empty                  { $$ = std::nullopt; }
  | ':' subtype_indication  { $$ = $2; }
  ;

alias_designator:
    identifier
  | CHARACTER_LITERAL
  | operator_symbol
  ;

allocator:
    NEW subtype_indication    { $$ = $2; }
  | NEW qualified_expression  { $$ = $2; }
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
                              { $$ = ArchitectureBody { {}, $2, $4, $6, $8, $11 }; }
  ;
  
architecture_body.architecture_opt:
    %empty
  | ARCHITECTURE
  ;

architecture_declarative_part:
    %empty                                                { }
  | architecture_declarative_part block_declarative_item  { $$ = $1; $$.push_back($2); }
  ;

architecture_statement_part:
    %empty                                            { }
  | architecture_statement_part concurrent_statement  { $$ = $1; $$.push_back($2); }
  ;

array_type_definition:
    unconstrained_array_definition
  | constrained_array_definition
  ;

assertion:
    ASSERT condition assertion.report_opt assertion.severity_opt
                                              { $$ = Assertion { {}, $2, $3, $4 }; }
  ;

assertion.report_opt:
    %empty              { }
  | REPORT expression   { $$ = $2; }
  ;

assertion.severity_opt:
    %empty              { }
  | SEVERITY expression { $$ = $2; }
  ;

assertion_statement:
    label_colon_opt assertion ';' { $$ = AssertionStatement { {}, $1, $2 }; }
  ;

association_element:
    formal_part "=>" actual_part  { $$ = AssociationElement { {}, $1, $3 }; }
  | actual_part                   { $$ = AssociationElement { {}, nullptr, $1 }; }
  ;

association_list:
    association_element                       { $$ = std::vector<AssociationElement> { $1 }; }
  | association_list ',' association_element  { $$ = $1; $$.push_back($3); }
  ;

attribute_declaration:
    ATTRIBUTE identifier ':' type_mark ';'    { $$ = AttributeDeclaration { {}, $2, $4 }; }
  ;

attribute_designator:
    // SEMANTIC: "simple_name" must be an ATTRIBUTE name
    simple_name
  ;

attribute_name:
    prefix signature '\'' attribute_designator attribute_name.expression_opt 
              { $$ = AttributeName { {}, $1, $2, $4, $5 }; }
  ;

attribute_name.expression_opt:
    %empty              { $$ = nullptr; }
  | '(' expression ')'  { $$ = $2; }
  ;

attribute_specification:
    ATTRIBUTE attribute_designator OF entity_specification IS expression ';'
              { $$ = AttributeSpecification { {}, $2, $4, $6 }; }
  ;

binding_indication:
    binding_indication.entity_aspect_opt
    generic_map_aspect_opt
    port_map_aspect_opt
              { $$ = BindingIndication { {}, $1, $2, $3 }; }
  ;

binding_indication.entity_aspect_opt:
    %empty              { $$ = nullopt; }
  | USE entity_aspect   { $$ = $2; }
  ;

block_configuration:
    FOR block_specification
      block_configuration.use_clause_mopt
      block_configuration.configuration_item_mopt
    END FOR ';'
              { $$ = BlockConfiguration { {}, $2, $3, $4 }; }
  ;

block_configuration.use_clause_mopt:
    %empty                                          { }
  | block_configuration.use_clause_mopt use_clause  { $$ = $1; $$.push_back($2); }
  ;

block_configuration.configuration_item_mopt:
    %empty                                                          { }
  | block_configuration.configuration_item_mopt configuration_item  { $$ = $1; $$.push_back($2); }
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
    %empty                                          {}
  | block_declarative_part block_declarative_item   { $$ = $1; $$.push_back($2); }
  ;

block_header:
    %empty                                                { $$ = BlockHeader { {}, std::nullopt, std::nullopt }; }
  | block_header.generic_clause                           { $$ = BlockHeader { {}, $1, std::nullopt }; }
  | block_header.port_clause                              { $$ = BlockHeader { {}, std::nullopt, $1 }; }
  | block_header.generic_clause block_header.port_clause  { $$ = BlockHeader { {}, $1, $2 }; }
  ;

block_header.generic_clause:
    generic_clause                          { $$ = BlockHeaderGenericClause { {}, $1, nullptr }; }
  | generic_clause generic_map_aspect ';'   { $$ = BlockHeaderGenericClause { {}, $1, $2 }; }
  ;

block_header.port_clause:
    port_clause                       { $$ = BlockHeaderPortClause { {}, $1, nullptr }; }
  | port_clause port_map_aspect ';'   { $$ = BlockHeaderPortClause { {}, $1, $2 }; }
  ;

/* block_specification:
    architecture_name
  | block_statement_label
  | generate_statement_label
  // TODO: Replace with another rule?
  | generate_statement_label '(' index_specification ')'
  ; */

block_specification:
    /* Name must be:
        - architecture_name
        - block_statement_label
        - generate_statement_label
    */
    name                                { $$ = BlockSpecification { {}, $1, std::nullopt }; }
    // Label must be a generate_statement_label
  | label '(' index_specification ')'   { $$ = BlockSpecification { {}, $1, $3 }; }
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
                  { $$ = BlockStatement { {}, $1, $4, $6, $7, $9, $12 }; }
  ;

block_statement.guard_expression_opt:
    %empty              { }
  // SEMANTIC: "expression" must be a GUARD (boolean) expression
  | '(' expression ')'  { $$ = $2; }
  ;

block_statement_part:
    %empty                                      { }
  | block_statement_part concurrent_statement   { $$ = $1; $$.push_back($2); }
  ;

bus_opt:
    %empty  { $$ = false; }
  | BUS     { $$ = true; }
  ;

case_statement:
    label_colon_opt
      CASE expression IS
        case_statement.case_statement_alternative_mul
      // SEMANTIC: "label_opt" must be a CASE label
      // SEMANTIC: "label_opt" == "label_colon_opt"
      END CASE label_opt ';'
                { $$ = CaseStatement { {}, $1, $3, $5, $8 }; }
  ;

case_statement.case_statement_alternative_mul:
    case_statement_alternative                                                { $$ = std::vector<CaseAlternative> { $1 }; }
  | case_statement.case_statement_alternative_mul case_statement_alternative  { $$ = $1; $$.push_back($2); }
  ;

case_statement_alternative:
    WHEN choices "=>" sequence_of_statements
                { $$ = CaseAlternative { {}, $2, $4 }; }
  ;

choice:
    simple_expression   { $$ = std::make_unique<SimpleExpression>($1); }
  | discrete_range
  // SEMANTIC: "simple_name" must be an ELEMENT name
  // "simple_name" can be parsed as a simple_expression
  /* | simple_name */
  | OTHERS              { $$ = Keywords::Others; }
  ;

choices:
    choice              { $$ = std::vector<Choice> { $1 }; }
  | choices '|' choice  { $$ = $1; $$.push_back($3); }
  ;

component_configuration:
    FOR component_specification
      component_configuration.binding_indication_opt
      component_configuration.block_configuration_opt
    END FOR ';'
              { $$ = ComponentConfiguration { {}, $2, $3, $4 }; }
  ;

component_configuration.binding_indication_opt:
    %empty                  { }
  | binding_indication ';'
  ;

component_configuration.block_configuration_opt:
    %empty                { }
  | block_configuration
  ;

component_declaration:
    COMPONENT identifier is_opt
      component_declaration.local_generic_clause_opt
      component_declaration.local_port_clause_opt
    // SEMANTIC: "simple_name_opt" must be a COMPONENT name
    // SEMANTIC: "simple_name_opt" == "identifier"
    END COMPONENT simple_name_opt ';'
              { $$ = ComponentDeclaration { {}, $2, $4, $5, $8 }; }
  ;

component_declaration.local_generic_clause_opt:
    %empty          { }
    // SEMANTIC: "generic_clause" must be a LOCAL generic clause
  | generic_clause
  ;

component_declaration.local_port_clause_opt:
    %empty        { }
    // SEMANTIC: "port_clause" must be a LOCAL port clause
  | port_clause
  ;

component_instantiation_statement:
    label ':'
      instantiated_unit
        generic_map_aspect_opt
        port_map_aspect_opt ';'
                { $$ = ComponentInstantiation { {}, $1, $3, $4, $5 }; }
  ;

component_specification:
    // SEMANTIC: "name" must be a COMPONENT name
    instantiation_list ':' name   { $$ = ComponentSpecification { {}, $1, $3 }; }
  ;

composite_type_definition:
    array_type_definition
  | record_type_definition
  ;

concurrent_assertion_statement:
    // FIXME: Correct initialization
    label_colon_opt postponed_opt assertion ';' { $$ = ConcurrentAssertion { {}, $3, $1, $2 }; }
  ;

label_colon_opt:
    %empty      { }
  | label ':'
  ;

postponed_opt:
    %empty      { $$ = false; }
  | POSTPONED   { $$ = true; }
  ;

concurrent_procedure_call_statement:
    label_colon_opt postponed_opt procedure_call ';'
            { $$ = ConcurrentProcedureCall { {}, $3 , $1, $2 }; }
  ;

concurrent_signal_assignment_statement:
    label_colon_opt postponed_opt conditional_signal_assignment
            { $$ = ConcurrentConditionalSignalAssignment { {}, $3, $1, $2 }; }
  | label_colon_opt postponed_opt selected_signal_assignment
            { $$ = ConcurrentSelectedSignalAssignment { {}, $3, $1, $2 }; }
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
    UNTIL condition   { $$ = ConditionClause { {}, $2 }; }
  ;

conditional_signal_assignment:
    target "<=" options conditional_waveforms ';'
              { $$ = ConditionalSignalAssignment { {}, $1, $3, $4 }; }
  ;

conditional_waveforms:
    conditional_waveforms.waveform_condition_mopt
      waveform conditional_waveforms.when_condition_opt
              { $$ = ConditionalWaveforms { {}, $1, $2, $3 }; }
  ;

conditional_waveforms.waveform_condition_mopt:
    %empty
              { }
  | conditional_waveforms.waveform_condition_mopt waveform WHEN condition ELSE
              { $$ = $1; $$.emplace_back({}, $2, $4); }
  ;

conditional_waveforms.when_condition_opt:
    %empty          { }
  | WHEN condition  { $$ = $2; }
  ;

configuration_declaration:
    // SEMANTIC: "name" must be an ENTITY name
    CONFIGURATION identifier OF name IS
      configuration_declarative_part
      block_configuration
    // SEMANTIC: "simple_name_opt" must be a CONFIGURATION name
    // SEMANTIC: "simple_name_opt" == identifier
    END configuration_declaration.configuration_opt simple_name_opt ';'
                { $$ = ConfigurationDeclaration { {}, $2, $4, $6, $7, $10 }; }
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
                { }
  | configuration_declarative_part configuration_declarative_item
                { $$ = $1; $$.push_back($2); }
  ;

configuration_item:
    block_configuration
  | component_configuration
  ;

configuration_specification:
    FOR component_specification binding_indication ';'
                { $$ = ConfigurationSpecification { {}, $2, $3 }; }
  ;

constant_declaration:
    CONSTANT identifier_list ':'
      subtype_indication constant_declaration.expression_opt ';'
                { $$ = ConstantDeclaration { {}, $2, $4, $5 }; }
  ;

constant_declaration.expression_opt:
    %empty            { }
  | ":=" expression   { $$ = $2; }
  ;

constrained_array_definition:
    // SEMANTIC: "subtype_indication" is an ELEMENT subtype indication
    ARRAY index_constraint OF subtype_indication
                { $$ = ConstrainedArrayDefinition { {}, $2, $4 }; }
  ;

constraint:
    range_constraint
  | index_constraint
  ;

constraint_opt:
    %empty      { }
  | constraint
  ;

context_clause:
    %empty                        { }
  | context_clause context_item   { $$ = $1; $1.push_back($2); }
  ;

context_item:
    library_clause
  | use_clause
  ;

delay_mechanism:
    TRANSPORT                         { $$ = Keyword::Transport {}; }
  | INERTIAL                          { $$ = Keyword::Inertial {}; }
  | REJECT time_expression INERTIAL   { $$ = $2; }
  ;

design_file:
    design_unit               { $$ = std::vector<DesignUnit> { $1 }; }
  | design_file design_unit   { $$ = $1; $$.push_back($2); }
  ;

design_unit:
    context_clause library_unit   { $$ = DesignUnit { {}, $1, $2 }; }
  ;

designator:
    identifier
  | operator_symbol
  ;

designator_opt:
    %empty      { }
  | designator
  ;

direction:
    TO        { $$ = Direction::to; }
  | DOWNTO    { $$ = Direction::downto; }
  ;

disconnection_specification:
    DISCONNECT guarded_signal_specification AFTER time_expression ';'
              { $$ = DisconnectionSpecification { {}, $2, $4 }; }
  ;

discrete_range:
    // SEMANTIC: "subtype_indication" must be a DISCRETE subtype
    subtype_indication
  | range
  ;

element_association:
    expression
  | choices "=>" expression   { $$ = ChoicesExpression { {}, $1, $3 }; }
  ;

element_declaration:
    identifier_list ':' element_subtype_definition ';'
              { $$ = ElementDeclaration { {}, $1, $3 }; }
  ;

element_subtype_definition:
    subtype_indication
  ;

entity_aspect:
    // SEMANTIC: "name" must be an ENTITY name
    ENTITY name                       { $$ = EntityName { {}, $2, std::nullopt }; }
    // SEMANTIC: "name" must be an ENTITY name
    // SEMANTIC: "identifier" must be an architecture identifier
  | ENTITY name '(' identifier ')'    { $$ = EntityName { {}, $2, $4 }; }
    // SEMANTIC: "name" must be a CONFIGURATION name
  | CONFIGURATION name                { $$ = ConfigurationName { {}, $2 }; }
  | OPEN                              { $$ = Keyword::Open {}; }
  ;

entity_class:
    ARCHITECTURE    { $$ = EntityClass::architecture ; }
  | COMPONENT       { $$ = EntityClass::component ; }
  | CONFIGURATION   { $$ = EntityClass::configuration ; }
  | CONSTANT        { $$ = EntityClass::constant ; }
  | ENTITY          { $$ = EntityClass::entity ; }
  | FUNCTION        { $$ = EntityClass::function ; }
  | LABEL           { $$ = EntityClass::label ; }
  | LITERAL         { $$ = EntityClass::literal ; }
  | PACKAGE         { $$ = EntityClass::package ; }
  | PROCEDURE       { $$ = EntityClass::procedure ; }
  | SIGNAL          { $$ = EntityClass::signal ; }
  | SUBTYPE         { $$ = EntityClass::subtype ; }
  | TYPE            { $$ = EntityClass::type ; }
  | UNITS           { $$ = EntityClass::units ; }
  | VARIABLE        { $$ = EntityClass::variable ; }
  ;

entity_class_entry:
    entity_class        { $$ = EntityClassEntry { {}, $1, false }; }
  | entity_class "<>"   { $$ = EntityClassEntry { {}, $1, true }; }
  ;

entity_class_entry_list:
    entity_class_entry
                  { $$ = std::vector<EntityClassEntry> { $1 }; }
  | entity_class_entry_list ',' entity_class_entry
                  { $$ = $1; $$.push_back($3); }
  ;


entity_declaration:
    ENTITY identifier IS
      entity_header
      entity_declarative_part
    entity_declaration.entity_statement_part_opt
    // SEMANTIC: "simple_name_opt" must be an ENTITY name
    // SEMANTIC: "simple_name_opt" == "identifier"
    END entity_declaration.entity_opt simple_name_opt ';'
                { $$ = EntityDeclaration { {}, $2, $4, $5, $6, $9 }; }
  ;

entity_declaration.entity_statement_part_opt:
    %empty                    { }
  | BEGIN
      entity_statement_part   { $$ = $2; }
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
                { }
  | entity_declarative_part entity_declarative_item
                { $$ = $1; $$.push_back($2); }
  ;

entity_designator:
    entity_tag            { $$ = EntityDesignator { {}, $1, std::nullopt }; }
  | entity_tag signature  { $$ = EntityDesignator { {}, $1, $2 }; }
  ;

entity_header:
    %empty                        { }
    // SEMANTIC: "generic_clause" must be a FORMAL generic clause
  | generic_clause                { $$ = EntityHeader { {}, $1, nullptr }; }
    // SEMANTIC: "port_clause" must be a FORMAL port clause
  | port_clause                   { $$ = EntityHeader { {}, nullptr, $1 }; }
    // SEMANTIC: "generic_clause" must be a FORMAL generic clause
    // SEMANTIC: "port_clause" must be a FORMAL port clause
  | generic_clause port_clause    { $$ = EntityHeader { {}, $1, $2 }; }
  ;

entity_name_list:
    entity_name_list.entity_designator_mul
  | OTHERS                                  { $$ = Keyword::Others {}; }
  | ALL                                     { $$ = Keyword::All {}; }
  ;

entity_name_list.entity_designator_mul:
    entity_designator
                { $$ = std::vector<EntityDesignator> { $1 }; }
  | entity_name_list.entity_designator_mul ',' entity_designator
                { $$ = $1; $$.push_back($3); }
  ;

entity_specification:
    entity_name_list ':' entity_class   { $$ = EntitySpecification { {}, $1, $3 }; }
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
    %empty                                  { }
  | entity_statement_part entity_statement  { $$ = $1; $$.push_back($2); }
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
    '(' enumeration_type_definition.inner ')'   { $$ = $2; }
  ;

enumeration_type_definition.inner:
    enumeration_literal
                { $$ = std::vector<EnumerationLiteral> { $1 }; }
  | enumeration_type_definition.inner ',' enumeration_literal
                { $$ = $1; $$.push_back($3); }
  ;

exit_statement:
    // SEMANTIC: "label_opt" must be a LOOP label
    label_colon_opt EXIT label_opt exit_statement.when_condition_opt ';'
                { $$ = ExitStatement { {}, $1, $3, $4 }; }
  ;

exit_statement.when_condition_opt:
    %empty          { }
  | WHEN condition  { $$ = $2; }
  ;

expanded_selected_name:
    simple_prefix '.' suffix  { $$ = ExpandedSelectedName { {}, $1, $3 }; }
  ;

simple_prefix:
    expanded_selected_name  { $$ = std::make_unique<ExpandedSelectedName>($1); }
  | simple_name
  ;

expression:
    relation                                { $$ = Expression { {}, std::vector<Relation> { $1 }, RelationKind::AND }; }
  | relation expression.and_relation_mul    { $2[0] = $1; $$ = Expression { {}, $2, RelationKind::AND }; }
  | relation expression.or_relation_mul     { $2[0] = $1; $$ = Expression { {}, $2, RelationKind::OR }; }
  | relation expression.xor_relation_mul    { $2[0] = $1; $$ = Expression { {}, $2, RelationKind::XOR }; }
  | relation NAND relation                  { $$ = Expression { {}, std::vector<Relation> { $1, $3 }, RelationKind::nand }; }
  | relation NOR relation                   { $$ = Expression { {}, std::vector<Relation> { $1, $3 }, RelationKind::nor }; }
  | relation expression.xnor_relation_mul   { $2[0] = $1; $$ = Expression { {}, $2, RelationKind::xnor }; }
  ;

expression.and_relation_mul:
    AND relation                              
              { auto temp { std::vector<Relation>(1) }; temp.push_back($2); $$ = std::move(temp); }
  | expression.and_relation_mul AND relation    { $$ = $1; $$.push_back($3); }
  ;

expression.or_relation_mul:
    OR relation
              { auto temp { std::vector<Relation>(1) }; temp.push_back($2); $$ = std::move(temp); }
  | expression.or_relation_mul OR relation      { $$ = $1; $$.push_back($3); }
  ;

expression.xor_relation_mul:
    XOR relation
              { auto temp { std::vector<Relation>(1) }; temp.push_back($2); $$ = std::move(temp); }
  | expression.xor_relation_mul XOR relation    { $$ = $1; $$.push_back($3); }
  ;

expression.xnor_relation_mul:
    XNOR relation
              { auto temp { std::vector<Relation>(1) }; temp.push_back($2); $$ = std::move(temp); }
  | expression.xnor_relation_mul XNOR relation  { $$ = $1; $$.push_back($3); }
  ;

factor:
    primary               { $$ = Factor { {}, $1, std::nullopt }; }
  | primary "**" primary  { $$ = Factor { {}, $1, $3, FactorKind::exp }; }
  | ABS primary           { $$ = Factor { {}, $2, std::nullopt, FactorKind:abd }; }
  | NOT primary           { $$ = Factor { {}, $2, std::nullopt, FactorKind:NOT }; }
  ;

file_declaration:
    FILE identifier_list ':' subtype_indication ';'
                { $$ = FileDeclaration { {}, $2, $4 }; }
  | FILE identifier_list ':' subtype_indication file_open_information ';'
                { $$ = FileDeclaration { {}, $2, $4, $5 }; }
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
                { $$ = FileOpenInformation { {}, $2 }; }
  | OPEN file_open_kind_expression IS file_logical_name
                { $$ = FileOpenInformation { {}, $4, $2 }; }
  ;

file_type_definition:
    FILE OF type_mark   { $$ = FileTypeDefinition { {}, $3 }; }
  ;

formal_designator:
    /**
     *  Name must be:
     *    - generic_name
     *    - port_name
     *    - parameter_name
    **/
    name
  ;

formal_parameter_list:
    // SEMANTIC: "interface_list" is a PARAMETER INTERFACE LIST
    interface_list
  ;

formal_part:
    formal_designator                 { $$ = FormalPart { {}, $1 }; }
    // Name must be function_name or a type_mark
  | name '(' formal_designator ')'    { $$ = FormalPart { {}, $3, $1 }; }
  ;

full_type_declaration:
    TYPE identifier IS type_definition ';'  { $$ = TypeDeclaration { {}, $2, $4 }; }
  ;

function_call:
    // SEMANTIC: "name" must be a function_name
    name                            { $$ = FunctionCall { {}, $1 }; }
    // SEMANTIC: "association_list" is a PARAMETER association list
    // SEMANTIC: "name" must be a function name
  | name '(' association_list ')'   { $$ = FunctionCall { {}, $1, $3 }; }
  ;

generate_statement:
    label ':'
      generation_scheme GENERATE
        generate_statement.block_declarative_statement_opt
        generate_statement.concurrent_statement_mopt
      // SEMANTIC: "label_opt" must be a GENERATE label
      // SEMANTIC: "label_opt" == "label"
      END GENERATE label_opt ';'
              { $$ = GenerateStatement { {}, $1, $3, $5, $6, $9 }; }
  ;

generate_statement.block_declarative_statement_opt:
    %empty
              { }
  | generate_statement.block_declarative_statement_opt.block_declarative_item_mopt BEGIN
  ;

generate_statement.block_declarative_statement_opt.block_declarative_item_mopt:
    %empty
              { }
  | generate_statement.block_declarative_statement_opt.block_declarative_item_mopt block_declarative_item
              { $$ = $1; $$.push_back($2); }
  ;

generate_statement.concurrent_statement_mopt:
    %empty
              { }
  | generate_statement.concurrent_statement_mopt concurrent_statement
              { $$ = $1; $$.push_back($2); }
  ;

generation_scheme:
    // SEMANTIC: "parameter_specification" must be a GENERATION parameter specification
    FOR parameter_specification   { $$ = $2; }
  | IF condition                  { $$ = std::make_unique<Expression> { $2 }; }
  ;

generic_clause:
    GENERIC '(' interface_constant_declaration_list ')' ';'
              { $$ = GenericClause { {}, $3 }; }
  ;

generic_map_aspect:
    // SEMANTIC: "association_list" is a GENERIC association list
    GENERIC MAP '(' association_list ')'  { $$ = $4; }
  ;

generic_map_aspect_opt:
    %empty              { }
  | generic_map_aspect
  ;

group_constituent:
    name
  | CHARACTER_LITERAL
  ;

group_constituent_list:
    group_constituent
              { $$ = std::vector<GroupConstituent> { $1 }; }
  | group_constituent_list ',' group_constituent
              { $$ = $1; $$.push_back($3); }
  ;

group_declaration:
    // SEMANTIC: "name" must be a GROUP TEMPLATE name
    GROUP identifier ':' name '(' group_constituent_list ')' ';'
              { $$ = GroupDeclaration { {}, $2, $4, $6 }; }
  ;

group_template_declaration:
    GROUP identifier IS '(' entity_class_entry_list ')' ';'
              { $$ = GroupTemplateDeclaration { {}, $2, $5 }; }
  ;

guarded_signal_specification:
    // SEMANTIC: "signal_list" must be a list of GUARDD signals + locally static
    signal_list ':' type_mark   { $$ = GuardedSignalSpecification { {}, $1, $3 }; }
  ;

identifier:
    BASIC_IDENTIFIER
  | EXTENDED_IDENTIFIER
  ;

identifier_list:
    identifier                      { $$ = std::vector<Identifier> { $1 }; }
  | identifier_list ',' identifier  { $$ = $1; $$.push_back($3); }
  ;

if_statement:
    label_colon_opt
      IF condition THEN
        sequence_of_statements
      if_statement.elsif_condition_mopt
      if_statement.else_opt
      // SEMANTIC: "label_opt" must be an IF STATEMENT label
      // SEMANTIC: "label_opt" == "label_colon_opt"
      END IF label_opt ';'
              { $6[0] =  std::make_pair($3, $5); $$ = IfStatement { {}, $1, $6, $7, $10 }; }
  ;

if_statement.elsif_condition_mopt:
    %empty
              { $$ = std::vector<IfStatementPair>(1); }
  | if_statement.elsif_condition_mopt ELSIF condition THEN sequence_of_statements
              { $$ = $1; $$.push_back({ $3, $5 }); }
  ;

if_statement.else_opt:
    %empty                        { }
  | ELSE sequence_of_statements   { $$ = $2; }
  ;

incomplete_type_declaration:
    TYPE identifier   { $$ = TypeDeclaration { {}, $2 }; }
  ;

index_constraint:
    '(' index_constraint.discrete_range_mul ')'   { $$ = $2; }
  ;

index_constraint.discrete_range_mul:
    discrete_range
              { $$ = std::vector<DiscreteRange> { $1 }; }
  | index_constraint.discrete_range_mul ',' discrete_range
              { $$ = $1; $$.push_back($3); }
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
              { $$ = std::vector<Name> { $1 }; }
  | index_subtype_definition_mul ',' index_subtype_definition
              { $$ = $1; $$.push_back($3); }
  ;

indexed_name:
    prefix '(' indexed_name.expression_mul ')'
              { $$ = IndexedName { {}, $1, $3 }}
  ;

indexed_name.expression_mul:
    expression
              { $$ = std::vector<Expression> { $1 }; }
  | indexed_name.expression_mul ',' expression
              { $$ = $1; $$.push_back($3); }
  ;

init_expression_opt:
    %empty            { }
  | ":=" expression   { $$ = $2; }
  ;

instantiated_unit:
    // SEMANTIC: "name" must be a COMPONENT name
    name                              { $$ = ComponentUnit { {}, $1 }; }
    // SEMANTIC: "name" must be a COMPONENT name
  | COMPONENT name                    { $$ = ComponentUnit { {}, $2 }; }
    // SEMANTIC: "name" must be an ENTITY name
  | ENTITY name                       { $$ = EntityName { {}, $2 }; }
    // SEMANTIC: "name" must be an ENTITY name
    // SEMANTIC: "identifier" must be an ARCHITECTURE identifier
  | ENTITY name '(' identifier ')'    { $$ = EntityName { {}, $2, $4 }; }
    // SEMANTIC: "name" must be a CONFIGURATION name
  | CONFIGURATION name                { $$ = ConfigurationName { {}, $2 }; }
  ;

instantiation_list:
    // SEMANTIC: "instantiation.label_mul" must be a list of COMPONENT INSTANCE labels
    instantiation_list.label_mul
  | OTHERS                        { $$ = Keyword::Others {}; }
  | ALL                           { $$ = Keyword::All {}; }
  ;

instantiation_list.label_mul:
    // SEMANTIC: "label" must be a COMPONENT INSTANCE label
    label
              { $$ = std::vector<Identifier> { $1 }; }
  | instantiation_list.label_mul ',' label
              { $$ = $1; $$.push_back($3); }
  ;

interface_constant_declaration:
    CONSTANT identifier_list ':'
      interface_constant_declaration.in_opt subtype_indication
        // SEMANTIC: "init_expression_opt" must be a STATIC expression
        init_expression_opt
              { $$ = InterfaceConstantDeclaration { {}, $2, $5, $6 }; }
  ;

interface_constant_declaration_list:
    interface_constant_declaration
              { $$ = std::vector<InterfaceConstantDeclaration> { $1 }; }
  | interface_constant_declaration_list ';' interface_constant_declaration
              { $$ = $1; $$.push_back($3); }
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
              { $$ = InterfaceFileDeclaration { {}, $2, $4 }; }
  ;

interface_list:
    interface_element
              { $$ = std::vector<InterfaceDeclaration> { $1 }; }
  | interface_list ';' interface_element
              { $$ = $1; $$.push_back($3); }
  ;

interface_signal_declaration:
    // SEMANTIC: "init_expression_opt" must be a STATIC expression
    SIGNAL identifier_list ':' mode_opt subtype_indication bus_opt init_expression_opt
              { $$ = InterfaceSignalDeclaration { {}, $2, $4, $5, $6, $7 }; }
  ;

// SEMANTIC: Can be signal/constant/variable, must be decoded from context?
// TODO: Is this correct?
interface_unresolved_declaration:
    identifier_list ':' mode_opt subtype_indication bus_opt init_expression_opt
              { $$ = InterfaceUnresolvedDeclaration { {}, $1, $3, $5, $5, $6 }; }
  ;

interface_variable_declaration:
    // SEMANTIC: "init_expression_opt" must be a STATIC expression
    VARIABLE identifier_list ':' mode_opt subtype_indication init_expression_opt
              { $$ = InterfaceVariableDeclaration { {}, $2, $4, $5, $6 }; }
  ;

is_opt:
    %empty
  | IS
  ;

iteration_scheme:
    WHILE condition               { $$ = $2; }
    // SEMANTIC: "parameter_specification" is a LOOP parameter specification
  | FOR parameter_specification   { $$ = $2; }
  ;

label:
    identifier
  ;

label_opt:
    %empty    { }
  | label
  ;

library_clause:
    LIBRARY logical_name_list ';'   { $$ = $2; }
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
  | ID_NULL             { $$ = Keyword::Null {}; }
  ;

logical_name:
    identifier
  ;

logical_name_list:
    logical_name
              { $$ = std::vector<Identifier> { $1 }; }
  | logical_name_list ',' logical_name
              { $$ = $1; $$.push_back($3); }
  ;

loop_statement:
    label_colon_opt
      loop_statement.iteration_scheme_opt LOOP
        sequence_of_statements
      // SEMANTIC: "label_opt" must be a LOOP label
      // SEMANTIC: "label_opt" == "label_colon_opt"
      END LOOP label_opt ';'
              { $$ = LoopStatement { {}, $1, $2, $4, $7 }; }
  ;

loop_statement.iteration_scheme_opt:
    %empty            { }
  | iteration_scheme
  ;

mode:
    IN        { $$ = Mode::in; }
  | OUT       { $$ = Mode::out; }
  | INOUT     { $$ = Mode::inout; }
  | BUFFER    { $$ = Mode::buffer;}
  | LINKAGE   { $$ = Mode::linkage; }
  ;

mode_opt:
    %empty    { }
  | mode
  ;

multiplying_operator:
    '*'   { $$ = MulOp::mul; }
  | '/'   { $$ = MulOp::div; }
  | MOD   { $$ = MulOp::mod; }
  | REM   { $$ = MulOp::rem; }
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
    label_colon_opt NEXT label_opt next_statement.when_opt ';'
              { $$ = NextStatement { {}, $1, $3, $4 }; }
  ;

next_statement.when_opt:
    %empty          { }
  | WHEN condition  { $$ = $2; }
  ;

null_statement:
    label_colon_opt ID_NULL ';'   { $$ = NullStatement { {}, $1 }; }
  ;

numeric_literal:
    abstract_literal
  | physical_literal
  ;

options:
    options.guarded_opt options.delay_mechanism_opt
              { $$ = Options { {}, $1, $2 }; }
  ;

options.guarded_opt:
    %empty    { $$ = false; }
  | GUARDED   { $$ = true; }
  ;

options.delay_mechanism_opt:
    %empty            { }
  | delay_mechanism
  ;

package_body:
    PACKAGE BODY simple_name IS
      package_body_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PACKAGE name
    // SEMANTIC: "simple_nam_opt" == "simple_name"
    END package_body.package_body_opt simple_name_opt ';'
            { $$ = PackageBody { {}, $3, $5, $8 }; }
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
              { }
  | package_body_declarative_part package_body_declarative_item
              { $$ = $1; $$.push_back($2); }
  ;

package_declaration:
    PACKAGE identifier IS
      package_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PACKAGE name
    // SEMANTIC: "simple_name_opt" == "identifier"
    END package.package_opt simple_name_opt ';'
              { $$ = PackageDeclaration { {}, $2, $4, $7 }; }
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
            { }
  | package_declarative_part package_declarative_item
            { $$ = $1; $$.push_back($2); }
  ;

parameter_specification:
    identifier IN discrete_range  { $$ = ParameterSpecification { {}, $1, $3 }; }
  ;

physical_literal:
    // SEMANTIC: "name" must be a PHYSICAL UNIT name
    name                    { $$ = PhysicalLiteral { {}, $1 }; }
    // SEMANTIC: "name" must be a PHYSICAL UNIT name
  | abstract_literal name   { $$ = PhysicalLiteral { {}, $2, $1 }; }
  ;

physical_type_definition:
    range_constraint
      UNITS
        primary_unit_declaration
        physical_type_definition.secondary_unit_declaration_mopt
      // SEMANTIC: "simple_name_opt" must be a PHYSICAL TYPE name
      // SEMANTIC: "simple_name_opt" should be equal to the type name it's enclosing
      END UNITS simple_name_opt
              { $$ = PhysicalTypeDefinition { {}, $1, $3, $4, $7 }; }
  ;

physical_type_definition.secondary_unit_declaration_mopt:
    %empty
              { }
  | physical_type_definition.secondary_unit_declaration_mopt secondary_unit_declaration
              { $$ = $1; $$.push_back($2); }
  ;

port_clause:
    PORT '(' port_list ')' ';'    { $$ = PortClause { {}, $3 }; }
  ;

port_interface_declaration:
    interface_signal_declaration
  ;

port_interface_element:
    port_interface_declaration
  ;

port_interface_list:
    port_interface_element
              { $$ = std::vector<InterfaceSignalDeclaration> { $1 }; }
  | port_interface_list ';' port_interface_element
              { $$ = $1; $$.push_back($3); }
  ;

port_list:
    port_interface_list
  ;

port_map_aspect:
    // SEMANTIC: "association_list" is a PORT association list
    PORT MAP '(' association_list ')'   { $$ = PortMapAspect { {}, $4 }; }
  ;

port_map_aspect_opt:
    %empty            { }
  | port_map_aspect
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
    // SEMANTIC: "name" must be a procedure name
    name                            { $$ = ProcedureCall { {}, $1 }; }
    // SEMANTIC: "association_list" is a PARAMETER association list
    // SEMANTIC: "name" must be a procedure name
  | name '(' association_list ')'   { $$ = ProcedureCall { {}, $1, $3 }; }
  ;

procedure_call_statement:
    label_colon_opt procedure_call ';'  { $$ = ProcedureCallStatement { {}, $2, $1 }; }
  ;

process_declarative_item:
    alias_declaration
  | attribute_declaration
  | attribute_specification
  | constant_declaration
  | file_declaration
  | group_declaration
  | group_template_declaration
  | subprogram_body
  | subprogram_declaration
  | subtype_declaration
  | type_declaration
  | use_clause
  | variable_declaration
  ;

process_declarative_part:
    %empty
              { $$ = std::vector<ProcessDeclarativeItem> {}; }
  | process_declarative_part process_declarative_item
              { $$ = $1; $$.push_back($2); }
  ;

process_statement:
    // FIXME: Fix postponed handling
    label_colon_opt
      process_statement.postponed_opt PROCESS process_statement.sensitivity_list_opt is_opt
        process_declarative_part
      BEGIN
        process_statement_part
      // SEMANTIC: "label_opt" must be a PROCESS label
      // SEMANTIC: "label_opt" == "label_colon_opt"
      END process_statement.postponed_opt PROCESS label_opt ';'
              { $$ = ProcessStatement { {}, $1, $2, $4, $6, $8, $10, $12 }; }
  ;

process_statement.postponed_opt:
    %empty      { $$ = false; }
  | POSTPONED   { $$ = true; }
  ;

process_statement.sensitivity_list_opt:
    %empty                    { }
  | '(' sensitivity_list ')'  { $$ = $2; }
  ;

process_statement_part:
    %empty
            { }
  | process_statement_part sequential_statement
            { $$ = $1; $$.push_back($2); }
  ;

protected_type_body:
    PROTECTED BODY
      protected_type_body_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PROTECTED TYPE name
    END PROTECTED BODY simple_name_opt
            { $$ = ProtectedTypeBody { {}, $3, $7 }; }
  ;

protected_type_body_declarative_item:
    alias_declaration
  | attribute_declaration
  | attribute_specification
  | constant_declaration
  | file_declaration
  | group_declaration
  | group_template_declaration
  | subprogram_body
  | subprogram_declaration
  | subtype_declaration
  | type_declaration
  | use_clause
  | variable_declaration
  ;

protected_type_body_declarative_part:
    %empty
              { }
  | protected_type_body_declarative_part protected_type_body_declarative_item
              { $$ = $1; $$.push_back($2); }
  ;

protected_type_declaration:
    PROTECTED
      protected_type_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PROTECTED TYPE name
    // SEMANTIC: "simple_name_opt" should be equal to the name in enclosing type definition
    END PROTECTED simple_name_opt
              { $$ = ProtectedTypeDeclaration { {}, $2, $5 }; }
  ;

protected_type_declarative_item:
    subprogram_specification
  | attribute_specification
  | use_clause
  ;

protected_type_declarative_part:
    %empty
              { }
  | protected_type_declarative_part protected_type_declarative_item
              { $$ = $1; $$.push_back($2); }
  ;

protected_type_definition:
    protected_type_declaration
  | protected_type_body
  ;

qualified_expression:
    type_mark '\'' '(' expression ')'   { $$ = QualifiedExpression { {}, $1, $4, std::nullopt }; }
  | type_mark '\'' aggregate            { $$ = QualifiedExpression { {}, $1, std::nullopt, $3 }; }
  ;

range:
    // SEMANTIC: "attribute_name" must be a RANGE attribute name
    attribute_name
  | simple_expression direction simple_expression
              { $$ = SimpleRange { {}, $1, $2, $3 }; }
  ;

range_constraint:
    RANGE range   { $$ = $2; }
  ;

record_type_definition:
    RECORD
      record_type_definition.element_declaration_mul
    // SEMANTIC: "simple_name_opt" must be a RECORD TYPE name
    // SEMANTIC: "simple_name_opt" should be equal to the enclosing type name
    END RECORD simple_name_opt
              { $$ = RecordTypeDefinition { {}, $2, $5 }; }
  ;

record_type_definition.element_declaration_mul:
    element_declaration
              { $$ = std::vector<ElementDeclaration> { $1 }; }
  | record_type_definition.element_declaration_mul element_declaration
              { $$ = $1; $$.push_back($2); }
  ;

relation:
    shift_expression
              { $$ = Relation { {}, $1 }; }
  | shift_expression relational_operator shift_expression
              { $$ = Relation { {}, $1, $2, $3 }; }
  ;

relational_operator:
    '='     { $$ = RelOp::eq; }
  |  "/="   { $$ = RelOp::neq; }
  | '<'     { $$ = RelOp::lt; }
  | "<="    { $$ = RelOp::lte; }
  | '>'     { $$ = RelOp::gt;}
  | ">="    { $$ = RelOp::gte; }
  ;

report_statement:
    label_colon_opt REPORT expression ';'
              { $$ = ReportStatement { {}, $1, $3 }; }
  | label_colon_opt REPORT expression
      SEVERITY expression ';'
              { $$ = ReportStatement { {}, $1, $3, $5 }; }
  ;

// FIXME: Check if this is correct
resolution_function_name_opt:
    // SEMANTIC: "designator_opt" must be a RESOLUTION FUNCTION name
    designator_opt
  ;

return_statement:
    label_colon_opt RETURN ';'
              { $$ = ReturnStatement { {}, $1 }; }
  | label_colon_opt RETURN expression ';'
              { $$ = ReturnStatement { {}, $1, $3 }; }
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
    identifier '=' physical_literal   { $$ = SecondaryUnitDeclaration { {}, $1, $3 }; }
  ;

selected_name:
    prefix '.' suffix   { $$ = SelectedName { {}, $1, $3 }; }
  ;

selected_signal_assignment:
    WITH expression SELECT
      target "<=" options selected_waveforms ';'
              { $$ = SelectedSignalAssignment { {}, $2, $4, $6, $7 }; }
  ;

// TODO: Check if correct
selected_waveforms:
    waveform WHEN choices
              { $$ = std::vector<SelectedWaveform> { SelectedWaveform { {}, $1, $3 } }; }
  | selected_waveforms ',' waveform WHEN choices
              { $$ = $1; $$.emplace_back<SelectedWaveform>($3, $5); }
  ;

sensitivity_clause:
    ON sensitivity_list   { $$ = $2; }
  ;

sensitivity_list:
    // SEMANTIC: "name" must be a SIGNAL name
    name
              { $$ = std::vector<Name> { $1 }; }
  | sensitivity_list ',' name
              { $$ = $1; $$.push_back($3); }
  ;

sequence_of_statements:
    %empty
              { }
  | sequence_of_statements sequential_statement
              { $$ = $1; $$.push_back($2); }
  ;

sequential_statement:
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
              { $$ = ShiftExpression { {}, $1 }; }
  | simple_expression shift_operator simple_expression
              { $$ = ShiftExpression { {}, $1, $2, $3 }; }
  ;

shift_operator:
    SLL   { $$ = ShiftOperator::sll; }
  | SRL   { $$ = ShiftOperator::srl; }
  | SLA   { $$ = ShiftOperator::sla; }
  | SRA   { $$ = ShiftOperator::sra; }
  | ROL   { $$ = ShiftOperator::rol; }
  | ROR   { $$ = ShiftOperator::ror; }
  ;

sign:
    '+'   { $$ = Sign::plus; }
  | '-'   { $$ = Sign::minus; }
  ;

signal_assignment_statement:
    label_colon_opt target "<=" signal_assignment_statement.delay_mechanism_opt waveform ';'
              { $$ = SignalAssignmentStatement { {}, $1, $2, $4, $5 }; }
  ;

signal_assignment_statement.delay_mechanism_opt:
    %empty            { }
  | delay_mechanism
  ;

signal_declaration:
    SIGNAL identifier_list ':' subtype_indication signal_declaration.signal_kind_opt init_expression_opt ';'
              { $$ = SignalDeclaration { {}, $2, $4, $5, $6 }; }
  ;

signal_declaration.signal_kind_opt:
    %empty        { }
  | signal_kind
  ;

signal_kind:
    REGISTER      { $$ = SignalKind::reg; }
  | BUS           { $$ = SignalKind::bus; }
  ;

signal_list:
    signal_list.name_mul
  | OTHERS                { $$ = Keyword::Others {}; }
  | ALL                   { $$ = Keyword::All {}; }
  ;

signal_list.name_mul:
    // SEMANTIC: "name" must be a SIGNAL name
    name
                { $$ = std::vector<Name> { $1 }; }
  | signal_list.name_mul ',' name
                { $$ = $1; $$.push_back($3); }
  ;

signature:
    %empty                                            { }
  | '[' signature.type_mark_mul ']'                   { $$ = Signature { {}, $2, std::nullopt }; }
  | '[' RETURN type_mark ']'                          { $$ = Signature { {}, std::nullopt, $3 }; }
  | '[' signature.type_mark_mul RETURN type_mark ']'  { $$ = Signature { {}, $2, $4 }; }
  ;

signature.type_mark_mul:
    type_mark
              { $$ = std::vector<Name> { $1 }; }
  | signature.type_mark_mul ',' type_mark
              { $$ = $1; $$.push_back($3); }
  ;

simple_expression:
    simple_expression.sign_opt term simple_expression.adding_operator_mopt
              { $$ = SimpleExpression { {}, $1, $2, $3 }; }
  ;

simple_expression.sign_opt:
    %empty    { }
  | sign
  ;

simple_expression.adding_operator_mopt:
    %empty
              { }
  | simple_expression.adding_operator_mopt adding_operator term
              { $$ = $1; $$.push_back(std::make_pair($2, $3)); }
  ;

simple_name:
    identifier
  ;

simple_name_opt:
    %empty        { }
  | simple_name
  ;

slice_name:
    prefix '(' discrete_range ')'   { $$ = SliceName { {}, $1, $3 }; }
  ;

subprogram_body:
    subprogram_specification IS
      subprogram_declarative_part
    BEGIN
      subprogram_statement_part
    END subprogram_body.subprogram_kind_opt designator_opt ';'
              { $$ = SubprogramBody { {}, $1, $3, $5, $7, $8 }; }
  ;

subprogram_body.subprogram_kind_opt:
    %empty            { }
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
              { }
  | subprogram_declarative_part subprogram_declarative_item
              { $$ = $1; $$.push_back($2); }
  ;

subprogram_kind:
    PROCEDURE   { $$ = SubprogramKind::procedure; }
  | FUNCTION    { $$ = SubprogramKind::function; }
  ;

subprogram_specification:
    PROCEDURE designator
            { $$ = ProcedureSpecification { {}, $2 }; }
  | PROCEDURE designator '(' formal_parameter_list ')'
            { $$ = ProcedureSpecification { {}, $2, $4 }; }
  | function_type FUNCTION designator RETURN type_mark
            { $$ = FunctionSpecification { {}, $1, $3, std::nullopt, $5 }; }
  | function_type FUNCTION designator '(' formal_parameter_list ')' RETURN type_mark
            { $$ = FunctionSpecification { {}, $1, $3, $5, $8 }; }
  ;

function_type:
    %empty    { }
  | IMPURE    { $$ = FunctionType::impure; }
  | PURE      { $$ = FunctionType::pure; }
  ;

subprogram_statement_part:
    %empty
              { }
  | subprogram_statement_part sequential_statement
              { $$ = $1; $$.push_back($2); }
  ;

subtype_declaration:
    SUBTYPE identifier IS subtype_indication ';'
              { $$ = SubtypeDeclaration { {}, $2, $4 }; }
  ;

subtype_indication:
    resolution_function_name_opt type_mark constraint_opt
              { $$ = SubtypeIndication { {}, $1, $2, $3 }; }
  ;


suffix:
    simple_name
  | CHARACTER_LITERAL
  | operator_symbol
  | ALL               { $$ = Keyword::All {}; }
  ;

target:
    name
  | aggregate
  ;

term:
    factor                            { $$ = Term { {}, nullptr, std::nullopt, $1 }; }
  | term multiplying_operator factor  { $$ = Term { {}, $1, $2, $3 }; }
  ;

time_expression:
    // SEMANTIC: "expression" must be a TIME expression
    expression
  ;

timeout_clause:
    FOR time_expression   { $$ = $2; }
  ;

type_conversion:
    type_mark '(' expression ')'  { $$ = TypeConversion { {}, $1, $3 }; }
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
            { $$ = UnconstrainedArrayDefinition { {}, $3, $6 }; }
  ;

use_clause:
    USE use_clause.selected_name_mul ';'
            { $$ = UseClause { {}, $2 }; }
  ;

use_clause.selected_name_mul:
    expanded_selected_name
            { $$ = std::vector<ExpandedSelectedName> { $1 }; }
  | use_clause.selected_name_mul ',' expanded_selected_name
            { $$ = $1; $$.push_back($3); }
  ;

value_expression:
    // SEMANTIC: "expression" must be a VALUE expression
    expression
  ;

variable_assignment_statement:
    label_colon_opt target ":=" expression ';'
            { $$ = VariableAssignmentStatement { {}, $1, $2, $4 }; }
  ;

variable_declaration:
    plain_variable_declaration
  | shared_variable_declaration
  ;

plain_variable_declaration:
    VARIABLE identifier_list ':' subtype_indication init_expression_opt ';'
            { $$ = PlainVariableDeclaration { {}, $2, $4, $5 }; }
  ;

shared_variable_declaration:
    SHARED plain_variable_declaration
            { $$ = SharedVariableDeclaration { {}, $2 }; }
  ;

wait_statement:
    label_colon_opt WAIT wait_statement.sensitivity_clause_opt wait_statement.condition_clause_opt wait_statement.timeout_clause_opt ';'
            { $$ = WaitStatement { {}, $1, $3, $4, $5 }; }
  ;

wait_statement.sensitivity_clause_opt:
    %empty              { }
  | sensitivity_clause
  ;

wait_statement.condition_clause_opt:
    %empty              { }
  | condition_clause
  ;

wait_statement.timeout_clause_opt:
    %empty              { }
  | timeout_clause
  ;

waveform:
    waveform.waveform_element_mul
  | UNAFFECTED                      { $$ = Keyword::Unaffected {}; }
  ;

waveform.waveform_element_mul:
    waveform_element
                { $$ = std::vector<WaveformElement> { $1 }; }
  | waveform.waveform_element_mul ',' waveform_element
                { $$ = $1; $$.push_back($3); }
  ;

waveform_element:
    value_expression
                { $$ = WaveformElementKind::ValueExpression { {}, $1 }; }
  | value_expression AFTER time_expression
                { $$ = WaveformElementKind::ValueExpression { {}, $1, $3 }; }
  | ID_NULL
                { $$ = WaveformElementKind::NullExpression {}; }
  | ID_NULL AFTER time_expression
                { $$ = WaveformElementKind::NullExpression { {}, $3 }; }
  ;
%%
