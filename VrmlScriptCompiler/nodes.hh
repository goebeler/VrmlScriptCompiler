#pragma once
#include "VrmlVariant.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "fmt/format.h"

namespace vrmlast {

enum class RelationalOperatorEnum { EQ, NE, LT, LE, GT, GE };
enum class LogicalOperatorEnum { LAND, LOR };
enum class CompoundAssignOperatorEnum {
    PLUS_EQ,
    MINUS_EQ,
    MULTIPLY_EQ,
    DIVIDE_EQ,
    MOD_EQ,
    LSHIFT_EQ,
    RSHIFT_EQ,
    RSHIFTFILL_EQ,
    AND_EQ,
    XOR_EQ,
    OR_EQ
};

class ArgumentList;
class FunctionDefinition;
class ParameterList;
class FunctionCallExpression;
class Statement;
class AssignmentExpression;
class BinaryArithmeticExpression;
class BinaryRelationalExpression;
class BinaryLogicalExpression;
class VariableExpression;
class IntConstantExpression;
class Expression;
class StatementList;
class Script;
class FunctionDefinitionList;
class Block;
class ReturnStatement;
class IfStatement;
class WhileStatement;
class NewExpression;
class NullLiteralExpression;
class UnaryNotExpression;
class CompoundAssignmentExpression;

class ASTVisitor {
public:
  virtual void visit(ArgumentList *args) = 0;
  virtual void visit(FunctionDefinition *func) = 0;
  virtual void visit(ParameterList *params) = 0;
  virtual void visit(FunctionCallExpression *call) = 0;
  virtual void visit(Statement *statement) = 0;
  virtual void visit(AssignmentExpression *a) = 0;
  virtual void visit(VariableExpression *a) = 0;
  virtual void visit(IntConstantExpression *a) = 0;
  virtual void visit(StatementList *s) = 0;
  virtual void visit(Expression *s) = 0;
  virtual void visit(Script *s) = 0;
  virtual void visit(FunctionDefinitionList *s) = 0;
  virtual void visit(BinaryArithmeticExpression *s) = 0;
  virtual void visit(Block *s) = 0;
  virtual void visit(BinaryRelationalExpression *s) = 0;
  virtual void visit(BinaryLogicalExpression *s) = 0;
  virtual void visit(UnaryNotExpression *s) = 0;
  virtual void visit(NullLiteralExpression *s) = 0;
  virtual void visit(ReturnStatement *s) = 0;
  virtual void visit(IfStatement *s) = 0;
  virtual void visit(WhileStatement *s) = 0;
  virtual void visit(NewExpression *s) = 0;
  virtual void visit(CompoundAssignmentExpression *s) = 0;
};

class ASTNode {
public:
  virtual ~ASTNode() {};
  virtual std::string to_string() const = 0;
  virtual void accept(ASTVisitor &visitor) = 0;
};

class Scope {
public:
  std::map<std::string, std::unique_ptr<VariableExpression>> m_members;
  Scope *m_parent{nullptr};
};



class Expression : public ASTNode {
public:
  std::string m_type;
  // std::string m_value;

  // Geerbt über ASTNode
  virtual std::string to_string() const = 0;
  virtual void accept(ASTVisitor &visitor) = 0;
  virtual vrmlscript::VrmlVariant evaluate() {
    return vrmlscript::VrmlVariant(std::monostate());
  }
};

class UnaryNotExpression : public Expression {
public:
  Expression *m_expr{nullptr};
  explicit UnaryNotExpression(Expression *e) : m_expr(e) {}
  std::string to_string() const override { return "!(" + (m_expr?m_expr->to_string():"") + ")"; }
  void accept(ASTVisitor &visitor) override { visitor.visit(this); }
};

class BinaryRelationalExpression : public Expression {
public:
  Expression *m_lhs{nullptr};
  Expression *m_rhs{nullptr};
  RelationalOperatorEnum m_op{RelationalOperatorEnum::EQ};
  BinaryRelationalExpression() = default;
  BinaryRelationalExpression(RelationalOperatorEnum op, Expression *lhs, Expression *rhs)
      : m_lhs(lhs), m_rhs(rhs), m_op(op) {}
  std::string to_string() const override;
  void accept(ASTVisitor &visitor) override { visitor.visit(this); }
};

class BinaryLogicalExpression : public Expression {
public:
  Expression *m_lhs{nullptr};
  Expression *m_rhs{nullptr};
  LogicalOperatorEnum m_op{LogicalOperatorEnum::LAND};
  BinaryLogicalExpression() = default;
  BinaryLogicalExpression(LogicalOperatorEnum op, Expression *lhs, Expression *rhs)
      : m_lhs(lhs), m_rhs(rhs), m_op(op) {}
  std::string to_string() const override;
  void accept(ASTVisitor &visitor) override { visitor.visit(this); }
};

class NullLiteralExpression : public Expression {
public:
  std::string to_string() const override { return "null"; }
  void accept(ASTVisitor &visitor) override { visitor.visit(this); }
};

enum ArithmeticOperatorEnum { PLUS, MULTIPLY, DIVIDE, MOD, MINUS };

class BinaryArithmeticExpression : public Expression {
public:
  Expression *m_lhs;
  Expression *m_rhs;
  ArithmeticOperatorEnum m_op;

