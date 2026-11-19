#pragma once
#include "api/ApiServer.hpp"

namespace als {
void registerAnalyticsRoutes(ApiApp& app, ServiceRegistry& services);
}  // namespace als
