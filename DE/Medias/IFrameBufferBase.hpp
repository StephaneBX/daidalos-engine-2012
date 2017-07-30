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

#ifndef __IFRAMEBUFFERBASE_HPP
#define __IFRAMEBUFFERBASE_HPP

#include <string>
#include <map>
#include "../Medias/ITexture2DBase.hpp"
#include "../Medias/IRenderTargetBase.hpp"
#include "../Core/IResource.hpp"

namespace DE {
	/**
	* Structure for a framebuffer element
	*/
	struct SFrameBufferElement {
		ITexture2DBase * texture;
		IRenderTargetBase * renderTarget;
	};

	/**
	* Structure for a framebuffer attachment
	*/
	struct SFrameBufferAttachment {
		int attachment;
		std::string name;
	};

	/**
	* Structure for a framebuffer declaration
	*/
	struct SFrameBufferDeclaration {
		std::string name;
		CVector2I size;
		EPixelFormat format;
		STexture2DFilterPolicy textureFilter;
		EFrameBufferAttachmentType type;
	};

	/**
	* Structure for a framebuffer depth declaration
	*/
	struct SFrameBufferDepthDeclaration {
		std::string name;
		CVector2I size;
		EPixelFormat textureFormat;
	};

	/**
	* Basic class used for frame buffers
	*/
	class IFrameBufferBase : public IResource {
	protected:
		bool m_ready;
		std::map<std::string, SFrameBufferElement> m_elements;
		std::map<std::string, IRenderTargetBase *> m_depthElements;
		std::vector<SFrameBufferAttachment> m_attachments;
		IRenderTargetBase * m_depth;

	public:
		/**
		* Destructor
		*/
		virtual ~IFrameBufferBase();

		/**
		* Add an element
		* @param name The element name
		* @param element The element
		*/
		void AddElement(const std::string & name, SFrameBufferElement & element);

		/**
		* Add a depth buffer to the frame buffer
		* @param name The buffer name
		* @param depth The depth buffer
		*/
		void AddDepth(const std::string & name, IRenderTargetBase * depth);

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

		/**
		* Finalize the modifications
		*/
		virtual void Finalize() = 0;

		/**
		* Get the texture of an element
		* @param name The element name
		* @return The texture
		*/
		ITexture2DBase * GetTexture(const std::string & name) const;

		/**
		* Get the render target of an element
		* @param name The element name
		* @return The render target
		*/
		IRenderTargetBase * GetRenderTarget(const std::string & name) const;

		/**
		* Get an attachment
		* @param name The element name
		* @return The attachment
		*/
		IRenderTargetBase * GetDepthRenderTarget(const std::string & name) const;

		/**
		* Get the currently attached depth render target
		* @return The render target
		*/
		IRenderTargetBase * GetDepthRenderTarget() const;

		/**
		* Get the current depth render target
		* @param number The attachment number
		* @return The render target
		*/
		SFrameBufferAttachment * GetAttachment(const int number);

		/**
		* Check if the frame buffer is ready
		* @return True if the frame buffer is ready and can be used, false if the buffer needs to be finalized
		*/
		const bool IsReady() const;

		/**
		* Get the frame buffer size
		* @return The frame buffer size
		*/
		const int GetBufferSize() const;

		/**
		* Get the number of attachments
		* @return The number of attachments
		*/
		const int GetAttachmentSize() const;

	protected:
		/**
		* Constructor
		*/
		IFrameBufferBase();

	private:
		IFrameBufferBase(const IFrameBufferBase & copy);
		const IFrameBufferBase & operator=(const IFrameBufferBase & copy);
	};
}

#endif
