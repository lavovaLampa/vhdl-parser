#ifndef PARSER_DEFS
#define PARSER_DEFS

#include "../lexer/lexer_defs.h"
#include <memory>
#include <optional>
#include <variant>
#include <vector>

struct AstNode {
};

namespace Keyword {
struct Transport;
struct Inertial;
struct Open;
struct Others;
struct Open;
struct All;
struct Null;
}

using AbstractLiteral = std::variant<
    Lexer::DecimalLiteral,
    Lexer::BasedLiteral>;

using ActualDesignator = std::variant<
    std::unique_ptr<Expression>,
    /**
     *  Name must be:
     *      - variable_name
     *      - signal_name
     *      - file_name
     */
    std::unique_ptr<Name>,
    Keyword::Open>;

struct ActualPart : AstNode {
    ActualDesignator actual_designator;
    // Name must be FUNCTION name or a TYPE mark
    std::optional<std::unique_ptr<Name>> name;
};

struct AliasDesignator : AstNode {
    std::variant<
        std::unique_ptr<Identifier>,
        std::unique_ptr<Lexer::CharacterLiteral>,
        std::unique_ptr<OperatorSymbol>>
        designator;
};

struct AliasDeclaration : AstNode {
    std::unique_ptr<AliasDesignator> alias_designator;
    std::optional<std::unique_ptr<SubtypeIndication>> subtype;
    Name name;
    std::optional<std::unique_ptr<Signature>> signature;

    /* data */
};

struct AccessTypeDefinition : AstNode {
    std::unique_ptr<SubtypeIndication> subtype;
};

using Allocator = std::variant<
    std::unique_ptr<SubtypeIndication>,
    std::unique_ptr<QualifiedExpression>>;

struct ArchitectureBody : AstNode {
    std::unique_ptr<Identifier> identifier;
    // Name must be an entity name
    std::unique_ptr<Name> name;
    std::optional<std::vector<BlockDeclarativeItem>> declarative_part;
    std::optional<std::vector<ConcurrentStatement>> statement_part;
    std::optional<std::unique_ptr<Identifier>> end_label;
};

struct ArrayTypeDefinition : AstNode {
    std::unique_ptr<SubtypeIndication> subtype;
};

struct UnconstrainedArrayDefinition : ArrayTypeDefinition {
    // Names must be type marks
    std::vector<Name> index_subtypes;
};

struct ConstrainedArrayDefinition : ArrayTypeDefinition {
    std::vector<DiscreteRange> index_constaints;
};

struct Assertion : AstNode {
    // Expression type must be "boolean"
    std::unique_ptr<Expression> condition;
    // Expression type must be "string"
    std::optional<Expression> report_expression;
    // Expression type must be "severity_level"
    std::optional<Expression> severity;
};

struct AssertionStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
    Assertion assertion;
};

struct AssociationElement : AstNode {
    std::optional<std::unique_ptr<FormalPart>> formal_part;
    std::unique_ptr<ActualPart> actual_part;
};

struct AttributeDeclaration : AstNode {
    std::unique_ptr<Identifier> identifier;
    // Must be a TYPE or a SUBTYPE name
    std::unique_ptr<Name> type_mark;
};

struct AttributeName : AstNode {
    std::unique_ptr<Prefix> prefix;
    std::optional<std::unique_ptr<Signature>> signature;
    // Must be an ATTRIBUTE name
    std::unique_ptr<Identifier> attribute_designator;
    std::optional<std::unique_ptr<Expression>> expression;
};

struct AttributeSpecification : AstNode {
    // Must be an ATTRIBUTE name
    std::unique_ptr<Identifier> attribute_designator;
    std::unique_ptr<EntitySpecification> entity_specification;
    std::unique_ptr<Expression> expression;
};

struct BindingIndication : AstNode {
    std::optional<std::unique_ptr<EntityAspect>> entity_aspect;
    std::optional<std::unique_ptr<GenericMapAspect>> generic_map_aspect;
    std::optional<std::unique_ptr<PortMapAspect>> port_map_aspect;
};

struct BlockConfiguration : AstNode {
    std::unique_ptr<BlockSpecification> block_specification;
    std::optional<std::vector<UseClause>> use_clauses;
    std::optional<std::vector<ConfigurationItem>> configuration_items;
};

using BlockDeclarativeItem = std::variant<
    // subprogram_declaration
    std::unique_ptr<SubprogramSpecification>,
    std::unique_ptr<SubprogramBody>,
    std::unique_ptr<TypeDeclaration>,
    std::unique_ptr<SubtypeDeclaration>,
    std::unique_ptr<ConstantDeclaration>,
    std::unique_ptr<SignalDeclaration>,
    std::unique_ptr<SharedVariableDeclaration>,
    std::unique_ptr<FileDeclaration>,
    std::unique_ptr<AliasDeclaration>,
    std::unique_ptr<ComponentDeclaration>,
    std::unique_ptr<AttributeDeclaration>,
    std::unique_ptr<AttributeSpecification>,
    std::unique_ptr<ConfigurationSpecification>,
    std::unique_ptr<DisconnectionSpecification>,
    std::unique_ptr<UseClause>,
    std::unique_ptr<GroupTemplateDeclaration>,
    std::unique_ptr<GroupDeclaration>>;

struct BlockHeader : AstNode {
    std::optional<std::unique_ptr<GenericClause>> generic_clause;
    std::optional<std::unique_ptr<GenericMapAspect>> generic_map_aspect;
    std::optional<std::unique_ptr<PortClause>> port_clause;
    std::optional<std::unique_ptr<PortMapAspect>> port_map_aspect;
};

