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

#ifndef __IRENDERER_HPP
#define __IRENDERER_HPP

#include <map>
#include "../Core/Enums.hpp"
#include "../Medias/CBuffer.hpp"
#include "../Medias/IShaderProgramBase.hpp"
#include "../Medias/ITexture2DBase.hpp"
#include "../Medias/IFrameBufferBase.hpp"
#include "../Core/IDeclaration.hpp"

namespace daidalosengine {
	/**
	* Screen quad element
	*/
	struct SScreenQuadElement {
		CVector3F vertex;
		CVector2F texcoord;
	};

	/**
	* IRenderer class
	*/
	class IRenderer {
	private:
		static IRenderer * s_instance;

		static std::string s_shaderProgramParameters[];

	protected:
		bool m_verticalSynchro;
		int m_anisotropicFiltering;
		int m_antialiasing;
		std::map<ECapability, bool> m_capabilities;
		IShaderProgramBase * m_currentShaderProgram;
		SScreenQuadElement m_screenQuadElement[SCREEN_QUAD_POINT_SIZE];
		CRectangleI m_displayBounds;
		CVector2I m_resolution;
		STexture2DFilterPolicy m_defaultTextureFilter;
		STexture2DFilterPolicy m_defaultBasicTextureFilter;

		CBuffer m_quadIB;

	public:
		/**
		* Destructor
		*/
		virtual ~IRenderer();

		/**
		* Create a vertex declaration
		* @param elements The CE declaration elements
		* @return The declaration
		*/
		template <std::size_t N> DeclarationPtr CreateVertexDeclaration(const SDeclarationElement(&elements)[N]) const;

		/**
		* Create a vertex buffer
		* @param stride The size of an element
		* @param count The number of elements
		* @param flags The flags
		* @param data The data
		* @return The vertex buffer
		*/
		CBuffer CreateVertexBuffer(const unsigned long stride, const unsigned long count, unsigned long flags, const void * data) const;

		/**
		* Create a vertex buffer
		* @param count The number of elements
		* @param flags The flags
		* @param data The data
		* @return The index buffer
		*/
		CBuffer CreateIndexBuffer(const unsigned long count, unsigned long flags, const int * data) const;

		/**
		* Get the current shader program
		* @return The current shader program
		*/
		IShaderProgramBase * GetCurrentShaderProgram() const;

		/**
		* Get the screen quad
		* @return The screen quad
		*/
		SScreenQuadElement * GetScreenQuad();

		/**
		* Get the default texture filter
		* @return The default textuer filter
		*/
		STexture2DFilterPolicy * GetDefaultTextureFilter();

		/**
		* Get the default basic texture filter
		* @return The default basic textuer filter
		*/
		STexture2DFilterPolicy * GetDefaultBasicTextureFilter();

		/**
		* Get the Index Buffer for a quad (0,1,2,2,3,0)
		* @return The IB
		*/
		const CBuffer & GetQuadIB() const;

		/**
		* Get the resolution
		* @return The resolution
		*/
		const CVector2I & GetResolution() const;

		/**
		* Get a screen quad element
		* @param point The screen quad point
		* @return The screen quad element
		*/
		const SScreenQuadElement & GetScreenQuadElement(EScreenQuadPoint point) const;

		/**
		* Get the scissor rectangle for a given light
		* @param positionRadius The light position and radius in view space
		* @return The scissor rectangle
		*/
		virtual const CRectangleI GetLightScissor(const CVector4F & positionRadius) const = 0;

		/**
		* Get the current viewport
		* @return The viewport
		*/
		virtual const CRectangleI GetViewport() const = 0;

		/**
		* Create a 2D texture
		* @param size The texture size
		* @param filter The texture filter
		* @param format The texture format
		* @param flags The texture flags
		* @return The texture
		*/
		virtual ITexture2DBase * CreateTexture2D(const CVector2I & size, const STexture2DFilterPolicy & filter, EPixelFormat format, const unsigned long flags = 0) const = 0;

		/**
		* Create a frame buffer
		* @param elements The DE frame buffer declaration
		* @param size The number of elements
		* @param depth The DE depth buffer declaration
		* @param size2 The number of depth buffers
		* @return The frame buffer
		*/
		virtual IFrameBufferBase * CreateFrameBuffer(const SFrameBufferDeclaration * elements, const std::size_t size, const SFrameBufferDepthDeclaration * depth, const std::size_t size2) const = 0;

