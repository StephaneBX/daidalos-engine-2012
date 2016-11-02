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

#ifndef __CDX9RENDERER_H
#define __CDX9RENDERER_H

#include <d3d9.h>
#include <d3dx9.h>
#include "../../Core/IRenderer.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class IBufferBase;
class IDeclaration;
class CDX9FrameBuffer;
struct SDeclarationElement;

namespace DE {
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
		~CDX9Renderer();

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
		* Get the DX9 device
		* @return The DX9 device
		*/
		IDirect3DDevice9 * GetDevice() const;

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
		* @param elements The DE frame buffer declaration
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
		* Define a stream frequency
		* @param stream The stream number
		* @param streamType the stream type
		* @param frequency The frequency
		*/
		void SetStreamSourceFrequency(const int stream, EStreamSourceType streamType, const unsigned int frequency);

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
		* Set the alpha reference value
		* @param value The reference value
		*/
		void SetAlphaReference(const unsigned long value);

		/**
		* Set the alpha function
		* @param function The alpha function
		*/
		void SetAlphaFunction(ECompare function);

		/**
		* Set the cull type
		* @param cullType The cull type
		*/
		void SetCullType(ERenderStateCullType cullType);

		/**
		* Set the fill mode
		* @param fillMode The fill mode
		*/
		void SetFillMode(ERenderStateFillMode fillMode);

		/**
		* Set the stencil buffer function
		* @param function The function
		*/
		void SetStencilFunction(ECompare function);

		/**
		* Set the stencil buffer reference value
		* @param ref The reference value
		*/
		void SetStencilRef(const int ref);

		/**
		* Set the stencil buffer mask value
		* @param mask The mask value
		*/
		void SetStencilMask(const int mask);

		/**
		* The stencil operation to perform when the stencil test fail
		* @param operation The operation
		*/
		void SetStencilFail(EStencilOperation operation);

		/**
		* The stencil operation to perform when the stencil test succeed but the z-test failed
		* @param operation The operation
		*/
		void SetStencilZFail(EStencilOperation operation);

		/**
		* The stencil operation to perform when the stencil test succeed
		* @param operation The operation
		*/
		void SetStencilPass(EStencilOperation operation);

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
		* Copy the back buffer to a texture
		* @param texture2D The texture
		*/
		void CopyBackBufferToTexture(ITexture2DBase * texture2D);

		/**
		* Copy a texture to another one
		* @param source The source texture
		* @param destination The destination texture
		*/
		void CopyTextureToTexture(ITexture2DBase * source, ITexture2DBase * destination);

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
		* Check if a format is available
		* @param format The resource format
		* @param resourceType The resource type
		* @param usage The usage flag
		* @return True if th format is available on this host, false otherwise
		*/
		const bool CheckFormat(D3DFORMAT format, D3DRESOURCETYPE resourceType, const unsigned long usage = 0) const;

		/**
		* Create a Vertex Buffer
		* @param stride The size of one element
		* @param count The number of elements
		* @param flags The flags
		* @return The vertex buffer
		*/
		IBufferBase * CreateVB(const unsigned long stride, const unsigned long count, const unsigned long flags) const;

		/**
		* Create an Index Buffer
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

	private:
		CDX9Renderer();
		CDX9Renderer(const CDX9Renderer & copy);
		const CDX9Renderer & operator=(const CDX9Renderer & copy);
	};
}

#endif