// TODO: block_specification
struct BlockSpecification : AstNode {
    /**
     *  Name must be:
     *      - architecture name
     *      - block_statement_label
     *      - generate_statement_label
     */
    std::unique_ptr<Name> name_or_label;
    // If used -> "name_or_label" must be a generate_statement_label
    std::optional<IndexSpecification> index_specification;
};

struct BlockStatement : AstNode {
    std::unique_ptr<Identifier> block_label;
    // Expression typ must be boolean
    std::optional<std::unique_ptr<Expression>> guard_expression;
    std::unique_ptr<BlockHeader> block_header;
    std::optional<std::vector<BlockDeclarativeItem>> declarative_items;
    // Block statement part
    std::optional<std::vector<ConcurrentStatement>> concurrent_statements;
    std::optional<std::unique_ptr<Identifier>> end_block_label;
};

struct CaseStatement : AstNode {
    std::unique_ptr<Expression> case_expression;
    std::vector<std::unique_ptr<CaseAlternative>> alternatives;
};

struct CaseAlternative : AstNode {
    std::vector<Choice> choices;
    std::optional<std::vector<SequentialStatement>> sequential_statements;
};

// FIXME: Remove and implement better
struct ChoiceOthers : AstNode {
};

using Choice = std::variant<
    std::unique_ptr<SimpleExpression>,
    std::unique_ptr<DiscreteRange>,
    // FIXME: Implement better!
    std::unique_ptr<ChoiceOthers>>;

struct ComponentConfiguration : AstNode {
    std::unique_ptr<ComponentSpecification> component_specification;
    std::optional<std::unique_ptr<BindingIndication>> binding_indication;
    std::optional<std::unique_ptr<BlockConfiguration>> block_configuration;
};

struct ComponentDeclaration : AstNode {
    std::unique_ptr<Identifier> identifier;
    // Must be a LOCAL generic clause
    std::optional<std::unique_ptr<GenericClause>> local_generic_clause;
    // Must be a LOCAL port clause
    std::optional<std::unique_ptr<PortClause>> local_port_clause;
    // Must be same as "identifier"
    std::optional<std::unique_ptr<Identifier>> block_end_label;
};

struct ComponentInstantiation : AstNode {
    std::unique_ptr<Identifier> label;
    std::unique_ptr<InstantiatedUnit> instantiated_unit;
    std::optional<std::unique_ptr<GenericMapAspect>> generic_map_aspect;
    std::optional<std::unique_ptr<PortMapAspect>> port_map_aspect;
};

struct ComponentSpecification : AstNode {
    std::unique_ptr<InstantiationList> instantiation_list;
    // Name must be a component name
    std::unique_ptr<Name> name;
};

using CompositeTypeDefinition = std::variant<
    std::unique_ptr<ArrayTypeDefinition>,
    std::unique_ptr<RecordTypeDefinition>>;

struct ConcurrentAssertion : Assertion {
    std::optional<std::unique_ptr<Identifier>> label;
    bool is_postponed { false };
};

struct ConcurrentProcedureCall : ProcedureCall {
    std::optional<std::unique_ptr<Identifier>> label;
    bool is_postponed { false };
};

using ConcurrentSignalAssignment = std::variant<
    std::unique_ptr<ConcurrentConditionalSignalAssignment>,
    std::unique_ptr<ConcurrentSelectedSignalAssignment>>;

struct ConcurrentConditionalSignalAssignment : ConditionalSignalAssignment {
    std::optional<std::unique_ptr<Identifier>> label;
    bool is_postponed { false };
};

struct ConcurrentSelectedSignalAssignment : SelectedSignalAssignment {
    std::optional<std::unique_ptr<Identifier>> label;
    bool is_postponed { false };
};

using ConcurrentStatement = std::variant<
    std::unique_ptr<BlockStatement>,
    std::unique_ptr<ProcessStatement>,
    std::unique_ptr<ConcurrentProcedureCall>,
    std::unique_ptr<ConcurrentAssertion>,
    std::unique_ptr<ConcurrentSignalAssignment>,
    std::unique_ptr<ComponentInstantiation>,
    std::unique_ptr<GenerateStatement>>;

struct ConditionClause : AstNode {
    // Condition must be a boolean expression
    std::unique_ptr<Expression> condition;
};

struct ConditionalSignalAssignment : AstNode {
    std::unique_ptr<Target> target;
    bool is_guarded;
    std::unique_ptr<DelayMechanism> delay_mechanism;
    std::unique_ptr<ConditionalWaveforms> conditional_waveforms;
};

struct ConditionalWaveform : AstNode {
    std::unique_ptr<Waveform> waveform;
    // Condition must be a boolean expression
    std::unique_ptr<Expression> condition;
};

struct ConditionalWaveforms : AstNode {
    std::vector<ConditionalWaveform> conditional_waveforms;
    std::unique_ptr<Waveform> default_waveform;
    // Default_condition must be a boolean expression
    std::optional<std::unique_ptr<Expression>> default_condition;
};

struct ConfigurationDeclaration : AstNode {
    std::unique_ptr<Identifier> identifier;
    std::unique_ptr<Name> name;
    std::optional<std::vector<ConfigurationDeclarativeItem>> declarative_part;
    std::unique_ptr<BlockConfiguration> block_configuration;
    // Optional label must be same as identifier
    std::optional<std::unique_ptr<Identifier>> block_simple_name_label;
};

