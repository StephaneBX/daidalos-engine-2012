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

#include "../Core/Core.hpp"
#include "../Core/Logger/ILogger.hpp"
#include "../Core/Memory/CExceptionAssert.hpp"
#include "../Medias/IFrameBufferBase.hpp"

namespace daidalosengine {
	IFrameBufferBase::~IFrameBufferBase() {
		LOG_TRACE();

		for(auto it = this->m_elements.begin(); it != this->m_elements.end(); ++it) {
			SAFE_DELETE(it->second.renderTarget);
			SAFE_DELETE(it->second.texture);
		}

		for(auto it = this->m_depthElements.begin(); it != this->m_depthElements.end(); ++it) {
			SAFE_DELETE(it->second);
		}
	}

	void IFrameBufferBase::AddElement(const std::string & name, SFrameBufferElement & element) {
		LOG_TRACE();

		auto it = this->m_elements.find(name);
		Assert(it == this->m_elements.end());

		this->m_elements[name] = element;

		this->m_ready = false;
	}

	void IFrameBufferBase::AddDepth(const std::string & name, IRenderTargetBase * depth) {
		LOG_TRACE();

		this->m_depthElements[name] = depth;
	}

	void IFrameBufferBase::AttachElement(const std::string & name, const int attachment) {
		LOG_TRACE();

		// Check the existence of the element
		const auto it = this->m_elements.find(name);
		Assert(it != this->m_elements.end());

		// Remove previous attachment to this attachment point
		for (auto it2 = this->m_attachments.begin(); it2 != this->m_attachments.end(); ++it2) {
			if (it2->attachment == attachment) {
				this->m_attachments.erase(it2);
			}
		}

		// Attach the element to this point
		const SFrameBufferAttachment tmp = { attachment, name };
		this->m_attachments.push_back(tmp);

		this->m_ready = false;
	}

	void IFrameBufferBase::AttachDepth(const std::string & name) {
		LOG_TRACE();

		// Check the existence of the buffer
		auto it = this->m_depthElements.find(name);
		Assert(it != this->m_depthElements.end());

		this->m_depth = it->second;
	}

	void IFrameBufferBase::DetachElement(const int attachment) {
		LOG_TRACE();

		// Remove the element attached to this attachment point
		for(auto it = this->m_attachments.begin(); it != this->m_attachments.end(); ++it)
			if(it->attachment == attachment)
				this->m_attachments.erase(it);

		this->m_ready = false;
	}

	void IFrameBufferBase::DetachAllElements() {
		LOG_TRACE();

		this->m_attachments.clear();
		this->m_ready = false;
	}

	ITexture2DBase * IFrameBufferBase::GetTexture(const std::string & name) const {
		LOG_TRACE();

		ITexture2DBase * ret = nullptr;
		auto it = this->m_elements.find(name);
		if(it != this->m_elements.end())
			ret = it->second.texture;

		return ret;
	}

	IRenderTargetBase * IFrameBufferBase::GetRenderTarget(const std::string & name) const {
		LOG_TRACE();

		IRenderTargetBase * ret = nullptr;
		auto it = this->m_elements.find(name);
		if(it != this->m_elements.end())
			ret = it->second.renderTarget;

		return ret;
	}

	SFrameBufferAttachment * IFrameBufferBase::GetAttachment(const int number) {
		LOG_TRACE();

		return &this->m_attachments[number];
	}

	IRenderTargetBase * IFrameBufferBase::GetDepthRenderTarget(const std::string & name) const {
		LOG_TRACE();

		IRenderTargetBase * ret = nullptr;
		auto it = this->m_depthElements.find(name);
		if(it != this->m_depthElements.end())
			ret = it->second;

		return ret;
	}

	IRenderTargetBase * IFrameBufferBase::GetDepthRenderTarget() const {
		LOG_TRACE();

		return this->m_depth;
	}

	bool IFrameBufferBase::IsReady() const {
		LOG_TRACE();

		return this->m_ready;
	}

	int IFrameBufferBase::GetBufferSize() const {
		LOG_TRACE();

		return this->m_elements.size();
	}

	int IFrameBufferBase::GetAttachmentSize() const {
		LOG_TRACE();

		return this->m_attachments.size();
	}

	IFrameBufferBase::IFrameBufferBase() {
		LOG_TRACE();

		this->m_depth = nullptr;
		this->m_ready = false;
	}
};