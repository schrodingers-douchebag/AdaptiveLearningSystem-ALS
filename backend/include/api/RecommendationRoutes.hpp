#pragma once
#include "api/ApiServer.hpp"

namespace als {
void registerRecommendationRoutes(ApiApp& app, ServiceRegistry& services);
}  // namespace als
