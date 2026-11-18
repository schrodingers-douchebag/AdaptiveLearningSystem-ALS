#pragma once
#include "api/ApiServer.hpp"

namespace als {
void registerLearningRoutes(ApiApp& app, ServiceRegistry& services);
}  // namespace als
