#include "duckdb/main/relation/update_relation.hpp"
#include "duckdb/parser/statement/update_statement.hpp"
#include "duckdb/planner/binder.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/parser/tableref/basetableref.hpp"

namespace duckdb {

UpdateRelation::UpdateRelation(shared_ptr<ClientContextWrapper> &context, unique_ptr<ParsedExpression> condition_p,
                               string catalog_name_p, string schema_name_p, string table_name_p,
                               vector<string> update_columns_p, vector<unique_ptr<ParsedExpression>> expressions_p)
    : Relation(context, RelationType::UPDATE_RELATION), condition(std::move(condition_p)),
      catalog_name(std::move(catalog_name_p)), schema_name(std::move(schema_name_p)),
      table_name(std::move(table_name_p)), update_columns(std::move(update_columns_p)),
      expressions(std::move(expressions_p)) {
	D_ASSERT(update_columns.size() == expressions.size());
	TryBindRelation(columns);
}

BoundStatement UpdateRelation::Bind(Binder &binder) {
	auto basetable = make_uniq<BaseTableRef>();
	basetable->catalog_name = catalog_name;
	basetable->schema_name = schema_name;
	basetable->table_name = table_name;

	UpdateStatement stmt;
	stmt.set_info = make_uniq<UpdateSetInfo>();

	stmt.set_info->condition = condition ? condition->Copy() : nullptr;
	stmt.table = std::move(basetable);
	stmt.set_info->columns = update_columns;
	for (auto &expr : expressions) {
		stmt.set_info->expressions.push_back(expr->Copy());
	}
	return binder.Bind(stmt.Cast<SQLStatement>());
}

const vector<ColumnDefinition> &UpdateRelation::Columns() {
	return columns;
}

string UpdateRelation::ToString(idx_t depth) {
	string str = RenderWhitespace(depth) + "UPDATE " +
	             ParseInfo::QualifierToString(catalog_name, schema_name, table_name) + " SET\n";
	for (idx_t i = 0; i < expressions.size(); i++) {
		str += update_columns[i] + " = " + expressions[i]->ToString() + "\n";
	}
	if (condition) {
		str += "WHERE " + condition->ToString() + "\n";
	}
	return str;
}

} // namespace duckdb
