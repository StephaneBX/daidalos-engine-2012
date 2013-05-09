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

#include "../../Core/Memory/CExceptionAssert.h"
#include "../../Renderers/OGL/COGLEnums.h"
#include "../../Renderers/OGL/COGLRenderTarget.h"
#include "../../Renderers/OGL/COGLTexture2D.h"
#include "../../Renderers/OGL/COGLRenderer.h"
#include "../../Renderers/OGL/COGLShaderProgram.h"
#include "../../Renderers/OGL/COGLFrameBuffer.h"

namespace DE
{
	COGLFrameBuffer::COGLFrameBuffer(const unsigned int frameBuffer) : IFrameBufferBase()
	{
		this->m_frameBuffer = frameBuffer;
		this->m_buffersListSize = 0;
		this->m_buffersList = nullptr;
	}

	COGLFrameBuffer::~COGLFrameBuffer()
	{
		COGLRenderer::glDeleteFramebuffers(1, &this->m_frameBuffer);
		SAFE_DELETE(this->m_buffersList);
	}

	void COGLFrameBuffer::Finalize()
	{
		COGLRenderer::glBindFramebuffer(GL_FRAMEBUFFER, this->m_frameBuffer);

		// Remove previous attachments
		for(int i = 0 ; i < 4 ; ++i)
		{
			COGLRenderer::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
			COGLRenderer::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, 0);
		}

		// Apply the attachments
		for(auto it = this->m_attachments.begin() ; it != this->m_attachments.end() ; ++it)
		{
			COGLRenderer::glBindFragDataLocation(static_cast<COGLShaderProgram*>(IRenderer::GetRenderer()->GetCurrentShaderProgram())->GetProgramId(), it->attachment, it->name.c_str());

			if(this->m_elements[it->name].texture != nullptr)
				COGLRenderer::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + it->attachment, GL_TEXTURE_2D, static_cast<COGLTexture2D*>(this->m_elements[it->name].texture)->GetTexture(), 0);

			if(this->m_elements[it->name].renderTarget != nullptr)
				COGLRenderer::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + it->attachment, GL_RENDERBUFFER, static_cast<COGLRenderTarget*>(this->m_elements[it->name].renderTarget)->GetRenderTarget());
		}

		// Attach the depth buffer
		COGLRenderer::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, static_cast<COGLRenderTarget*>(this->m_depth)->GetRenderTarget());
		COGLRenderer::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, static_cast<COGLRenderTarget*>(this->m_depth)->GetRenderTarget());

		// Fill the buffers list
		if(this->m_buffersListSize < (int)this->m_attachments.size())
		{
			SAFE_DELETE(this->m_buffersList);
			this->m_buffersList = new GLenum[this->m_attachments.size()];
			this->m_buffersListSize = this->m_attachments.size();
		}

		for(int i = 0 ; i < (int)this->m_attachments.size() ; ++i)
			this->m_buffersList[i] = GL_COLOR_ATTACHMENT0 + this->m_attachments.at(i).attachment;

		COGLRenderer::glBindFramebuffer(GL_FRAMEBUFFER, 0);

		this->m_ready = true;
	}

	const unsigned int COGLFrameBuffer::GetFrameBuffer() const
	{
		return this->m_frameBuffer;
	}

	GLenum * COGLFrameBuffer::GetActiveBuffers() const
	{
		return this->m_buffersList;
	}
}
