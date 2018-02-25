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

#ifndef __CDX9BUFFER_HPP
#define __CDX9BUFFER_HPP

#include "../../Renderers/DX9/DX9Global.hpp"
#include "../../Renderers/DX9/CDX9Enums.hpp"
#include "../../Medias/IBufferBase.hpp"

namespace daidalosengine {
	/**
	* DX9 class used for buffers
	*/
	template <class T> class CDX9Buffer : public IBufferBase {
	private:
		T * m_buffer;

	public:
		/**
		* Constructor
		* @param size The buffer size
		* @param buffer The buffer
		*/
		CDX9Buffer(const unsigned long size, T * buffer);

		/**
		* Destructor
		*/
		virtual ~CDX9Buffer();

		/**
		* Get the buffer
		* @return The buffer
		*/
		T * GetBuffer() const;

	private:
		/**
		* Lock the buffer
		* @param offset The offset
		* @param size The size
		* @param flags The flags
		* @return A pointer to the data
		*/
		virtual void * Lock(const unsigned long offset, const unsigned long size, const unsigned long flags) override;

		/**
		* Unlock the buffer
		*/
		virtual void Unlock() override;

	private:
		CDX9Buffer();
		CDX9Buffer(const CDX9Buffer & copy);
		const CDX9Buffer & operator=(const CDX9Buffer & copy);
	};

	template <class T> inline CDX9Buffer<T>::CDX9Buffer(const unsigned long size, T * buffer) : IBufferBase(size) {
		this->m_buffer = buffer;
	}

	template <class T> inline CDX9Buffer<T>::~CDX9Buffer() {
		SAFE_RELEASE(this->m_buffer);
	}

	template <class T> inline T * CDX9Buffer<T>::GetBuffer() const {
		return this->m_buffer;
	}

	template <class T> inline void * CDX9Buffer<T>::Lock(const unsigned long offset, const unsigned long size, const unsigned long flags) {
		void * data = nullptr;
		this->m_buffer->Lock(offset, size, &data, CDX9Enums::LockFlags(flags));

		return data;
	}

	template <class T> inline void CDX9Buffer<T>::Unlock() {
		this->m_buffer->Unlock();
	}

	/**
	* Types declaration
	*/
	typedef CDX9Buffer<IDirect3DVertexBuffer9> CDX9VertexBuffer;
	typedef CDX9Buffer<IDirect3DIndexBuffer9>  CDX9IndexBuffer;
}

#endif
