#include "log.h"
#include "spdlog/common.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"


namespace engine {

    std::shared_ptr<spdlog::logger> Log::core_logger;
    std::shared_ptr<spdlog::logger> Log::client_logger;

    void Log::init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        core_logger = spdlog::stdout_color_mt("ENGINE");
        core_logger->set_level(spdlog::level::trace);

        client_logger = spdlog::stdout_color_mt("APP");
        client_logger->set_level(spdlog::level::trace);
    }


}