		/**
		* Create a shader program
		* @param name The shader program base name
		* @return The shader program
		*/
		virtual IShaderProgramBase * CreateShaderProgram(const std::string & name) = 0;

		/**
		* Associate a vertex buffer to a stream
		* @param stream The stream number
		* @param buffer The buffer
		* @param stride The size of an element
		* @param offset The offset
		* @param count The number of elements in the buffer
		*/
		virtual void SetVertexBuffer(const int stream, IBufferBase * buffer, const unsigned long stride, const unsigned long offset = 0, const unsigned long count = 0) = 0;

		/**
		* Define a stream frequency
		* @param stream The stream number
		* @param streamType the stream type
		* @param frequency The frequency
		*/
		virtual void SetStreamSourceFrequency(const int stream, EStreamSourceType streamType, const unsigned int frequency) = 0;

		/**
		* Set the current index buffer
		* @param buffer The index buffer
		* @param stride The size of an element
		*/
		virtual void SetIndexBuffer(IBufferBase * buffer, const unsigned long stride) = 0;

		/**
		* Set the shader program
		* @param shaderProgram The shader program
		*/
		virtual void SetShaderProgram(IShaderProgramBase * shaderProgram) = 0;

		/**
		* Set the vertex declaration
		* @param declaration The vertex declaration
		*/
		virtual void SetVertexDeclaration(const IDeclaration * declaration) = 0;

		/**
		* Attach the texture to the unit
		* @param unit The unit number
		* @param texture The texture
		*/
		virtual void SetTexture(const int unit, const ITexture2DBase * texture) = 0;

		/**
		* Set the framebuffer
		* @param frameBuffer The frame buffer
		*/
		virtual void SetFrameBuffer(IFrameBufferBase * frameBuffer) = 0;

		/**
		* Change a render state
		* @param state The state
		* @param value The value
		*/
		virtual void SetRenderState(ERenderStateType state, const bool value) = 0;

		/**
		* Set the depth function
		* @param depth The depth function
		*/
		virtual void SetDepthFunction(ECompare depth) = 0;

		/**
		* Set the blend function
		* @param src The source function
		* @param dest The destination function
		*/
		virtual void SetBlendFunction(ERenderStateBlendFunction src, ERenderStateBlendFunction dest) = 0;

		/**
		* Set the blend operation
		* @param operation The blend operation
		*/
		virtual void SetBlendOperation(ERenderStateBlendOperation operation) = 0;

		/**
		* Set the blend color
		* @param color The blend color
		*/
		virtual void SetBlendColor(const CColor & color) = 0;

		/**
		* Set the alpha reference value
		* @param value The reference value
		*/
		virtual void SetAlphaReference(const unsigned long value) = 0;

		/**
		* Set the alpha function
		* @param function The alpha function
		*/
		virtual void SetAlphaFunction(ECompare function) = 0;

		/**
		* Set the cull type
		* @param cullType The cull type
		*/
		virtual void SetCullType(ERenderStateCullType cullType) = 0;

		/**
		* Set the fill mode
		* @param fillMode The fill mode
		*/
		virtual void SetFillMode(ERenderStateFillMode fillMode) = 0;

		/**
		* Set the stencil buffer function
		* @param function The function
		*/
		virtual void SetStencilFunction(ECompare function) = 0;

		/**
		* Set the stencil buffer reference value
		* @param ref The reference value
		*/
		virtual void SetStencilRef(const int ref) = 0;

		/**
		* Set the stencil buffer mask value
		* @param mask The mask value
		*/
		virtual void SetStencilMask(const int mask) = 0;

		/**
		* The stencil operation to perform when the stencil test fail
		* @param operation The operation
		*/
		virtual void SetStencilFail(EStencilOperation operation) = 0;

		/**
		* The stencil operation to perform when the stencil test succeed but the z-test failed
		* @param operation The operation
		*/
		virtual void SetStencilZFail(EStencilOperation operation) = 0;

		/**
		* The stencil operation to perform when the stencil test succeed
		* @param operation The operation
		*/
		virtual void SetStencilPass(EStencilOperation operation) = 0;

		/**
		* Set the scissor
		* @param rectangle The rectangle
		*/
		virtual void SetScissor(const CRectangleI & rectangle) = 0;