using ConfigurationDeclarativeItem = std::variant<
    std::unique_ptr<UseClause>,
    std::unique_ptr<AttributeSpecification>,
    std::unique_ptr<GroupDeclaration>>;

using ConfigurationItem = std::variant<
    std::unique_ptr<BlockConfiguration>,
    std::unique_ptr<ComponentConfiguration>>;

struct ConfigurationSpecification : AstNode {
    std::unique_ptr<ComponentSpecification> component_specification;
    std::unique_ptr<BindingIndication> binding_indication;
};

struct ConstantDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifiers;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    std::optional<std::unique_ptr<Expression>> init_expression;
};

using Constraint = std::variant<
    std::unique_ptr<Range>,
    std::unique_ptr<IndexConstraint>>;

using ContextItem = std::variant<
    std::unique_ptr<LibraryClause>,
    std::unique_ptr<UseClause>>;

using DelayMechanism = std::variant<
    Keyword::Transport,
    Keyword::Inertial,
    // SYNTAX: REJECT time_expression INERTIAL
    std::unique_ptr<Expression>>;

struct DesignFile : AstNode {
    std::vector<std::unique_ptr<DesignUnit>> design_units;
};

struct DesignUnit : AstNode {
    std::optional<std::vector<std::unique_ptr<ContextItem>>> context_items;
    LibraryUnit library_unit;
};

using Designator = std::variant<
    std::unique_ptr<Identifier>,
    std::unique_ptr<OperatorSymbol>>;

struct DisconnectionSpecification : AstNode {
    std::unique_ptr<GuardedSignalSpecification> guarded_signal_specification;
    // Expression must have a result of type TIME
    std::unique_ptr<Expression> time_expression;
};

using DiscreteRange = std::variant<
    std::unique_ptr<SubtypeIndication>,
    std::unique_ptr<Range>>;

// Element association
struct ChoicesExpression : AstNode {
    std::vector<std::unique_ptr<Choice>> choices;
    std::unique_ptr<Expression> expression;
};

using ElementAssociation = std::variant<
    std::unique_ptr<Expression>,
    std::unique_ptr<ChoicesExpression>>;

struct ElementDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifier_list;
    std::unique_ptr<SubtypeIndication> element_subtype_definition;
};

struct EntityName : AstNode {
    // Name must be an entity name
    std::unique_ptr<Name> name;
    // Must be an architecture identifier (optional)
    std::optional<std::unique_ptr<Identifier>> architecture_identifier { std::nullopt };
};

struct ConfigurationName : AstNode {
    // Must be a configuration name
    std::unique_ptr<Name> name;
};

// FIXME: Fix
using EntityAspect = std::variant<
    EntityName,
    ConfigurationName,
    Keyword::Open>;

enum class EntityClass {
    architecture,
    component,
    configuration,
    constant,
    entity,
    function,
    label,
    literal,
    package,
    procedure,
    signal,
    subtype,
    type,
    units,
    variable
};

struct EntityClassEntry : AstNode {
    EntityClass entity_class;
    // TODO: Better semantic name
    bool has_box;
};

class EntityDeclaration : AstNode {
    std::unique_ptr<Identifier> identifier;
    std::optional<EntityHeader> entity_header;
    std::optional<std::vector<EntityDeclarativeItem>> declarative_items;
    std::optional<std::vector<EntityStatement>> statement_items;
    // Optional block end label (must be same as identifier)
    std::optional<std::unique_ptr<Identifier>> block_simple_name;
};

using EntityDeclarativeItem = std::variant<
    std::unique_ptr<SubprogramSpecification>,
    std::unique_ptr<SubprogramBody>,
    std::unique_ptr<TypeDeclaration>,
    std::unique_ptr<SubtypeDeclaration>,
    std::unique_ptr<ConstantDeclaration>,
    std::unique_ptr<SignalDeclaration>,
    std::unique_ptr<SharedVariableDeclaration>,
    std::unique_ptr<FileDeclaration>,
    std::unique_ptr<AliasDeclaration>,
    std::unique_ptr<AttributeDeclaration>,
    std::unique_ptr<AttributeSpecification>,
    std::unique_ptr<DisconnectionSpecification>,
    std::unique_ptr<UseClause>,
    std::unique_ptr<GroupTemplateDeclaration>,
    std::unique_ptr<GroupDeclaration>>;

struct EntityDesignator : AstNode {
    EntityTag entity_tag;
    std::optional<Signature> signature;
};

class EntityHeader : AstNode {
    std::optional<std::unique_ptr<GenericClause>> generic_clause;
    std::optional<std::unique_ptr<PortClause>> port_clause;
};

using EntityNameList = std::variant<
    std::vector<std::unique_ptr<EntityDesignator>>,
    Keyword::Others,
    Keyword::All>;

struct EntitySpecification : AstNode {
    EntityNameList entity_name_list;
    EntityClass entity_class;
};

using EntityStatement = std::variant<
    std::unique_ptr<ConcurrentAssertion>,
    // Must be a passive procedure call
    std::unique_ptr<ConcurrentProcedureCall>,
    // Must be a passive process
    std::unique_ptr<ProcessStatement>>;

using EntityTag = std::variant<
    // simple_name
    std::unique_ptr<Identifier>,
    std::unique_ptr<Lexer::CharacterLiteral>,
    std::unique_ptr<OperatorSymbol>>;

using EnumerationLiteral = std::variant<
    std::unique_ptr<Identifier>,
    std::unique_ptr<Lexer::CharacterLiteral>>;

