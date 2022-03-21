#ifndef PARSER_DEFS
#define PARSER_DEFS

#include <memory>
#include <optional>
#include <variant>
#include <vector>

#include <gsl/pointers>

#include "lexer/lexer_defs.h"

namespace parser {

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
    struct Unaffected;
}

using AbstractLiteral = std::variant<
    Lexer::DecimalLiteral,
    Lexer::BasedLiteral>;

using ActualDesignator = std::variant<
    gsl::not_null<std::unique_ptr<Expression>>,
    /**
     *  Name must be:
     *      - variable_name
     *      - signal_name
     *      - file_name
     */
    gsl::not_null<std::unique_ptr<Name>>,
    Keyword::Open>;

struct ActualPart : AstNode {
    // Name must be FUNCTION name or a TYPE mark
    std::unique_ptr<Name> name; // optional
    ActualDesignator actual_designator;
};

using AliasDesignator = std::variant<
    std::unique_ptr<Identifier>,
    std::unique_ptr<Lexer::CharacterLiteral>,
    std::unique_ptr<OperatorSymbol>>;

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

using ArrayTypeDefinition = std::variant<
    std::unique_ptr<UnconstrainedArrayDefinition>,
    std::unique_ptr<ConstrainedArrayDefinition>>;

struct UnconstrainedArrayDefinition : AstNode {
    std::unique_ptr<SubtypeIndication> subtype;
    // Names must be type marks
    std::vector<Name> index_subtypes;
};

struct ConstrainedArrayDefinition : AstNode {
    std::vector<DiscreteRange> index_constraints;
    std::unique_ptr<SubtypeIndication> subtype;
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
    std::optional<Identifier> opt_label; // optional
    Assertion assertion;
};

struct AssociationElement : AstNode {
    std::unique_ptr<FormalPart> formal_part_opt; // Optional
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
    Identifier attribute_designator;
    std::unique_ptr<EntitySpecification> entity_specification;
    std::unique_ptr<Expression> expression;
};

struct BindingIndication : AstNode {
    std::unique_ptr<EntityAspect> entity_aspect; // Optional
    std::unique_ptr<GenericMapAspect> generic_map_aspect; // Optional
    std::unique_ptr<PortMapAspect> port_map_aspect; // Optional
};

