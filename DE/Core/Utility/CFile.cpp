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

#include <algorithm>
#include <fstream>
#include "../../Core/Logger/ILogger.hpp"
#include "../../Core/Utility/CFile.hpp"

namespace daidalosengine {
	/**
	* Constructor
	*/
	CFile::CFile() {
		LOG_TRACE();
	}

	/**
	* Constructor
	* @param name The full filename
	*/
	CFile::CFile(const std::string & name) {

		LOG_TRACE();
		this->m_name = name;
		std::replace(this->m_name.begin(), this->m_name.end(), '/', '\\');
	}

	/**
	* Constructor
	* @param name The full filename
	*/
	CFile::CFile(const char * name) {
		LOG_TRACE();

		this->m_name = std::string(name);
		std::replace(this->m_name.begin(), this->m_name.end(), '/', '\\');
	}

	/**
	* Check if a file exists
	* @return True if the file exists, false otherwise
	*/
	bool CFile::FileExists() const {
		LOG_TRACE();

		bool result = false;
		std::ifstream file(this->m_name.c_str());

		if(file.is_open()) {
			result = true;
			file.close();
		}

		return result;
	}

	/**
	* Get the file size (in Byte)
	* @return The file size in Byte
	*/
	int CFile::GetFileSize() const {
		LOG_TRACE();

		int size = 0;
		std::ifstream file(this->m_name.c_str());
		if(file.is_open()) {
			file.seekg(0, std::ios::end);
			size = static_cast<int>(file.tellg()) * sizeof(char);
			file.close();
		}
		return size;
	}

	/**
	* Get the full filename
	* @return The full filename
	*/
	const std::string & CFile::GetFullname() const {
		LOG_TRACE();
		return this->m_name;
	}

	/**
	* Get the filename
	* @return The filename
	*/
	std::string CFile::GetFilename() const {
		LOG_TRACE();

		const auto position = this->m_name.find_last_of("\\/");

		if (position != std::string::npos) {
			return this->m_name.substr(position + 1, std::string::npos);
		} else {
			return this->m_name;
		}
	}

	/**
	* Get the short filename
	* @return The file full name
	*/
	std::string CFile::GetShortname() const {
		LOG_TRACE();
		return GetFilename().substr(0, GetFilename().find_last_of('.'));
	}

	/**
	* Get the extension
	* @return The extension
	*/
	std::string CFile::GetExtension() const {
		LOG_TRACE();

		const auto position = this->m_name.find_last_of('.');
		if(position != std::string::npos)
			return this->m_name.substr(position + 1, std::string::npos);
		else
			return "";
	}
}
