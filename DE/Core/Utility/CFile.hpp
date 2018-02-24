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

#ifndef __CFILE_HPP
#define __CFILE_HPP

#include <string>

namespace DE {
	/**
	* CFile class
	* File information class
	*/
	class CFile {
	private:
		std::string m_name;

	public:
		/**
		* Constructor
		*/
		CFile();

		/**
		* Constructor
		* @param name The full filename
		*/
		explicit CFile(const std::string & name);

		/**
		* Constructor
		* @param name The full filename
		*/
		explicit CFile(const char * name);

		/**
		* Check if a file exists
		* @return True if the file exists, false otherwise
		*/
		const bool FileExists() const;

		/**
		* Get the file size (in Byte)
		* @return The file size in Byte
		*/
		const int GetFileSize() const;

		/**
		* Get the full filename
		* @return The full filename
		*/
		const std::string & GetFullname() const;

		/**
		* Get the filename
		* @return The filename
		*/
		const std::string GetFilename() const;

		/**
		* Get the short filename
		* @return The file full name
		*/
		const std::string GetShortname() const;

		/**
		* Get the extension
		* @return The extension
		*/
		const std::string GetExtension() const;
	};
}

#endif
