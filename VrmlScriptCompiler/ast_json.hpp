#pragma once
#include <nlohmann/json.hpp>
#include "nodes.hh"

namespace vrmlast {

	/// Serialisiert den AST mit dem vorhandenen Visitor-Interface in nlohmann::json.
	/// Nutzung:
	///   JsonAstVisitor vis;
	///   root->accept(vis);
	///   nlohmann::json j = vis.take();
	class JsonAstVisitor : public ASTVisitor {
	public:
		using json = nlohmann::json;

		JsonAstVisitor() = default;

		// Nach Abschluss des Besuchs am Root-Knoten abholen.
		json take() {
			return std::move(root_);
		}

		// ---- Script/Top-Level ----
		void visit(Script* n) override;
		void visit(FunctionDefinitionList* n) override;
		void visit(FunctionDefinition* n) override;

		// ---- Statements ----
		void visit(StatementList* n) override;
		void visit(Block* n) override;

		// ---- Expressions ----
		void visit(AssignmentExpression* n) override;
		void visit(VariableExpression* n) override;
		void visit(IntConstantExpression* n) override;
		void visit(FunctionCallExpression* n) override;
		void visit(BinaryArithmeticExpression* n) override;

		// ---- Aux-Listen ----
		void visit(ParameterList* n) override;
		void visit(ArgumentList* n) override;

		// Geerbt über ASTVisitor
		void visit(Statement* statement) override;
		void visit(Expression* s) override;
		void visit(BinaryRelationalExpression* s) override;
		void visit(BinaryLogicalExpression* s) override;
		void visit(UnaryNotExpression* s) override;
		void visit(NullLiteralExpression* s) override;
		void visit(ReturnStatement* s) override;
		void visit(IfStatement* s) override;
		void visit(WhileStatement* s) override;
		void visit(NewExpression* s) override;
		void visit(CompoundAssignmentExpression* s) override;

	private:
		// Stack für "aktuelles JSON-Elternobjekt".
		// Strategie: Jeder visit(...) erzeugt sein JSON und schiebt es als "aktuelles"
		// Objekt; Eltern holen es per pop() und hängen es an.
		std::vector<json> stack_;
		json root_; // Ergebnis nach Besuch des Root-Knotens

		// Hilfen
		void push(json j) { stack_.emplace_back(std::move(j)); }
		json pop() {
			auto j = std::move(stack_.back());
			stack_.pop_back();
			return j;
		}

		// Anhängen eines Kind-JSONs an ein Arrayfeld.
		static void append_child(json& parent, const char* key, json child);

		// Kleiner Helfer für Operator-String (PLUS etc.)
		constexpr std::string_view to_op_string(ArithmeticOperatorEnum op);
		constexpr std::string_view to_op_string(RelationalOperatorEnum op);
		constexpr std::string_view to_op_string(LogicalOperatorEnum op);

		
	};

} // namespace vrmlast