struct EnumerationTypeDefinition : AstNode {
    std::vector<std::unique_ptr<EnumerationLiteral>> enumeration_literals;
};

struct ExitStatement : AstNode {
    // Optional label, must be a loop label
    std::optional<std::unique_ptr<Identifier>> label;
    // Must be a boolean expression
    std::optional<std::unique_ptr<Expression>> condition;
};

struct ExpandedSelectedName : AstNode {
    SimplePrefix simple_prefix;
    std::unique_ptr<Suffix> suffix;
};

using SimplePrefix = std::variant<
    std::unique_ptr<ExpandedSelectedName>,
    // simple_name
    std::unique_ptr<Identifier>>;

enum class RelationKind {
    AND,
    OR,
    XOR,
    NAND,
    NOR,
    XNOR,

    // No operator, just a single relation
    NONE
};

struct Expression : AstNode {
    std::vector<std::unique_ptr<Relation>> relations;

    // Assert: (relations.size() == 1) implies (kind == RelationKind::NONE)
    RelationKind kind { RelationKind::NONE };
};

enum class FactorKind {
    EXPONENTIATION,
    ABS,
    NOT,

    // No operator, just a single primary
    NONE
};

struct Factor : AstNode {
    std::unique_ptr<Primary> primary_left;
    std::optional<std::unique_ptr<Primary>> primary_right;

    // Assert: (primary_right == std::nullopt) iff (FactorKind::NONE)
    FactorKind kind { FactorKind::NONE };
};

struct FileDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifier_list;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    std::optional<std::unique_ptr<FileOpenInformation>> file_open_information;
};

struct FileOpenInformation : AstNode {
    // Must be a string expression
    std::unique_ptr<Expression> file_logical_name;
    // Must be a FILE_OPEN_KIND expression
    std::optional<std::unique_ptr<Expression>> file_open_kind;
};

struct FileTypeDefinition : AstNode {
    // Name must be a type mark
    std::unique_ptr<Name> type_mark;
};

struct FormalDesignator : AstNode {
    // Name must be a GENERIC/PORT/PARAMETER name
    std::unique_ptr<Name> name;
};

struct FormalPart : AstNode {
    // Name must be a GENERIC/PORT/PARAMETER/FUNCTION/TYPE_MARK name
    std::unique_ptr<Name> name;
    // If formal_designator is not nullopt, then the "name" must be a function name
    // or a type mark.
    // Name must be a GENERIC/PORT/PARAMETER name
    std::optional<std::unique_ptr<Name>> formal_designator;
};

struct TypeDeclaration : AstNode {
    std::unique_ptr<Identifier> identifier;
    // Can be optional for incomplete type declaration
    std::optional<TypeDefinition> type_definition;
};

struct FunctionCall : AstNode {
    // Name must be a FUNCTION name
    std::unique_ptr<Name> function_name;
    // Must be a PARAMETER association list
    std::optional<std::vector<std::unique_ptr<AssociationElement>>> association_list;
};

struct GenerateStatement : AstNode {
    std::unique_ptr<Identifier> label;
    std::unique_ptr<GenerationScheme> generation_scheme;
    std::optional<std::vector<BlockDeclarativeItem>> declarative_statements;
    std::optional<std::vector<ConcurrentStatement>> concurrent_statements;
    // Optional, must be equal to "label" if defined
    std::optional<std::unique_ptr<Identifier>> end_label;
};

using GenerationScheme = std::variant<
    // FOR generation scheme
    // Must be a GENERATION parameter specification
    std::unique_ptr<ParameterSpecification>,
    // IF generation scheme
    // Must be a boolean expression
    std::unique_ptr<Expression>>;

struct GenericClause : AstNode {
    std::vector<std::unique_ptr<InterfaceConstantDeclaration>> generic_list;
};

struct GenericMapAspect : AstNode {
    // Must be a GENERIC association list
    std::vector<std::unique_ptr<AssociationElement>> association_list;
};

using GroupConstituent = std::variant<
    std::unique_ptr<Lexer::CharacterLiteral>,
    std::unique_ptr<Name>>;

struct GroupDeclaration : AstNode {
    std::unique_ptr<Identifier> identifier;
    std::vector<std::unique_ptr<EntityClassEntry>> entity_class_entry_list;
};

struct GroupTemplateDeclaration : AstNode {
    std::unique_ptr<Identifier> identifier;
    std::vector<EntityClassEntry> entity_class_entry_list;
};

struct GuardedSignalSpecification : AstNode {
    // Must be a list of guarded signals
    SignalList signal_list;
    // Must be a TYPE or a SUBTYPE name
    std::unique_ptr<Name> type_mark;
};

using Identifier = std::variant<Lexer::BasicIdentifier, Lexer::ExtendedIdentifier>;

struct IfStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
    // Must be a boolean expression
    std::unique_ptr<Expression> if_condition;
    std::vector<SequentialStatement> if_sequence_of_statements;

    // Must be a boolean expression
    std::optional<std::unique_ptr<Expression>> elsif_condition;
    std::optional<std::vector<SequentialStatement>> elsif_sequence_of_statements;

    std::optional<std::vector<SequentialStatement>> else_sequence_of_statements;
};

// TODO: Maybe remove?
struct IndexConstraint : AstNode {
    std::vector<DiscreteRange> discrete_ranges;
};

using IndexSpecification = std::variant<
    std::unique_ptr<DiscreteRange>,
    // Must be a static expression
    std::unique_ptr<Expression>>;

