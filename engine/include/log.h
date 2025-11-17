#pragma once
#include <spdlog/spdlog.h>
#include <memory.h>

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

#define DU_CORE_TRACE(...)    Log::get_core_logger()->trace(__VA_ARGS__)
#define DU_CORE_INFO(...)     Log::get_core_logger()->info(__VA_ARGS__)
#define DU_CORE_WARN(...)     Log::get_core_logger()->warn(__VA_ARGS__)
#define DU_CORE_ERROR(...)    Log::get_core_logger()->error(__VA_ARGS__)
#define DU_CORE_CRITICAL(...) Log::get_core_logger()->critical(__VA_ARGS__)

#define DU_TRACE(...)    Log::get_client_logger()->trace(__VA_ARGS__)
#define DU_INFO(...)     Log::get_client_logger()->info(__VA_ARGS__)
#define DU_WARN(...)     Log::get_client_logger()->warn(__VA_ARGS__)
#define DU_ERROR(...)    Log::get_client_logger()->error(__VA_ARGS__)
#define DU_CRITICAL(...) Log::get_client_logger()->critical(__VA_ARGS__)

#ifdef DU_DEBUG
    #define DU_CORE_DEBUG_TRACE(...) DU_CORE_TRACE(__VA_ARGS__)
    #define DU_DEBUG_TRACE(...) DU_TRACE(__VA_ARGS__)
#else
    #define DU_CORE_DEBUG_TRACE(...) 
    #define DU_DEBUG_TRACE(...) 
#endif
