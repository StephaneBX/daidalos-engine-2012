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

#include <algorithm>
#include "../../Core/CMediaManager.h"
#include "../../Core/CConfigurationManager.h"
#include "../../Core/Logger/ILogger.h"
#include "../../Renderers/OGL/COGLEnums.h"
#include "../../Renderers/OGL/COGLBuffer.h"
#include "../../Renderers/OGL/COGLException.h"
#include "../../Renderers/OGL/COGLDeclaration.h"
#include "../../Renderers/OGL/COGLShader.h"
#include "../../Renderers/OGL/COGLShaderProgram.h"
#include "../../Renderers/OGL/COGLShadersLoader.h"
#include "../../Renderers/OGL/COGLTexture2D.h"
#include "../../Renderers/OGL/COGLFrameBuffer.h"
#include "../../Renderers/OGL/COGLRenderTarget.h"
#include "../../Renderers/OGL/COGLRenderer.h"

using namespace std;

namespace DE
{
	#define BUFFER_OFFSET(n) ((char*)NULL + (n))

	unsigned int COGLRenderer::s_vertexAttributeSize[] = {1, 2, 3, 4, 4};
	unsigned int COGLRenderer::s_vertexAttributeType[] = {GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT};
	std::string COGLRenderer::s_vertexAttributeName[] = {	"vertexPosition0", "vertexPosition1", "vertexPosition2", "vertexPosition3", 
										"vertexNormal0", "vertexNormal1", "vertexNormal2", "vertexNormal3",
										"vertexTexcoord0", "vertexTexcoord1", "vertexTexcoord2", "vertexTexcoord3",
										"vertexColor0", "vertexColor1", "vertexColor2", "vertexColor3"};

	COGLRenderer::COGLRenderer(const bool verticalSynchro, const int anisotropicFiltering, const HWND hWnd) : IRenderer(verticalSynchro, anisotropicFiltering)
	{
		this->m_hwnd = hWnd;
		Initialize();
		CheckCapabilities();
	}

	COGLRenderer::~COGLRenderer()
	{
		CMediaManager::GetInstance()->RemoveLoader<IShaderBase>();
	}

	const CRectangleI COGLRenderer::GetLightScissor(const CVector4F & positionRadius) const
	{
		int rect[4] = {0, 0, this->m_resolution.x, this->m_resolution.y};

		CVector3F l = CVector3F(positionRadius.x, positionRadius.y, positionRadius.z);
		CVector3F l2 = l * l;
		float r = positionRadius.w;
		float r2 = r * r;
		float e = 1.2f;
		float a = this->m_resolution.x / (float)this->m_resolution.y;

		float d = r2 * l2.x - (l2.x + l2.z) * (r2 - l2.z);
		if(d > 0)
		{
			float nx1 = ((r * l.x) + sqrtf(d)) / (l2.x + l2.z);
			float nx2 = ((r * l.x) - sqrtf(d)) / (l2.x + l2.z);

			float nz1 = (r - nx1 * l.x) / l.z;
			float nz2 = (r - nx2 * l.x) / l.z;

			float pz1 = (l2.x + l2.z - r2) / (l.z - (nz1 / nx1) * l.x);
			float pz2 = (l2.x + l2.z - r2) / (l.z - (nz2 / nx2) * l.x);

			if(pz1 > 0)
			{
				float fx = (-nz1 * e) / nx1;
				int x = (int)((fx + 1) * this->m_resolution.x * 0.5f);
				float px = -pz1 * nz1 / nx1;

				if (px < l.x)
					rect[0] = max(rect[0], x);
				else
					rect[2] = min(rect[2], x);
			}

			if(pz2 > 0)
			{
				float fx = (-nz2 * e) / nx2;
				int x = (int)((fx + 1) * this->m_resolution.x * 0.5f);
				float px = -pz2 * nz2 / nx2;

				if (px < l.x)
					rect[0] = max(rect[0], x);
				else
					rect[2] = min(rect[2], x);
			}
		}

		d = r2 * l2.y - (l2.y + l2.z) * (r2 - l2.z);
		if(d > 0)
		{
			float ny1 = (r * l.y + sqrtf(d)) / (l2.y + l2.z);
			float ny2 = (r * l.y - sqrtf(d)) / (l2.y + l2.z);
			
			float nz1 = (r - ny1 * l.y) / l.z;
			float nz2 = (r - ny2 * l.y) / l.z;

			float pz1 = (l2.y + l2.z - r2) / (l.z - (nz1 / ny1) * l.y);
			float pz2 = (l2.y + l2.z - r2) / (l.z - (nz2 / ny2) * l.y);

			if(pz1 > 0)
			{
				float fy = (-nz1 * e * a) / ny1;
				int y = (int)((fy + 1) * this->m_resolution.y * 0.5f);
				float py = -pz1 * nz1 / ny1;

				if (py < l.y)
					rect[1] = max(rect[1], y);
				else
					rect[3] = min(rect[3], y);
			}

			if(pz2 > 0)
			{
				float fy = (-nz2 * e * a) / ny2;
				int y = (int)((fy + 1) * this->m_resolution.y * 0.5f);
				float py = -pz2 * nz2 / ny2;

				if (py < l.y)
					rect[1] = max(rect[1], y);
				else
					rect[3] = min(rect[3], y);
			}
		}

		return CRectangleI(rect[0], rect[1], rect[2] - rect[0], rect[3] - rect[1]);
	}