struct IndexedName : AstNode {
    std::unique_ptr<Prefix> prefix;
    std::vector<std::unique_ptr<Expression>> expression_list;
};

struct ComponentUnit : AstNode {
    // Must be a COMPONENT name
    std::unique_ptr<Name> component_name;
};

struct EntityUnit : AstNode {
    // Must be an ENTITY name
    std::unique_ptr<Name> entity_name;
    // Must be an ARCHITECTURE identifier (if used)
    std::optional<Identifier> architecture_identifier;
};

struct ConfigurationUnit : AstNode {
    // Must be a CONFIGURATION name
    std::unique_ptr<Name> configuration_name;
};

using InstantiatedUnit = std::variant<
    std::unique_ptr<ComponentUnit>,
    std::unique_ptr<EntityUnit>,
    std::unique_ptr<ConfigurationUnit>>;

// FIXME: Correctly redefine
using InstantiationList = std::variant<
    // Must be a list of component instance labels
    std::vector<std::unique_ptr<Identifier>>,
    Keyword::Others,
    Keyword::All>;

struct InterfaceConstantDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifier_list;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    // Must be a STATIC expression
    std::optional<std::unique_ptr<Expression>> init_expression;
};

using InterfaceDeclaration = std::variant<
    std::unique_ptr<InterfaceUnresolvedDeclaration>,
    std::unique_ptr<InterfaceConstantDeclaration>,
    std::unique_ptr<InterfaceSignalDeclaration>,
    std::unique_ptr<InterfaceVariableDeclaration>,
    std::unique_ptr<InterfaceFileDeclaration>>;

struct InterfaceFileDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifier_list;
    std::unique_ptr<SubtypeIndication> subtype_indication;
};

enum class Mode {
    IN,
    OUT,
    INOUT,
    BUFFER,
    LINKAGE
};

struct InterfaceSignalDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifier_list;
    Mode mode { Mode::IN };
    std::unique_ptr<SubtypeIndication> subtype_indication;
    bool is_bus { false };
    // Must be a STATIC expression
    std::optional<std::unique_ptr<Expression>> init_expression;
};

struct InterfaceUnresolvedDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifier_list;
    Mode mode { Mode::IN };
    std::unique_ptr<SubtypeIndication> subtype_indication;
    bool is_bus { false };
    // Must be a STATIC expression
    std::optional<std::unique_ptr<Expression>> init_expression;
};

struct InterfaceVariableDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifier_list;
    Mode mode { Mode::IN };
    std::unique_ptr<SubtypeIndication> subtype_indication;
    // Must be a STATIC expression
    std::optional<std::unique_ptr<Expression>> init_expression;
};

using IterationScheme = std::variant<
    // WHILE condition
    // Must be a boolean expression (i.e., a condition)
    std::unique_ptr<Expression>,
    // FOR condition
    // Must be a LOOP parameter specification
    std::unique_ptr<ParameterSpecification>>;

struct LibraryClause : AstNode {
    std::vector<std::unique_ptr<Identifier>> logical_name_list;
};

using LibraryUnit = std::variant<PrimaryUnit, SecondaryUnit>;

// FIXME: Fix
using Literal = std::variant<
    std::unique_ptr<NumericLiteral>,
    std::unique_ptr<EnumerationLiteral>,
    std::unique_ptr<Lexer::StringLiteral>,
    std::unique_ptr<Lexer::BitStringLiteral>,
    Keyword::Null>;

struct LoopStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
    std::optional<IterationScheme> iteration_scheme;
    std::vector<SequentialStatement> sequence_of_statements;
    // Optional
    std::optional<std::unique_ptr<Identifier>> end_label;
};

using Name = std::variant<
    std::unique_ptr<AttributeName>,
    std::unique_ptr<IndexedName>,
    std::unique_ptr<OperatorSymbol>,
    std::unique_ptr<SelectedName>,
    // Simple name
    std::unique_ptr<Identifier>,
    std::unique_ptr<SliceName>>;

struct NextStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> statement_label;
    std::optional<std::unique_ptr<Identifier>> next_label;
    // Optional, expression must be a boolean condition
    std::optional<std::unique_ptr<Expression>> when_condition;
};

struct NullStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
};

using NumericLiteral = std::variant<
    std::unique_ptr<AbstractLiteral>,
    std::unique_ptr<PhysicalLiteral>>;

struct OperatorSymbol : AstNode {
    Lexer::StringLiteral operator_symbol;
};

struct Options : AstNode {
    bool is_guarded { false };
    DelayMechanism delay_mechanism;
};

struct PackageBody : AstNode {
    std::unique_ptr<Identifier> package_simple_name;
    std::optional<std::vector<PackageBodyDeclarativeItem>> declarative_part;
    // Optional, should be equal to "package_simple_name"
    std::unique_ptr<Identifier> simple_name_opt;
};

using PackageBodyDeclarativeItem = std::variant<
    std::unique_ptr<AliasDeclaration>,
    std::unique_ptr<ConstantDeclaration>,
    std::unique_ptr<FileDeclaration>,
    std::unique_ptr<GroupDeclaration>,
    std::unique_ptr<GroupTemplateDeclaration>,
    std::unique_ptr<SharedVariableDeclaration>,
    std::unique_ptr<SubprogramBody>,
    // subprogram declaration
    std::unique_ptr<SubprogramSpecification>,
    std::unique_ptr<SubtypeDeclaration>,
    std::unique_ptr<TypeDeclaration>,
    std::unique_ptr<UseClause>>;

