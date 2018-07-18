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

#ifndef __CDX9RENDERER_HPP
#define __CDX9RENDERER_HPP

#include <d3d9.h>
#include "../../Core/IRenderer.hpp"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace daidalosengine {
	class IBufferBase;
	class IDeclaration;
	class CDX9FrameBuffer;
	struct SDeclarationElement;

	/**
	* DX9 Renderer class
	*/
	class CDX9Renderer : public IRenderer {
	private:
		HWND m_hwnd;
		IDirect3DDevice9 * m_device;
		IDirect3D9 * m_d3d;
		D3DPRESENT_PARAMETERS m_d3dpp;
		CDX9FrameBuffer * m_mainFrameBuffer;
		unsigned long m_minVertexIndex;
		unsigned long m_numVertices;

	public:
		/**
		* Constructor
		* @param verticalSynchro The vertical synchronization
		* @param anisotropicFiltering The anisotropic filtering value
		* @param antialiasing The antialiasing value
		* @param hWnd Handler
		*/
		CDX9Renderer(const bool verticalSynchro, const int anisotropicFiltering, const int antialiasing, const HWND hWnd);

		/**
		* Destructor
		*/
		virtual ~CDX9Renderer();

		/**
		* Get the scissor rectangle for a given light
		* @param positionRadius The light position and radius in view space
		* @return The scissor rectangle
		*/
		virtual const CRectangleI GetLightScissor(const CVector4F & positionRadius) const override;

		/**
		* Get the current viewport
		* @return The viewport
		*/
		virtual const CRectangleI GetViewport() const override;

		/**
		* Get the DX9 device
		* @return The DX9 device
		*/
		virtual IDirect3DDevice9 * GetDevice() const;

		/**
		* Create a 2D texture
		* @param size The texture size
		* @param filter The texture filter policy
		* @param format The texture format
		* @param flags The texture flags
		* @return The texture
		*/
		virtual ITexture2DBase * CreateTexture2D(const CVector2I & size, const STexture2DFilterPolicy & filter, EPixelFormat format, const unsigned long flags = 0) const override;

		/**
		* Create a frame buffer
		* @param elements The DE frame buffer declaration
		* @param size The number of elements
		* @param depth The depth declaration
		* @param size2 The number of elements
		* @return The frame buffer
		*/
		virtual IFrameBufferBase * CreateFrameBuffer(const SFrameBufferDeclaration * elements, const std::size_t size, const SFrameBufferDepthDeclaration * depth, const std::size_t size2) const override;

		/**
		* Create a shader program
		* @param name The shader program base name
		* @return The shader program
		*/
		virtual IShaderProgramBase * CreateShaderProgram(const std::string & name) override;

		/**
		* Associate a vertex buffer to a stream
		* @param stream The stream number
		* @param buffer The buffer
		* @param stride The element stride
		* @param offset The offset
		* @param count The number of elements in the buffer
		*/
		virtual void SetVertexBuffer(const int stream, IBufferBase * buffer, const unsigned long stride, const unsigned long offset = 0, const unsigned long count = 0) override;

		/**
		* Define a stream frequency
		* @param stream The stream number
		* @param streamType the stream type
		* @param frequency The frequency
		*/
		virtual void SetStreamSourceFrequency(const int stream, EStreamSourceType streamType, const unsigned int frequency) override;

		/**
		* Set the current index buffer
		* @param buffer The index buffer
		* @param stride The stride
		*/
		virtual void SetIndexBuffer(IBufferBase * buffer, const unsigned long stride) override;

		/**
		* Set the shader program
		* @param shaderProgram The shader program
		*/
		virtual void SetShaderProgram(IShaderProgramBase * shaderProgram) override;

		/**
		* Set the vertex declaration
		* @param declaration The vertex declaration
		*/
		virtual void SetVertexDeclaration(const IDeclaration * declaration) override;

		/**
		* Attach the texture to the unit
		* @param unit The unit number
		* @param texture The texture
		*/
		virtual void SetTexture(const int unit, const ITexture2DBase * texture) override;

		/**
		* Set the framebuffer
		* @param frameBuffer The frame buffer
		*/
		virtual void SetFrameBuffer(IFrameBufferBase * frameBuffer) override;

		/**
		* Change a render state
		* @param state The state
		* @param value The value
		*/
		virtual void SetRenderState(ERenderStateType state, const bool value) override;

		/**
		* Set the depth function
		* @param depth The depth function
		*/
		virtual void SetDepthFunction(ECompare depth) override;

		/**
		* Set the blend function
		* @param src The source function
		* @param dest The destination function
		*/
		virtual void SetBlendFunction(ERenderStateBlendFunction src, ERenderStateBlendFunction dest) override;

		/**
		* Set the blend operation
		* @param operation The blend operation
		*/
		virtual void SetBlendOperation(ERenderStateBlendOperation operation) override;

		/**
		* Set the blend color
		* @param color The blend color
		*/
		virtual void SetBlendColor(const CColor & color) override;

		/**
		* Set the alpha reference value
		* @param value The reference value
		*/
		virtual void SetAlphaReference(const unsigned long value) override;

		/**
		* Set the alpha function
		* @param function The alpha function
		*/
		virtual void SetAlphaFunction(ECompare function) override;

		/**
		* Set the cull type
		* @param cullType The cull type
		*/
		virtual void SetCullType(ERenderStateCullType cullType) override;

		/**
		* Set the fill mode
		* @param fillMode The fill mode
		*/
		virtual void SetFillMode(ERenderStateFillMode fillMode) override;

		/**
		* Set the stencil buffer function
		* @param function The function
		*/
		virtual void SetStencilFunction(ECompare function) override;

		/**
		* Set the stencil buffer reference value
		* @param ref The reference value
		*/
		virtual void SetStencilRef(const int ref) override;

		/**
		* Set the stencil buffer mask value
		* @param mask The mask value
		*/
		virtual void SetStencilMask(const int mask) override;

		/**
		* The stencil operation to perform when the stencil test fail
		* @param operation The operation
		*/
		virtual void SetStencilFail(EStencilOperation operation) override;

		/**
		* The stencil operation to perform when the stencil test succeed but the z-test failed
		* @param operation The operation
		*/
		virtual void SetStencilZFail(EStencilOperation operation) override;

		/**
		* The stencil operation to perform when the stencil test succeed
		* @param operation The operation
		*/
		virtual void SetStencilPass(EStencilOperation operation) override;

		/**
		* Set the scissor
		* @param rectangle The rectangle
		*/
		virtual void SetScissor(const CRectangleI & rectangle) override;

		/**
		* Change the viewport
		* @param viewport The viewport
		*/
		virtual void SetViewport(const CRectangleI & viewport) override;

		/**
		* Copy the back buffer to a texture
		* @param texture2D The texture
		*/
		virtual void CopyBackBufferToTexture(ITexture2DBase * texture2D) override;

		/**
		* Copy a texture to another one
		* @param source The source texture
		* @param destination The destination texture
		*/
		virtual void CopyTextureToTexture(ITexture2DBase * source, ITexture2DBase * destination) override;

		/**
		* Copy the frame buffer depth to the backbuffer
		* @param frameBuffer the source frame buffer
		*/
		virtual void CopyDepthBufferToBackBuffer(IFrameBufferBase * frameBuffer) override;

		/**
		* Begin a rendering process
		*/
		virtual void BeginScene() override;

		/**
		* End a rendering process
		*/
		virtual void EndScene() override;

		/**
		* Clear the surfaces
		* @param surfaceFlags The surfaces flags
		* @param color The clear color
		*/
		virtual void Clear(const unsigned int surfaceFlags, const CColor & color) override;

		/**
		* Draw a primitive
		* @param type The primitive type
		* @param offset The offset
		* @param count The number of elements
		*/
		virtual void DrawPrimitives(EPrimitiveType type, unsigned long offset, unsigned long count) const override;

		/**
		* Draw an indexed primitive
		* @param type The primitive type
		* @param offset The offset
		* @param count The number of elements
		*/
		virtual void DrawIndexedPrimitives(EPrimitiveType type, unsigned long offset, unsigned long count) const override;

	private:
		/**
		* Check if a format is available
		* @param format The resource format
		* @param resourceType The resource type
		* @param usage The usage flag
		* @return True if th format is available on this host, false otherwise
		*/
		bool CheckFormat(D3DFORMAT format, D3DRESOURCETYPE resourceType, const unsigned long usage = 0) const;

		/**
		* Create a Vertex Buffer
		* @param stride The size of one element
		* @param count The number of elements
		* @param flags The flags
		* @return The vertex buffer
		*/
		virtual IBufferBase * CreateVB(const unsigned long stride, const unsigned long count, const unsigned long flags) const override;

		/**
		* Create an Index Buffer
		* @param stride The size of one element
		* @param count The number of elements
		* @param flags The flags
		* @return The index buffer
		*/
		virtual IBufferBase * CreateIB(const unsigned long stride, const unsigned long count, const unsigned long flags) const override;

		/**
		* Create a vertex declaration
		* @param elements The CE declaration
		* @param size The number of elements
		* @return The vertex declaration
		*/
		virtual DeclarationPtr CreateVD(const SDeclarationElement * elements, const std::size_t size) const override;

		/**
		* Create a render target
		* @param size The size
		* @param format The format
		* @return The render target
		*/
		virtual IRenderTargetBase * CreateRT(const CVector2I & size, EPixelFormat format) const override;

		/**
		* Check the host capabilities
		*/
		void CheckCapabilities() override;

		/**
		* Initialize the renderer
		*/
		void Initialize();

	public:
		CDX9Renderer() = delete;
		CDX9Renderer(const CDX9Renderer & copy) = delete;
		const CDX9Renderer & operator=(const CDX9Renderer & copy) = delete;
	};
}

#endif
