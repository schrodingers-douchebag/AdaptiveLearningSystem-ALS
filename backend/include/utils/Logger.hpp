#pragma once
#include <memory>
#include <string>
#include <spdlog/spdlog.h>

namespace als {
class Logger {
public:
    // Console + file. Call once at startup. Pass "" to log to the console only.
    static void init(const std::string& logFile);
    static std::shared_ptr<spdlog::logger> get();
};
}  // namespace als