struct PackageDeclaration : AstNode {
    std::unique_ptr<Identifier> identifier;
    std::optional<std::vector<PackageDeclarativeItem>> declarative_part;
    // Optional, should be equal to the "identifier" if used
    std::optional<std::unique_ptr<Identifier>> simple_name_opt;
};

using PackageDeclarativeItem = std::variant<
    std::unique_ptr<AliasDeclaration>,
    std::unique_ptr<AttributeDeclaration>,
    std::unique_ptr<AttributeSpecification>,
    std::unique_ptr<ComponentDeclaration>,
    std::unique_ptr<ConstantDeclaration>,
    std::unique_ptr<DisconnectionSpecification>,
    std::unique_ptr<FileDeclaration>,
    std::unique_ptr<GroupDeclaration>,
    std::unique_ptr<GroupTemplateDeclaration>,
    std::unique_ptr<SharedVariableDeclaration>,
    std::unique_ptr<SignalDeclaration>,
    // subprogram declaration
    std::unique_ptr<SubprogramSpecification>,
    std::unique_ptr<SubtypeDeclaration>,
    std::unique_ptr<TypeDeclaration>,
    std::unique_ptr<UseClause>>;

struct ParameterSpecification : AstNode {
    std::unique_ptr<Identifier> identifier;
    DiscreteRange discrete_range;
};

struct PhysicalLiteral : AstNode {
    std::unique_ptr<Name> name;
    std::optional<std::unique_ptr<AbstractLiteral>> abstract_literal;
};

struct PhysicalTypeDefinition : AstNode {
    Range range_constraint;
    std::unique_ptr<Identifier> primary_unit_declaration;
    std::vector<std::unique_ptr<SecondaryUnitDeclaration>> secondary_unit_declarations;
    std::optional<std::unique_ptr<Identifier>> simple_name_opt;
};

struct PortClause : AstNode {
    std::vector<std::unique_ptr<InterfaceSignalDeclaration>> port_list;
};

struct PortMapAspect : AstNode {
    // Must be a PORT association list
    std::vector<std::unique_ptr<AssociationElement>> association_list;
};

using Prefix = std::variant<
    std::unique_ptr<Name>,
    std::unique_ptr<FunctionCall>>;

using Primary = std::variant<
    std::unique_ptr<Name>,
    std::unique_ptr<Literal>,
    std::unique_ptr<Aggregate>,
    std::unique_ptr<FunctionCall>,
    std::unique_ptr<QualifiedExpression>,
    std::unique_ptr<TypeConversion>,
    std::unique_ptr<Allocator>,
    std::unique_ptr<Expression>>;

using PrimaryUnit = std::variant<
    std::unique_ptr<EntityDeclaration>,
    std::unique_ptr<ConfigurationDeclaration>,
    std::unique_ptr<PackageDeclaration>>;

struct ProcedureCall : AstNode {
    std::unique_ptr<Name> procedure_name;
    // Must be a PARAMETER association list
    std::optional<std::vector<std::unique_ptr<AssociationElement>>> association_list;
};

struct ProcedureCallStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
    ProcedureCall procedure_call;
};

using ProcessDeclarativeItem = std::variant<
    std::unique_ptr<AliasDeclaration>,
    std::unique_ptr<AttributeDeclaration>,
    std::unique_ptr<AttributeSpecification>,
    std::unique_ptr<ConstantDeclaration>,
    std::unique_ptr<FileDeclaration>,
    std::unique_ptr<GroupDeclaration>,
    std::unique_ptr<GroupTemplateDeclaration>,
    std::unique_ptr<SubprogramBody>,
    // subprogram declaration
    std::unique_ptr<SubprogramSpecification>,
    std::unique_ptr<SubtypeDeclaration>,
    std::unique_ptr<TypeDeclaration>,
    std::unique_ptr<UseClause>,
    std::unique_ptr<VariableDeclaration>>;

struct ProcessStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
    bool is_postponed;
    // Names must be SIGNAL names
    std::optional<std::vector<std::unique_ptr<Name>>> sensitivity_list;
    std::optional<std::vector<ProcessDeclarativeItem>> declarative_part;
    std::optional<std::vector<SequentialStatement>> statement_part;
    // Must be equal to is_postponed
    // FIXME: Change grammar?
    bool end_is_postponed;
    // Optional, must be equal to label if defined
    std::optional<std::unique_ptr<Identifier>> end_label;
};

struct ProtectedTypeBody : AstNode {
    std::optional<std::vector<ProtectedTypeBodyDeclarativeItem>> declarative_part;
    std::optional<std::unique_ptr<Identifier>> simple_name_opt;
};

using ProtectedTypeBodyDeclarativeItem = std::variant<
    std::unique_ptr<AliasDeclaration>,
    std::unique_ptr<AttributeDeclaration>,
    std::unique_ptr<AttributeSpecification>,
    std::unique_ptr<ConstantDeclaration>,
    std::unique_ptr<FileDeclaration>,
    std::unique_ptr<GroupDeclaration>,
    std::unique_ptr<GroupTemplateDeclaration>,
    std::unique_ptr<SubprogramBody>,
    // subprogram declaration
    std::unique_ptr<SubprogramSpecification>,
    std::unique_ptr<SubtypeDeclaration>,
    std::unique_ptr<TypeDeclaration>,
    std::unique_ptr<UseClause>,
    std::unique_ptr<VariableDeclaration>>;

struct ProtectedTypeDeclaration : AstNode {
    std::optional<std::vector<ProtectedTypeDeclarativeItem>> declarative_items;
    std::optional<std::unique_ptr<Identifier>> simple_name_opt;
};

