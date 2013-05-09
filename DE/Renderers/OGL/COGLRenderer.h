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

#ifndef __COGLRENDERER_H
#define __COGLRENDERER_H

#define NOMINMAX
#define MAXIMUM_TEXTURE_UNIT 8

#include <Windows.h>
#include <gl/GL.h>
#include "../../Renderers/OGL/Extensions/glext.h"
#include "../../Core/IRenderer.h"

#pragma comment(lib, "opengl32.lib")

namespace DE
{
	class COGLDeclaration;
	class COGLShader;

	/**
	* OGL Renderer class
	*/
	class COGLRenderer : public IRenderer
	{
	private:
		HWND m_hwnd;
		HDC m_handle;
		HGLRC m_context;
		const COGLDeclaration * m_currentDeclaration;
		unsigned long m_indexStride;
		std::string m_extensions;

	public:
		/* Misc */
		static PFNGLBLENDEQUATIONPROC				glBlendEquation;
		static PFNGLBLENDCOLORPROC					glBlendColor;

		/* Vertex Buffer Objects */
		static PFNGLGENBUFFERSPROC					glGenBuffers;
		static PFNGLDELETEBUFFERSPROC				glDeleteBuffers;
		static PFNGLBINDBUFFERPROC					glBindBuffer;
        static PFNGLBUFFERDATAPROC					glBufferData;
        static PFNGLBUFFERSUBDATAPROC				glBufferSubData;
        static PFNGLGETBUFFERSUBDATAPROC			glGetBufferSubData;
        static PFNGLMAPBUFFERPROC					glMapBuffer;
        static PFNGLUNMAPBUFFERPROC					glUnmapBuffer;
		static PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
		static PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray;
		static PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
		static PFNGLVERTEXATTRIBIPOINTERPROC		glVertexAttribIPointer;
		static PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation;
		static PFNGLBINDATTRIBLOCATIONPROC			glBindAttribLocation;

		/* Textures */
        static PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB;
        static PFNGLCLIENTACTIVETEXTUREARBPROC		glClientActiveTextureARB;
        static PFNGLCOMPRESSEDTEXIMAGE2DARBPROC		glCompressedTexImage2DARB;
        static PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC	glCompressedTexSubImage2DARB;

		/* Frame Buffer Objects */
		static PFNGLGENRENDERBUFFERSPROC			glGenRenderbuffers;
		static PFNGLDELETERENDERBUFFERSPROC			glDeleteRenderbuffers;
		static PFNGLBINDRENDERBUFFERPROC			glBindRenderbuffer;
		static PFNGLRENDERBUFFERSTORAGEPROC			glRenderbufferStorage;
		static PFNGLGENFRAMEBUFFERSPROC				glGenFramebuffers;
		static PFNGLDELETEFRAMEBUFFERSPROC			glDeleteFramebuffers;
		static PFNGLBINDFRAMEBUFFERPROC				glBindFramebuffer;
		static PFNGLGETFRAGDATALOCATIONPROC			glGetFragDataLocation;
		static PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D;
		static PFNGLFRAMEBUFFERRENDERBUFFERPROC		glFramebufferRenderbuffer;
		static PFNGLCHECKFRAMEBUFFERSTATUSPROC		glCheckFramebufferStatus;
		static PFNGLGENERATEMIPMAPPROC				glGenerateMipmap;
		static PFNGLBINDFRAGDATALOCATIONPROC		glBindFragDataLocation;
		static PFNGLDRAWBUFFERSPROC					glDrawBuffers;

