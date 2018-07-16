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

#include <fstream>
#include "../../Core/Core.hpp"
#include "../../Core/Logger/ILogger.hpp"
#include "../../Core/Utility/CTokenizer.hpp"

namespace daidalosengine {
	/**
	* Constructor.
	*/
	CTokenizer::CTokenizer() {
		LOG_TRACE();
	}

	/**
	* Constructor.
	* @param filename The file to be tokenized.
	*/
	CTokenizer::CTokenizer(const std::string & filename) {
		LOG_TRACE();
		OpenFile(filename);
	}

	/**
	* Constructor.
	* @param filename The file to be tokenized.
	* @param filterList The filter list that contains all the accepted keywords.
	*/
	CTokenizer::CTokenizer(const std::string & filename, const std::vector<std::string> & filterList) {
		LOG_TRACE();

		bool valid = false;
		std::string tmpLine;
		this->m_tokens.clear();
		this->m_buffer.clear();

		// Open the file.
		std::fstream file(filename.c_str());
		if(file.is_open()) {
			// While there is no errors.
			while(file.good()) {
				// Get a line.
				std::getline(file, tmpLine);
				tmpLine = TrimString(tmpLine);

				// Ignore it if it start with a comment.
				if(tmpLine.compare(0, strlen(TOKENIZER_COMMENT), TOKENIZER_COMMENT) != 0) {
					// If a filter list is given, verify that the beginning of the line is recognized.
					for(unsigned int i = 0; i < filterList.size() && !valid; i++) {
						if(tmpLine.substr(0, filterList[i].length()) == filterList[i])
							valid = true;
					}

					// If the line is valid, add it to the tokens.
					if(valid) {
						this->m_tokens.push_back(tmpLine);
						valid = false;
					}
				}
			}
			file.close();
		}
	}

	/**
	* Destructor.
	*/
	CTokenizer::~CTokenizer() {
		LOG_TRACE();
	}

	/**
	* Open a file, read its content and put it in the token list.
	* @param filename The file to open.
	*/
	void CTokenizer::OpenFile(const std::string & filename) {
		LOG_TRACE();

		std::string tmpLine;
		this->m_tokens.clear();
		this->m_buffer.clear();

		// Open the file.
		std::fstream file(filename.c_str());

		// While there is no errors.
		while(file.good()) {
			// Get a line.
			std::getline(file, tmpLine);
			tmpLine = TrimString(tmpLine);

			// If the line isn't a comment, add it to the tokens.
			if(tmpLine.compare(0, strlen(TOKENIZER_COMMENT), TOKENIZER_COMMENT) != 0)
				this->m_tokens.push_back(tmpLine);
		}
		file.close();
	}

	/**
	* Add a line to the line list.
	* @param line The line to add.
	*/
	void CTokenizer::AddLine(const std::string & line) {
		LOG_TRACE();
		this->m_tokens.push_back(TrimString(line));
	}

	/**
	* Clear the tokens.
	*/
	void CTokenizer::Clear() {
		LOG_TRACE();
		this->m_tokens.clear();
	}

	/**
	* Tokenize the token list.
	* @param delimiter The delimiter that is used to split.
	* @param maxSplit The maximum split accepted for each split process.
	* @return True if the split is valid
	*/
	bool CTokenizer::Tokenize(const std::string & delimiter, const int maxSplit) {
		LOG_TRACE();

		bool valid = true;
		this->m_buffer.clear();

		// Iterate through the tokens.
		for(unsigned int i = 0; i < this->m_tokens.size() && valid; i++) {
			// Split each token with the given delimiter.
			const int elementCount = SplitString(this->m_tokens[i], delimiter);

			// The tokenization process is valid if the number of split is not over maxSplit or if maxSplit = 0
			if(maxSplit > 0 && elementCount != 1 && elementCount != maxSplit)
				valid = false;
		}

		// Clear the tokens
		this->m_tokens.clear();

		// Put back the tokens in the tokens buffer.
		for(unsigned int i = 0; i < this->m_buffer.size() && valid; i++)
			this->m_tokens.push_back(this->m_buffer[i]);

		return valid;
	}

	/**
	* Count the number of 'token' tokens.
	* @param token The token to count.
	* @return The number of 'token' tokens found.
	*/
	int CTokenizer::Count(const std::string & token) const {
		LOG_TRACE();

		int count = 0;

		for(auto &it : this->m_tokens) {
			if(it == token)
				count++;
		}

		return count;
	}

	/**
	* Get the tokens.
	* @return The tokens.
	*/
	const TToken & CTokenizer::GetTokens() const {
		LOG_TRACE();

		return this->m_tokens;
	}

	/**
	* Split a token with a delimiter and push the result in the buffer.
	* @param token The token to split.
	* @param delimiter The delimiter to use.
	* @return The number of pushed tokens.
	*/
	int CTokenizer::SplitString(const std::string & token, const std::string & delimiter) {
		LOG_TRACE();

		int elementCount = 1;
		unsigned int start = 0;

		// While the end of the token is not reached.
		while(start < token.length()) {
			// Find the next delimiter position.
			unsigned int end = token.find(delimiter, start);

			// If a new split position is found.
			if(end >= start && end < token.length()) {
				// Push it in the temporary buffer.
				unsigned int size = end - start;
				if(size > 0)
					this->m_buffer.push_back(token.substr(start, size));

				start = end + 1;
				elementCount++;
			} else {
				// Push the remaining data in the temporary buffer.
				this->m_buffer.push_back(token.substr(start, token.length() - start));
				start = token.length() + 1;
			}
		}
		return elementCount;
	}
}