struct BlockConfiguration : AstNode {
    std::unique_ptr<BlockSpecification> block_specification;
    std::vector<UseClause> use_clauses; // Optional
    std::vector<ConfigurationItem> configuration_items; // Optional
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

struct BlockHeaderGenericClause : AstNode {
    std::unique_ptr<GenericClause> generic_clause; // Optional
    std::unique_ptr<GenericMapAspect> generic_map_aspect; // Optional
};

struct BlockHeaderPortClause : AstNode {
    std::unique_ptr<PortClause> port_clause; // Optional
    std::unique_ptr<PortMapAspect> port_map_aspect; // Optional
};

struct BlockHeader : AstNode {
    std::optional<BlockHeaderGenericClause> generic_clause; // Optional
    std::optional<BlockHeaderPortClause> port_clause; // Optional
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
    std::unique_ptr<Expression> guard_expression; // Optional
    std::unique_ptr<BlockHeader> block_header;
    std::vector<BlockDeclarativeItem> declarative_items; // Optional
    // Block statement part
    std::vector<ConcurrentStatement> concurrent_statements; // Optional
    std::optional<Identifier> end_block_label;
};

struct CaseStatement : AstNode {
    std::optional<Identifier> label_opt; // Optional
    std::unique_ptr<Expression> case_expression;
    std::vector<CaseAlternative> alternatives;
    std::optional<Identifier> end_label_opt; // Optional
};

struct CaseAlternative : AstNode {
    std::vector<Choice> choices;
    std::vector<SequentialStatement> sequential_statements; // Optional
};

using Choice = std::variant<
    std::unique_ptr<SimpleExpression>,
    std::unique_ptr<DiscreteRange>,
    Keyword::Others>;

struct ComponentConfiguration : AstNode {
    std::unique_ptr<ComponentSpecification> component_specification;
    std::optional<BindingIndication> binding_indication; // Optional
    std::optional<BlockConfiguration> block_configuration; // Optional
};

struct ComponentDeclaration : AstNode {
    Identifier identifier;
    // Must be a LOCAL generic clause
    std::optional<GenericClause> local_generic_clause; // Optional
    // Must be a LOCAL port clause
    std::optional<PortClause> local_port_clause; // Optional
    // Must be same as "identifier"
    std::optional<Identifier> block_end_label; // Optional
};

struct ComponentInstantiation : AstNode {
    Identifier label;
    std::unique_ptr<InstantiatedUnit> instantiated_unit;
    std::unique_ptr<GenericMapAspect> generic_map_aspect; // Optional
    std::unique_ptr<PortMapAspect> port_map_aspect; // Optional
};

struct ComponentSpecification : AstNode {
    InstantiationList instantiation_list;
    // Name must be a component name
    Name name;
};

using CompositeTypeDefinition = std::variant<
    std::unique_ptr<ArrayTypeDefinition>,
    std::unique_ptr<RecordTypeDefinition>>;

struct ConcurrentAssertion : Assertion {
    std::optional<Identifier> label; // Optional
    bool is_postponed { false };
};

struct ConcurrentProcedureCall : ProcedureCall {
    std::optional<Identifier> label; // Optional
    bool is_postponed { false };
};

using ConcurrentSignalAssignment = std::variant<
    ConcurrentConditionalSignalAssignment,
    ConcurrentSelectedSignalAssignment>;

struct ConcurrentConditionalSignalAssignment : ConditionalSignalAssignment {
    std::optional<Identifier> label;
    bool is_postponed { false };
};

struct ConcurrentSelectedSignalAssignment : SelectedSignalAssignment {
    std::optional<Identifier> label;
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
    Target target;
    Options options;
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
    std::unique_ptr<Expression> default_condition; // Optional
};

struct ConfigurationDeclaration : AstNode {
    Identifier identifier;
    Name name;
    std::vector<ConfigurationDeclarativeItem> declarative_part; // Optional
    std::unique_ptr<BlockConfiguration> block_configuration;
    // Optional label must be same as identifier
    std::optional<Identifier> block_simple_name_label; // Optional
};

using ConfigurationDeclarativeItem = std::variant<
    UseClause,
    AttributeSpecification,
    GroupDeclaration>;

using ConfigurationItem = std::variant<BlockConfiguration, ComponentConfiguration>;

struct ConfigurationSpecification : AstNode {
    std::unique_ptr<ComponentSpecification> component_specification;
    std::unique_ptr<BindingIndication> binding_indication;
};

struct ConstantDeclaration : AstNode {
    std::vector<Identifier> identifiers;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    std::unique_ptr<Expression> init_expression; // Optional
};

using Constraint = std::variant<Range, IndexConstraint>;

using ContextItem = std::variant<LibraryClause, UseClause>;

using DelayMechanism = std::variant<
    Keyword::Transport,
    Keyword::Inertial,
    // SYNTAX: REJECT time_expression INERTIAL
    std::unique_ptr<Expression>>;

struct DesignFile : AstNode {
    std::vector<DesignUnit> design_units;
};

struct DesignUnit : AstNode {
    std::vector<ContextItem> context_items; // Optional
    LibraryUnit library_unit;
};

using Designator = std::variant<Identifier, OperatorSymbol>;

struct DisconnectionSpecification : AstNode {
    GuardedSignalSpecification guarded_signal_specification;
    // Expression must have a result of type TIME
    std::unique_ptr<Expression> time_expression;
};

using DiscreteRange = std::variant<
    std::unique_ptr<SubtypeIndication>,
    std::unique_ptr<Range>>;

// Element association
struct ChoicesExpression : AstNode {
    std::vector<Choice> choices;
    std::unique_ptr<Expression> expression;
};

using ElementAssociation = std::variant<std::unique_ptr<Expression>, ChoicesExpression>;

struct ElementDeclaration : AstNode {
    std::vector<Identifier> identifier_list;
    std::unique_ptr<SubtypeIndication> element_subtype_definition;
};

struct EntityName : AstNode {
    // Name must be an entity name
    Name name;
    // Must be an architecture identifier (optional)
    std::optional<Identifier> architecture_identifier;
};

struct ConfigurationName : AstNode {
    // Must be a configuration name
    Name name;
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
    Identifier identifier;
    std::optional<EntityHeader> entity_header;
    std::vector<EntityDeclarativeItem> declarative_items; // Optional
    std::vector<EntityStatement> statement_items; // Optional
    // Optional block end label (must be same as identifier)
    std::optional<Identifier> block_simple_name;
};

using EntityDeclarativeItem = std::variant<
    SubprogramSpecification,
    SubprogramBody,
    TypeDeclaration,
    SubtypeDeclaration,
    ConstantDeclaration,
    SignalDeclaration,
    SharedVariableDeclaration,
    FileDeclaration,
    AliasDeclaration,
    AttributeDeclaration,
    AttributeSpecification,
    DisconnectionSpecification,
    UseClause,
    GroupTemplateDeclaration,
    GroupDeclaration>;

struct EntityDesignator : AstNode {
    EntityTag entity_tag;
    std::optional<Signature> signature;
};

class EntityHeader : AstNode {
    std::unique_ptr<GenericClause> generic_clause; // Optional
    std::unique_ptr<PortClause> port_clause; // Optional
};

using EntityNameList = std::variant<
    std::vector<EntityDesignator>,
    Keyword::Others,
    Keyword::All>;

struct EntitySpecification : AstNode {
    EntityNameList entity_name_list;
    EntityClass entity_class;
};

using EntityStatement = std::variant<
    ConcurrentAssertion,
    // Must be a passive procedure call
    ConcurrentProcedureCall,
    // Must be a passive process
    ProcessStatement>;

using EntityTag = std::variant<
    // simple_name
    Identifier,
    Lexer::CharacterLiteral,
    OperatorSymbol>;

using EnumerationLiteral = std::variant<Identifier, Lexer::CharacterLiteral>;

struct EnumerationTypeDefinition : AstNode {
    std::vector<EnumerationLiteral> enumeration_literals;
};

struct ExitStatement : AstNode {
    std::optional<Identifier> label;
    // Optional label, must be a loop label
    std::optional<Identifier> loop_label;
    // Must be a boolean expression
    std::unique_ptr<Expression> condition; // Optional
};

struct ExpandedSelectedName : AstNode {
    SimplePrefix simple_prefix;
    Suffix suffix;
};

using SimplePrefix = std::variant<
    std::unique_ptr<ExpandedSelectedName>,
    // simple_name
    Identifier>;

enum class RelationKind {
    AND,
    OR,
    XOR,
    nand,
    nor,
    xnor,
};

struct Expression : AstNode {
    std::vector<Relation> relations;

