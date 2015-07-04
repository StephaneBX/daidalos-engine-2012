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

#include "../Core/Logger/ILogger.h"
#include "../Medias/CBuffer.h"

namespace DE {
	CBuffer::CBuffer(IBufferBase * buffer) {
		LOG_TRACE();

		this->m_buffer = std::shared_ptr<IBufferBase>(buffer);
	}

	void CBuffer::Unlock() {
		LOG_TRACE();

		this->m_buffer->Unlock();
	}

	void CBuffer::Fill(const void * data, const unsigned long stride, const unsigned long count) {
		LOG_TRACE();

		void * buf = Lock(stride);
		memcpy(buf, data, stride * count);
		Unlock();
	}

	void CBuffer::Release() {
		LOG_TRACE();

		this->m_buffer = nullptr;
	}

	void * CBuffer::Lock(const unsigned long stride, const unsigned long count, const unsigned long offset, unsigned long flags) {
		LOG_TRACE();

		return reinterpret_cast<void*>(this->m_buffer->Lock(offset * stride, count * stride, flags));
	}

	const IBufferBase * CBuffer::GetBuffer() const {
		LOG_TRACE();

		return this->m_buffer.get();
	}

	const unsigned long CBuffer::GetSize() const {
		LOG_TRACE();

		unsigned long ret = 0;

		if(this->m_buffer != nullptr)
			ret = this->m_buffer->GetSize();

		return ret;
	}
}