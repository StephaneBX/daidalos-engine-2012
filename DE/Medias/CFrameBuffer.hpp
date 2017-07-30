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

#ifndef __CFRAMEBUFFER_HPP
#define __CFRAMEBUFFER_HPP

#include "../Medias/IFrameBufferBase.hpp"

namespace DE {
	/**
	* Frame buffer enclosing class
	*/
	class CFrameBuffer {
	private:
		IFrameBufferBase * m_frameBuffer;

	public:
		/**
		* Constructor
		*/
		CFrameBuffer();

		/**
		* Destructor
		*/
		~CFrameBuffer();

		/**
		* Get the frame buffer
		* @return The frame buffer
		*/
		IFrameBufferBase * GetFrameBuffer() const;

		/**
		* Get the frame buffer size
		* @return The frame buffer size
		*/
		const int GetBufferSize() const;

		/**
		* Create a frame buffer from a declaration structure
		* @param elements The declaration structure
		* @param depth The depth declaration
		*/
		template <std::size_t N, std::size_t M> void CreateFromDeclaration(const SFrameBufferDeclaration(&elements)[N], const SFrameBufferDepthDeclaration(&depth)[M]);

		/**
		* Create an empty frame buffer
		*/
		void CreateEmpty();

		/**
		* Add an element
		* @param name The element name
		* @param element The element
		*/
		void AddElement(const std::string & name, SFrameBufferElement & element);

		/**
		* Attach an element to the frame buffer
		* @param name The element name
		* @param attachment The attachment position
		*/
		void AttachElement(const std::string & name, const int attachment);

		/**
		* Attach a depth buffer to the frame buffer
		* @param name The depth buffer name
		*/
		void AttachDepth(const std::string & name);

		/**
		* Detach the element at an attachment position
		* @param attachment The attachment position
		*/
		void DetachElement(const int attachment);

		/**
		* Detach all the elements
		*/
		void DetachAllElements();
	};

	template <std::size_t N, std::size_t M> void CFrameBuffer::CreateFromDeclaration(const SFrameBufferDeclaration(&elements)[N], const SFrameBufferDepthDeclaration(&depth)[M]) {
		SAFE_DELETE(this->m_frameBuffer);

		this->m_frameBuffer = IRenderer::GetRenderer()->CreateFrameBuffer(elements, N, depth, M);
	}
}

#endif
