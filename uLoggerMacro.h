#pragma once

#include "uLoggerInterface.h"
#include <cstring>
#include <mutex>
#include <iostream>

namespace LoggerImpl = Diatech::Service;

extern LoggerImpl::ILogger* LOGGER;

static std::mutex g_mutex;

static inline
void print(LoggerImpl::ILogger *logger,
           LoggerImpl::LogLevel log_level,
           const char *file,
           int32_t line,
           const char *function,
           const wchar_t *message,
           ...)
{
    std::unique_lock<std::mutex> locker(g_mutex);
    va_list args;
    wchar_t printfBuf[LoggerImpl::MAX_LOG_LEN];
    va_start(args, message);
    vswprintf(&printfBuf[0], LoggerImpl::MAX_LOG_LEN - 1, message, args);
    printfBuf[LoggerImpl::MAX_LOG_LEN - 1] = L'\0';
    logger->writeMessage(log_level, file, line, function, printfBuf);
    va_end(args);
}

#define LOG_MSG(logger, logLevel, ...) if(logger) print(logger, logLevel, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#define LOG_TRACE(...)     LOG_MSG(LOGGER, LoggerImpl::LogLevel::LOG_LEVEL_TRACE,    ##__VA_ARGS__)
#define LOG_DEBUG(...)     LOG_MSG(LOGGER, LoggerImpl::LogLevel::LOG_LEVEL_DEBUG,    ##__VA_ARGS__)
#define LOG_INFO(...)      LOG_MSG(LOGGER, LoggerImpl::LogLevel::LOG_LEVEL_INFO,     ##__VA_ARGS__)
#define LOG_WARNING(...)   LOG_MSG(LOGGER, LoggerImpl::LogLevel::LOG_LEVEL_WARNING,  ##__VA_ARGS__)
#define LOG_ERROR(...)     LOG_MSG(LOGGER, LoggerImpl::LogLevel::LOG_LEVEL_ERROR,    ##__VA_ARGS__)
#define LOG_CRITICAL(...)  LOG_MSG(LOGGER, LoggerImpl::LogLevel::LOG_LEVEL_CRITICAL, ##__VA_ARGS__)
