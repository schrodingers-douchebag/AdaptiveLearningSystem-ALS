#include "utils/Logger.hpp"

#include <filesystem>
#include <vector>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace als {

static std::shared_ptr<spdlog::logger> g_logger;

void Logger::init(const std::string& logFile) {
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    if (!logFile.empty()) {
        auto parent = std::filesystem::path(logFile).parent_path();
        if (!parent.empty()) std::filesystem::create_directories(parent);
        sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, false));
    }
    g_logger = std::make_shared<spdlog::logger>("als", sinks.begin(), sinks.end());
    g_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
    g_logger->flush_on(spdlog::level::info);
}

std::shared_ptr<spdlog::logger> Logger::get() {
    if (!g_logger) init("");  // safe default, e.g. inside tests
    return g_logger;
}

}  // namespace als