		/**
		* Change the viewport
		* @param viewport The viewport
		*/
		virtual void SetViewport(const CRectangleI & viewport) = 0;

		/**
		* Copy the back buffer to a texture
		* @param texture2D The texture
		*/
		virtual void CopyBackBufferToTexture(ITexture2DBase * texture2D) = 0;

		/**
		* Copy a texture to another one
		* @param source The source texture
		* @param destination The destination texture
		*/
		virtual void CopyTextureToTexture(ITexture2DBase * source, ITexture2DBase * destination) = 0;

		/**
		* Copy the frame buffer depth to the backbuffer
		* @param frameBuffer the source frame buffer
		*/
		virtual void CopyDepthBufferToBackBuffer(IFrameBufferBase * frameBuffer) = 0;

		/**
		* Begin a rendering process
		*/
		virtual void BeginScene() = 0;

		/**
		* End a rendering process
		*/
		virtual void EndScene() = 0;

		/**
		* Clear the surfaces
		* @param surfaceFlags The surfaces flags
		* @param color The clear color
		*/
		virtual void Clear(const unsigned int surfaceFlags, const CColor & color) = 0;

		/**
		* Draw a primitive
		* @param type The primitive type
		* @param offset The offset
		* @param count The number of elements
		*/
		virtual void DrawPrimitives(EPrimitiveType type, unsigned long offset, unsigned long count) const = 0;

		/**
		* Draw an indexed primitive
		* @param type The primitive type
		* @param offset The offset
		* @param count The number of elements
		*/
		virtual void DrawIndexedPrimitives(EPrimitiveType type, unsigned long offset, unsigned long count) const = 0;

		/**
		* Set the display bounds
		* @param rect The display bounds
		*/
		void SetDisplayBounds(const CRectangleI & rect);

		/**
		* Set the resolution
		* @param resolution The resolution
		*/
		void SetResolution(const CVector2I & resolution);

		/**
		* Check if a capability is available
		* @param capability The capability to test
		* @return The capability result
		*/
		const bool HasCapability(ECapability capability) const;

		/**
		* Set a renderer
		* @param renderer The renderer
		*/
		static void SetRenderer(IRenderer * renderer);

		/**
		* Kill the current renderer
		*/
		static void Kill();

		/**
		* Get the current renderer
		* @return The current renderer
		*/
		static IRenderer * GetRenderer();

		/**
		* Get the string for the shader program parameter
		*/
		static const std::string & GetShaderProgramParameterString(EShaderProgramParameter parameter);

	protected:
		/**
		* Constructor
		* @param verticalSynchro The vertical synchronization
		* @param anisotropicFiltering The anisotropic filtering value
		* @param antialiasing The antialiasing value
		*/
		IRenderer(const bool verticalSynchro, const int anisotropicFiltering, const int antialiasing);

		/**
		* Create a veretx buffer
		* @param stride The size of an element
		* @param count The number of elements
		* @param stride The size of one element
		* @param flags The flags
		* @return The vertex buffer
		*/
		virtual IBufferBase * CreateVB(const unsigned long stride, const unsigned long count, const unsigned long flags) const = 0;

		/**
		* Create an index buffer
		* @param stride The size of one element
		* @param count The number of elements
		* @param flags The flags
		* @return The index buffer
		*/
		virtual IBufferBase * CreateIB(const unsigned long stride, const unsigned long count, const unsigned long flags) const = 0;

		/**
		* Create a vertex declaration
		* @param elements The CE declaration
		* @param size The number of elements
		* @return The vertex declaration
		*/
		virtual DeclarationPtr CreateVD(const SDeclarationElement * elements, const std::size_t size) const = 0;

		/**
		* Create a render target
		* @param size The size
		* @param format The format
		* @return The render target
		*/
		virtual IRenderTargetBase * CreateRT(const CVector2I & size, EPixelFormat format) const = 0;

		/**
		* Check the host capabilities
		*/
		virtual void CheckCapabilities() = 0;

	private:
		IRenderer();
		IRenderer(const IRenderer & copy);
		const IRenderer & operator=(const IRenderer & copy);
	};

	template <std::size_t N> DeclarationPtr IRenderer::CreateVertexDeclaration(const SDeclarationElement(&elements)[N]) const {
		return CreateVD(elements, N);
	}
}

#endif
