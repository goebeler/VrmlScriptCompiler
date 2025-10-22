#include "ast_json.hpp"
#include <utility> // std::move
#include <string>

namespace vrmlast {

	using json = nlohmann::json;

	void JsonAstVisitor::append_child(json& parent, const char* key, json child) {
		if (!parent.contains(key)) parent[key] = json::array();
		parent[key].push_back(std::move(child));
	}

	constexpr std::string_view JsonAstVisitor::to_op_string(ArithmeticOperatorEnum op) {
		switch (op) {
		case ArithmeticOperatorEnum::PLUS: return "+";
		case ArithmeticOperatorEnum::DIVIDE: return "/";
		case ArithmeticOperatorEnum::MINUS: return "-";
		case ArithmeticOperatorEnum::MOD: return "%";
		case ArithmeticOperatorEnum::MULTIPLY: return "*";
		default: return "?";
		}
	}

	constexpr std::string_view JsonAstVisitor::to_op_string(RelationalOperatorEnum op) {
		switch (op) {
		case RelationalOperatorEnum::EQ: return "==";
		case RelationalOperatorEnum::GE: return ">=";
		case RelationalOperatorEnum::GT: return ">";
		case RelationalOperatorEnum::LE: return "<=";
		case RelationalOperatorEnum::LT: return "<";
		case RelationalOperatorEnum::NE: return "!=";
		default: return "?";
		}
	}

	constexpr std::string_view JsonAstVisitor::to_op_string(LogicalOperatorEnum op) {
		switch (op) {
		case LogicalOperatorEnum::LAND: return "&&";
		case LogicalOperatorEnum::LOR: return "||";
		default: return "?";
		}
	}

