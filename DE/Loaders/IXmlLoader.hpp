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

#ifndef __IXMLLOADER_HPP
#define __IXMLLOADER_HPP

#include <TinyXML2/tinyxml2.h>
#include "../Core/Utility/CVector2.hpp"
#include "../Core/Utility/CVector3.hpp"
#include "../Core/Utility/CColor.hpp"
#include "../Core/ILoader.hpp"

namespace DE {

	/**
	* Abstract loader for xml files using TinyXML
	*/
	template <class T> class IXmlLoader : public ILoader < T > {

	public:
		/**
		* Destructor
		*/
		virtual ~IXmlLoader() {}

	protected:
		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, bool & data, const bool defaultValue) {
			LOG_TRACE();

			tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
			if (text != nullptr) {
				std::string tmpText = TrimString(text->Value());
				std::transform(tmpText.begin(), tmpText.end(), tmpText.begin(), tolower);

				if (tmpText == "true" || tmpText == "1")
					data = true;
				if (tmpText == "false" || tmpText == "0")
					data = false;
			} else {
				data = defaultValue;
			}
		}

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, int & data, const int defaultValue) {
			LOG_TRACE();

			tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
			if (text != nullptr) {
				std::istringstream ss(text->Value());
				ss >> data;
			} else {
				data = defaultValue;
			}
		}

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector2I & data, const CVector2I & defaultValue) {
			LOG_TRACE();

			CTokenizer tokenizer;
			data = defaultValue;
			tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
			if (text != nullptr) {
				data = CVector2I::GetFrom(std::string(text->Value()), defaultValue);
			}
		}

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector3F & data, const CVector3F & defaultValue) {
			LOG_TRACE();

			CTokenizer tokenizer;
			data = defaultValue;
			tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
			if (text != nullptr) {
				data = CVector3F::GetFrom(std::string(text->Value()), defaultValue);
			}
		}

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CVector4F & data, const CVector4F & defaultValue) {
			LOG_TRACE();

			CTokenizer tokenizer;
			data = defaultValue;
			tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
			if (text != nullptr) {
				data = CVector4F::GetFrom(std::string(text->Value()), defaultValue);
			}
		}

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, CColor & data, const CColor & defaultValue) {
			LOG_TRACE();

			CTokenizer tokenizer;
			tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
			if (text != nullptr)
				tokenizer.AddLine(text->Value());

			if (tokenizer.Tokenize(":", 4) && tokenizer.GetTokens().size() == 4) {
				std::istringstream ss;
				float r;
				float g;
				float b;
				float a;

				ss.str(tokenizer.GetTokens()[0]);
				ss >> r;
				ss.clear();
				ss.str(tokenizer.GetTokens()[1]);
				ss >> g;
				ss.clear();
				ss.str(tokenizer.GetTokens()[2]);
				ss >> b;
				ss.clear();
				ss.str(tokenizer.GetTokens()[3]);
				ss >> a;

				data = CColor(r, g, b, a);
			} else {
				data = defaultValue;
			}
		}

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, float & data, const float defaultValue) {
			LOG_TRACE();

			tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
			if (text != nullptr) {
				std::istringstream ss(text->Value());
				ss >> data;
			} else {
				data = defaultValue;
			}
		}

		/**
		* Get a node data
		* @param handle The handle for the node
		* @param name The data name
		* @param data The data type
		* @param defaultValue The default value to use if no data is found in the node
		*/
		void GetNodeData(tinyxml2::XMLHandle & handle, const std::string & name, std::string & data, const std::string & defaultValue) {
			LOG_TRACE();

			tinyxml2::XMLText * text = handle.FirstChildElement(name.c_str()).FirstChild().ToText();
			if (text != nullptr) {
				data = text->Value();
			} else {
				data = defaultValue;
			}
		}
	};
}

#endif