using ProtectedTypeDeclarativeItem = std::variant<
    std::unique_ptr<SubprogramSpecification>,
    std::unique_ptr<AttributeSpecification>,
    std::unique_ptr<UseClause>>;

using ProtectedTypeDefinition = std::variant<
    std::unique_ptr<ProtectedTypeDeclaration>,
    std::unique_ptr<ProtectedTypeBody>>;

struct QualifiedExpression : AstNode {
    std::unique_ptr<Name> type_mark;
    std::optional<std::unique_ptr<Expression>> expression;
    std::optional<std::unique_ptr<Aggregate>> aggregate;
};

enum class Direction {
    TO,
    DOWNTO
};

struct SimpleRange : AstNode {
    std::unique_ptr<SimpleExpression> simple_expression;
    Direction direction;
    std::unique_ptr<SimpleExpression> simple_expression;
};

using Range = std::variant<
    // Must be an ATTRIBUTE name
    std::unique_ptr<Name>,
    std::unique_ptr<SimpleRange>>;

struct RecordTypeDefinition : AstNode {
    std::vector<std::unique_ptr<ElementDeclaration>> element_declaration_list;
    std::optional<std::unique_ptr<Identifier>> simple_name_opt;
};

struct Relation : AstNode {
    std::unique_ptr<ShiftExpression> left_shift_expression;
    std::optional<RelationalOperator> relational_operator;
    std::optional<std::unique_ptr<ShiftExpression>> right_shift_expression;
};

enum class RelationalOperator {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LT_OR_EQUAL,
    GREATER_THAN,
    GT_OR_EQUAL
};

struct ReportStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
    std::unique_ptr<Expression> report_expression;
    std::optional<std::unique_ptr<Expression>> severity_expression;
};

struct ReturnStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
    std::optional<std::unique_ptr<Expression>> return_expression;
};

using ScalarTypeDefinition = std::variant<
    std::unique_ptr<EnumerationTypeDefinition>,
    // range constraint
    std::unique_ptr<Range>,
    std::unique_ptr<PhysicalTypeDefinition>>;

using SecondaryUnit = std::variant<
    std::unique_ptr<ArchitectureBody>,
    std::unique_ptr<PackageBody>>;

struct SecondaryUnitDeclaration : AstNode {
    std::unique_ptr<Identifier> left_identifier;
    std::unique_ptr<PhysicalLiteral> physical_literal;
};

// TODO: Maybe convert to vector of prefixes/sufixes
struct SelectedName : AstNode {
    std::unique_ptr<Prefix> prefix;
    std::unique_ptr<Suffix> suffix;
};

struct SelectedSignalAssignment : AstNode {
    std::unique_ptr<Expression> with_expression;
    Target target;
    Options options;
    std::vector<SelectedWaveform> selected_waveforms;
};

struct SelectedWaveform : AstNode {
    std::unique_ptr<Waveform> waveform;
    std::vector<Choice> choices;
};

using SequentialStatement = std::variant<
    std::unique_ptr<WaitStatement>,
    std::unique_ptr<AssertionStatement>,
    std::unique_ptr<ReportStatement>,
    std::unique_ptr<SignalAssignmentStatement>,
    std::unique_ptr<VariableAssignmentStatement>,
    std::unique_ptr<ProcedureCallStatement>,
    std::unique_ptr<IfStatement>,
    std::unique_ptr<CaseStatement>,
    std::unique_ptr<LoopStatement>,
    std::unique_ptr<NextStatement>,
    std::unique_ptr<ExitStatement>,
    std::unique_ptr<ReturnStatement>,
    std::unique_ptr<NullStatement>>;

struct ShiftExpression : AstNode {
    std::unique_ptr<SimpleExpression> left_expression;
    std::optional<ShiftOperator> shift_operator;
    std::optional<std::unique_ptr<SimpleExpression>> right_expression;
};

enum class ShiftOperator {
    SLL,
    SRL,
    SLA,
    SRA,
    ROL,
    ROR
};

enum class Sign {
    PLUS,
    MINUS
};

struct SignalAssignmentStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
    Target target;
    std::optional<DelayMechanism> delay_mechanism;
    std::unique_ptr<Waveform> waveform;
};

struct SignalDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifier_list;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    std::optional<SignalKind> signal_kind;
    // TODO: Optional, must be a static expression?
    std::optional<std::unique_ptr<Expression>> init_expression;
};

enum class SignalKind {
    REGISTER,
    BUS
};

// FIXME: Fix
using SignalList = std::variant<
    // Names must be SIGNAL names
    std::vector<std::unique_ptr<Name>>,
    Keyword::Others,
    Keyword::All>;

struct Signature : AstNode {
    // Name must be a type mark
    std::optional<std::vector<std::unique_ptr<Name>>> type_marks;
    // Name must be a type mark
    std::optional<std::unique_ptr<Name>> return_type_mark;
};

enum class Sign {
    plus,
    minus
};

enum class AddingOperator {
    plus,
    minus,
    ampersand
};

struct SimpleExpression : AstNode {
    std::optional<Sign> sign;
    std::unique_ptr<Term> term;
    std::optional<std::vector<std::pair<AddingOperator, std::unique_ptr<Term>>>> adding_terms;
};

struct SliceName : AstNode {
    Prefix prefix;
    DiscreteRange discrete_range;
};