  BinaryArithmeticExpression() : m_lhs(nullptr), m_rhs(nullptr), m_op(PLUS) {}
  BinaryArithmeticExpression(ArithmeticOperatorEnum arithmetic_operator,
                             Expression *lhs, Expression *rhs)
      : m_lhs(lhs), m_rhs(rhs), m_op(arithmetic_operator) {}

  // Geerbt über Expression
  [[nodiscard]] std::string to_string() const override;
  void accept(ASTVisitor &visitor) override;
  vrmlscript::VrmlVariant evaluate() override;
};

class Script : public ASTNode {
public:
  FunctionDefinitionList *m_functions;
  Scope m_scope;
  // Geerbt über ASTNode
  virtual std::string to_string() const override;
  virtual void accept(ASTVisitor &visitor) override;
};

/* Lvalues are Expressions that can appear on the LHS of an assignment. */
class LValueExpression : public Expression {
public:
  virtual ~LValueExpression() = default;
};

class VariableExpression : public LValueExpression {
public:
  VariableExpression() {}
  VariableExpression(std::string name,
                     vrmlscript::VrmlVariant value = std::monostate{})
      : m_name{name}, m_value{value} {}

  std::string m_name;
  vrmlscript::VrmlVariant m_value;
  // Geerbt über ASTNode
  virtual std::string to_string() const override;
  virtual void accept(ASTVisitor &visitor) override;
};

class VariableDeclarationExpression : public Expression {
public:
  VariableDeclarationExpression() {}
  VariableDeclarationExpression(
      std::string name, vrmlscript::VrmlVariant value = std::monostate{})
      : m_name{name} {}

  std::string m_name;
  Expression *m_initializer{nullptr};

  // Geerbt über ASTNode
  virtual std::string to_string() const override;
  virtual void accept(ASTVisitor &visitor) override;
};

class IntConstantExpression : public Expression {
public:
  int m_value;

  void set_value(int value) { m_value = value; }
  // Geerbt über ASTNode
  virtual std::string to_string() const override;
  virtual void accept(ASTVisitor &visitor) override;
};

class AssignmentExpression : public Expression {
public:
  LValueExpression *m_lhs;
  Expression *m_rhs;

  // Geerbt über ASTNode
  virtual std::string to_string() const override;
  virtual void accept(ASTVisitor &visitor) override;
};

class CompoundAssignmentExpression : public AssignmentExpression {
public:
  CompoundAssignOperatorEnum m_op{CompoundAssignOperatorEnum::PLUS_EQ};

  CompoundAssignmentExpression() = default;

  [[nodiscard]] static constexpr std::string_view op_to_string(
      CompoundAssignOperatorEnum op) noexcept {
    using enum CompoundAssignOperatorEnum;
    switch (op) {
      case PLUS_EQ:        return "+=";
      case MINUS_EQ:       return "-=";
      case MULTIPLY_EQ:    return "*=";
      case DIVIDE_EQ:      return "/=";
      case MOD_EQ:         return "%=";
      case LSHIFT_EQ:      return "<<=";
      case RSHIFT_EQ:      return ">>=";
      case RSHIFTFILL_EQ:  return ">>>=";
      case AND_EQ:         return "&=";
      case XOR_EQ:         return "^=";
      case OR_EQ:          return "|=";
    }
    return "?=";
  }

  std::string to_string() const override {
    return fmt::format("{} {} {}",
                       m_lhs ? m_lhs->to_string() : "<null>",
                       op_to_string(m_op),
                       m_rhs ? m_rhs->to_string() : "<null>");
  }

  void accept(ASTVisitor &visitor) override { visitor.visit(this); }
};

class Statement : public ASTNode {
public:
  std::vector<Expression *> m_expressions;
  void add_expression(Expression *exp);

