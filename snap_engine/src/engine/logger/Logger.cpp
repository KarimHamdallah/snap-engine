#include "Logger.h"

std::shared_ptr<spdlog::logger> Log::m_logger;

void Log::init()
{
	// Set logger pattern >> [KRAZY_CORE]
	//spdlog::set_pattern("%^[%T] %n:  %v%$");
	spdlog::set_pattern("[%n]:  %v%$");

	m_logger = spdlog::stdout_color_mt("Engine Logger");
	m_logger->set_level(spdlog::level::trace);
}