    // Assert: (relations.size() == 1) implies (kind == RelationKind::NONE)
    RelationKind kind;
};

enum class FactorKind {
    exp,
    abs,
    NOT,

    // No operator, just a single primary
    NONE
};

struct Factor : AstNode {
    Primary primary_left;
    std::optional<Primary> primary_right;

    // Assert: (primary_right == std::nullopt) iff (FactorKind::NONE)
    FactorKind kind { FactorKind::NONE };
};

struct FileDeclaration : AstNode {
    std::vector<Identifier> identifier_list;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    std::optional<FileOpenInformation> file_open_information { std::nullopt };
};

struct FileOpenInformation : AstNode {
    // Must be a string expression
    std::unique_ptr<Expression> file_logical_name;
    // Must be a FILE_OPEN_KIND expression
    std::unique_ptr<Expression> file_open_kind { nullptr }; // Optional
};

struct FileTypeDefinition : AstNode {
    // Name must be a type mark
    Name type_mark;
};

struct FormalDesignator : AstNode {
    // Name must be a GENERIC/PORT/PARAMETER name
    std::unique_ptr<Name> name;
};

struct FormalPart : AstNode {
    // If formal_designator is not nullopt, then the "name" must be a function name
    // or a type mark.
    // Name must be a GENERIC/PORT/PARAMETER name
    Name formal_designator;
    // Name must be a GENERIC/PORT/PARAMETER/FUNCTION/TYPE_MARK name
    std::optional<Name> name { std::nullopt }; // Optional
};

struct TypeDeclaration : AstNode {
    Identifier identifier;
    // Can be optional for incomplete type declaration
    std::optional<TypeDefinition> type_definition { std::nullopt };
};

struct FunctionCall : AstNode {
    // Name must be a FUNCTION name
    Name function_name;
    // Must be a PARAMETER association list
    std::vector<AssociationElement> association_list_opt; // Optional
};

struct GenerateStatement : AstNode {
    Identifier label;
    GenerationScheme generation_scheme;
    std::vector<BlockDeclarativeItem> declarative_statements; // Optional
    std::vector<ConcurrentStatement> concurrent_statements; // Optional
    // Optional, must be equal to "label" if defined
    std::optional<Identifier> end_label; // Optional
};

using GenerationScheme = std::variant<
    // FOR generation scheme
    // Must be a GENERATION parameter specification
    ParameterSpecification,
    // IF generation scheme
    // Must be a boolean expression
    std::unique_ptr<Expression>>;

struct GenericClause : AstNode {
    std::vector<InterfaceConstantDeclaration> generic_list;
};

struct GenericMapAspect : AstNode {
    // Must be a GENERIC association list
    std::vector<AssociationElement> association_list;
};

using GroupConstituent = std::variant<Lexer::CharacterLiteral, Name>;

struct GroupDeclaration : AstNode {
    Identifier identifier;
    Name name;
    std::vector<GroupConstituent> group_constituent_list;
};

struct GroupTemplateDeclaration : AstNode {
    Identifier identifier;
    std::vector<EntityClassEntry> entity_class_entry_list;
};

struct GuardedSignalSpecification : AstNode {
    // Must be a list of guarded signals
    SignalList signal_list;
    // Must be a TYPE or a SUBTYPE name
    Name type_mark;
};

using Identifier = std::variant<Lexer::BasicIdentifier, Lexer::ExtendedIdentifier>;

using IfStatementPair = std::pair<
    // Must be a boolean condition
    std::unique_ptr<Expression>,
    std::vector<SequentialStatement>>;

struct IfStatement : AstNode {
    std::optional<Identifier> label; // Optional
    std::vector<IfStatementPair> if_statements;
    std::vector<SequentialStatement> else_statement; // Optional
    std::optional<Identifier> label_end; // Optional
};

// TODO: Maybe remove?
struct IndexConstraint : AstNode {
    std::vector<DiscreteRange> discrete_ranges;
};

using IndexSpecification = std::variant<
    DiscreteRange,
    // Must be a static expression
    std::unique_ptr<Expression>>;

struct IndexedName : AstNode {
    Prefix prefix;
    std::vector<Expression> expression_list;
};

struct ComponentUnit : AstNode {
    // Must be a COMPONENT name
    Name component_name;
};

struct EntityUnit : AstNode {
    // Must be an ENTITY name
    Name entity_name;
    // Must be an ARCHITECTURE identifier (if used)
    std::optional<Identifier> architecture_identifier { std::nullopt };
};

struct ConfigurationUnit : AstNode {
    // Must be a CONFIGURATION name
    Name configuration_name;
};

using InstantiatedUnit = std::variant<ComponentUnit, EntityUnit, ConfigurationUnit>;

// FIXME: Correctly redefine
using InstantiationList = std::variant<
    // Must be a list of component instance labels
    std::vector<Identifier>,
    Keyword::Others,
    Keyword::All>;

struct InterfaceConstantDeclaration : AstNode {
    std::vector<Identifier> identifier_list;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    // Must be a STATIC expression
    std::optional<Expression> init_expression; // Optional
};

using InterfaceDeclaration = std::variant<
    InterfaceUnresolvedDeclaration,
    InterfaceConstantDeclaration,
    InterfaceSignalDeclaration,
    InterfaceVariableDeclaration,
    InterfaceFileDeclaration>;

struct InterfaceFileDeclaration : AstNode {
    std::vector<Identifier> identifier_list;
    std::unique_ptr<SubtypeIndication> subtype_indication;
};

enum class Mode {
    in,
    out,
    inout,
    buffer,
    linkage
};

struct InterfaceSignalDeclaration : AstNode {
    std::vector<Identifier> identifier_list;
    std::optional<Mode> mode;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    bool is_bus;
    // Must be a STATIC expression
    std::optional<Expression> init_expression;
};

struct InterfaceUnresolvedDeclaration : AstNode {
    std::vector<std::unique_ptr<Identifier>> identifier_list;
    std::optional<Mode> mode;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    bool is_bus;
    // Must be a STATIC expression
    std::optional<Expression> init_expression;
};

struct InterfaceVariableDeclaration : AstNode {
    std::vector<Identifier> identifier_list;
    std::optional<Mode> mode;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    // Must be a STATIC expression
    std::optional<Expression> init_expression;
};

using IterationScheme = std::variant<
    // WHILE condition
    // Must be a boolean expression (i.e., a condition)
    std::unique_ptr<Expression>,
    // FOR condition
    // Must be a LOOP parameter specification
    std::unique_ptr<ParameterSpecification>>;

struct LibraryClause : AstNode {
    std::vector<Identifier> logical_name_list;
};

using LibraryUnit = std::variant<PrimaryUnit, SecondaryUnit>;

using Literal = std::variant<
    NumericLiteral,
    EnumerationLiteral,
    Lexer::StringLiteral,
    Lexer::BitStringLiteral,
    Keyword::Null>;

struct LoopStatement : AstNode {
    std::optional<Identifier> label;
    std::optional<IterationScheme> iteration_scheme;
    std::vector<SequentialStatement> sequence_of_statements;
    // Optional
    std::optional<Identifier> end_label;
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
    std::optional<Identifier> statement_label;
    std::optional<Identifier> next_label;
    // Optional, expression must be a boolean condition
    std::optional<Expression> when_condition;
};

struct NullStatement : AstNode {
    std::optional<Identifier> label; // Optional
};

using NumericLiteral = std::variant<AbstractLiteral, PhysicalLiteral>;

struct OperatorSymbol : AstNode {
    Lexer::StringLiteral operator_symbol;
};

struct Options : AstNode {
    bool is_guarded;
    std::optional<DelayMechanism> delay_mechanism;
};

struct PackageBody : AstNode {
    std::unique_ptr<Identifier> package_simple_name;
    std::vector<PackageBodyDeclarativeItem> declarative_part; // Optional
    // Optional, should be equal to "package_simple_name"
    std::optional<Identifier> simple_name_opt;
};

using PackageBodyDeclarativeItem = std::variant<
    AliasDeclaration,
    ConstantDeclaration,
    FileDeclaration,
    GroupDeclaration,
    GroupTemplateDeclaration,
    SharedVariableDeclaration,
    SubprogramBody,
    // subprogram declaration
    SubprogramSpecification,
    SubtypeDeclaration,
    TypeDeclaration,
    UseClause>;

struct PackageDeclaration : AstNode {
    Identifier identifier;
    std::vector<PackageDeclarativeItem> declarative_part;
    // Optional, should be equal to the "identifier" if used
    std::optional<Identifier> simple_name_opt;
};

using PackageDeclarativeItem = std::variant<
    AliasDeclaration,
    AttributeDeclaration,
    AttributeSpecification,
    ComponentDeclaration,
    ConstantDeclaration,
    DisconnectionSpecification,
    FileDeclaration,
    GroupDeclaration,
    GroupTemplateDeclaration,
    SharedVariableDeclaration,
    SignalDeclaration,
    // subprogram declaration
    SubprogramSpecification,
    SubtypeDeclaration,
    TypeDeclaration,
    UseClause>;

struct ParameterSpecification : AstNode {
    Identifier identifier;
    DiscreteRange discrete_range;
};

struct PhysicalLiteral : AstNode {
    Name name;
    std::optional<AbstractLiteral> abstract_literal { std::nullopt };
};

struct PhysicalTypeDefinition : AstNode {
    Range range_constraint;
    Identifier primary_unit_declaration;
    std::vector<SecondaryUnitDeclaration> secondary_unit_declarations;
    std::optional<Identifier> simple_name_opt;
};

struct PortClause : AstNode {
    std::vector<InterfaceSignalDeclaration> port_list;
};

struct PortMapAspect : AstNode {
    // Must be a PORT association list
    std::vector<AssociationElement> association_list;
};

using Prefix = std::variant<Name, FunctionCall>;

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
    EntityDeclaration,
    ConfigurationDeclaration,
    PackageDeclaration>;

struct ProcedureCall : AstNode {
    Name procedure_name;
    // Must be a PARAMETER association list
    std::optional<std::vector<AssociationElement>> association_list { std::nullopt }; // Optional
};

struct ProcedureCallStatement : ProcedureCall {
    std::optional<Identifier> label;
};

using ProcessDeclarativeItem = std::variant<
    AliasDeclaration,
    AttributeDeclaration,
    AttributeSpecification,
    ConstantDeclaration,
    FileDeclaration,
    GroupDeclaration,
    GroupTemplateDeclaration,
    SubprogramBody,
    // subprogram declaratin
    SubprogramSpecification,
    SubtypeDeclaration,
    TypeDeclaration,
    UseClause,
    VariableDeclaration>;

struct ProcessStatement : AstNode {
    std::optional<Identifier> label;
    bool is_postponed;
    // Names must be SIGNAL names
    std::optional<std::vector<Name>> sensitivity_list; // Optional
    std::vector<ProcessDeclarativeItem> declarative_part; // Optional
    std::vector<SequentialStatement> statement_part; // Optional
    // Must be equal to is_postponed
    // FIXME: Change grammar?
    bool end_is_postponed;
    // Optional, must be equal to label if defined
    std::optional<Identifier> end_label; // Optional
};

struct ProtectedTypeBody : AstNode {
    std::vector<ProtectedTypeBodyDeclarativeItem> declarative_part;
    std::optional<Identifier> simple_name_opt;
};

using ProtectedTypeBodyDeclarativeItem = std::variant<
    AliasDeclaration,
    AttributeDeclaration,
    AttributeSpecification,
    ConstantDeclaration,
    FileDeclaration,
    GroupDeclaration,
    GroupTemplateDeclaration,
    SubprogramBody,
    // subprogram declaration
    SubprogramSpecification,
    SubtypeDeclaration,
    TypeDeclaration,
    UseClause,
    VariableDeclaration>;

struct ProtectedTypeDeclaration : AstNode {
    std::vector<ProtectedTypeDeclarativeItem> declarative_items;
    std::optional<Identifier> simple_name_opt;
};

using ProtectedTypeDeclarativeItem = std::variant<
    SubprogramSpecification,
    AttributeSpecification,
    UseClause>;

using ProtectedTypeDefinition = std::variant<
    ProtectedTypeDeclaration,
    ProtectedTypeBody>;

struct QualifiedExpression : AstNode {
    Name type_mark;
    std::optional<Expression> expression;
    std::optional<std::unique_ptr<Aggregate>> aggregate;
};

enum class Direction {
    to,
    downto
};

struct SimpleRange : AstNode {
    std::unique_ptr<SimpleExpression> simple_expression;
    Direction direction;
    std::unique_ptr<SimpleExpression> simple_expression;
};

// Must be an ATTRIBUTE name
using Range = std::variant<Name, SimpleRange>;

struct RecordTypeDefinition : AstNode {
    std::vector<ElementDeclaration> element_declaration_list;
    std::optional<Identifier> simple_name_opt;
};

struct Relation : AstNode {
    std::unique_ptr<ShiftExpression> left_shift_expression;
    std::optional<RelOp> relational_operator { std::nullopt };
    std::optional<std::unique_ptr<ShiftExpression>> right_shift_expression { std::nullopt };
};

enum class RelOp {
    eq,
    neq,
    lt,
    lte,
    gt,
    gte
};

struct ReportStatement : AstNode {
    std::optional<Identifier> label;
    Expression report_expression;
    std::optional<Expression> severity_expression { std::nullopt };
};

struct ReturnStatement : AstNode {
    std::optional<Identifier> label { std::nullopt };
    std::optional<Expression> return_expression { std::nullopt };
};

using ScalarTypeDefinition = std::variant<
    EnumerationTypeDefinition,
    // range constraint
    Range,
    PhysicalTypeDefinition>;

using SecondaryUnit = std::variant<
    ArchitectureBody,
    PackageBody>;

struct SecondaryUnitDeclaration : AstNode {
    Identifier identifier;
    std::unique_ptr<PhysicalLiteral> physical_literal;
};

// TODO: Maybe convert to vector of prefixes/sufixes
struct SelectedName : AstNode {
    Prefix prefix;
    Suffix suffix;
};

struct SelectedSignalAssignment : AstNode {
    Expression with_expression;
    Target target;
    Options options;
    std::vector<SelectedWaveform> selected_waveforms;
};

struct SelectedWaveform : AstNode {
    std::unique_ptr<Waveform> waveform;
    std::vector<Choice> choices;
};

using SequentialStatement = std::variant<
    WaitStatement,
    AssertionStatement,
    ReportStatement,
    SignalAssignmentStatement,
    VariableAssignmentStatement,
    ProcedureCallStatement,
    IfStatement,
    CaseStatement,
    LoopStatement,
    NextStatement,
    ExitStatement,
    ReturnStatement,
    NullStatement>;

struct ShiftExpression : AstNode {
    std::unique_ptr<SimpleExpression> left_expression;
    std::optional<ShiftOperator> shift_operator { std::nullopt };
    std::optional<std::unique_ptr<SimpleExpression>> right_expression { std::nullopt };
};

enum class ShiftOperator {
    sll,
    srl,
    sla,
    sra,
    rol,
    ror
};

enum class Sign {
    plus,
    minus
};

struct SignalAssignmentStatement : AstNode {
    std::optional<Identifier> label;
    Target target;
    std::optional<DelayMechanism> delay_mechanism;
    std::unique_ptr<Waveform> waveform;
};

struct SignalDeclaration : AstNode {
    std::vector<Identifier> identifier_list;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    std::optional<SignalKind> signal_kind;
    // TODO: Optional, must be a static expression?
    std::optional<Expression> init_expression;
};

enum class SignalKind {
    reg,
    bus
};

// FIXME: Fix
using SignalList = std::variant<
    // Names must be SIGNAL names
    std::vector<std::unique_ptr<Name>>,
    Keyword::Others,
    Keyword::All>;

struct Signature : AstNode {
    // Name must be a type mark
    std::optional<std::vector<Name>> type_marks;
    // Name must be a type mark
    std::optional<Name> return_type_mark;
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

using TermOpPair = std::pair<AddingOperator, std::unique_ptr<Term>>;

struct SimpleExpression : AstNode {
    std::optional<Sign> sign;
    std::unique_ptr<Term> term;
    std::vector<TermOpPair> adding_terms; // Optional (can be empty)
};

struct SliceName : AstNode {
    Prefix prefix;
    DiscreteRange discrete_range;
};

struct SubprogramBody : AstNode {
    SubprogramSpecification subprogram_specification;
    std::vector<SubprogramDeclarativeItem> declarative_part; // Optional
    std::vector<SequentialStatement> statement_part; // Optional
    // Subprogram kind (optional), must be same as identifier in specification
    std::optional<SubprogramKind> subprogram_kind;
    std::optional<Designator> designator;
};

using SubprogramDeclarativeItem = std::variant<
    // subprogram declaration
    SubprogramSpecification,
    SubprogramBody,
    TypeDeclaration,
    SubtypeDeclaration,
    ConstantDeclaration,
    VariableDeclaration,
    FileDeclaration,
    AliasDeclaration,
    AttributeDeclaration,
    AttributeSpecification,
    UseClause,
    GroupTemplateDeclaration,
    GroupDeclaration>;

enum class SubprogramKind {
    function,
    procedure,
};

struct ProcedureSpecification : AstNode {
    Designator designator;
    // Must be a parameter interface list
    std::optional<std::vector<InterfaceDeclaration>> formal_parameter_list { std::nullopt };
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
    Name return_type;
};

using SubprogramSpecification = std::variant<
    ProcedureSpecification,
    FunctionSpecification>;

struct SubtypeDeclaration : AstNode {
    Identifier identifier;
    SubtypeIndication subtype_indication;
};

struct SubtypeIndication : AstNode {
    // Optional, desgiantor must be a RESOLUTION FUNCTION name
    std::optional<Designator> resolution_function_name;
    // Name must be a TYPE or SUBTYPE name
    Name type_mark;
    // A range or an index constraint (optional)
    std::optional<Constraint> constraint;
};

using Suffix = std::variant<
    // TODO: Must be a simple name?
    Identifier,
    Lexer::CharacterLiteral,
    OperatorSymbol,
    Keyword::All>;

using Aggregate = std::vector<ElementAssociation>;

using Target = std::variant<Name, Aggregate>;

enum class MulOp {
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
    std::unique_ptr<Term> term; // Optional
    std::optional<MulOp> multiplying_operator;
    std::unique_ptr<Factor> factor;
};

struct TypeConversion : AstNode {
    // Name must be a TYPE or a SUBTYPE name
    Name type_mark;
    Expression expression;
};

using TypeDefinition = std::variant<
    ScalarTypeDefinition,
    CompositeTypeDefinition,
    AccessTypeDefinition,
    FileTypeDefinition,
    ProtectedTypeDefinition>;

struct UseClause : AstNode {
    std::vector<ExpandedSelectedName> selected_name;
};

struct VariableAssignmentStatement : AstNode {
    std::optional<Identifier> label;
    Target assignment_target;
    Expression expression;
};

using VariableDeclaration = std::variant<
    PlainVariableDeclaration,
    SharedVariableDeclaration>;

struct PlainVariableDeclaration : AstNode {
    std::vector<Identifier> identifier_list;
    std::unique_ptr<SubtypeIndication> subtype_indication;
    std::optional<Expression> init_expression;
};

struct SharedVariableDeclaration : PlainVariableDeclaration {
    // TODO: Any differences?
};

struct WaitStatement : AstNode {
    std::optional<Identifier> label;
    // Name must be a SIGNAL name
    // SYNTAX: ON sensitivity_list
    std::optional<std::vector<Name>> sensitivity_list;
    // Expression must be a CONDITION (i.e., have a boolean type)
    std::optional<Expression> until_condition_clause;
    // SYNTAX: FOR time_expression
    // Expression must be a TIME expression
    std::optional<Expression> for_timeout_clause;
};

using Waveform = std::variant<
    std::vector<WaveformElement>,
    Keyword::Unaffected>;

using WaveformElement = std::variant<
    WaveformElementKind::ValueExpression,
    WaveformElementKind::NullExpression>;

namespace WaveformElementKind {
    struct ValueExpression : AstNode {
        // Expression must be a VALUE expression
        Expression value_expression;
        // Expression must be a TIME expression
        std::optional<Expression> time_expression { std::nullopt };
    };

    struct NullExpression : AstNode {
        // Expression must be a TIME expression
        std::optional<std::unique_ptr<Expression>> time_expression { std::nullopt };
    };
}

} // namespace parser

#endif /* PARSER_DEFS */
