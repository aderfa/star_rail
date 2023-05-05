#pragma once
#include <Windows.h>


#define LOG(info, level) Logger::Log(info, level, __FILE__, __LINE__)
#define LOG_NONE(info) LOG(info, Logger::LEVEL::LOG_NONE)
#define LOG_INFO(info) LOG(info, Logger::LEVEL::LOG_INFO)
#define LOG_ERROR(info) LOG(info, Logger::LEVEL::LOG_ERROR)
#define LOG_DEBUG(info) LOG(info, Logger::LEVEL::LOG_DEBUG)
#define LOG_WARNING(info) LOG(info, Logger::LEVEL::LOG_WARNING)

namespace Logger {
	enum class LEVEL : BYTE {
		LOG_NONE,
		LOG_INFO,
		LOG_ERROR,
		LOG_DEBUG,
		LOG_WARNING
	};

	extern bool isConsole;

	void OpenConsole(std::string name = "");

	void CloseConsole();

	void ClearConsole();

	void Log(std::string info, LEVEL level, LPCSTR file, int line);
}