		/* Shaders */
		static PFNGLCREATESHADERPROC				glCreateShader;
		static PFNGLDELETESHADERPROC				glDeleteShader;
		static PFNGLISSHADERPROC					glIsShader;
		static PFNGLSHADERSOURCEPROC				glShaderSource;
		static PFNGLCOMPILESHADERPROC				glCompileShader;
		static PFNGLGETSHADERIVPROC					glGetShaderiv;
		static PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog;
		static PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation;
		static PFNGLUNIFORM1FPROC					glUniform1f;
		static PFNGLUNIFORM2FPROC					glUniform2f;
		static PFNGLUNIFORM3FPROC					glUniform3f;
		static PFNGLUNIFORM4FPROC					glUniform4f;
		static PFNGLUNIFORM1IPROC					glUniform1i;
		static PFNGLUNIFORM2IPROC					glUniform2i;
		static PFNGLUNIFORM3IPROC					glUniform3i;
		static PFNGLUNIFORM4IPROC					glUniform4i;
		static PFNGLUNIFORM1FVPROC					glUniform1fv;
		static PFNGLUNIFORM2FVPROC					glUniform2fv;
		static PFNGLUNIFORM3FVPROC					glUniform3fv;
		static PFNGLUNIFORM4FVPROC					glUniform4fv;
		static PFNGLUNIFORM1IVPROC					glUniform1iv;
		static PFNGLUNIFORM2IVPROC					glUniform2iv;
		static PFNGLUNIFORM3IVPROC					glUniform3iv;
		static PFNGLUNIFORM4IVPROC					glUniform4iv;
		static PFNGLUNIFORMMATRIX2FVPROC			glUniformMatrix2fv;
		static PFNGLUNIFORMMATRIX3FVPROC			glUniformMatrix3fv;
		static PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv;
		static PFNGLUNIFORMMATRIX2X3FVPROC			glUniformMatrix2x3fv;
		static PFNGLUNIFORMMATRIX3X2FVPROC			glUniformMatrix3x2fv;
		static PFNGLUNIFORMMATRIX2X4FVPROC			glUniformMatrix2x4fv;
		static PFNGLUNIFORMMATRIX4X2FVPROC			glUniformMatrix4x2fv;
		static PFNGLUNIFORMMATRIX3X4FVPROC			glUniformMatrix3x4fv;
		static PFNGLUNIFORMMATRIX4X3FVPROC			glUniformMatrix4x3fv;
		static PFNGLCREATEPROGRAMPROC				glCreateProgram;
		static PFNGLDELETEPROGRAMPROC				glDeleteProgram;
		static PFNGLATTACHSHADERPROC				glAttachShader;
		static PFNGLDETACHSHADERPROC				glDetachShader;
		static PFNGLLINKPROGRAMPROC					glLinkProgram;
		static PFNGLGETPROGRAMIVPROC				glGetProgramiv;
		static PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog;
		static PFNGLUSEPROGRAMPROC					glUseProgram;
		static PFNGLVALIDATEPROGRAMPROC				glValidateProgram;

	private:
		static unsigned int s_vertexAttributeSize[];
		static unsigned int s_vertexAttributeType[];
		static std::string s_vertexAttributeName[];

	public:
		/**
		* Constructor
		* @param verticalSynchro The vertical synchronization
		* @param anisotropicFiltering The anisotropic filtering value
		* @param hWnd Handler
		*/
		COGLRenderer(const bool verticalSynchro, const int anisotropicFiltering, const HWND hWnd);

		/**
		* Destructor
		*/
		~COGLRenderer();

		/**
		* Get the scissor rectangle for a given light
		* @param positionRadius The light position and radius in view space
		* @return The scissor rectangle
		*/
		const CRectangleI GetLightScissor(const CVector4F & positionRadius) const;

		/**
		* Get the current viewport
		* @return The viewport
		*/
		const CRectangleI GetViewport() const;

		/**
		* Create a 2D texture
		* @param size The texture size
		* @param filter The texture filter policy
		* @param format The texture format
		* @param flags The texture flags
		* @return The texture
		*/
		ITexture2DBase * CreateTexture2D(const CVector2I & size, const STexture2DFilterPolicy & filter, EPixelFormat format, const unsigned long flags = 0) const;

		/**
		* Create a frame buffer
		* @param elements The CE frame buffer declaration
		* @param size The number of elements
		* @param depth The depth declaration
		* @param size2 The number of elements
		* @return The frame buffer
		*/
		IFrameBufferBase * CreateFrameBuffer(const SFrameBufferDeclaration * elements, const std::size_t size, const SFrameBufferDepthDeclaration * depth, const std::size_t size2) const;

		/**
		* Create a shader program
		* @param name The shader program base name
		* @return The shader program
		*/
		IShaderProgramBase * CreateShaderProgram(const std::string & name);

		/**
		* Associate a vertex buffer to a stream
		* @param stream The stream number
		* @param buffer The buffer
		* @param stride The element stride
		* @param offset The offset
		* @param count The number of elements in the buffer
		*/
		void SetVertexBuffer(const int stream, const CBuffer & buffer, const unsigned long stride, const unsigned long offset = 0, const unsigned long count = 0);

		/**
		* Set the current index buffer
		* @param buffer The index buffer
		* @param stride The stride
		*/
		void SetIndexBuffer(const CBuffer & buffer, const unsigned long stride);

		/**
		* Set the shader program
		* @param shaderProgram The shader program
		*/
		void SetShaderProgram(IShaderProgramBase * shaderProgram);

