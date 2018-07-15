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

#include <sstream>
#include "../Core/Logger/ILogger.hpp"
#include "../Core/IResource.hpp"

namespace daidalosengine {
	IResource::IResource() {
		LOG_TRACE();

		std::ostringstream stream;
		stream << s_resourceCounter;
		this->m_name = CFile(stream.str());
		this->m_referenceCount = 0;

		++s_resourceCounter;
	}

	IResource::IResource(const std::string & name) {
		LOG_TRACE();

		this->m_name = CFile(name);
		this->m_referenceCount = 0;

		++s_resourceCounter;
	}

	IResource::~IResource() {
		LOG_TRACE();
	}

	const std::string IResource::GetName() const {
		LOG_TRACE();

		return this->m_name.GetFilename();
	}

	const int IResource::Release() {
		LOG_TRACE();

		int tmpRef = --this->m_referenceCount;

		if(tmpRef == 0)
			delete this;

		return tmpRef;
	}

	void IResource::SetName(const std::string & name) {
		this->m_name = CFile(name);
	}

	void IResource::AddReference() {
		LOG_TRACE();

		++this->m_referenceCount;
	}

	void IResource::OnDeviceLost() {
		LOG_TRACE();
	}

	void IResource::OnDeviceReset() {
		LOG_TRACE();
	}

	/**
	* Resource counter
	*/
	int IResource::s_resourceCounter = 0;
}
