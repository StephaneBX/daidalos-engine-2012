/*
==============================================================================================================================================================

Author: Stephane Baudoux
Attribution-Noncommercial-Share Alike 3.0 Unported

You are free:
to Share - to copy, distribute and transmit the work
to Remix - to adapt the work

Under the following conditions:
Attribution - You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
Noncommercial - You may not use this work for commercial purposes.
Share Alike - If you alter, transform, or build upon this work, you may distribute the resulting work only under the same or similar license to this one.

With the understanding that:
Waiver - Any of the above conditions can be waived if you get permission from the copyright holder.
Public Domain - Where the work or any of its elements is in the public domain under applicable law, that status is in no way affected by the license.
Other Rights - In no way are any of the following rights affected by the license:
Your fair dealing or fair use rights, or other applicable copyright exceptions and limitations;
The author's moral rights;
Rights other persons may have either in the work itself or in how the work is used, such as publicity or privacy rights.
Notice - For any reuse or distribution, you must make clear to others the license terms of this work. The best way to do this is with a link to this web page.

==============================================================================================================================================================
*/

#ifndef __ILOGGER_HPP
#define __ILOGGER_HPP

#include "../Core.hpp"

#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_WARNING 3
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_TRACE 0

#if LOG_LEVEL_ERROR >= LOG_LEVEL
#define LOG_ERROR(stream) ILogger::Log() << GetCurrentTimeAsString() << " [ERROR] " << __FUNCTION__ << " : " << stream << "\n";
#else
#define LOG_ERROR(stream) {}
#endif

#if LOG_LEVEL_WARNING >= LOG_LEVEL
#define LOG_WARNING(stream) ILogger::Log() << GetCurrentTimeAsString() << " [WARNING] - " << __FUNCTION__ << " : " << stream << "\n";
#else
#define LOG_WARNING(stream) {}
#endif

#if LOG_LEVEL_INFO >= LOG_LEVEL
#define LOG_INFO(stream) ILogger::Log() << GetCurrentTimeAsString() << " [INFO] " << __FUNCTION__ << " : " << stream << "\n";
#else
#define LOG_INFO(stream) {}
#endif

#if LOG_LEVEL_DEBUG >= LOG_LEVEL
#define LOG_DEBUG(stream) ILogger::Log() << GetCurrentTimeAsString() << " [DEBUG] " << __FUNCTION__ << " : " << stream << "\n";
#else
#define LOG_DEBUG(stream) {}
#endif

#if LOG_LEVEL_TRACE >= LOG_LEVEL
#define LOG_TRACE() ILogger::Log() << GetCurrentTimeAsString() << " [TRACE] " << __FUNCTION__ << "\n";
#else
#define LOG_TRACE() {}
#endif

#include <string>
#include <sstream>
#include <memory>

namespace daidalosengine {
	/**
	* CLogger class
	* Basic class for handling data logging
	*/
	class ILogger {
	private:
		static ILogger * s_logger;

	public:
		/**
		* Destructor
		*/
		virtual ~ILogger();

		/**
		* << operator override
		* @param log The data to log
		* @return The logger
		*/
		template <class T> ILogger & operator <<(const T & log);

		/**
		* Get the logger
		* @return The logger
		*/
		static ILogger & Log();

		/**
		* Set the logger
		* @param logger The logger to use
		*/
		static void SetLogger(ILogger * const logger);

		/**
		* Kill the logger
		*/
		static void Kill();

	private:
		/**
		* Write a message
		* @param message The message
		*/
		virtual void Write(const std::string & message);
	};

	template <class T> ILogger & ILogger::operator<<(const T & log) {
		std::ostringstream stream;
		stream << log;
		Write(stream.str());

		return Log();
	}
}

#endif