		/**
		* Set the vertex declaration
		* @param declaration The vertex declaration
		*/
		void SetVertexDeclaration(const IDeclaration * declaration);

		/**
		* Attach the texture to the unit
		* @param unit The unit number
		* @param texture The texture
		*/
		void SetTexture(const int unit, const ITexture2DBase * texture);

		/**
		* Set the framebuffer
		* @param frameBuffer The frame buffer
		*/
		void SetFrameBuffer(IFrameBufferBase * frameBuffer);

		/**
		* Change a render state
		* @param state The state
		* @param value The value
		*/
		void SetRenderState(ERenderStateType state, const bool value);

		/**
		* Set the depth function
		* @param depth The depth function
		*/
		void SetDepthFunction(ECompare depth);

		/**
		* Set the blend function
		* @param src The source function
		* @param dest The destination function
		*/
		void SetBlendFunction(ERenderStateBlendFunction src, ERenderStateBlendFunction dest);

		/**
		* Set the blend operation
		* @param operation The blend operation
		*/
		void SetBlendOperation(ERenderStateBlendOperation operation);

		/**
		* Set the blend color
		* @param color The blend color
		*/
		void SetBlendColor(const CColor & color);

		/**
		* Set the scissor
		* @param rectangle The rectangle
		*/
		void SetScissor(const CRectangleI & rectangle);

		/**
		* Change the viewport
		* @param viewport The viewport
		*/
		void SetViewport(const CRectangleI & viewport);

		/**
		* Begin a rendering process
		*/
		void BeginScene();

		/**
		* End a rendering process
		*/
		void EndScene();

		/**
		* Clear the surfaces
		* @param surfaceFlags The surfaces flags
		* @param color The clear color
		*/
		void Clear(const unsigned int surfaceFlags, const CColor & color);

		/**
		* Draw a primitive
		* @param type The primitive type
		* @param offset The offset
		* @param count The number of elements
		*/
		void DrawPrimitives(EPrimitiveType type, unsigned long offset, unsigned long count) const;

		/**
		* Draw an indexed primitive
		* @param type The primitive type
		* @param offset The offset
		* @param count The number of elements
		*/
		void DrawIndexedPrimitives(EPrimitiveType type, unsigned long offset, unsigned long count) const;

	private:
		/**
		* Check if an extension is available
		* @extension The extension name
		* @return True if the extension is available
		*/
		const bool CheckExtension(const std::string & extension) const;

		/**
		* Get a description of the renderer.
		* @return A string representation of the renderer.
		*/
		const std::string GetRendererDescription() const;

		/**
		* Create a Vertex Buffer
		* @param stride The size of one element
		* @param count The number of elements
		* @param flags The flags
		* @return The vertex buffer
		*/
		IBufferBase * CreateVB(const unsigned long stride, const unsigned long count, const unsigned long flags) const;

		/**
		* Create an index Buffer
		* @param stride The size of one element
		* @param count The number of elements
		* @param flags The flags
		* @return The index buffer
		*/
		IBufferBase * CreateIB(const unsigned long stride, const unsigned long count, const unsigned long flags) const;

		/**
		* Create a vertex declaration
		* @param elements The CE declaration
		* @param size The number of elements
		* @return The vertex declaration
		*/
		DeclarationPtr CreateVD(const SDeclarationElement * elements, const std::size_t size) const;

		/**
		* Create a render target
		* @param size The size
		* @param format The format
		* @return The render target
		*/
		IRenderTargetBase * CreateRT(const CVector2I & size, EPixelFormat format) const;

		/**
		* Check the host capabilities
		*/
		void CheckCapabilities();

		/**
		* Initialize the renderer
		*/
		void Initialize();

		/**
		* Load the extensions
		*/
        void LoadExtensions();

		/**
		* Load an extension
		* @param func The function to receive the extension
		* @param name The extension name
		*/
		template <class T> void LoadExtension(T & func, const std::string name);

	private:
		COGLRenderer();
		COGLRenderer(const COGLRenderer & copy);
		const COGLRenderer & operator=(const COGLRenderer & copy);
	};

	template <class T> inline void COGLRenderer::LoadExtension(T & func, const std::string name)
	{
#ifdef _WIN32
		func = reinterpret_cast<T>(wglGetProcAddress(name.c_str()));
#else
		func = reinterpret_cast<T>(glXGetProcAddress((const GLubyte*)name.c_str()));
#endif
	}
}

#endif
