#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger
{
public:
    Logger(const std::string &saveDir , const std::string &saveLogName);
    ~Logger();
    void print_info(const std::string &message);
    void print_error(const std::string &message);
    void print_warning(const std::string &message);

private:
    std::shared_ptr<spdlog::logger> _logger;
};

#endif // LOGGER_H
