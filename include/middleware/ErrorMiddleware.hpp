#pragma once
#include <exception>
#include <nlohmann/json.hpp>
#include "utils/AppError.hpp"
#include "utils/Logger.hpp"
#include "utils/ResponseBuilder.hpp"

namespace als {
// Wraps a route body: any exception becomes a clean JSON error instead of a crash.
template <typename Fn>
crow::response guarded(Fn&& fn) {
    try {
        return fn();
    } catch (const AppError& e) {
        return ResponseBuilder::error(e.status(), e.what());
    } catch (const nlohmann::json::exception&) {
        return ResponseBuilder::error(400, "A field has the wrong type or is missing");
    } catch (const std::exception& e) {
        Logger::get()->error("Unhandled error: {}", e.what());  // details stay in the log
        return ResponseBuilder::error(500, "Internal server error");
    }
}
}  // namespace als
