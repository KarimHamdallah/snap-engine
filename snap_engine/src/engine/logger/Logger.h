#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Log
{
public:
	static void init();

	inline static std::shared_ptr<spdlog::logger> get_logger() { return m_logger; }
private:
	static std::shared_ptr<spdlog::logger> m_logger;
};


#define LOG_INFO(...)      Log::get_logger()->info(__VA_ARGS__)
#define LOG_DEBUG(...)     Log::get_logger()->debug(__VA_ARGS__)
#define LOG_TRACE(...)     Log::get_logger()->trace(__VA_ARGS__)
#define LOG_WARN(...)      Log::get_logger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)     Log::get_logger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)  Log::get_logger()->critical(__VA_ARGS__)