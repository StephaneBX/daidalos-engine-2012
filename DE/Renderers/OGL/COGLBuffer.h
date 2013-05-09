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

#ifndef __COGLBUFFER_H
#define __COGLBUFFER_H

#include "../../Renderers/OGL/COGLRenderer.h"
#include "../../Medias/IBufferBase.h"

namespace DE
{
	/**
	* OGL class used for buffers
	*/
	template <int type> class COGLBuffer : public IBufferBase
	{
	private:
		unsigned int m_buffer;

	public:
		/**
		* Constructor
		* @param size The buffer size
		* @param buffer The buffer
		*/
		COGLBuffer(const unsigned long size, const unsigned int buffer);

		/**
		* Destructor
		*/
		~COGLBuffer();

		/**
		* Get the buffer
		* @return The buffer
		*/
		const unsigned int GetBuffer() const;

	private:
		/**
		* Lock the buffer
		* @param offset The offset
		* @param size The size
		* @param flags The flags
		* @return A pointer to the data
		*/
		void * Lock(const unsigned long offset, const unsigned long size, const unsigned long flags);

		/**
		* Unlock the buffer
		*/
		void Unlock();

	private:
		COGLBuffer();
		COGLBuffer(const COGLBuffer & copy);
		const COGLBuffer & operator=(const COGLBuffer & copy);
	};

	template <int type> inline COGLBuffer<type>::COGLBuffer(const unsigned long size, const unsigned int buffer) : IBufferBase(size)
	{
		this->m_buffer = buffer;
	}

	template <int type> inline COGLBuffer<type>::~COGLBuffer()
	{
		if(this->m_buffer)
			COGLRenderer::glDeleteBuffers(1, &this->m_buffer);
	}

	template <int type> inline const unsigned int COGLBuffer<type>::GetBuffer() const
	{
		return this->m_buffer;
	}

	template <int type> inline void * COGLBuffer<type>::Lock(const unsigned long offset, const unsigned long size, const unsigned long flags)
	{
		COGLRenderer::glBindBuffer(type, this->m_buffer);
		unsigned char * buffer = reinterpret_cast<unsigned char*>(COGLRenderer::glMapBuffer(type, COGLEnums::LockFlags(flags)));

		return buffer + offset;
	}

	template <int type> inline void COGLBuffer<type>::Unlock()
	{
		 COGLRenderer::glUnmapBuffer(type);
	}

	/**
	* Types declaration
	*/
	typedef COGLBuffer<GL_ARRAY_BUFFER> COGLVertexBuffer;
	typedef COGLBuffer<GL_ELEMENT_ARRAY_BUFFER> COGLIndexBuffer;
}

#endif