	const CRectangleI COGLRenderer::GetViewport() const 
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		return CRectangleI(viewport[0], viewport[1], viewport[2], viewport[3]);
	}

	ITexture2DBase * COGLRenderer::CreateTexture2D(const CVector2I & size, const STexture2DFilterPolicy & filter, EPixelFormat format, const unsigned long flags) const
	{
		unsigned int texture;
		glGenTextures(1, &texture);

		// Bind the texture.
		glBindTexture(GL_TEXTURE_2D, texture);

		// Anisotropic filtering
		if(filter.maxAnisotropy > 0)
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)filter.maxAnisotropy );
				
		// Initialize the filtrates.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, COGLEnums::Get(filter.addressU));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, COGLEnums::Get(filter.addressV));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, COGLEnums::Get(filter.magFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, COGLEnums::Get(filter.minFilter));

		// Create the texture and the mipmaps.
		glTexImage2D(GL_TEXTURE_2D, 0, COGLEnums::Get(format).internalFormat, size.x, size.y, 0, COGLEnums::Get(format).format, COGLEnums::Get(format).type, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);


		return new COGLTexture2D(size, filter, format, true, HasCapability(CAPABILITY_HARDWARE_MIPMAP), texture);
	}

	IFrameBufferBase * COGLRenderer::CreateFrameBuffer(const SFrameBufferDeclaration * elements, const std::size_t size, const SFrameBufferDepthDeclaration * depth, const std::size_t size2) const
	{
		unsigned int frameBuffer;

		glGenFramebuffers(1, &frameBuffer);

		IFrameBufferBase * fbb = new COGLFrameBuffer(frameBuffer);

		if(elements != nullptr)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
			for(unsigned int i = 0 ; i < size ; ++i)
			{
				SFrameBufferElement element;
				switch(elements[i].type)
				{
				case FRAMEBUFFER_ATTACHMENT_TYPE_TEXTURE:
					element.texture = CreateTexture2D(elements[i].size, elements[i].textureFilter, elements[i].format);
					element.renderTarget = nullptr;
					fbb->AddElement(elements[i].name, element);
					break;

				case FRAMEBUFFER_ATTACHMENT_TYPE_BUFFER:
					element.texture = nullptr;
					element.renderTarget = CreateRT(elements[i].size, elements[i].format);
					fbb->AddElement(elements[i].name, element);
					break;
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		if(depth != nullptr)
		{
			for(unsigned int i = 0 ; i < size2 ; ++i)
			{
				fbb->AddDepth(depth[i].name, CreateRT(depth[i].size, depth[i].textureFormat));
			}
		}

		return fbb;
	}

	IShaderProgramBase * COGLRenderer::CreateShaderProgram(const std::string & name)
	{
		return new COGLShaderProgram(name);
	}

	void COGLRenderer::SetVertexBuffer(const int stream, const CBuffer & buffer, const unsigned long stride, const unsigned long offset, const unsigned long count)
	{
		const IBufferBase * base = buffer.GetBuffer();

		const COGLVertexBuffer * vertexBuffer = static_cast<const COGLVertexBuffer*>(base);
		COGLShaderProgram * shaderProgram = static_cast<COGLShaderProgram*>(this->m_currentShaderProgram);
		const TOGLDeclarationElementArray & declaration = this->m_currentDeclaration->GetStreamDeclaration(stream);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetBuffer());

		// Loop through the declaration
		for(auto i = declaration.begin() ; i != declaration.end() ; ++i)
		{
			// Get the attribute index
			const int attribIndex = glGetAttribLocation(shaderProgram->GetProgramId(), COGLRenderer::s_vertexAttributeName[i->usage].c_str());

			if(attribIndex == -1)
			{
				// Log the error.
				LOG_ERROR("Unable to get the attribute location '" << COGLRenderer::s_vertexAttributeName[i->usage] << "' for the shader program " << shaderProgram->GetName());
				ILogger::Kill();

				throw COGLException("SetVertexBuffer", "glGetAttribLocation");
				break;
			}

			// Enable the attribute
			glEnableVertexAttribArray(attribIndex);

			// Link the buffer
			glVertexAttribPointer(attribIndex, COGLRenderer::s_vertexAttributeSize[i->type], COGLRenderer::s_vertexAttributeType[i->type], false, stride, BUFFER_OFFSET(i->offset + offset * stride));
		}

		// Samplers
		for(int i = 0 ; i < MAXIMUM_TEXTURE_UNIT ; ++i)
			COGLRenderer::glUniform1i(shaderProgram->GetSamplers()[i], i);
	}

	void COGLRenderer::SetIndexBuffer(const CBuffer & buffer, const unsigned long stride)
	{
		const IBufferBase * base = buffer.GetBuffer();

		const COGLIndexBuffer * indexBuffer = static_cast<const COGLIndexBuffer*>(base);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetBuffer());

		this->m_indexStride = stride;
	}

	void COGLRenderer::SetShaderProgram(IShaderProgramBase * shaderProgram)
	{
		this->m_currentShaderProgram = shaderProgram;

		if(shaderProgram != nullptr)
		{
			glUseProgram(static_cast<COGLShaderProgram*>(shaderProgram)->GetProgramId());
		}
		else
			glUseProgram(0);
	}

	void COGLRenderer::SetVertexDeclaration(const IDeclaration * declaration)
	{
		// Disable textures
		for(int i = 0; i < MAXIMUM_TEXTURE_UNIT; ++i)
			glActiveTextureARB(GL_TEXTURE0 + i);

		if(declaration != nullptr)
		{
			const COGLDeclaration * tmp = static_cast<const COGLDeclaration *>(declaration);

			// Enable the correct number of vertex attributes.
			for(int i = 0 ; i < tmp->GetSize() ; ++i)
				glEnableVertexAttribArray(i);

			// Assign the current declaration.
			this->m_currentDeclaration = tmp;
		}
		else
			this->m_currentDeclaration = nullptr;
	}

	void COGLRenderer::SetTexture(const int unit, const ITexture2DBase * texture)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + unit);

		if(texture != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D, static_cast<const COGLTexture2D*>(texture)->GetTexture());
		}
		else
			glBindTexture(GL_TEXTURE_2D, NULL);
	}

	void COGLRenderer::SetFrameBuffer(IFrameBufferBase * frameBuffer)
	{
		if(frameBuffer != nullptr)
		{
			// Finalize if necessary
			if(!frameBuffer->IsReady())
				frameBuffer->Finalize();

			COGLFrameBuffer * buffer = static_cast<COGLFrameBuffer*>(frameBuffer);

			glBindFramebuffer(GL_FRAMEBUFFER, buffer->GetFrameBuffer());

			// Update the buffers call
			glDrawBuffers(buffer->GetAttachmentSize(), buffer->GetActiveBuffers());

			// Verify the frame buffer
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if(status != GL_FRAMEBUFFER_COMPLETE)
			{
				LOG_ERROR("Unable to bind the framebuffer");
				ILogger::Kill();

				throw COGLException("SetFrameBuffer", "glCheckFramebufferStatus");
			}		
		}
		else
		{
			// Unbind any framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			GLenum buffer = GL_COLOR_ATTACHMENT0;
			glDrawBuffers(1, &buffer);
		}
	}

	void COGLRenderer::SetRenderState(ERenderStateType state, const bool value) 
	{
		// Get the gl state
		GLenum glState;
		switch(state)
		{
		case RENDER_STATE_TYPE_DEPTHTEST:
		case RENDER_STATE_TYPE_DEPTH_BUFFER_WRITE:
			glState = GL_DEPTH_TEST;
			break;

		case RENDER_STATE_TYPE_ALPHATEST:
			glState = GL_ALPHA_TEST;
			break;

		case RENDER_STATE_TYPE_ALPHABLEND:
			glState = GL_BLEND;
			break;

		case RENDER_STATE_TYPE_SCISSORTEST:
			glState = GL_SCISSOR_TEST;
			break;
		}

		// Set the render state
		if(value)
			glEnable(glState);
		else
			glDisable(glState);
	}

	void COGLRenderer::SetDepthFunction(ECompare depth) 
	{
		glDepthFunc(COGLEnums::Get(depth));
	}

	void COGLRenderer::SetBlendFunction(ERenderStateBlendFunction src, ERenderStateBlendFunction dest) 
	{
		glBlendFunc(COGLEnums::Get(src), COGLEnums::Get(dest));
	}

	void COGLRenderer::SetBlendOperation(ERenderStateBlendOperation operation) 
	{
		glBlendEquation(COGLEnums::Get(operation));
	}

	void COGLRenderer::SetBlendColor(const CColor & color) 
	{
		CVector4F vector = color.ToRGBAVector();
		glBlendColor(vector.x, vector.y, vector.z, vector.w);
	}

	void COGLRenderer::SetScissor(const CRectangleI & rectangle)
	{
		glScissor(rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetWidth(), rectangle.GetHeight());
	}

	void COGLRenderer::SetViewport(const CRectangleI & viewport) 
	{
		glViewport(viewport.GetLeft(), viewport.GetTop(), viewport.GetWidth(), viewport.GetHeight());
	}

	void COGLRenderer::BeginScene()
	{
		SetScissor(CRectangleI(0, 0, this->m_resolution.x, this->m_resolution.y));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void COGLRenderer::EndScene()
	{
		SwapBuffers(this->m_handle);
	}

	void COGLRenderer::Clear(const unsigned int surfaceFlags, const CColor & color) 
	{
		CVector4F color2 = color.ToRGBAVector();
		glClearColor(color2.x, color2.y, color2.z, color2.w);
		glClear(COGLEnums::SurfaceFlags(surfaceFlags));
	}

	void COGLRenderer::DrawPrimitives(EPrimitiveType type, unsigned long offset, unsigned long count) const
	{
		switch(type)
		{
			case PRIMITIVE_TYPE_TRIANGLE_LIST :  glDrawArrays(GL_TRIANGLES,      offset, count * 3); break;
			case PRIMITIVE_TYPE_TRIANGLE_STRIP : glDrawArrays(GL_TRIANGLE_STRIP, offset, count + 2); break;
			case PRIMITIVE_TYPE_TRIANGLE_FAN :   glDrawArrays(GL_TRIANGLE_FAN,   offset, count + 1); break;
			case PRIMITIVE_TYPE_LINE_LIST :      glDrawArrays(GL_LINES,          offset, count * 2); break; 
			case PRIMITIVE_TYPE_POINT_LIST :     glDrawArrays(GL_LINE_STRIP,     offset, count + 1); break;
			case PRIMITIVE_TYPE_LINE_STRIP :     glDrawArrays(GL_POINTS,         offset, count);     break;
		}
	}

	void COGLRenderer::DrawIndexedPrimitives(EPrimitiveType type, unsigned long offset, unsigned long count) const
	{
		unsigned long indicesType = (this->m_indexStride == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT);
		const void * indices = BUFFER_OFFSET(offset * this->m_indexStride);

		switch (type)
		{
			case PRIMITIVE_TYPE_TRIANGLE_LIST :  glDrawElements(GL_TRIANGLES,      count * 3, indicesType, indices); break;
			case PRIMITIVE_TYPE_TRIANGLE_STRIP : glDrawElements(GL_TRIANGLE_STRIP, count + 2, indicesType, indices); break;
			case PRIMITIVE_TYPE_TRIANGLE_FAN :   glDrawElements(GL_TRIANGLE_FAN,   count + 1, indicesType, indices); break;
			case PRIMITIVE_TYPE_LINE_LIST :      glDrawElements(GL_LINES,          count * 2, indicesType, indices); break; 
			case PRIMITIVE_TYPE_POINT_LIST :     glDrawElements(GL_LINE_STRIP,     count + 1, indicesType, indices); break;
			case PRIMITIVE_TYPE_LINE_STRIP :     glDrawElements(GL_POINTS,         count,     indicesType, indices); break;
		}
	}

	const bool COGLRenderer::CheckExtension(const std::string & extension) const
	{
		return this->m_extensions.find(extension) != std::string::npos;
	}

	const std::string COGLRenderer::GetRendererDescription() const
	{
		return std::string("OpenGL ") + reinterpret_cast<const char*>(glGetString(GL_VERSION));
	}

	
	IBufferBase * COGLRenderer::CreateVB(const unsigned long stride, const unsigned long count, const unsigned long flags) const
	{
		unsigned int vertexBuffer = 0;
		glGenBuffers(1, &vertexBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, count * stride, NULL, COGLEnums::BufferFlags(flags));

		return new COGLVertexBuffer(stride * count, vertexBuffer);
	}

	IBufferBase * COGLRenderer::CreateIB(const unsigned long stride, const unsigned long count, const unsigned long flags) const
	{
		unsigned int indexBuffer = 0;
		glGenBuffers(1, &indexBuffer);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * stride, NULL, COGLEnums::BufferFlags(flags));

		return new COGLIndexBuffer(stride * count, indexBuffer);
	}

	DeclarationPtr COGLRenderer::CreateVD(const SDeclarationElement * elements, const std::size_t size) const
	{
		COGLDeclaration * declaration = new COGLDeclaration();
		const unsigned int typeSize[] = {4, 8, 12, 16, 4};
		std::vector<int> offset(size, 0);

		for(const SDeclarationElement * i = elements ; i < elements + size ; ++i)
		{
			// Copy the values.
			SOGLDeclarationElement declarationElement;
			declarationElement.type = i->type;
			declarationElement.usage = i->usage;
			declarationElement.offset = offset[i->stream];

			// Add the element.
			declaration->AddElement(i->stream, declarationElement);

			// Increase the offset.
			offset[i->stream] += typeSize[i->type];
		}

		return std::shared_ptr<IDeclaration>(declaration);
	}

	IRenderTargetBase * COGLRenderer::CreateRT(const CVector2I & size, EPixelFormat format) const
	{
		ERenderTargetType type;
		switch(format)
		{
		case PIXEL_FORMAT_DEPTH24S8:
			type = RENDER_TARGET_TYPE_DEPTH;
			break;

		default:
			type = RENDER_TARGET_TYPE_COLOR;
			break;
		}

		unsigned int renderTarget;

		glGenRenderbuffers(1, &renderTarget);
		glBindRenderbuffer(GL_RENDERBUFFER, renderTarget);
		glRenderbufferStorage(GL_RENDERBUFFER, COGLEnums::Get(format).internalFormat, size.x, size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		return new COGLRenderTarget(renderTarget, type);
	}

	void COGLRenderer::CheckCapabilities()
	{
		this->m_capabilities[CAPABILITY_HARDWARE_SHADERS] = CheckExtension("GL_ARB_shading_language_100") &&
															CheckExtension("GL_ARB_shader_objects") &&
															CheckExtension("GL_ARB_vertex_shader") &&
															CheckExtension("GL_ARB_fragment_shader");

		this->m_capabilities[CAPABILITY_HARDWARE_MIPMAP] = CheckExtension("GL_SGIS_generate_mipmap");

		this->m_capabilities[CAPABILITY_HARDWARE_DXT_COMPRESSION] = CheckExtension("GL_ARB_texture_compression") &&
																	CheckExtension("GL_EXT_texture_compression_s3tc");

		this->m_capabilities[CAPABILITY_HARDWARE_NON_POW2_TEXTURES] =  CheckExtension("GL_ARB_texture_non_power_of_two");

		this->m_capabilities[CAPABILITY_HARDWARE_MRT] =  CheckExtension("GL_ARB_draw_buffers") && CheckExtension("GL_EXT_framebuffer_object");

		this->m_capabilities[CAPABILITY_HARDWARE_MULTITEXTURE] = CheckExtension("GL_ARB_multitexture");

		this->m_capabilities[CAPABILITY_HARDWARE_ADVANCED_BUFFERS] = CheckExtension("GL_ARB_vertex_buffer_object");
	}

	void COGLRenderer::Initialize()
	{
		CMediaManager::GetInstance()->AddLoader(new COGLShadersLoader(), "vsh psh");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Shaders/OGL/");

		this->m_handle = GetDC(this->m_hwnd);

		PIXELFORMATDESCRIPTOR pixelDescriptor = 
		{ 
			sizeof(PIXELFORMATDESCRIPTOR),	// size of this pfd  
			1,								// version number  
			PFD_DRAW_TO_WINDOW |			// support window  
			PFD_SUPPORT_OPENGL |			// support OpenGL  
			PFD_DOUBLEBUFFER,				// double buffered  
			PFD_TYPE_RGBA,					// RGBA type  
			32,								// 24-bit color depth  
			0, 0, 0, 0, 0, 0,				// color bits ignored  
			0,								// no alpha buffer  
			0,								// shift bit ignored  
			0,								// no accumulation buffer  
			0, 0, 0, 0,						// accum bits ignored  
			32,								// 32-bit z-buffer  
			32,								// no stencil buffer  
			0,								// no auxiliary buffer  
			PFD_MAIN_PLANE,					// main layer  
			0,								// reserved  
			0, 0, 0							// layer masks ignored  
		};

		// Set the pixel format of the device context
		if(!SetPixelFormat(this->m_handle, ChoosePixelFormat(this->m_handle, &pixelDescriptor), &pixelDescriptor))
		{
			LOG_ERROR("Unable to set the context pixel format");
			ILogger::Kill();

			throw COGLException("SetPixelFormat", "Initialize");
		}
			
		// Context creation
		this->m_context = wglCreateContext(this->m_handle);
		if (!wglMakeCurrent(this->m_handle, this->m_context))
		{
			LOG_ERROR("Unable to create the context");
			ILogger::Kill();

			throw COGLException("wglMakeCurrent", "Initialize");
		}

		// Extensions initialization
		this->m_extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
		LoadExtensions();

		// Canvas resolution
		CRectangleI viewport = GetViewport();
		this->m_resolution.x = viewport.GetWidth();
		this->m_resolution.y = viewport.GetHeight();

		// Internal resolution
		SiniP options = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);
		this->m_internalResolution.x = options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_INTERNAL_RESOLUTION_X, this->m_resolution.x);
		this->m_internalResolution.y = options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_INTERNAL_RESOLUTION_Y, this->m_resolution.y);

		// Default states
		glStencilMask(0xFFFFFFFF);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glEnable(GL_DEPTH_TEST);

		this->m_currentDeclaration = nullptr;

		// Screen quad information
		SScreenQuadElement sq[] =
		{
			{ CVector3F(  1, -1, 0 ), CVector2F(1,0) },
			{ CVector3F( -1, -1, 0 ), CVector2F(0,0) },
			{ CVector3F( -1,  1, 0 ), CVector2F(0,1) },
			{ CVector3F(  1,  1, 0 ), CVector2F(1,1) }
		};
		memcpy(this->m_screenQuadElement, sq, sizeof(SScreenQuadElement) * 4);

		// Quad Index Buffer
		int indices[] = {0,1,2,2,3,0};
		this->m_quadIB = CreateIndexBuffer(6, 0, indices);
	}

    void COGLRenderer::LoadExtensions()
	{
		/* Misc */
		LoadExtension(glBlendEquation, "glBlendEquation");
		LoadExtension(glBlendColor, "glBlendColor");

		/* Vertex Buffer Object */
		LoadExtension(glBindBuffer, "glBindBuffer");
		LoadExtension(glDeleteBuffers, "glDeleteBuffers");
		LoadExtension(glGenBuffers,"glGenBuffers");
		LoadExtension(glBufferData, "glBufferData");
		LoadExtension(glBufferSubData, "glBufferSubData");
		LoadExtension(glGetBufferSubData, "glGetBufferSubData");
		LoadExtension(glMapBuffer, "glMapBuffer");
		LoadExtension(glUnmapBuffer, "glUnmapBuffer");
		LoadExtension(glEnableVertexAttribArray, "glEnableVertexAttribArray");
		LoadExtension(glDisableVertexAttribArray, "glDisableVertexAttribArray");
		LoadExtension(glVertexAttribPointer, "glVertexAttribPointer");
		LoadExtension(glVertexAttribIPointer, "glVertexAttribIPointer");
		LoadExtension(glGetAttribLocation, "glGetAttribLocation");
		LoadExtension(glBindAttribLocation, "glBindAttribLocation");

		/* ARB textures */
		LoadExtension(glActiveTextureARB, "glActiveTextureARB");
		LoadExtension(glClientActiveTextureARB, "glClientActiveTextureARB");
		LoadExtension(glCompressedTexImage2DARB," glCompressedTexImage2DARB");
		LoadExtension(glCompressedTexSubImage2DARB, "glCompressedTexSubImage2DARB");

		/* Frame Buffer Object */
		LoadExtension(glGenRenderbuffers, "glGenRenderbuffers");
		LoadExtension(glDeleteRenderbuffers, "glDeleteRenderbuffers");
		LoadExtension(glBindRenderbuffer, "glBindRenderbuffer");
		LoadExtension(glRenderbufferStorage, "glRenderbufferStorage");
		LoadExtension(glGenFramebuffers, "glGenFramebuffers");
		LoadExtension(glDeleteFramebuffers, "glDeleteFramebuffers");
		LoadExtension(glBindFramebuffer, "glBindFramebuffer");
		LoadExtension(glFramebufferTexture2D, "glFramebufferTexture2D");
		LoadExtension(glFramebufferRenderbuffer, "glFramebufferRenderbuffer");
		LoadExtension(glCheckFramebufferStatus, "glCheckFramebufferStatus");
		LoadExtension(glGenerateMipmap, "glGenerateMipmap");
		LoadExtension(glBindFragDataLocation, "glBindFragDataLocation");
		LoadExtension(glGetFragDataLocation, "glGetFragDataLocation");
		LoadExtension(glDrawBuffers, "glDrawBuffers");

		/* Shaders */
		LoadExtension(glCreateShader, "glCreateShader");
		LoadExtension(glDeleteShader, "glDeleteShader");
		LoadExtension(glIsShader, "glIsShader");
		LoadExtension(glShaderSource, "glShaderSource");
		LoadExtension(glCompileShader, "glCompileShader");
		LoadExtension(glGetShaderiv, "glGetShaderiv");
		LoadExtension(glGetShaderInfoLog, "glGetShaderInfoLog");
		LoadExtension(glGetUniformLocation, "glGetUniformLocation");
		LoadExtension(glUniform1f, "glUniform1f");
		LoadExtension(glUniform2f, "glUniform2f");
		LoadExtension(glUniform3f, "glUniform3f");
		LoadExtension(glUniform4f, "glUniform4f");
		LoadExtension(glUniform1i, "glUniform1i");
		LoadExtension(glUniform2i, "glUniform2i");
		LoadExtension(glUniform3i, "glUniform3i");
		LoadExtension(glUniform4i, "glUniform4i");
		LoadExtension(glUniform1fv, "glUniform1fv");
		LoadExtension(glUniform2fv, "glUniform2fv");
		LoadExtension(glUniform3fv, "glUniform3fv");
		LoadExtension(glUniform4fv, "glUniform4fv");
		LoadExtension(glUniform1iv, "glUniform1iv");
		LoadExtension(glUniform2iv, "glUniform2iv");
		LoadExtension(glUniform3iv, "glUniform3iv");
		LoadExtension(glUniform4iv, "glUniform4iv");
		LoadExtension(glUniformMatrix2fv, "glUniformMatrix2fv");
		LoadExtension(glUniformMatrix3fv, "glUniformMatrix3fv");
		LoadExtension(glUniformMatrix4fv, "glUniformMatrix4fv");
		LoadExtension(glUniformMatrix2x3fv, "glUniformMatrix2x3fv");
		LoadExtension(glUniformMatrix3x2fv, "glUniformMatrix3x2fv");
		LoadExtension(glUniformMatrix2x4fv, "glUniformMatrix2x4fv");
		LoadExtension(glUniformMatrix4x2fv, "glUniformMatrix4x2fv");
		LoadExtension(glUniformMatrix3x4fv, "glUniformMatrix3x4fv");
		LoadExtension(glUniformMatrix4x3fv, "glUniformMatrix4x3fv");
		LoadExtension(glCreateProgram, "glCreateProgram");
		LoadExtension(glDeleteProgram, "glDeleteProgram");
		LoadExtension(glAttachShader, "glAttachShader");
		LoadExtension(glDetachShader, "glDetachShader");
		LoadExtension(glLinkProgram, "glLinkProgram");
		LoadExtension(glGetProgramiv, "glGetProgramiv");
		LoadExtension(glGetProgramInfoLog, "glGetProgramInfoLog");
		LoadExtension(glUseProgram, "glUseProgram");
		LoadExtension(glValidateProgram, "glValidateProgram");
	}

	PFNGLBLENDEQUATIONPROC				COGLRenderer::glBlendEquation;
	PFNGLBLENDCOLORPROC					COGLRenderer::glBlendColor;
	PFNGLGENBUFFERSPROC					COGLRenderer::glGenBuffers;
	PFNGLDELETEBUFFERSPROC				COGLRenderer::glDeleteBuffers;
	PFNGLBINDBUFFERPROC					COGLRenderer::glBindBuffer;
    PFNGLBUFFERDATAPROC					COGLRenderer::glBufferData;
    PFNGLBUFFERSUBDATAPROC				COGLRenderer::glBufferSubData;
    PFNGLGETBUFFERSUBDATAPROC			COGLRenderer::glGetBufferSubData;
    PFNGLMAPBUFFERPROC					COGLRenderer::glMapBuffer;
    PFNGLUNMAPBUFFERPROC				COGLRenderer::glUnmapBuffer;
	PFNGLENABLEVERTEXATTRIBARRAYPROC	COGLRenderer::glEnableVertexAttribArray;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC	COGLRenderer::glDisableVertexAttribArray;
	PFNGLVERTEXATTRIBPOINTERPROC		COGLRenderer::glVertexAttribPointer;
	PFNGLVERTEXATTRIBIPOINTERPROC		COGLRenderer::glVertexAttribIPointer;
	PFNGLGETATTRIBLOCATIONPROC			COGLRenderer::glGetAttribLocation;
	PFNGLBINDATTRIBLOCATIONPROC			COGLRenderer::glBindAttribLocation;
	PFNGLACTIVETEXTUREARBPROC           COGLRenderer::glActiveTextureARB;
	PFNGLCLIENTACTIVETEXTUREARBPROC     COGLRenderer::glClientActiveTextureARB;
	PFNGLCOMPRESSEDTEXIMAGE2DARBPROC    COGLRenderer::glCompressedTexImage2DARB;
	PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC COGLRenderer::glCompressedTexSubImage2DARB;
	PFNGLGENRENDERBUFFERSPROC			COGLRenderer::glGenRenderbuffers;
	PFNGLDELETERENDERBUFFERSPROC		COGLRenderer::glDeleteRenderbuffers;
	PFNGLBINDRENDERBUFFERPROC			COGLRenderer::glBindRenderbuffer;
	PFNGLRENDERBUFFERSTORAGEPROC		COGLRenderer::glRenderbufferStorage;
	PFNGLGENFRAMEBUFFERSPROC			COGLRenderer::glGenFramebuffers;
	PFNGLDELETEFRAMEBUFFERSPROC			COGLRenderer::glDeleteFramebuffers;
	PFNGLBINDFRAMEBUFFERPROC			COGLRenderer::glBindFramebuffer;
	PFNGLFRAMEBUFFERTEXTURE2DPROC		COGLRenderer::glFramebufferTexture2D;
	PFNGLFRAMEBUFFERRENDERBUFFERPROC	COGLRenderer::glFramebufferRenderbuffer;
	PFNGLCHECKFRAMEBUFFERSTATUSPROC		COGLRenderer::glCheckFramebufferStatus;
	PFNGLGENERATEMIPMAPPROC				COGLRenderer::glGenerateMipmap;
	PFNGLBINDFRAGDATALOCATIONPROC		COGLRenderer::glBindFragDataLocation;
	PFNGLGETFRAGDATALOCATIONPROC		COGLRenderer::glGetFragDataLocation;
	PFNGLDRAWBUFFERSPROC				COGLRenderer::glDrawBuffers;
	PFNGLCREATESHADERPROC				COGLRenderer::glCreateShader;
	PFNGLDELETESHADERPROC				COGLRenderer::glDeleteShader;
	PFNGLISSHADERPROC					COGLRenderer::glIsShader;
	PFNGLSHADERSOURCEPROC				COGLRenderer::glShaderSource;
	PFNGLCOMPILESHADERPROC				COGLRenderer::glCompileShader;
	PFNGLGETSHADERIVPROC				COGLRenderer::glGetShaderiv;
	PFNGLGETSHADERINFOLOGPROC			COGLRenderer::glGetShaderInfoLog;
	PFNGLGETUNIFORMLOCATIONPROC			COGLRenderer::glGetUniformLocation;
	PFNGLUNIFORM1FPROC					COGLRenderer::glUniform1f;
	PFNGLUNIFORM2FPROC					COGLRenderer::glUniform2f;
	PFNGLUNIFORM3FPROC					COGLRenderer::glUniform3f;
	PFNGLUNIFORM4FPROC					COGLRenderer::glUniform4f;
	PFNGLUNIFORM1IPROC					COGLRenderer::glUniform1i;
	PFNGLUNIFORM2IPROC					COGLRenderer::glUniform2i;
	PFNGLUNIFORM3IPROC					COGLRenderer::glUniform3i;
	PFNGLUNIFORM4IPROC					COGLRenderer::glUniform4i;
	PFNGLUNIFORM1FVPROC					COGLRenderer::glUniform1fv;
	PFNGLUNIFORM2FVPROC					COGLRenderer::glUniform2fv;
	PFNGLUNIFORM3FVPROC					COGLRenderer::glUniform3fv;
	PFNGLUNIFORM4FVPROC					COGLRenderer::glUniform4fv;
	PFNGLUNIFORM1IVPROC					COGLRenderer::glUniform1iv;
	PFNGLUNIFORM2IVPROC					COGLRenderer::glUniform2iv;
	PFNGLUNIFORM3IVPROC					COGLRenderer::glUniform3iv;
	PFNGLUNIFORM4IVPROC					COGLRenderer::glUniform4iv;
	PFNGLUNIFORMMATRIX2FVPROC			COGLRenderer::glUniformMatrix2fv;
	PFNGLUNIFORMMATRIX3FVPROC			COGLRenderer::glUniformMatrix3fv;
	PFNGLUNIFORMMATRIX4FVPROC			COGLRenderer::glUniformMatrix4fv;
	PFNGLUNIFORMMATRIX2X3FVPROC			COGLRenderer::glUniformMatrix2x3fv;
	PFNGLUNIFORMMATRIX3X2FVPROC			COGLRenderer::glUniformMatrix3x2fv;
	PFNGLUNIFORMMATRIX2X4FVPROC			COGLRenderer::glUniformMatrix2x4fv;
	PFNGLUNIFORMMATRIX4X2FVPROC			COGLRenderer::glUniformMatrix4x2fv;
	PFNGLUNIFORMMATRIX3X4FVPROC			COGLRenderer::glUniformMatrix3x4fv;
	PFNGLUNIFORMMATRIX4X3FVPROC			COGLRenderer::glUniformMatrix4x3fv;
	PFNGLCREATEPROGRAMPROC				COGLRenderer::glCreateProgram;
	PFNGLDELETEPROGRAMPROC				COGLRenderer::glDeleteProgram;
	PFNGLATTACHSHADERPROC				COGLRenderer::glAttachShader;
	PFNGLDETACHSHADERPROC				COGLRenderer::glDetachShader;
	PFNGLLINKPROGRAMPROC				COGLRenderer::glLinkProgram;
	PFNGLGETPROGRAMIVPROC				COGLRenderer::glGetProgramiv;
	PFNGLGETPROGRAMINFOLOGPROC			COGLRenderer::glGetProgramInfoLog;
	PFNGLUSEPROGRAMPROC					COGLRenderer::glUseProgram;
	PFNGLVALIDATEPROGRAMPROC			COGLRenderer::glValidateProgram;
}
