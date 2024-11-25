#pragma once
#include <espch.h>


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

#define EG_CORE_TRACE(...)    Log::get_core_logger()->trace(__VA_ARGS__)
#define EG_CORE_INFO(...)     Log::get_core_logger()->info(__VA_ARGS__)
#define EG_CORE_WARN(...)     Log::get_core_logger()->warn(__VA_ARGS__)
#define EG_CORE_ERROR(...)    Log::get_core_logger()->error(__VA_ARGS__)
#define EG_CORE_CRITICAL(...) Log::get_core_logger()->critical(__VA_ARGS__)

#define EG_TRACE(...)    Log::get_client_logger()->trace(__VA_ARGS__)
#define EG_INFO(...)     Log::get_client_logger()->info(__VA_ARGS__)
#define EG_WARN(...)     Log::get_client_logger()->warn(__VA_ARGS__)
#define EG_ERROR(...)    Log::get_client_logger()->error(__VA_ARGS__)
#define EG_CRITICAL(...) Log::get_client_logger()->critical(__VA_ARGS__)


