#pragma once
#include "spdlog/spdlog.h"
#include <memory>


namespace engine {
    struct Log {
        static void init();

        inline static std::shared_ptr<spdlog::logger>& get_core_logger() {return core_logger;}
        inline static std::shared_ptr<spdlog::logger>& get_client_logger() {return client_logger;}
    private:
        static std::shared_ptr<spdlog::logger> core_logger;
        static std::shared_ptr<spdlog::logger> client_logger;
    };

}
