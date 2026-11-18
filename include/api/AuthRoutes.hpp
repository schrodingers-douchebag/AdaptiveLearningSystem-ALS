#pragma once
#include "api/ApiServer.hpp"

namespace als {
void registerAuthRoutes(ApiApp& app, ServiceRegistry& services);
}  // namespace als
