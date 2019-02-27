#pragma once

namespace Diatech {
namespace Service {

/**
* \enum	LogLevel
* \brief	Values that represent log levels
*/
enum class LogLevel
{
    LOG_LEVEL_TRACE = 0,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_CRITICAL,

    LOG_LEVEL_COUNT
};

constexpr static const size_t MAX_LOG_LEN = 1024;

/**
 * \class   ILogger
 *
 * \brief   Interface for any logger which will be developed
 *
 * \author  Abalov
 */
class ILogger
{
public:
    ILogger(const LogLevel& verbosity = LogLevel::LOG_LEVEL_TRACE,
            const std::wstring& dirPath = L"")
        : m_verbosity(verbosity), m_logDirPath(dirPath)
    {}

    virtual ~ILogger() = default;

    virtual bool configureLogger(const LogLevel& verbosity,
                                 const std::wstring& dirPath) = 0;

    /**
     * \fn  virtual void ILogger::writeMessage(LogLevel logLevel, const std::wstring& message, const char* file, int32_t line, const char* function) = 0;
     *
     * \brief   Writes a message
     *
     * \author  Abalov
     * \date    
     *
     * \param   logLevel    The log level.
     * \param   file        The file.
     * \param   line        The line.
     * \param   function    The function.
     * \param   message     The message.
     */
    virtual void writeMessage(LogLevel logLevel,
                              const char* file,
                              int32_t line,
                              const char* function,
                              const std::wstring& message) = 0;

    virtual void writeMessage(LogLevel logLevel,
                              const char* file,
                              int32_t line,
                              const char* function,
                              const wchar_t* message,
                              va_list ) = 0;
    

protected:
    LogLevel m_verbosity;      /*!< verbosity level of logger output, messages of below level will not be included to file */
    std::wstring m_logDirPath; /*!< name of directory for log files*/
};

}
}