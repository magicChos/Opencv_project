// #include "logger.h"
#include "include/logger.h"
#include "spdlog/sinks/daily_file_sink.h"

Logger::Logger(const std::string &saveDir, const std::string &saveLogName)
{
    _logger = spdlog::daily_logger_mt("daily_logger" ,  saveDir + "/" + saveLogName , 0 , 0);
    spdlog::set_default_logger(_logger);
    spdlog::flush_every(std::chrono::seconds(1));
}

Logger::~Logger()
{
    spdlog::drop_all();
}

void Logger::print_info(const std::string &message)
{
    spdlog::info(message);
}

void Logger::print_error(const std::string &message)
{
    spdlog::error(message);
}

void Logger::print_warning(const std::string &message)
{
    spdlog::warn(message);
}
