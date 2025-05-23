//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/catalog/catalog_entry/duck_schema_entry.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/catalog/catalog_entry/schema_catalog_entry.hpp"

namespace duckdb {

//! A schema in the catalog
class DuckSchemaEntry : public SchemaCatalogEntry {
public:
	DuckSchemaEntry(Catalog &catalog, CreateSchemaInfo &info);

private:
	//! The catalog set holding the tables
	CatalogSet tables;
	//! The catalog set holding the indexes
	CatalogSet indexes;
	//! The catalog set holding the table functions
	CatalogSet table_functions;
	//! The catalog set holding the copy functions
	CatalogSet copy_functions;
	//! The catalog set holding the pragma functions
	CatalogSet pragma_functions;
	//! The catalog set holding the scalar and aggregate functions
	CatalogSet functions;
	//! The catalog set holding the sequences
	CatalogSet sequences;
	//! The catalog set holding the collations
	CatalogSet collations;
	//! The catalog set holding the types
	CatalogSet types;

public:
	optional_ptr<CatalogEntry> AddEntry(CatalogTransaction transaction, unique_ptr<StandardEntry> entry,
	                                    OnCreateConflict on_conflict);
	optional_ptr<CatalogEntry> AddEntryInternal(CatalogTransaction transaction, unique_ptr<StandardEntry> entry,
	                                            OnCreateConflict on_conflict, LogicalDependencyList dependencies);

	optional_ptr<CatalogEntry> CreateTable(CatalogTransaction transaction, BoundCreateTableInfo &info) override;
	optional_ptr<CatalogEntry> CreateFunction(CatalogTransaction transaction, CreateFunctionInfo &info) override;
	optional_ptr<CatalogEntry> CreateIndex(CatalogTransaction transaction, CreateIndexInfo &info,
	                                       TableCatalogEntry &table) override;
	optional_ptr<CatalogEntry> CreateView(CatalogTransaction transaction, CreateViewInfo &info) override;
	optional_ptr<CatalogEntry> CreateSequence(CatalogTransaction transaction, CreateSequenceInfo &info) override;
	optional_ptr<CatalogEntry> CreateTableFunction(CatalogTransaction transaction,
	                                               CreateTableFunctionInfo &info) override;
	optional_ptr<CatalogEntry> CreateCopyFunction(CatalogTransaction transaction,
	                                              CreateCopyFunctionInfo &info) override;
	optional_ptr<CatalogEntry> CreatePragmaFunction(CatalogTransaction transaction,
	                                                CreatePragmaFunctionInfo &info) override;
	optional_ptr<CatalogEntry> CreateCollation(CatalogTransaction transaction, CreateCollationInfo &info) override;
	optional_ptr<CatalogEntry> CreateType(CatalogTransaction transaction, CreateTypeInfo &info) override;
	void Alter(CatalogTransaction transaction, AlterInfo &info) override;
	void Scan(ClientContext &context, CatalogType type, const std::function<void(CatalogEntry &)> &callback) override;
	void Scan(CatalogType type, const std::function<void(CatalogEntry &)> &callback) override;
	void DropEntry(ClientContext &context, DropInfo &info) override;
	optional_ptr<CatalogEntry> LookupEntry(CatalogTransaction transaction, const EntryLookupInfo &lookup_info) override;
	CatalogSet::EntryLookup LookupEntryDetailed(CatalogTransaction transaction,
	                                            const EntryLookupInfo &lookup_info) override;
	SimilarCatalogEntry GetSimilarEntry(CatalogTransaction transaction, const EntryLookupInfo &lookup_info) override;

	unique_ptr<CatalogEntry> Copy(ClientContext &context) const override;

	void Verify(Catalog &catalog) override;

	//! Get the catalog set for the specified type
	CatalogSet &GetCatalogSet(CatalogType type);

private:
	void OnDropEntry(CatalogTransaction transaction, CatalogEntry &entry);
};
} // namespace duckdb