	void JsonAstVisitor::visit(Statement* statement)
	{
		json j{
			{"kind","Statement"},
			{"expressions", json::array()}
		};
		for (auto* f : statement->m_expressions) {
			if (!f) continue;
			f->accept(*this);
			append_child(j, "expressions", pop());
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(Expression* s)
	{
		json j{
			{"kind","Expression"},
			{"type", s->m_type}
		};

		push(std::move(j));
	}

	void JsonAstVisitor::visit(BinaryRelationalExpression* s)
	{
		json j{
			{"kind","BinaryRelationalExpression"},
			{"op", JsonAstVisitor::to_op_string(s->m_op) },
			{"lhs", nullptr},
			{"rhs", nullptr}
		};
		
		if(s->m_rhs){
			s->m_rhs->accept(*this);
			j["rhs"] = pop();
		}

		if(s->m_lhs){
			s->m_lhs->accept(*this);
			j["lhs"] = pop();
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(BinaryLogicalExpression* s)
	{
		json j{
			{"kind","BinaryLogicalExpression"},
			{"op", JsonAstVisitor::to_op_string(s->m_op) },
			{"lhs", nullptr},
			{"rhs", nullptr}
		};
		
		if(s->m_rhs){
			s->m_rhs->accept(*this);
			j["rhs"] = pop();
		}

		if(s->m_lhs){
			s->m_lhs->accept(*this);
			j["lhs"] = pop();
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(UnaryNotExpression* s)
	{
		json j{
			{"kind","UnaryNotExpression"},
			{"expression", nullptr}
		};
		
		if(s->m_expr){
			s->m_expr->accept(*this);
			j["expression"] = pop();
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(NullLiteralExpression* s)
	{
		json j{
			{"kind","NullLiteralExpression"}
		};

		push(std::move(j));
	}

	void JsonAstVisitor::visit(ReturnStatement* s)
	{
		json j{
			{"kind","ReturnStatement"},
			{"expression", nullptr}
		};
		
		if(s->m_expression){
			s->m_expression->accept(*this);
			j["expression"] = pop();
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(IfStatement* s)
	{
		json j{
			{"kind","IfStatement"},
			{"condition", nullptr},
			{"then", nullptr},
			{"else", nullptr}
		};
		
		if(s->m_condition){
			s->m_condition->accept(*this);
			j["condition"] = pop();
		}

		if(s->m_then){
			s->m_then->accept(*this);
			j["then"] = pop();
		}

		if(s->m_else){
			s->m_else->accept(*this);
			j["else"] = pop();
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(WhileStatement* s)
	{
		json j{
			{"kind","WhileStatement"},
			{"condition", nullptr},
			{"body", nullptr}
		};
		
		if(s->m_condition){
			s->m_condition->accept(*this);
			j["condition"] = pop();
		}

		if(s->m_body){
			s->m_body->accept(*this);
			j["body"] = pop();
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(NewExpression* s)
	{
		json j{
			{"kind","NewExpression"},
			{"type_name", s->m_type_name},
			{"arguments", nullptr}
		};
		
		if(s->m_arguments){
			s->m_arguments->accept(*this);
			j["arguments"] = pop();
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(CompoundAssignmentExpression* s)
	{
		json j{
			{"kind","CompoundAssignmentExpression"},
			{"lhs", nullptr},
			{"rhs", nullptr}
		};
		if (s->m_lhs) {
			s->m_lhs->accept(*this);
			j["lhs"] = pop();
		}
		if (s->m_rhs) {
			s->m_rhs->accept(*this);
			j["rhs"] = pop();
		}
		push(std::move(j));
	}

	// ---------------- Script / Top-Level ----------------

	void JsonAstVisitor::visit(Script* n) {
		json j{
			{"kind","Script"},
			{"functions", json::array()}
		};

		if (n->m_functions) {
			n->m_functions->accept(*this);
			append_child(j, "functions", pop()["functions"]);
		}

		// Root festhalten (oberstes Ergebnis)
		root_ = std::move(j);
		push(root_); // optional, falls Aufrufer pop() erwartet; wir lassen es auf dem Stack.
	}

	void JsonAstVisitor::visit(FunctionDefinitionList* n) {
		json j{
			{"kind","FunctionDefinitionList"},
			{"functions", json::array()}
		};
		for (auto* f : n->m_functions) {
			if (!f) continue;
			f->accept(*this);
			append_child(j, "functions", pop());
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(FunctionDefinition* n) {
		if(!n)
			return;

		json j{
			{"kind","FunctionDefinition"},
			{"name", n ? n->m_name : std::string{}},
			{"params", json::array()},
			{"statement", nullptr}
		};

		if (n->m_parameter_list) {
			n->m_parameter_list->accept(*this);
			j["params"] = pop(); // ParameterList liefert array
		}
		if (n->m_statement) {
			n->m_statement->accept(*this);
			j["statement"] = pop();
		}

		push(std::move(j));
	}

	// ---------------- Statements ----------------

	void JsonAstVisitor::visit(StatementList* n) {
		json arr = json::array();
		for (auto* st : n->m_statements) {
			if (!st) continue;
			st->accept(*this);
			arr.push_back(pop());
		}
		// StatementList selbst geben wir als Array zurück
		push(std::move(arr));
	}

	void JsonAstVisitor::visit(Block* n) {
		json j{
			{"kind","Block"},
			{"statements", json::array()}
		};
		if (n->m_statements) {
			n->m_statements->accept(*this);
			j["statements"] = pop(); // Array aus StatementList
		}
		push(std::move(j));
	}

	// ---------------- Expressions ----------------

	void JsonAstVisitor::visit(AssignmentExpression* n) {
		json j{
			{"kind","AssignmentExpression"},
			{"lhs", nullptr},
			{"rhs", nullptr}
		};
		if (n->m_lhs) {
			n->m_lhs->accept(*this);
			j["lhs"] = pop();
		}
		if (n->m_rhs) {
			n->m_rhs->accept(*this);
			j["rhs"] = pop();
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(VariableExpression* n) {
		json j{
			{"kind","VariableExpression"},
			{"name", n ? n->m_name : std::string{}}
		};
		push(std::move(j));
	}

	void JsonAstVisitor::visit(IntConstantExpression* n) {
		json j{
			{"kind","IntConstantExpression"},
			{"value", n->m_value}
		};
		push(std::move(j));
	}

	void JsonAstVisitor::visit(FunctionCallExpression* n) {
		json j{
			{"kind","FunctionCallExpression"},
			{"name", n->m_function_name},
			{"arguments", json::array()}
		};
		if (n->m_argument_list) {
			n->m_argument_list->accept(*this);
			j["arguments"] = pop(); // ArgumentList liefert array
		}
		push(std::move(j));
	}

	void JsonAstVisitor::visit(BinaryArithmeticExpression* n) {
		json j{
			{"kind","BinaryArithmeticExpression"},
			{"op",  to_op_string(n->m_op)},
			{"lhs", nullptr},
			{"rhs", nullptr}
		};
		if (n->m_lhs) { n->m_lhs->accept(*this); j["lhs"] = pop(); }
		if (n->m_rhs) { n->m_rhs->accept(*this); j["rhs"] = pop(); }
		push(std::move(j));
	}

	void JsonAstVisitor::visit(ParameterList* n) {
		json arr = json::array();
		if (n) {
			for (auto p : n->m_parameters) {
				json jparam{
					{"kind","Parameter"},
					{"name", p}
				};
				arr.push_back(std::move(jparam));
			}
		}
		push(std::move(arr));
	}

	void JsonAstVisitor::visit(ArgumentList* n) {
		json arr = json::array();
		if (n) {
			for (auto* a : n->m_arguments) {
				if (!a) continue;
				a->accept(*this);
				arr.push_back(pop());
			}
		}
		push(std::move(arr));
	}
} // namespace vrmllast
