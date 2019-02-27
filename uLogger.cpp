#include "uLogger.h"
#include <stdexcept>
#include <iostream>
#include <ctime>

constexpr wchar_t* MainLoggerInstanceName = TM("MainLogger");

constexpr wchar_t* DebugPrefix = L"Отладка";
constexpr wchar_t* InfoPrefix  = L"Информация";
constexpr wchar_t* WarnPrefix  = L"Предупреждение";
constexpr wchar_t* ErrorPrefix = L"Ошибка";
constexpr wchar_t* TracePrefix = L"Trace";
constexpr wchar_t* EmptyPrefix = L"EMPTY";

using namespace Diatech::Service;

/**
 * \fn	static constexpr wchar_t* prefixByLevel(LogLevel level)
 *
 * \brief	Conversion logLevel to the word in Russian
 *
 * \author	Abalov
 * \date	15.09.2017
 *
 * \param	level	The level.
 *
 * \return	EMPTY if it fails, else a pointer to a wchar_t.
 */
static wchar_t* prefixByLevel(LogLevel level)
{
	if (level == LogLevel::LOG_LEVEL_INFO)
		return InfoPrefix;
	else if (level == LogLevel::LOG_LEVEL_WARNING)
		return WarnPrefix;
	else if (level == LogLevel::LOG_LEVEL_ERROR)
		return ErrorPrefix;
	else if (level == LogLevel::LOG_LEVEL_DEBUG)
		return DebugPrefix;
	else if (level == LogLevel::LOG_LEVEL_TRACE)
		return TracePrefix;
    else
		return EmptyPrefix;
}

Logger::Logger(const LogLevel& verbosity,
               const std::wstring& dirPath)
    : ILogger(verbosity, dirPath)
{
}

void Logger::writeMessage(LogLevel logLevel,
                          const char* file,
                          int32_t line,
                          const char* function,
                          const std::wstring& message)
{
    QString strFile{ file };
    QString strFunc{ function };
    m_pTrace->Trace_Managed(0, static_cast<eP7Trace_Level>(logLevel), m_hModule, line, strFile.toStdWString().c_str(), strFunc.toStdWString().c_str(), message.c_str());

    char       buf[80];
    auto timeVal = time(nullptr);
    struct tm  tstruct;
	localtime_s(&tstruct, &timeVal);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    auto postFix = prefixByLevel(logLevel);
    m_messageNumber++;
    QString formattedString = QString("#%1 | %2 [%3] %4").arg(m_messageNumber).arg(buf).arg(QString::fromWCharArray(postFix)).arg(QString::fromStdWString(message));
    emit logMessage(formattedString);
}

bool Logger::configureLogger(const LogLevel& verbosity,
                             const std::wstring& dirPath)
{
    m_verbosity = verbosity;
    std::wstring configString = TM("/P7.Sink=FileTxt /P7.Format=\"#%ix | [%tf] %lv Tr:%ti:%tn {%fs:%fl %fn} %ms\"");
    if (!dirPath.empty())
    {
        configString.append(TM(" /P7.Dir = ") + dirPath);
    }

    m_Client = P7_Create_Client(configString.c_str());
    if (m_Client == nullptr)
    {
        std::cerr << "Error: P7 engine was not initialized\n";
        return false;
    }

    m_pTrace = P7_Create_Trace(m_Client, TM("Main log"));
    if (m_pTrace == nullptr)
    {
        std::cerr << "Error: P7 trace was not initialized\n";
        return false;
    }
    m_pTrace->Register_Module(TM("Discover"), &m_hModule);
    m_pTrace->Set_Verbosity(m_hModule, static_cast<eP7Trace_Level>(verbosity));
    return true;
}

Logger::~Logger()
{
	if(m_pTrace) m_pTrace->Release();
	if(m_Client) m_Client->Release();
}
