#pragma once

#include <QObject>

#include "P7_Trace.h"
#include "P7_Client.h"

#include "uPatterns.h"
#include "uLoggerInterface.h"

namespace Diatech {
namespace Service {

/**
 * \class	Logger
 *
 * \brief	Implementation of ILogger interface through P7 logger library.
 *
 * \author	Abalov
 * \date	
 */
class Logger : public QObject, public ILogger
{
	Q_OBJECT
public:
    Logger(const LogLevel& verbosity = LogLevel::LOG_LEVEL_TRACE,
               const std::wstring& dirPath = L"");

	~Logger() override;

	template<class Trecv, class Tfunc>
	void connectMessageReceiver(const Trecv& receiver, Tfunc func)
	{
		connect(this, &Logger::logMessage, &receiver, func);
	}

	bool configureLogger(const LogLevel& verbosity = LogLevel::LOG_LEVEL_TRACE,
		                 const std::wstring& dirPath = L"") override;

    void writeMessage(LogLevel logLevel,
                      const char* file,
                      int32_t line,
                      const char* function,
                      const std::wstring& message) override;

    void writeMessage(LogLevel /*logLevel*/,
                      const char* /*file*/,
                      int32_t /*line*/,
                      const char* /*function*/,
                      const wchar_t* /*message*/,
                      va_list /*args*/) override {}

private:
	IP7_Client* m_Client = nullptr;
    IP7_Trace* m_pTrace = nullptr;
    IP7_Trace::hModule m_hModule = nullptr;

    uint32_t m_messageNumber = 0;
signals:
	void logMessage(const QString& messageString);
};

using LoggerSingleton = dth::Singleton<Logger>;

}
}