struct SubprogramBody : AstNode {
    SubprogramSpecification subprogram_specification;
    std::optional<std::vector<SubprogramDeclarativeItem>> declarative_part;
    std::optional<std::vector<SequentialStatement>> statement_part;
    // Subprogram kind (optional), must be same as identifier in specification
    std::optional<SubprogramKind> subprogram_kind;
    std::optional<Designator> designator;
};

using SubprogramDeclarativeItem = std::variant<
    // subprogram declaration
    std::unique_ptr<SubprogramSpecification>,
    std::unique_ptr<SubprogramBody>,
    std::unique_ptr<TypeDeclaration>,
    std::unique_ptr<SubtypeDeclaration>,
    std::unique_ptr<ConstantDeclaration>,
    std::unique_ptr<VariableDeclaration>,
    std::unique_ptr<FileDeclaration>,
    std::unique_ptr<AliasDeclaration>,
    std::unique_ptr<AttributeDeclaration>,
    std::unique_ptr<AttributeSpecification>,
    std::unique_ptr<UseClause>,
    std::unique_ptr<GroupTemplateDeclaration>,
    std::unique_ptr<GroupDeclaration>>;

enum class SubprogramKind {
    function,
    procedure,
};

struct ProcedureSpecification : AstNode {
    Designator designator;
    // Must be a parameter interface list
    std::optional<std::vector<InterfaceDeclaration>> formal_parameter_list;
};

enum class FunctionType {
    impure,
    pure,
};

struct FunctionSpecification : AstNode {
    std::optional<FunctionType> function_type;
    Designator designator;
    std::optional<std::vector<InterfaceDeclaration>> formal_parameter_list;
    // Name must be a TYPE or a SUBTYPE name
    std::unique_ptr<Name> return_type;
};

using SubprogramSpecification = std::variant<
    std::unique_ptr<ProcedureSpecification>,
    std::unique_ptr<FunctionSpecification>>;

struct SubtypeDeclaration : AstNode {
    std::unique_ptr<Identifier> identifier;
    std::unique_ptr<SubtypeIndication> subtype_indication;
};

struct SubtypeIndication : AstNode {
    // Optional, desgiantor must be a RESOLUTION FUNCTION name
    std::optional<Designator> resolution_function_name;
    // Name must be a TYPE or SUBTYPE name
    std::unique_ptr<Name> type_mark;
    // A range or an index constraint (optional)
    std::optional<Constraint> constraint;
};

using Suffix = std::variant<
    // TODO: Must be a simple name?
    std::unique_ptr<Identifier>,
    std::unique_ptr<Lexer::CharacterLiteral>,
    std::unique_ptr<OperatorSymbol>,
    Keyword::All>;

using Aggregate = std::vector<ElementAssociation>;

using Target = std::variant<
    std::unique_ptr<Name>,
    std::unique_ptr<Aggregate>>;

enum class MultiplyingOperator {
    // Multiplication
    mul,
    // Division
    div,
    // Modulo
    mod,
    // Reminder
    rem
};

struct Term : AstNode {
    std::optional<std::unique_ptr<Term>> term;
    std::optional<MultiplyingOperator> multiplying_operator;
    std::unique_ptr<Factor> factor;
};

struct TypeConversion : AstNode {
    // Name must be a TYPE or a SUBTYPE name
    std::unique_ptr<Name> type_mark;
    std::unique_ptr<Expression> expression;
};

using TypeDefinition = std::variant<
    std::unique_ptr<ScalarTypeDefinition>,
    std::unique_ptr<CompositeTypeDefinition>,
    std::unique_ptr<AccessTypeDefinition>,
    std::unique_ptr<FileTypeDefinition>,
    std::unique_ptr<ProtectedTypeDefinition>>;

struct UseClause : AstNode {
    std::vector<ExpandedSelectedName> selected_name;
};

struct VariableAssignmentStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
    Target assignment_target;
    std::unique_ptr<Expression> expression;
};

using VariableDeclaration = std::variant<
    std::unique_ptr<PlainVariableDeclaration>,
    std::unique_ptr<SharedVariableDeclaration>>;

struct PlainVariableDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifier_list;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    std::optional<std::unique_ptr<Expression>> init_expression;
};

struct SharedVariableDeclaration : PlainVariableDeclaration {
    // TODO: Any differences?
};

struct WaitStatement : AstNode {
    std::optional<std::unique_ptr<Identifier>> label;
    // Name must be a SIGNAL name
    // SYNTAX: ON sensitivity_list
    std::optional<std::vector<std::unique_ptr<Name>>> sensitivity_list;
    // Expression must be a CONDITION (i.e., have a boolean type)
    std::optional<std::unique_ptr<Expression>> until_condition_clause;
    // SYNTAX: FOR time_expression
    // Expression must be a TIME expression
    std::optional<std::unique_ptr<Expression>> for_timeout_clause;
};

namespace TempKeyword {
struct Unaffected;
}

using Waveform = std::variant<
    std::vector<WaveformElement>,
    TempKeyword::Unaffected>;

using WaveformElement = std::variant<
    std::unique_ptr<WaveformElementKind::ValueExpression>,
    std::unique_ptr<WaveformElementKind::NullExpression>>;

namespace WaveformElementKind {
struct ValueExpression : AstNode {
    // Expression must be a VALUE expression
    std::unique_ptr<Expression> value_expression;
    // Expression must be a TIME expression
    std::optional<std::unique_ptr<Expression>> time_expression;
};

struct NullExpression : AstNode {
    // Expression must be a TIME expression
    std::optional<std::unique_ptr<Expression>> time_expression;
};
}

#endif /* PARSER_DEFS */
