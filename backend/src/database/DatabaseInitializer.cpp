#include "database/DatabaseInitializer.hpp"

#include "database/DatabaseMigrations.hpp"
#include "utils/Logger.hpp"

namespace als {
void DatabaseInitializer::initialize() {
    DatabaseMigrations::apply(db_);
    Logger::get()->info("Database ready");
}
}  // namespace als
