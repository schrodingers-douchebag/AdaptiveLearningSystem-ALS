#pragma once
#include "database/Database.hpp"

namespace als {
// Versioned schema changes. Each migration runs once, inside a transaction,
// and the applied version is remembered in the schema_version table.
class DatabaseMigrations {
public:
    static void apply(Database& db);
};
}  // namespace als