  // Geerbt über ASTNode
  virtual std::string to_string() const override;
  virtual void accept(ASTVisitor &visitor) override;
};

class ReturnStatement : public Statement {
public:
  Expression *m_expression{nullptr};
  std::string to_string() const override {
    return std::string("return ") + (m_expression?m_expression->to_string():"") + ";";
  }
  void accept(ASTVisitor &visitor) override { visitor.visit(this); }
};

class IfStatement : public Statement {
public:
  Expression *m_condition{nullptr};
  Block *m_then{nullptr};
  Block *m_else{nullptr};
  std::string to_string() const override;
  void accept(ASTVisitor &visitor) override { visitor.visit(this); }
};

class WhileStatement : public Statement {
public:
  Expression *m_condition{nullptr};
  Block *m_body{nullptr};
  std::string to_string() const override;
  void accept(ASTVisitor &visitor) override { visitor.visit(this); }
};

class ArgumentList : public ASTNode {
public:
  std::vector<Expression *> m_arguments;

  ArgumentList() {}

  void add_argument(Expression *expression);

  // Geerbt über ASTNode
  std::string to_string() const override;
  void accept(ASTVisitor &visitor) override;
};

class NewExpression : public Expression {
public:
  std::string m_type_name;
  ArgumentList *m_arguments{nullptr};
  std::string to_string() const override { return "new " + m_type_name + "(" + (m_arguments?m_arguments->to_string():"") + ")"; }
  void accept(ASTVisitor &visitor) override { visitor.visit(this); }
};

class StatementList : public ASTNode {
public:
  std::vector<Statement *> m_statements;
  void add_statement(Statement *statement);
  friend std::ostream &operator<<(std::ostream &out,
                                  const StatementList &statement_list);

  // Geerbt über ASTNode
  std::string to_string() const override;
  void accept(ASTVisitor &visitor) override;
};

class FunctionDefinition : public ASTNode {
public:
  std::string m_name;
  ParameterList *m_parameter_list{nullptr};
  Statement *m_statement{nullptr};
  Scope m_scope;

  FunctionDefinition() = default;
  void set_name(std::string name) { m_name = std::move(name); }
  void set_arguments(ParameterList *arguments) { m_parameter_list = arguments; }
  void set_statement(Statement *statement) { m_statement = statement; }

  // Geerbt über ASTNode
  std::string to_string() const override;
  void accept(ASTVisitor &visitor) override;
};

class FunctionDefinitionList : public ASTNode {
public:
  std::vector<FunctionDefinition *> m_functions;
  void add_function(FunctionDefinition *func);
  Scope m_scope;

  // Geerbt über ASTNode
  std::string to_string() const override;
  void accept(ASTVisitor &visitor) override;
};

class Block : public Statement {
public:
  Block() = default;
  Scope m_scope;
  StatementList *m_statements{nullptr};
  std::string to_string() const override;
  void accept(ASTVisitor &visitor) override;
};

class ParameterList : public ASTNode {
public:
  std::vector<std::string> m_parameters;
  void add_parameter(std::string name);

  // Geerbt über ASTNode
  std::string to_string() const override;
  void accept(ASTVisitor &visitor) override;
};

class FunctionCallExpression : public Expression {
public:
  std::string m_function_name;
  ArgumentList *m_argument_list;
  Expression *m_this{nullptr};
  Expression *m_callee{nullptr};

  [[nodiscard]] std::string to_string() const override;

  FunctionCallExpression() : m_argument_list(nullptr) {}

  void accept(ASTVisitor &visitor) override;
};

class IndexExpression : public LValueExpression {
public:
  Expression *m_collection_expression;
  Expression *m_index_expression;
  [[nodiscard]] std::string to_string() const override;

  IndexExpression()
      : m_collection_expression(nullptr), m_index_expression(nullptr) {}

  void accept(ASTVisitor &visitor) override;
};

class MemberAccessExpression : public LValueExpression {
public:
  Expression *m_object;
  std::string m_member_name;
  [[nodiscard]] std::string to_string() const override;

  MemberAccessExpression() : m_object(nullptr), m_member_name{} {}

  void accept(ASTVisitor &visitor) override;
};

class StringConstantExpression : public Expression {
public:
  std::string m_value;

  [[nodiscard]] std::string to_string() const override {
    // Re-emit with single quotes, preserving exactly what the user wrote:
    // escaped sequences stay escaped; literal newlines remain literal.
    return "'" + m_value + "'";
  }
  void accept(ASTVisitor &visitor) override { visitor.visit(static_cast<Expression*>(this)); }
};

// inline std::ostream& operator<<(std::ostream& out, const Expression* node)
//{
//	out << node->to_string();
//	return out;
// }

inline std::ostream &operator<<(std::ostream &out, const ASTNode *node) {
  if (!node) {
    return out;
  }

  out << node->to_string();
  return out;
}
} // namespace vrmlast
