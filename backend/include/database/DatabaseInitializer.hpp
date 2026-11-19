#pragma once
#include "database/Database.hpp"

namespace als {
class DatabaseInitializer {
public:
    explicit DatabaseInitializer(Database& db) : db_(db) {}
    void initialize();  // creates / upgrades all tables

private:
    Database& db_;
};
}  // namespace als
