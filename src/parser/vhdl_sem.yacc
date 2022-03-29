%require "3.8"
%language "c++"
%skeleton "glr2.cc"
%glr-parser

%define api.token.constructor
%header
%locations
%debug

%define api.value.type variant
%define api.namespace {Parser}
%define parse.error detailed

%code top
{
  #include "lexer_defs.h"
  #include "lexer.h"
  #include "parser_defs.h"
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

%token <Lexer::BasedLiteral>        BASED_LITERAL       "based literal"
%token <Lexer::BasicIdentifier>     BASIC_IDENTIFIER    "basic identifier"
%token <Lexer::BitStringLiteral>    BIT_STRING_LITERAL  "bit string literal"
%token <Lexer::CharacterLiteral>    CHARACTER_LITERAL   "character literal"
%token <Lexer::DecimalLiteral>      DECIMAL_LITERAL     "decimal literal"
%token <Lexer::ExtendedIdentifier>  EXTENDED_IDENTIFIER "extended identifier"
%token <Lexer::StringLiteral>       STRING_LITERAL      "string symbol"

%nterm <AbstractLiteral>                                    abstract_literal
%nterm <AccessTypeDefinition>                               access_type_definition
%nterm <ActualDesignator>                                   actual_designator
%nterm <ActualPart>                                         actual_part
%nterm <AddingOperator>                                     adding_operator
%nterm <Aggregate>                                          aggregate
%nterm <Aggregate>                                          aggregate.element_association_mul
%nterm <AliasDeclaration>                                   alias_declaration
%nterm <std::optional<std::unique_ptr<SubtypeIndication>>>  alias_declaration.opt_subtype
%nterm <AliasDesignator>                                    alias_designator
%nterm <Allocator>                                          allocator
%nterm <ArchitectureBody>                                   architecture_body
%nterm <std::vector<BlockDeclarativeItem>>                  architecture_declarative_part
%nterm <std::vector<ConcurrentStatement>>                   architecture_statement_part
%nterm <ArrayTypeDefinition>                                array_type_definition
%nterm <Assertion>                                          assertion
%nterm <std::unique_ptr<Expression>>                        assertion.report_opt
%nterm <std::unique_ptr<Expression>>                        assertion.severity_opt
%nterm <AssertionStatement>                                 assertion_statement
%nterm <AssociationElement>                                 association_element
%nterm <std::vector<AssociationElement>>                    association_list
%nterm <AttributeDeclaration>                               attribute_declaration
%nterm <Identifier>                                         attribute_designator
%nterm <AttributeName>                                      attribute_name
%nterm <std::unique_ptr<Expression>>                        attribute_name.expression_opt
%nterm <AttributeSpecification>                             attribute_specification
%nterm <BindingIndication>                                  binding_indication
%nterm <std::unique_ptr<EntityAspect>>                      binding_indication.entity_aspect_opt
%nterm <BlockConfiguration>                                 block_configuration
%nterm <std::vector<UseClause>>                             block_configuration.use_clause_mopt
%nterm <std::vector<ConfigurationItem>>                     block_configuration.configuration_item_mopt
%nterm <BlockDeclarativeItem>                               block_declarative_item
%nterm <std::vector<BlockDeclarativeItem>>                  block_declarative_part
%nterm <BlockHeader>                                        block_header
%nterm <BlockHeaderGenericClause>                           block_header.generic_clause
%nterm <BlockHeaderPortClause>                              block_header.port_clause
%nterm <BlockSpecification>                                 block_specification
%nterm <BlockStatement>                                     block_statement
%nterm <std::unique_ptr<Expression>>                        block_statement.guard_expression_opt
%nterm <std::vector<ConcurrentStatement>>                   block_statement_part
%nterm <bool>                                               bus_opt
%nterm <CaseStatement>                                      case_statement
%nterm <std::vector<CaseAlternative>>                       case_statement.case_statement_alternative_mul
%nterm <CaseAlternative>                                    case_statement_alternative
%nterm <Choice>                                             choice
%nterm <std::vector<Choice>>                                choices
%nterm <ComponentConfiguration>                             component_configuration
%nterm <std::optional<BindingIndication>>                   component_configuration.binding_indication_opt
%nterm <std::optional<BlockConfiguration>>                  component_configuration.block_configuration_opt
%nterm <ComponentDeclaration>                               component_declaration
%nterm <std::optional<GenericClause>>                       component_declaration.local_generic_clause_opt
%nterm <std::optional<PortClause>>                          component_declaration.local_port_clause_opt
%nterm <ComponentInstantiation>                             component_instantiation_statement
%nterm <ComponentSpecification>                             component_specification
%nterm <CompositeTypeDefinition>                            composite_type_definition
%nterm <ConcurrentAssertion>                                concurrent_assertion_statement
%nterm <bool>                                               postponed_opt
%nterm <ConcurrentSignalAssignment>                         concurrent_procedure_call_statement
%nterm <ConcurrentSelectedSignalAssignment>                 concurrent_signal_assignment_statement
%nterm <ConcurrentStatement>                                concurrent_statement
%nterm <Expression>                                         condition
%nterm <ConditionClause>                                    condition_clause
%nterm <ConditionalSignalAssignment>                        conditional_signal_assignment
%nterm <ConditionalWaveforms>                               conditional_waveforms
%nterm <std::vector<ConditionalWaveform>>                   conditional_waveforms.waveform_condition_mopt
%nterm <std::unique_ptr<Expression>>                        conditional_waveforms.when_condition_opt
%nterm <ConfigurationDeclaration>                           configuration_declaration
%nterm <ConfigurationDeclarativeItem>                       configuration_declarative_item
%nterm <std::vector<ConfigurationDeclarativeItem>>          configuration_declarative_part
%nterm <ConfigurationItem>                                  configuration_item
%nterm <ConfigurationSpecification>                         configuration_specification
%nterm <ConstantDeclaration>                                constant_declaration
%nterm <std::unique_ptr<Expression>>                        constant_declaration.expression_opt
%nterm <ConstrainedArrayDefinition>                         constrained_array_definition
%nterm <Constraint>                                         constraint
%nterm <std::optional<Constraint>>                          constraint_opt
%nterm <std::vector<ContextItem>>                           context_clause
%nterm <ContextItem>                                        context_item
%nterm <DelayMechanism>                                     delay_mechanism
%nterm <std::vector<DesignUnit>>                            design_file
%nterm <DesignUnit>                                         design_unit
%nterm <Designator>                                         designator
%nterm <std::optional<Designator>>                          designator_opt
%nterm <Direction>                                          direction
%nterm <DisconnectionSpecification>                         disconnection_specification
%nterm <DiscreteRange>                                      discrete_range
%nterm <ElementAssociation>                                 element_association
%nterm <ElementDeclaration>                                 element_declaration
%nterm <SubtypeIndication>                                  element_subtype_definition
%nterm <EntityAspect>                                       entity_aspect
%nterm <EntityClass>                                        entity_class
%nterm <EntityClassEntry>                                   entity_class_entry
%nterm <std::vector<EntityClassEntry>>                      entity_class_entry_list
%nterm <EntityDeclaration>                                  entity_declaration
%nterm <std::vector<EntityStatement>>                       entity_declaration.entity_statement_part_opt
%nterm <EntityDeclarativeItem>                              entity_declarative_item
%nterm <std::vector<EntityDeclarativeItem>>                 entity_declarative_part
%nterm <EntityDesginator>                                   entity_designator
%nterm <std::optional<EntityHeader>>                        entity_header
%nterm <EntitynameList>                                     entity_name_list
%nterm <std::vector<EntityDesignator>>                      entity_name_list.entity_designator_mul
%nterm <EntitySpecification>                                entity_specification
%nterm <EntityStatement>                                    entity_statement
%nterm <std::vector<EntityStatement>>                       entity_statement_part
%nterm <EntityTag>                                          entity_tag
%nterm <EnumerationLiteral>                                 enumeration_literal
%nterm <std::vector<EnumerationLiteral>>                    enumeration_type_definition
%nterm <std::vector<EnumerationLiteral>>                    enumeration_type_definition.inner
%nterm <ExitStatement>                                      exit_statement
%nterm <std::optional<Expression>>                          exit_statement.when_condition_opt
%nterm <ExpandedSelectedName>                               expanded_selected_name
%nterm <SimplePrefix>                                       simple_prefix
%nterm <Expression>                                         expression
%nterm <std::vector<Relation>>                              expression.and_relation_mul
%nterm <std::vector<Relation>>                              expression.or_relation_mul
%nterm <std::vector<Relation>>                              expression.xor_relation_mul
%nterm <std::vector<Relation>>                              expression.xnor_relation_mul
%nterm <Factor>                                             factor
%nterm <FileDeclaration>                                    file_declaration
%nterm <Expression>                                         file_logical_name
%nterm <Expression>                                         file_open_kind_expression
%nterm <FileOpenInformation>                                file_open_information
%nterm <FileTypeDefinition>                                 file_type_definition
%nterm <Name>                                               formal_designator
%nterm <std::vector<InterfaceDeclaration>>                  formal_parameter_list
%nterm <FormalPart>                                         formal_part
%nterm <TypeDeclaration>                                    full_type_declaration
%nterm <FunctionCall>                                       function_call
%nterm <GenerateStatement>                                  generate_statement
%nterm <std::vector<BlockDeclarativeItem>>                  generate_statement.block_declarative_statement_opt
%nterm <std::vector<BlockDeclarativeItem>>                  generate_statement.block_declarative_statement_opt.block_declarative_item_mopt
%nterm <std::vector<ConcurrentStatement>>                   generate_statement.concurrent_statement_mopt
%nterm <GenerationScheme>                                   generation_scheme
%nterm <GenericClause>                                      generic_clause
%nterm <std::vector<AssociationElement>>                    generic_map_aspect
%nterm <std::optional<std::vector<AssociationElement>>>     generic_map_aspect_opt
%nterm <GroupConstituent>                                   group_constituent
%nterm <std::vector<GroupConstituent>>                      group_constituent_list
%nterm <GroupDeclaration>                                   group_declaration
%nterm <GroupTemplateDeclaration>                           group_template_declaration
%nterm <GuardedSignalSpecification>                         guarded_signal_specification
%nterm <Identifier>                                         identifier
%nterm <std::vector<Identifier>>                            identifier_list
%nterm <IfStatement>                                        if_statement
%nterm <std::vector<IfStatementPair>>                       if_statement.elsif_condition_mopt
%nterm <std::vector<SequentialStatement>>                   if_statement.else_opt
%nterm <TypeDeclaration>                                    incomplete_type_declaration
%nterm <std::vector<DiscreteRange>>                         index_constraint
%nterm <std::vector<DiscreteRange>>                         index_constraint.discrete_range_mul
%nterm <IndexSpecification>                                 index_specification
%nterm <Name>                                               index_subtype_definition
%nterm <std::vector<Name>>                                  index_subtype_definition_mul
%nterm <IndexedName>                                        indexed_name
%nterm <std::vector<Expression>>                            indexed_name.expression_mul
%nterm <std::unique_ptr<Expression>>                        init_expression_opt
%nterm <InstantiatedUnit>                                   instantiated_unit
%nterm <InstantiationList>                                  instantiation_list
%nterm <std::vector<Identifier>>                            instantiation_list.label_mul
%nterm <InterfaceConstantDeclaration>                       interface_constant_declaration
%nterm <std::vector<InterfaceConstantDeclaration>>          interface_constant_declaration_list
%nterm <InterfaceDeclaration>                               interface_declaration
%nterm <InterfaceDeclaration>                               interface_element
%nterm <InterfaceFileDeclaration>                           interface_file_declaration
%nterm <std::vector<InterfaceDeclaration>>                  interface_list
%nterm <InterfaceSignalDeclaration>                         interface_signal_declaration
%nterm <InterfaceUnresolvedDeclaration>                     interface_unresolved_declaration
%nterm <InterfaceVariableDeclaration>                       interface_variable_declaration
%nterm <IterationScheme>                                    iteration_scheme
%nterm <Identifier>                                         label
%nterm <std::optional<Identifier>>                          label_colon_opt
%nterm <std::optional<Identifier>>                          label_opt
%nterm <std::vector<Identifier>>                            library_clause
%nterm <LibraryUnit>                                        library_unit
%nterm <Literal>                                            literal
%nterm <Identifier>                                         logical_name
%nterm <std::vector<Identifier>>                            logical_name_list
%nterm <LoopStatement>                                      loop_statement
%nterm <std::optional<IterationScheme>>                     loop_statement.iteration_scheme_opt
%nterm <Mode>                                               mode
%nterm <std::optional<Mode>>                                mode_opt
%nterm <MulOp>                                              multiplying_operator
%nterm <Name>                                               name
%nterm <NextStatement>                                      next_statement
%nterm <std::optional<Expression>>                          next_statement.when_opt
%nterm <NullStatement>                                      null_statement
%nterm <NumericLiteral>                                     numeric_literal
%nterm <Options>                                            options
%nterm <bool>                                               options.guarded_opt
%nterm <std::optional<DelayMechanism>>                      options.delay_mechanism_opt 
%nterm <PackageBody>                                        package_body
%nterm <PackageBodyDeclarativeItem>                         package_body_declarative_item
%nterm <std::vector<PackageBodyDeclarativeItem>>            package_body_declarative_part
%nterm <PackageDeclaration>                                 package_declaration
%nterm <PackageDeclarativeItem>                             package_declarative_item
%nterm <std::vector<PackageDeclarativeItem>>                package_declarative_part
%nterm <ParameterSpecification>                             parameter_specification
%nterm <PhysicalLiteral>                                    physical_literal
%nterm <PhysicalTypeDefinition>                             physical_type_definition
%nterm <std::vector<SecondaryUnitDeclaration>>              physical_type_definition.secondary_unit_declaration_mopt
%nterm <PortClause>                                         port_clause
%nterm <InterfaceSignalDeclaration>                         port_interface_declaration
%nterm <InterfaceSignalDeclaration>                         port_interface_element
%nterm <std::vector<InterfaceSignalDeclaration>>            port_interface_list
%nterm <std::vector<InterfaceSignalDeclaration>>            port_list
%nterm <PortMapAspect>                                      port_map_aspect
%nterm <std::optional<PortMapAspect>>                       port_map_aspect_opt
%nterm <Prefix>                                             prefix
%nterm <Primary>                                            primary
%nterm <PrimaryUnit>                                        primary_unit
%nterm <Identifier>                                         primary_unit_declaration
%nterm <ProcedureCall>                                      procedure_call
%nterm <ProcedureCallStatement>                             procedure_call_statement
%nterm <ProcessDeclarativeItem>                             process_declarative_item
%nterm <std::vector<ProcessDeclarativeItem>>                process_declarative_part
%nterm <ProcessStatement>                                   process_statement
%nterm <bool>                                               process_statement.postponed_opt
%nterm <std::optional<std::vector<Name>>>                   process_statement.sensitivity_list_opt
%nterm <std::vector<SequentialStatement>>                   process_statement_part
%nterm <ProtectedTypeBody>                                  protected_type_body
%nterm <ProtectedTypeBodyDeclarativeItem>                   protected_type_body_declarative_item
%nterm <std::vector<ProtectedTypeBodyDeclarativeItem>>      protected_type_body_declarative_part
%nterm <ProtectedTypeDeclaration>                           protected_type_declaration
%nterm <ProtectedTypeDeclarativeItem>                       protected_type_declarative_item
%nterm <std::vector<ProtectedTypeDeclarativeItem>>          protected_type_declarative_part
%nterm <ProtectedTypeDefinition>                            protected_type_definition
%nterm <QualifiedExpression>                                qualified_expression
%nterm <SimpleRange>                                        range
%nterm <SimpleRange>                                        range_constraint
%nterm <RecordTypeDefinition>                               record_type_definition
%nterm <std::vector<ElementDeclaration>>                    record_type_definition.element_declaration_mul
%nterm <Relation>                                           relation
%nterm <RelOp>                                              relational_operator
%nterm <ReportStatement>                                    report_statement
%nterm <std::optional<Designator>>                          resolution_function_name_opt
%nterm <ReturnStatement>                                    return_statement
%nterm <ScalarTypeDefinition>                               scalar_type_definition
%nterm <SecondaryUnit>                                      secondary_unit
%nterm <SecondaryUnitDeclaration>                           secondary_unit_declaration
%nterm <SelectedName>                                       selected_name
%nterm <SelectedSignalAssignment>                           selected_signal_assignment
%nterm <std::vector<SelectedWaveform>>                      selected_waveforms
%nterm <std::vector<Name>>                                  sensitivity_clause
%nterm <std::vector<Name>>                                  sensitivity_list
%nterm <std::vector<SequentialStatement>>                   sequence_of_statements
%nterm <SequentialStatement>                                sequential_statement
%nterm <ShiftExpression>                                    shift_expression
%nterm <ShiftOp>                                            shift_operator
%nterm <Sign>                                               sign
%nterm <SignalAssignmentStatement>                          signal_assignment_statement
%nterm <std::optional<DelayMechanism>>                      signal_assignment_statement.delay_mechanism_opt
%nterm <SignalDeclaration>                                  signal_declaration
%nterm <std::optional<SignalKind>>                          signal_declaration.signal_kind_opt
%nterm <SignalKind>                                         signal_kind
%nterm <SignalList>                                         signal_list
%nterm <std::vector<Name>>                                  signal_list.name_mul
%nterm <std::optional<Signature>>                           signature
%nterm <std::vector<Name>>                                  signature.type_mark_mul
%nterm <SimpleExpression>                                   simple_expression
%nterm <std::optional<Sign>>                                simple_expression.sign_opt
%nterm <std::vector<TermOpPair>>                            simple_expression.adding_operator_mopt
%nterm <Identifier>                                         simple_name
%nterm <std::optional<Identifier>>                          simple_name_opt
%nterm <SliceName>                                          slice_name
%nterm <SubprogramBody>                                     subprogram_body
%nterm <std::optional<SubprogramKind>>                      subprogram_body.subprogram_kind_opt
%nterm <SubprogramSpecification>                            subprogram_declaration
%nterm <SubprogramDeclarativeItem>                          subprogram_declarative_item
%nterm <std::vector<SubprogramDeclarativeItem>>             subprogram_declarative_part
%nterm <SubprogramKind>                                     subprogram_kind
%nterm <SubprogramSpecification>                            subprogram_specification
%nterm <std::optional<FunctionType>>                        function_type
%nterm <std::vector<SequentialStatement>>                   subprogram_statement_part
%nterm <SubtypeDeclaration>                                 subtype_declaration
%nterm <SubtypeIndication>                                  subtype_indication
%nterm <Suffix>                                             suffix
%nterm <Target>                                             target
%nterm <Term>                                               term
%nterm <Expression>                                         time_expression
%nterm <Expression>                                         timeout_clause
%nterm <TypeConversion>                                     type_conversion
%nterm <TypeDeclaration>                                    type_declaration
%nterm <TypeDefinition>                                     type_definition
%nterm <Name>                                               type_mark
%nterm <UnconstrainedArrayDefinition>                       unconstrained_array_definition
%nterm <UseClause>                                          use_clause
%nterm <std::vector<ExpandedSelectedName>>                  use_clause.selected_name_mul
%nterm <Expresion>                                          value_expression
%nterm <VariableAssignmentStatement>                        variable_assignment_statement
%nterm <VariableDeclaration>                                variable_declaration
%nterm <PlainVariableDeclaration>                           plain_variable_declaration
%nterm <SharedVariableDeclaration>                          shared_variable_declaration
%nterm <WaitStatement>                                      wait_statement
%nterm <std::optional<std::vector<Name>>>                   wait_statement.sensitivity_clause_opt
%nterm <std::optional<ConditionClause>>                     wait_statement.condition_clause_opt
%nterm <std::optional<Expression>>                          wait_statement.timeout_clause_opt
%nterm <Waveform>                                           waveform
%nterm <std::vector<WaveformElement>>                       waveform.waveform_element_mul
%nterm <WaveformElement>                                    waveform_element

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
    ACCESS subtype_indication { $$ = AccessTypeDefinition { $2 }; }
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
    actual_designator               { $$ = ActualPart { std::nullopt, $1 }; }
    // SEMANTIC: Name must be a function_name or a type_mark
  | name '(' actual_designator ')'  { $$ = ActualPart { $1, $3 }; }
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
  | aggregate.element_association_mul ',' element_association   { $1.push_back($3); $$ = $1; }
  ;

alias_declaration:
    ALIAS alias_designator alias_declaration.opt_subtype        
      IS name signature                                     { $$ = AliasDeclaration { $2, $3, $5, $6 }; }
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
                              { $$ = ArchitectureBody { $2, $4, $6, $8, $11 }; }
  ;
  
architecture_body.architecture_opt:
    %empty
  | ARCHITECTURE
  ;

architecture_declarative_part:
    %empty                                                { $$ = std::vector<BlockDeclarativeItem> {}; }
  | architecture_declarative_part block_declarative_item  { $1.push_back($2); $$ = $1; }
  ;

architecture_statement_part:
    %empty                                            { $$ = std::vector<ConcurrentStatement> {}; }
  | architecture_statement_part concurrent_statement  { $1.push_back($2); $$ = $1; }
  ;

array_type_definition:
    unconstrained_array_definition
  | constrained_array_definition
  ;

assertion:
    ASSERT condition assertion.report_opt assertion.severity_opt
                                              { $$ = Assertion { $2, $3, $4 }; }
  ;

assertion.report_opt:
    %empty              { $$ = nullptr; }
  | REPORT expression   { $$ = $2; }
  ;

assertion.severity_opt:
    %empty              { $$ = nullptr; }
  | SEVERITY expression { $$ = $2; }
  ;

assertion_statement:
    label_colon_opt assertion ';' { $$ = AssertionStatement { $1, $2 }; }
  ;

association_element:
    formal_part "=>" actual_part  { $$ = AssociationElement { $1, $3 }; }
  | actual_part                   { $$ = AssociationElement { nullptr, $1 }; }
  ;

association_list:
    association_element                       { $$ = std::vector<AssociationElement> { $1 }; }
  | association_list ',' association_element  { $1.push_back($3); $$ = $1; }
  ;

attribute_declaration:
    ATTRIBUTE identifier ':' type_mark ';'    { $$ = AttributeDeclaration { $2, $4 }; }
  ;

attribute_designator:
    // SEMANTIC: "simple_name" must be an ATTRIBUTE name
    simple_name
  ;

attribute_name:
    prefix signature '\'' attribute_designator attribute_name.expression_opt 
              { $$ = AttributeName { $1, $2, $4, $5 }; }
  ;

attribute_name.expression_opt:
    %empty              { $$ = nullptr; }
  | '(' expression ')'  { $$ = $2; }
  ;

attribute_specification:
    ATTRIBUTE attribute_designator OF entity_specification IS expression ';'
              { $$ = AttributeSpecification { $2, $4, $6 }; }
  ;

binding_indication:
    binding_indication.entity_aspect_opt
    generic_map_aspect_opt
    port_map_aspect_opt
              { $$ = BindingIndication { $1, $2, $3 }; }
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
              { $$ = BlockConfiguration { $2, $3, $4 }; }
  ;

block_configuration.use_clause_mopt:
    %empty                                          { $$ = std::vector<UseClause> {}; }
  | block_configuration.use_clause_mopt use_clause  { $1.push_back($2); $$ = $1; }
  ;

block_configuration.configuration_item_mopt:
    %empty                                                          { $$ = std::vector<ConfigurationItem> {}; }
  | block_configuration.configuration_item_mopt configuration_item  { $1.push_back($2); $$ = $1; }
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
    %empty                                          { $$ = std::vector<BlockDeclarativeItem> {}; }
  | block_declarative_part block_declarative_item   { $1.push_back($2); $$ = $1; }
  ;

block_header:
    %empty                                                { $$ = BlockHeader { std::nullopt, std::nullopt }; }
  | block_header.generic_clause                           { $$ = BlockHeader { $1, std::nullopt }; }
  | block_header.port_clause                              { $$ = BlockHeader { std::nullopt, $1 }; }
  | block_header.generic_clause block_header.port_clause  { $$ = BlockHeader { $1, $2 }; }
  ;

block_header.generic_clause:
    generic_clause                          { $$ = BlockHeaderGenericClause { $1, nullptr }; }
  | generic_clause generic_map_aspect ';'   { $$ = BlockHeaderGenericClause { $1, $2 }; }
  ;

block_header.port_clause:
    port_clause                       { $$ = BlockHeaderPortClause { $1, nullptr }; }
  | port_clause port_map_aspect ';'   { $$ = BlockHeaderPortClause { $1, $2 }; }
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
    name                                { $$ = BlockSpecification { $1, std::nullopt }; }
    // Label must be a generate_statement_label
  | label '(' index_specification ')'   { $$ = BlockSpecification { $1, $3 }; }
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
                  { $$ = BlockStatement { $1, $4, $6, $7, $9, $12 }; }
  ;

block_statement.guard_expression_opt:
    %empty              { $$ = nullopt; }
  // SEMANTIC: "expression" must be a GUARD (boolean) expression
  | '(' expression ')'  { $$ = $2; }
  ;

block_statement_part:
    %empty                                      { $$ = std::vector<ConcurrentStatement> {}; }
  | block_statement_part concurrent_statement   { $1.push_back($2); $$ = $1; }
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
                { $$ = CaseStatement { $1, $3, $5, $8 }; }
  ;

case_statement.case_statement_alternative_mul:
    case_statement_alternative                                                { $$ = std::vector<CaseAlternative> { $1 }; }
  | case_statement.case_statement_alternative_mul case_statement_alternative  { $1.push_back($2); $$ = $1; }
  ;

case_statement_alternative:
    WHEN choices "=>" sequence_of_statements
                { $$ = CaseAlternative { $2, $4 }; }
  ;

choice:
    simple_expression   { $$ = std::make_unique<SimpleExpression>(std::move($1)); }
  | discrete_range
  // SEMANTIC: "simple_name" must be an ELEMENT name
  // "simple_name" can be parsed as a simple_expression
  /* | simple_name */
  | OTHERS              { $$ = Keywords::Others; }
  ;

choices:
    choice              { $$ = std::vector<Choice> { $1 }; }
  | choices '|' choice  { $1.push_back($3); $$ = $1; }
  ;

component_configuration:
    FOR component_specification
      component_configuration.binding_indication_opt
      component_configuration.block_configuration_opt
    END FOR ';'
              { $$ = ComponentConfiguration { $2, $3, $4 }; }
  ;

component_configuration.binding_indication_opt:
    %empty                  { $$ = std::nullopt; }
  | binding_indication ';'
  ;

component_configuration.block_configuration_opt:
    %empty                { $$ = std::nullopt; }
  | block_configuration
  ;

component_declaration:
    COMPONENT identifier is_opt
      component_declaration.local_generic_clause_opt
      component_declaration.local_port_clause_opt
    // SEMANTIC: "simple_name_opt" must be a COMPONENT name
    // SEMANTIC: "simple_name_opt" == "identifier"
    END COMPONENT simple_name_opt ';'
              { $$ = ComponentDeclaration { $2, $4, $5, $8 }; }
  ;

component_declaration.local_generic_clause_opt:
    %empty          { $$ = std::nullopt; }
    // SEMANTIC: "generic_clause" must be a LOCAL generic clause
  | generic_clause
  ;

component_declaration.local_port_clause_opt:
    %empty        { $$ = std::nullopt; }
    // SEMANTIC: "port_clause" must be a LOCAL port clause
  | port_clause
  ;

component_instantiation_statement:
    label ':'
      instantiated_unit
        generic_map_aspect_opt
        port_map_aspect_opt ';'
                { $$ = ComponentInstantiation { $1, $3, $4, $5 }; }
  ;

component_specification:
    // SEMANTIC: "name" must be a COMPONENT name
    instantiation_list ':' name   { $$ = ComponentSpecification { $1, $3 }; }
  ;

composite_type_definition:
    array_type_definition
  | record_type_definition
  ;

concurrent_assertion_statement:
    // FIXME: Correct initialization
    label_colon_opt postponed_opt assertion ';' { $$ = ConcurrentAssertion { std::move($3), $1, $2 }; }
  ;

label_colon_opt:
    %empty      { $$ = std::nullopt; }
  | label ':'
  ;

postponed_opt:
    %empty      { $$ = false; }
  | POSTPONED   { $$ = true; }
  ;

concurrent_procedure_call_statement:
    label_colon_opt postponed_opt procedure_call ';'
            { $$ = ConcurrentProcedureCall { std::move($3) , $1, $2 }; }
  ;

concurrent_signal_assignment_statement:
    label_colon_opt postponed_opt conditional_signal_assignment
            { $$ = ConcurrentConditionalSignalAssignment { std::move($3), $1, $2 }; }
  | label_colon_opt postponed_opt selected_signal_assignment
            { $$ = ConcurrentSelectedSignalAssignment { std::move($3), $1, $2 }; }
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
    UNTIL condition   { $$ = ConditionClause { $2 }; }
  ;

conditional_signal_assignment:
    target "<=" options conditional_waveforms ';'
              { $$ = ConditionalSignalAssignment { $1, $3, $4 }; }
  ;

conditional_waveforms:
    conditional_waveforms.waveform_condition_mopt
      waveform conditional_waveforms.when_condition_opt
              { $$ = ConditionalWaveforms { $1, $2, $3 }; }
  ;

conditional_waveforms.waveform_condition_mopt:
    %empty
              { $$ = std::vector<ConditionalWaveform> {}; }
  | conditional_waveforms.waveform_condition_mopt waveform WHEN condition ELSE
              { $1.emplace_back($2, $4); $$ = $1; }
  ;

conditional_waveforms.when_condition_opt:
    %empty          { $$ = std::nulptr; }
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
                { $$ = ConfigurationDeclaration { $2, $4, $6, $7, $10 }; }
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
                { $$ = std::vector<ConfigurationDeclarativeItem> {}; }
  | configuration_declarative_part configuration_declarative_item
                { $1.push_back($2); $$ = $1; }
  ;

configuration_item:
    block_configuration
  | component_configuration
  ;

configuration_specification:
    FOR component_specification binding_indication ';'
                { $$ = ConfigurationSpecification { $2, $3 }; }
  ;

constant_declaration:
    CONSTANT identifier_list ':'
      subtype_indication constant_declaration.expression_opt ';'
                { $$ = ConstantDeclaration { $2, $4, $5 }; }
  ;

constant_declaration.expression_opt:
    %empty            { $$ = nullopt; }
  | ":=" expression   { $$ = $2; }
  ;

constrained_array_definition:
    // SEMANTIC: "subtype_indication" is an ELEMENT subtype indication
    ARRAY index_constraint OF subtype_indication
                { $$ = ConstrainedArrayDefinition { $2, $4 }; }
  ;

constraint:
    range_constraint
  | index_constraint
  ;

constraint_opt:
    %empty      { $$ = std::nullopt; }
  | constraint
  ;

context_clause:
    %empty                        { $$ = std::vector<ContexItem> {}; }
  | context_clause context_item   { $1.push_back($2); $$ = $1; }
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
    design_unit               { $$ = std::vector<DesignUnit> { std::move($1) }; }
  | design_file design_unit   { $1.push_back($2); $$ = $1; }
  ;

design_unit:
    context_clause library_unit   { $$ = DesignUnit { $1, $2 }; }
  ;

designator:
    identifier
  | operator_symbol
  ;

designator_opt:
    %empty      { $$ = std::nullopt; }
  | designator
  ;

direction:
    TO        { $$ = Direction::to; }
  | DOWNTO    { $$ = Direction::downto; }
  ;

disconnection_specification:
    DISCONNECT guarded_signal_specification AFTER time_expression ';'
              { $$ = DisconnectionSpecification { $2, $4 }; }
  ;

discrete_range:
    // SEMANTIC: "subtype_indication" must be a DISCRETE subtype
    subtype_indication
  | range
  ;

element_association:
    expression
  | choices "=>" expression   { $$ = ChoicesExpression { $1, $3 }; }
  ;

element_declaration:
    identifier_list ':' element_subtype_definition ';'
              { $$ = ElementDeclaration { $1, $3 }; }
  ;

element_subtype_definition:
    subtype_indication
  ;

entity_aspect:
    // SEMANTIC: "name" must be an ENTITY name
    ENTITY name                       { $$ = EntityName { $2, std::nullopt }; }
    // SEMANTIC: "name" must be an ENTITY name
    // SEMANTIC: "identifier" must be an architecture identifier
  | ENTITY name '(' identifier ')'    { $$ = EntityName { $2, $4 }; }
    // SEMANTIC: "name" must be a CONFIGURATION name
  | CONFIGURATION name                { $$ = ConfigurationName { $2 }; }
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
    entity_class        { $$ = EntityClassEntry { $1, false }; }
  | entity_class "<>"   { $$ = EntityClassEntry { $1, true }; }
  ;

entity_class_entry_list:
    entity_class_entry
                  { $$ = std::vector<EntityClassEntry> { std::move($1) }; }
  | entity_class_entry_list ',' entity_class_entry
                  { $1.push_back($3); $$ = $1; }
  ;


entity_declaration:
    ENTITY identifier IS
      entity_header
      entity_declarative_part
    entity_declaration.entity_statement_part_opt
    // SEMANTIC: "simple_name_opt" must be an ENTITY name
    // SEMANTIC: "simple_name_opt" == "identifier"
    END entity_declaration.entity_opt simple_name_opt ';'
                { $$ = EntityDeclaration { $2, $4, $5, $6, $9 }; }
  ;

entity_declaration.entity_statement_part_opt:
    %empty                    { $$ = std::vector<EntityStatement> {}; }
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
                { $$ = std::vector<EntityDeclarativeItem> {}; }
  | entity_declarative_part entity_declarative_item
                { $1.push_back($2); $$ = $1; }
  ;

entity_designator:
    entity_tag            { $$ = EntityDesginator { $1, std::nullopt }; }
  | entity_tag signature  { $$ = EntityDesignator { $1, $2 }; }
  ;

entity_header:
    %empty                        { $$ = std::nullopt; }
    // SEMANTIC: "generic_clause" must be a FORMAL generic clause
  | generic_clause                { $$ = EntityHeader { $1, nullptr }; }
    // SEMANTIC: "port_clause" must be a FORMAL port clause
  | port_clause                   { $$ = EntityHeader { nullptr, $1 }; }
    // SEMANTIC: "generic_clause" must be a FORMAL generic clause
    // SEMANTIC: "port_clause" must be a FORMAL port clause
  | generic_clause port_clause    { $$ = EntityHeader { $1, $2 }; }
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
                { $1.push_back($3); $$ = $1; }
  ;

entity_specification:
    entity_name_list ':' entity_class   { $$ = EntitySpecification { $1, $3 }; }
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
    %empty                                  { $$ = std::vector<EntityStatement> {}; }
  | entity_statement_part entity_statement  { $1.push_back($2); $$ = $1; }
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
                { $1.push_back($3); $$ = $1; }
  ;

exit_statement:
    // SEMANTIC: "label_opt" must be a LOOP label
    label_colon_opt EXIT label_opt exit_statement.when_condition_opt ';'
                { $$ = ExitStatement { $1, $3, $4 }; }
  ;

exit_statement.when_condition_opt:
    %empty          { $$ = nullptr; }
  | WHEN condition  { $$ = $2; }
  ;

expanded_selected_name:
    simple_prefix '.' suffix  { $$ = ExpandedSelectedName { $1, $3 }; }
  ;

simple_prefix:
    expanded_selected_name  { $$ = std::make_unique<ExpandedSelectedName>(std::move($1)); }
  | simple_name
  ;

expression:
    relation                                { $$ = Expression { std::vector<Relation> { $1 }, RelationKind::AND }; }
  | relation expression.and_relation_mul    { $2[0] = $1; $$ = Expression { $2, RelationKind::AND }; }
  | relation expression.or_relation_mul     { $2[0] = $1; $$ = Expression { $2, RelationKind::OR }; }
  | relation expression.xor_relation_mul    { $2[0] = $1; $$ = Expression { $2, RelationKind::XOR }; }
  | relation NAND relation                  { $$ = Expression { std::vector<Relation> { $1, $3 }, RelationKind::nand }; }
  | relation NOR relation                   { $$ = Expression { std::vector<Relation> { $1, $3 }, RelationKind::nor }; }
  | relation expression.xnor_relation_mul   { $2[0] = $1; $$ = Expression { $2, RelationKind::xnor }; }
  ;

expression.and_relation_mul:
    AND relation                              
              { auto temp { std::vector<Relation>(1) }; temp.push_back($2); $$ = std::move(temp); }
  | expression.and_relation_mul AND relation    { $1.push_back($3); $$ = $1; }
  ;

expression.or_relation_mul:
    OR relation
              { auto temp { std::vector<Relation>(1) }; temp.push_back($2); $$ = std::move(temp); }
  | expression.or_relation_mul OR relation      {$1.push_back($3); $$ = $1; }
  ;

expression.xor_relation_mul:
    XOR relation
              { auto temp { std::vector<Relation>(1) }; temp.push_back($2); $$ = std::move(temp); }
  | expression.xor_relation_mul XOR relation    { $1.push_back($3); $$ = $1; }
  ;

expression.xnor_relation_mul:
    XNOR relation
              { auto temp { std::vector<Relation>(1) }; temp.push_back($2); $$ = std::move(temp); }
  | expression.xnor_relation_mul XNOR relation  { $1.push_back($3); $$ = $1; }
  ;

factor:
    primary               { $$ = Factor { $1, std::nullopt }; }
  | primary "**" primary  { $$ = Factor { $1, $3, FactorKind::exp }; }
  | ABS primary           { $$ = Factor { $2, std::nullopt, FactorKind:abd }; }
  | NOT primary           { $$ = Factor { $2, std::nullopt, FactorKind:NOT }; }
  ;

file_declaration:
    FILE identifier_list ':' subtype_indication ';'
                { $$ = FileDeclaration { $2, $4 }; }
  | FILE identifier_list ':' subtype_indication file_open_information ';'
                { $$ = FileDeclaration { $2, $4, $5 }; }
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
                { $$ = FileOpenInformation { $2 }; }
  | OPEN file_open_kind_expression IS file_logical_name
                { $$ = FileOpenInformation { $4, $2 }; }
  ;

file_type_definition:
    FILE OF type_mark   { $$ = FileTypeDefinition { $3 }; }
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
    formal_designator                 { $$ = FormalPart { $1 }; }
    // Name must be function_name or a type_mark
  | name '(' formal_designator ')'    { $$ = FormalPart { $3, $1 }; }
  ;

full_type_declaration:
    TYPE identifier IS type_definition ';'  { $$ = TypeDeclaration { $2, $4 }; }
  ;

function_call:
    // SEMANTIC: "name" must be a function_name
    name                            { $$ = FunctionCall { $1 }; }
    // SEMANTIC: "association_list" is a PARAMETER association list
    // SEMANTIC: "name" must be a function name
  | name '(' association_list ')'   { $$ = FunctionCall { $1, $3 }; }
  ;

generate_statement:
    label ':'
      generation_scheme GENERATE
        generate_statement.block_declarative_statement_opt
        generate_statement.concurrent_statement_mopt
      // SEMANTIC: "label_opt" must be a GENERATE label
      // SEMANTIC: "label_opt" == "label"
      END GENERATE label_opt ';'
              { $$ = GenerateStatement { $1, $3, $5, $6, $9 }; }
  ;

generate_statement.block_declarative_statement_opt:
    %empty
              { $$ = std::vector<BlockDeclarativeItem> {}; }
  | generate_statement.block_declarative_statement_opt.block_declarative_item_mopt BEGIN
  ;

generate_statement.block_declarative_statement_opt.block_declarative_item_mopt:
    %empty
              { $$ = std::vector<BlockDeclarativeItem> {}; }
  | generate_statement.block_declarative_statement_opt.block_declarative_item_mopt block_declarative_item
              { $1.push_back($2); $$ = $1; }
  ;

generate_statement.concurrent_statement_mopt:
    %empty
              { $$ = std::vector<ConcurrentStatement> {}; }
  | generate_statement.concurrent_statement_mopt concurrent_statement
              { $1.push_back($2); $$ = $1; }
  ;

generation_scheme:
    // SEMANTIC: "parameter_specification" must be a GENERATION parameter specification
    FOR parameter_specification   { $$ = $2; }
  | IF condition                  { $$ = std::make_unique<Expression> { std::move($2) }; }
  ;

generic_clause:
    GENERIC '(' interface_constant_declaration_list ')' ';'
              { $$ = GenericClause { $3 }; }
  ;

generic_map_aspect:
    // SEMANTIC: "association_list" is a GENERIC association list
    GENERIC MAP '(' association_list ')'  { $$ = $4; }
  ;

generic_map_aspect_opt:
    %empty              { $$ = std::nullopt; }
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
              { $1.push_back($3); $$ = $1; }
  ;

group_declaration:
    // SEMANTIC: "name" must be a GROUP TEMPLATE name
    GROUP identifier ':' name '(' group_constituent_list ')' ';'
              { $$ = GroupDeclaration { $2, $4, $6 }; }
  ;

group_template_declaration:
    GROUP identifier IS '(' entity_class_entry_list ')' ';'
              { $$ = GroupTemplateDeclaration { $2, $5 }; }
  ;

guarded_signal_specification:
    // SEMANTIC: "signal_list" must be a list of GUARDD signals + locally static
    signal_list ':' type_mark   { $$ = GuardedSignalSpecification { $1, $3 }; }
  ;

identifier:
    BASIC_IDENTIFIER
  | EXTENDED_IDENTIFIER
  ;

identifier_list:
    identifier                      { $$ = std::vector<Identifier> { $1 }; }
  | identifier_list ',' identifier  { $1.push_back($3); $$ = $1; }
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
              { $6[0] =  std::make_pair($3, $5); $$ = IfStatement { $1, $6, $7, $10 }; }
  ;

if_statement.elsif_condition_mopt:
    %empty
              { $$ = std::vector<IfStatementPair>(1); }
  | if_statement.elsif_condition_mopt ELSIF condition THEN sequence_of_statements
              { $1.push_back({ $3, $5 }); $$ = $1; }
  ;

if_statement.else_opt:
    %empty                        { $$ = std::vector<SequentialStatement> {}; }
  | ELSE sequence_of_statements   { $$ = $2; }
  ;

incomplete_type_declaration:
    TYPE identifier   { $$ = TypeDeclaration { $2 }; }
  ;

index_constraint:
    '(' index_constraint.discrete_range_mul ')'   { $$ = $2; }
  ;

index_constraint.discrete_range_mul:
    discrete_range
              { $$ = std::vector<DiscreteRange> { $1 }; }
  | index_constraint.discrete_range_mul ',' discrete_range
              { $1.push_back($3); $$ = $1; }
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
              { $1.push_back($3); $$ = $1; }
  ;

indexed_name:
    prefix '(' indexed_name.expression_mul ')'
              { $$ = IndexedName { $1, $3 }}
  ;

indexed_name.expression_mul:
    expression
              { $$ = std::vector<Expression> { $1 }; }
  | indexed_name.expression_mul ',' expression
              { $1.push_back($3); $$ = $1; }
  ;

init_expression_opt:
    %empty            { std::nullopt; }
  | ":=" expression   { $$ = $2; }
  ;

instantiated_unit:
    // SEMANTIC: "name" must be a COMPONENT name
    name                              { $$ = ComponentUnit { $1 }; }
    // SEMANTIC: "name" must be a COMPONENT name
  | COMPONENT name                    { $$ = ComponentUnit { $2 }; }
    // SEMANTIC: "name" must be an ENTITY name
  | ENTITY name                       { $$ = EntityName { $2 }; }
    // SEMANTIC: "name" must be an ENTITY name
    // SEMANTIC: "identifier" must be an ARCHITECTURE identifier
  | ENTITY name '(' identifier ')'    { $$ = EntityName { $2, $4 }; }
    // SEMANTIC: "name" must be a CONFIGURATION name
  | CONFIGURATION name                { $$ = ConfigurationName { $2 }; }
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
              { $1.push_back($3); $$ = $1; }
  ;

interface_constant_declaration:
    CONSTANT identifier_list ':'
      interface_constant_declaration.in_opt subtype_indication
        // SEMANTIC: "init_expression_opt" must be a STATIC expression
        init_expression_opt
              { $$ = InterfaceConstantDeclaration { $2, $5, $6 }; }
  ;

interface_constant_declaration_list:
    interface_constant_declaration
              { $$ = std::vector<InterfaceConstantDeclaration> { $1 }; }
  | interface_constant_declaration_list ';' interface_constant_declaration
              { $1.push_back($3); $$ = $1; }
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
              { $$ = InterfaceFileDeclaration { $2, $4 }; }
  ;

interface_list:
    interface_element
              { $$ = std::vector<InterfaceDeclaration> { $1 }; }
  | interface_list ';' interface_element
              { $1.push_back($3); $$ = $1; }
  ;

interface_signal_declaration:
    // SEMANTIC: "init_expression_opt" must be a STATIC expression
    SIGNAL identifier_list ':' mode_opt subtype_indication bus_opt init_expression_opt
              { $$ = InterfaceSignalDeclaration { $2, $4, $5, $6, $7 }; }
  ;

// SEMANTIC: Can be signal/constant/variable, must be decoded from context?
// TODO: Is this correct?
interface_unresolved_declaration:
    identifier_list ':' mode_opt subtype_indication bus_opt init_expression_opt
              { $$ = InterfaceUnresolvedDeclaration { $1, $3, $5, $5, $6 }; }
  ;

interface_variable_declaration:
    // SEMANTIC: "init_expression_opt" must be a STATIC expression
    VARIABLE identifier_list ':' mode_opt subtype_indication init_expression_opt
              { $$ = InterfaceVariableDeclaration { $2, $4, $5, $6 }; }
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
    %empty    { $$ = std::nullopt; }
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
              { $1.push_back($3); $$ = $1; }
  ;

loop_statement:
    label_colon_opt
      loop_statement.iteration_scheme_opt LOOP
        sequence_of_statements
      // SEMANTIC: "label_opt" must be a LOOP label
      // SEMANTIC: "label_opt" == "label_colon_opt"
      END LOOP label_opt ';'
              { $$ = LoopStatement { $1, $2, $4, $7 }; }
  ;

loop_statement.iteration_scheme_opt:
    %empty            { std::nullopt; }
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
    %empty    { $$ = std::nullopt; }
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
              { $$ = NextStatement { $1, $3, $4 }; }
  ;

next_statement.when_opt:
    %empty          { $$ = std::nullopt; }
  | WHEN condition  { $$ = $2; }
  ;

null_statement:
    label_colon_opt ID_NULL ';'   { $$ = NullStatement { $1 }; }
  ;

numeric_literal:
    abstract_literal
  | physical_literal
  ;

options:
    options.guarded_opt options.delay_mechanism_opt
              { $$ = Options { $1, $2 }; }
  ;

options.guarded_opt:
    %empty    { $$ = false; }
  | GUARDED   { $$ = true; }
  ;

options.delay_mechanism_opt:
    %empty            { std::nullopt; }
  | delay_mechanism
  ;

package_body:
    PACKAGE BODY simple_name IS
      package_body_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PACKAGE name
    // SEMANTIC: "simple_nam_opt" == "simple_name"
    END package_body.package_body_opt simple_name_opt ';'
            { $$ = PackageBody { $3, $5, $8 }; }
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
              { $$ = std::vector<PackageBodyDeclarativeItem> {}; }
  | package_body_declarative_part package_body_declarative_item
              { $1.push_back($2), $$ = $1; }
  ;

package_declaration:
    PACKAGE identifier IS
      package_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PACKAGE name
    // SEMANTIC: "simple_name_opt" == "identifier"
    END package.package_opt simple_name_opt ';'
              { $$ = PackageDeclaration { $2, $4, $7 }; }
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
            { $$ = std::vector<PackageDeclarativeItem> {}; }
  | package_declarative_part package_declarative_item
            { $1.push_back($2); $$ = $1; }
  ;

parameter_specification:
    identifier IN discrete_range  { $$ = ParameterSpecification { $1, $3 }; }
  ;

physical_literal:
    // SEMANTIC: "name" must be a PHYSICAL UNIT name
    name                    { $$ = PhysicalLiteral { $1 }; }
    // SEMANTIC: "name" must be a PHYSICAL UNIT name
  | abstract_literal name   { $$ = PhysicalLiteral { $2, $1 }; }
  ;

physical_type_definition:
    range_constraint
      UNITS
        primary_unit_declaration
        physical_type_definition.secondary_unit_declaration_mopt
      // SEMANTIC: "simple_name_opt" must be a PHYSICAL TYPE name
      // SEMANTIC: "simple_name_opt" should be equal to the type name it's enclosing
      END UNITS simple_name_opt
              { $$ = PhysicalTypeDefinition { $1, $3, $4, $7 }; }
  ;

physical_type_definition.secondary_unit_declaration_mopt:
    %empty
              { $$ = std::vector<SecondaryUnitDeclaration> {}; }
  | physical_type_definition.secondary_unit_declaration_mopt secondary_unit_declaration
              { $1.push_back($2); $$ = $1; }
  ;

port_clause:
    PORT '(' port_list ')' ';'    { $$ = PortClause { $3 }; }
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
              { $1.push_back($3); $$ = $1; }
  ;

port_list:
    port_interface_list
  ;

port_map_aspect:
    // SEMANTIC: "association_list" is a PORT association list
    PORT MAP '(' association_list ')'   { $$ = PortMapAspect { $4 }; }
  ;

port_map_aspect_opt:
    %empty            { $$ = std::nullopt; }
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
    name                            { $$ = ProcedureCall { $1 }; }
    // SEMANTIC: "association_list" is a PARAMETER association list
    // SEMANTIC: "name" must be a procedure name
  | name '(' association_list ')'   { $$ = ProcedureCall { $1, $3 }; }
  ;

procedure_call_statement:
    label_colon_opt procedure_call ';'  { $$ = ProcedureCallStatement { $2, $1 }; }
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
              { $1.push_back($2); $$ = $1; }
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
              { $$ = ProcessStatement { $1, $2, $4, $6, $8, $10, $12 }; }
  ;

process_statement.postponed_opt:
    %empty      { $$ = false; }
  | POSTPONED   { $$ = true; }
  ;

process_statement.sensitivity_list_opt:
    %empty                    { $$ = std::nullopt; }
  | '(' sensitivity_list ')'  { $$ = $2; }
  ;

process_statement_part:
    %empty
            { $$ = std::vector<SequentialStatement> {}; }
  | process_statement_part sequential_statement
            { $1.push_back($2); $$ = $1; }
  ;

protected_type_body:
    PROTECTED BODY
      protected_type_body_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PROTECTED TYPE name
    END PROTECTED BODY simple_name_opt
            { $$ = ProtectedTypeBody { $3, $7 }; }
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
              { $$ = std::vector<ProtectedTypeBodyDeclarativeItem> {}; }
  | protected_type_body_declarative_part protected_type_body_declarative_item
              { $1.push_back($2); $$ = $1; }
  ;

protected_type_declaration:
    PROTECTED
      protected_type_declarative_part
    // SEMANTIC: "simple_name_opt" must be a PROTECTED TYPE name
    // SEMANTIC: "simple_name_opt" should be equal to the name in enclosing type definition
    END PROTECTED simple_name_opt
              { $$ = ProtectedTypeDeclaration { $2, $5 }; }
  ;

protected_type_declarative_item:
    subprogram_specification
  | attribute_specification
  | use_clause
  ;

protected_type_declarative_part:
    %empty
              { $$ = std::vector<ProtectedTypeDeclarativeItem> {}; }
  | protected_type_declarative_part protected_type_declarative_item
              { $1.push_back($2); $$ = $1; }
  ;

protected_type_definition:
    protected_type_declaration
  | protected_type_body
  ;

qualified_expression:
    type_mark '\'' '(' expression ')'   { $$ = QualifiedExpression { $1, $4, std::nullopt }; }
  | type_mark '\'' aggregate            { $$ = QualifiedExpression { $1, std::nullopt, $3 }; }
  ;

range:
    // SEMANTIC: "attribute_name" must be a RANGE attribute name
    attribute_name
  | simple_expression direction simple_expression
              { $$ = SimpleRange { $1, $2, $3 }; }
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
              { $$ = RecordTypeDefinition { $2, $5 }; }
  ;

record_type_definition.element_declaration_mul:
    element_declaration
              { $$ = std::vector<ElementDeclaration> { $1 }; }
  | record_type_definition.element_declaration_mul element_declaration
              { $1.push_back($2); $$ = $1; }
  ;

relation:
    shift_expression
              { $$ = Relation { $1 }; }
  | shift_expression relational_operator shift_expression
              { $$ = Relation { $1, $2, $3 }; }
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
              { $$ = ReportStatement { $1, $3 }; }
  | label_colon_opt REPORT expression
      SEVERITY expression ';'
              { $$ = ReportStatement { $1, $3, $5 }; }
  ;

// FIXME: Check if this is correct
resolution_function_name_opt:
    // SEMANTIC: "designator_opt" must be a RESOLUTION FUNCTION name
    designator_opt
  ;

return_statement:
    label_colon_opt RETURN ';'
              { $$ = ReturnStatement { $1 }; }
  | label_colon_opt RETURN expression ';'
              { $$ = ReturnStatement { $1, $3 }; }
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
    identifier '=' physical_literal   { $$ = SecondaryUnitDeclaration { $1, $3 }; }
  ;

selected_name:
    prefix '.' suffix   { $$ = SelectedName { $1, $3 }; }
  ;

selected_signal_assignment:
    WITH expression SELECT
      target "<=" options selected_waveforms ';'
              { $$ = SelectedSignalAssignment { $2, $4, $6, $7 }; }
  ;

// TODO: Check if correct
selected_waveforms:
    waveform WHEN choices
              { $$ = std::vector<SelectedWaveform> { SelectedWaveform { $1, $3 } }; }
  | selected_waveforms ',' waveform WHEN choices
              { $1.emplace_back<SelectedWaveform>($3, $5); $$ = $1; }
  ;

sensitivity_clause:
    ON sensitivity_list   { $$ = $2; }
  ;

sensitivity_list:
    // SEMANTIC: "name" must be a SIGNAL name
    name
              { $$ = std::vector<Name> { $1 }; }
  | sensitivity_list ',' name
              { $1.push_back($3); $$ = $1; }
  ;

sequence_of_statements:
    %empty
              { $$ = std::vector<SequentialStatement> {}; }
  | sequence_of_statements sequential_statement
              { $1.push_back($2); $$ = $1; }
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
              { $$ = ShiftExpression { $1 }; }
  | simple_expression shift_operator simple_expression
              { $$ = ShiftExpression { $1, $2, $3 }; }
  ;

shift_operator:
    SLL   { $$ = ShiftOp::sll; }
  | SRL   { $$ = ShiftOp::srl; }
  | SLA   { $$ = ShiftOp::sla; }
  | SRA   { $$ = ShiftOp::sra; }
  | ROL   { $$ = ShiftOp::rol; }
  | ROR   { $$ = ShiftOp::ror; }
  ;

sign:
    '+'   { $$ = Sign::plus; }
  | '-'   { $$ = Sign::minus; }
  ;

signal_assignment_statement:
    label_colon_opt target "<=" signal_assignment_statement.delay_mechanism_opt waveform ';'
              { $$ = SignalAssignmentStatement { $1, $2, $4, $5 }; }
  ;

signal_assignment_statement.delay_mechanism_opt:
    %empty            { $$ = std::nullopt; }
  | delay_mechanism
  ;

signal_declaration:
    SIGNAL identifier_list ':' subtype_indication signal_declaration.signal_kind_opt init_expression_opt ';'
              { $$ = SignalDeclaration { $2, $4, $5, $6 }; }
  ;

signal_declaration.signal_kind_opt:
    %empty        { $$ = std::nullopt; }
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
                { $1.push_back($3); $$ = $1; }
  ;

signature:
    %empty                                            { $$ = std::nullopt; }
  | '[' signature.type_mark_mul ']'                   { $$ = Signature { $2, std::nullopt }; }
  | '[' RETURN type_mark ']'                          { $$ = Signature { std::nullopt, $3 }; }
  | '[' signature.type_mark_mul RETURN type_mark ']'  { $$ = Signature { $2, $4 }; }
  ;

signature.type_mark_mul:
    type_mark
              { $$ = std::vector<Name> { $1 }; }
  | signature.type_mark_mul ',' type_mark
              { $1.push_back($3); $$ = $1; }
  ;

simple_expression:
    simple_expression.sign_opt term simple_expression.adding_operator_mopt
              { $$ = SimpleExpression { $1, $2, $3 }; }
  ;

simple_expression.sign_opt:
    %empty    { $$ = std::nullopt; }
  | sign
  ;

simple_expression.adding_operator_mopt:
    %empty
              { $$ = std::vector<TermOpPair> {}; }
  | simple_expression.adding_operator_mopt adding_operator term
              { $1.push_back(std::make_pair($2, $3)); $$ = $1; }
  ;

simple_name:
    identifier
  ;

simple_name_opt:
    %empty        { $$ = std::nullopt; }
  | simple_name
  ;

slice_name:
    prefix '(' discrete_range ')'   { $$ = SliceName { $1, $3 }; }
  ;

subprogram_body:
    subprogram_specification IS
      subprogram_declarative_part
    BEGIN
      subprogram_statement_part
    END subprogram_body.subprogram_kind_opt designator_opt ';'
              { $$ = SubprogramBody { $1, $3, $5, $7, $8 }; }
  ;

subprogram_body.subprogram_kind_opt:
    %empty            { $$ = std::nullopt; }
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
              { $$ = std::vector<SubprogramDeclarativeItem> {}; }
  | subprogram_declarative_part subprogram_declarative_item
              { $1.push_back($2); $$ = $1; }
  ;

subprogram_kind:
    PROCEDURE   { $$ = SubprogramKind::procedure; }
  | FUNCTION    { $$ = SubprogramKind::function; }
  ;

subprogram_specification:
    PROCEDURE designator
            { $$ = ProcedureSpecification { $2 }; }
  | PROCEDURE designator '(' formal_parameter_list ')'
            { $$ = ProcedureSpecification { $2, $4 }; }
  | function_type FUNCTION designator RETURN type_mark
            { $$ = FunctionSpecification { $1, $3, std::nullopt, $5 }; }
  | function_type FUNCTION designator '(' formal_parameter_list ')' RETURN type_mark
            { $$ = FunctionSpecification { $1, $3, $5, $8 }; }
  ;

function_type:
    %empty    { $$ = std::nullopt; }
  | IMPURE    { $$ = FunctionType::impure; }
  | PURE      { $$ = FunctionType::pure; }
  ;

subprogram_statement_part:
    %empty
              { $$ = std::vector<SequentialStatement> {}; }
  | subprogram_statement_part sequential_statement
              { $1.push_back($2); $$ = $1; }
  ;

subtype_declaration:
    SUBTYPE identifier IS subtype_indication ';'
              { $$ = SubtypeDeclaration { $2, $4 }; }
  ;

subtype_indication:
    resolution_function_name_opt type_mark constraint_opt
              { $$ = SubtypeIndication { $1, $2, $3 }; }
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
    factor                            { $$ = Term { nullptr, std::nullopt, $1 }; }
  | term multiplying_operator factor  { $$ = Term { $1, $2, $3 }; }
  ;

time_expression:
    // SEMANTIC: "expression" must be a TIME expression
    expression
  ;

timeout_clause:
    FOR time_expression   { $$ = $2; }
  ;

type_conversion:
    type_mark '(' expression ')'  { $$ = TypeConversion { $1, $3 }; }
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
            { $$ = UnconstrainedArrayDefinition { $3, $6 }; }
  ;

use_clause:
    USE use_clause.selected_name_mul ';'
            { $$ = UseClause { $2 }; }
  ;

use_clause.selected_name_mul:
    expanded_selected_name
            { $$ = std::vector<ExpandedSelectedName> { $1 }; }
  | use_clause.selected_name_mul ',' expanded_selected_name
            { $1.push_back($3); $$ = $1; }
  ;

value_expression:
    // SEMANTIC: "expression" must be a VALUE expression
    expression
  ;

variable_assignment_statement:
    label_colon_opt target ":=" expression ';'
            { $$ = VariableAssignmentStatement { $1, $2, $4 }; }
  ;

variable_declaration:
    plain_variable_declaration
  | shared_variable_declaration
  ;

plain_variable_declaration:
    VARIABLE identifier_list ':' subtype_indication init_expression_opt ';'
            { $$ = PlainVariableDeclaration { $2, $4, $5 }; }
  ;

shared_variable_declaration:
    SHARED plain_variable_declaration
            { $$ = SharedVariableDeclaration { $2 }; }
  ;

wait_statement:
    label_colon_opt WAIT wait_statement.sensitivity_clause_opt wait_statement.condition_clause_opt wait_statement.timeout_clause_opt ';'
            { $$ = WaitStatement { $1, $3, $4, $5 }; }
  ;

wait_statement.sensitivity_clause_opt:
    %empty              { $$ = std::nullopt; }
  | sensitivity_clause
  ;

wait_statement.condition_clause_opt:
    %empty              { $$ = std::nullopt; }
  | condition_clause
  ;

wait_statement.timeout_clause_opt:
    %empty              { $$ = std::nullopt; }
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
                { $1.push_back($3); $$ = $1; }
  ;

waveform_element:
    value_expression
                { $$ = WaveformElementKind::ValueExpression { $1 }; }
  | value_expression AFTER time_expression
                { $$ = WaveformElementKind::ValueExpression { $1, $3 }; }
  | ID_NULL
                { $$ = WaveformElementKind::NullExpression {}; }
  | ID_NULL AFTER time_expression
                { $$ = WaveformElementKind::NullExpression { $3 }; }
  ;
%%
