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
#include "../../Core/CMediaManager.hpp"
#include "../../Core/CConfigurationManager.hpp"
#include "../../Core/Logger/ILogger.hpp"
#include "../../Renderers/DX9/DX9Global.hpp"
#include "../../Renderers/DX9/CDX9Exception.hpp"
#include "../../Renderers/DX9/CDX9Enums.hpp"
#include "../../Renderers/DX9/CDX9VertexShader.hpp"
#include "../../Renderers/DX9/CDX9PixelShader.hpp"
#include "../../Renderers/DX9/CDX9PixelShadersLoader.hpp"
#include "../../Renderers/DX9/CDX9VertexShadersLoader.hpp"
#include "../../Renderers/DX9/CDX9Buffer.hpp"
#include "../../Renderers/DX9/CDX9Declaration.hpp"
#include "../../Renderers/DX9/CDX9Texture2D.hpp"
#include "../../Renderers/DX9/CDX9FrameBuffer.hpp"
#include "../../Renderers/DX9/CDX9RenderTarget.hpp"
#include "../../Renderers/DX9/CDX9Renderer.hpp"

using namespace std;

namespace daidalosengine {
	CDX9Renderer::CDX9Renderer(const bool verticalSynchro, const int anisotropicFiltering, const int antialiasing, const HWND hWnd) : IRenderer(verticalSynchro, anisotropicFiltering, antialiasing) {
		LOG_TRACE();

		this->m_hwnd = hWnd;
		Initialize();
		CheckCapabilities();
	}

	CDX9Renderer::~CDX9Renderer() {
		LOG_TRACE();

		CMediaManager::GetInstance()->RemoveLoader<IShaderBase>();
		SAFE_DELETE(this->m_mainFrameBuffer);
	}

	const CRectangleI CDX9Renderer::GetLightScissor(const CVector4F & positionRadius) const {
		LOG_TRACE();

		int rect[4] = { 0, 0, GetResolution().x, GetResolution().y };

		CVector3F l = CVector3F(positionRadius.x, positionRadius.y, positionRadius.z);
		CVector3F l2 = l * l;
		float r = positionRadius.w;
		float r2 = r * r;
		float e = 1.2F;
		float a = GetResolution().x / (float) GetResolution().y;

		float d = r2 * l2.x - (l2.x + l2.z) * (r2 - l2.z);
		if(d > 0) {
			float nx1 = ((r * l.x) + sqrtf(d)) / (l2.x + l2.z);
			float nx2 = ((r * l.x) - sqrtf(d)) / (l2.x + l2.z);

			float nz1 = (r - nx1 * l.x) / l.z;
			float nz2 = (r - nx2 * l.x) / l.z;

			float pz1 = (l2.x + l2.z - r2) / (l.z - (nz1 / nx1) * l.x);
			float pz2 = (l2.x + l2.z - r2) / (l.z - (nz2 / nx2) * l.x);

			if(pz1 > 0) {
				float fx = (-nz1 * e) / nx1;
				int x = (int)((fx + 1) * GetResolution().x * 0.5F);
				float px = -pz1 * nz1 / nx1;

				if(px < l.x) {
					rect[0] = max(rect[0], x);
				} else {
					rect[2] = min(rect[2], x);
				}
			}

			if(pz2 > 0) {
				float fx = (-nz2 * e) / nx2;
				int x = (int)((fx + 1) * GetResolution().x * 0.5F);
				float px = -pz2 * nz2 / nx2;

				if(px < l.x) {
					rect[0] = max(rect[0], x);
				} else {
					rect[2] = min(rect[2], x);
				}
			}
		}

		d = r2 * l2.y - (l2.y + l2.z) * (r2 - l2.z);
		if(d > 0) {
			float ny1 = (r * l.y + sqrtf(d)) / (l2.y + l2.z);
			float ny2 = (r * l.y - sqrtf(d)) / (l2.y + l2.z);

			float nz1 = (r - ny1 * l.y) / l.z;
			float nz2 = (r - ny2 * l.y) / l.z;

			float pz1 = (l2.y + l2.z - r2) / (l.z - (nz1 / ny1) * l.y);
			float pz2 = (l2.y + l2.z - r2) / (l.z - (nz2 / ny2) * l.y);

			if(pz1 > 0) {
				float fy = (nz1 * e * a) / ny1;
				int y = (int)((fy + 1) * GetResolution().y * 0.5F);
				float py = -pz1 * nz1 / ny1;

				if(py < l.y) {
					rect[3] = min(rect[3], y);
				} else {
					rect[1] = max(rect[1], y);
				}
			}

			if(pz2 > 0) {
				float fy = (nz2 * e * a) / ny2;
				int y = (int)((fy + 1) * GetResolution().y * 0.5F);
				float py = -pz2 * nz2 / ny2;

				if(py < l.y) {
					rect[3] = min(rect[3], y);
				} else {
					rect[1] = max(rect[1], y);
				}
			}
		}

		CRectangleI finalRect = CRectangleI(rect[0], rect[1], rect[2] - rect[0], rect[3] - rect[1]);

		// Boundary check
		if(finalRect.GetWidth() <= 0 || finalRect.GetHeight() <= 0) {
			finalRect = CRectangleI(0, 0, GetResolution().x, GetResolution().y);
		}

		return finalRect;
	}

	const CRectangleI CDX9Renderer::GetViewport() const {
		LOG_TRACE();

		D3DVIEWPORT9 viewport;
		this->m_device->GetViewport(&viewport);

		return CRectangleI(viewport.X, viewport.Y, viewport.Width, viewport.Height);
	}

	IDirect3DDevice9 * CDX9Renderer::GetDevice() const {
		LOG_TRACE();

		return this->m_device;
	}

	ITexture2DBase * CDX9Renderer::CreateTexture2D(const CVector2I & size, const STexture2DFilterPolicy & filter, EPixelFormat format, const unsigned long flags) const {
		LOG_TRACE();

		const bool mipmap = (flags & TEXTURE_FLAG_NOMIPMAP) != 0;
		const bool frameBufferType = (flags & TEXTURE_FLAG_FRAMEBUFFER_TYPE) != 0;
		const bool autoMipmap = HasCapability(CAPABILITY_HARDWARE_MIPMAP) && CheckFormat(CDX9Enums::Get(format), D3DRTYPE_TEXTURE, D3DUSAGE_AUTOGENMIPMAP);
		unsigned long usage;
		D3DPOOL pool;

		if(frameBufferType) {
			usage = D3DUSAGE_RENDERTARGET;
			pool = D3DPOOL_DEFAULT;
		} else {
			usage = autoMipmap ? D3DUSAGE_AUTOGENMIPMAP : 0;
			pool = D3DPOOL_MANAGED;
		}

		IDirect3DTexture9 * texture = nullptr;
		if(FAILED(D3DXCreateTexture(this->m_device, size.x, size.y, frameBufferType ? 1 : 0, usage, CDX9Enums::Get(format), pool, &texture))) {
			LOG_ERROR("Unable to create a texture");
			ILogger::Kill();

			throw CDX9Exception("D3DXCreateTexture", "CreateTexture2D");
		}

		return new CDX9Texture2D(size, filter, format, mipmap, autoMipmap, texture);
	}

	IFrameBufferBase * CDX9Renderer::CreateFrameBuffer(const SFrameBufferDeclaration * elements, const std::size_t size, const SFrameBufferDepthDeclaration * depth, const std::size_t size2) const {
		LOG_TRACE();

		CDX9FrameBuffer * frameBuffer = new CDX9FrameBuffer();

		if(elements != nullptr) {
			for(unsigned int i = 0; i < size; ++i) {
				SFrameBufferElement element = { CreateTexture2D(elements[i].size, elements[i].textureFilter, elements[i].format, TEXTURE_FLAG_FRAMEBUFFER_TYPE), CreateRT(elements[i].size, elements[i].format) };
				static_cast<CDX9RenderTarget*>(element.renderTarget)->LinkWithTexture(static_cast<CDX9Texture2D*>(element.texture));

				frameBuffer->AddElement(elements[i].name, element);
			}
		}

		if(depth != nullptr) {
			for(unsigned int i = 0; i < size2; ++i) {
				frameBuffer->AddDepth(depth[i].name, CreateRT(depth[i].size, depth[i].textureFormat));
			}
		}

		return frameBuffer;
	}

	IShaderProgramBase * CDX9Renderer::CreateShaderProgram(const std::string & name) {
		LOG_TRACE();

		return new IShaderProgramBase(name);
	}

	void CDX9Renderer::SetVertexBuffer(const int stream, IBufferBase * buffer, const unsigned long stride, const unsigned long offset, const unsigned long count) {
		LOG_TRACE();

		if(buffer != nullptr) {
			const CDX9VertexBuffer * vertexBuffer = static_cast<const CDX9VertexBuffer *>(buffer);
			this->m_device->SetStreamSource(stream, vertexBuffer->GetBuffer() != nullptr ? vertexBuffer->GetBuffer() : nullptr, offset, stride);
			this->m_minVertexIndex = offset;
			this->m_numVertices = count;
		} else {
			this->m_device->SetStreamSource(stream, nullptr, 0, stride);
		}
	}

	void CDX9Renderer::SetStreamSourceFrequency(const int stream, EStreamSourceType streamType, const unsigned int frequency) {
		LOG_TRACE();

		this->m_device->SetStreamSourceFreq(stream, CDX9Enums::Get(streamType) | frequency);
	}

	void CDX9Renderer::SetIndexBuffer(IBufferBase * buffer, const unsigned long stride) {
		LOG_TRACE();

		if(buffer != nullptr) {
			const CDX9IndexBuffer * indexBuffer = static_cast<const CDX9IndexBuffer *>(buffer);
			this->m_device->SetIndices(indexBuffer->GetBuffer() != nullptr ? indexBuffer->GetBuffer() : nullptr);
		} else {
			this->m_device->SetIndices(nullptr);
		}
	}

	void CDX9Renderer::SetShaderProgram(IShaderProgramBase * shaderProgram) {
		LOG_TRACE();

		SetCurrentShaderProgram(shaderProgram);

		if(shaderProgram != nullptr) {
			const CDX9VertexShader * vertexShader = static_cast<const CDX9VertexShader *>(shaderProgram->GetVertexShader());
			const CDX9PixelShader * pixelShader = static_cast<const CDX9PixelShader *>(shaderProgram->GetPixelShader());

			this->m_device->SetVertexShader(vertexShader ? vertexShader->GetVertexShader() : nullptr);
			this->m_device->SetPixelShader(pixelShader ? pixelShader->GetPixelShader() : nullptr);
		} else {
			this->m_device->SetVertexShader(nullptr);
		}
	}

	void CDX9Renderer::SetVertexDeclaration(const IDeclaration * declaration) {
		LOG_TRACE();

		if(declaration != nullptr) {
			const CDX9Declaration * vertexDeclaration = static_cast<const CDX9Declaration *>(declaration);
			this->m_device->SetVertexDeclaration(vertexDeclaration->GetDeclaration() != nullptr ? vertexDeclaration->GetDeclaration() : nullptr);
		} else {
			this->m_device->SetVertexDeclaration(nullptr);
		}
	}

	void CDX9Renderer::SetTexture(const int unit, const ITexture2DBase * texture) {
		LOG_TRACE();

		if(texture != nullptr) {
			const CDX9Texture2D * dx9Texture = static_cast<const CDX9Texture2D*>(texture);
			this->m_device->SetTexture(unit, dx9Texture->GetTexture());

			// Sampler state update
			this->m_device->SetSamplerState(unit, D3DSAMP_MINFILTER, CDX9Enums::Get(dx9Texture->GetTextureFilterPolicy().minFilter));
			this->m_device->SetSamplerState(unit, D3DSAMP_MAGFILTER, CDX9Enums::Get(dx9Texture->GetTextureFilterPolicy().magFilter));
			this->m_device->SetSamplerState(unit, D3DSAMP_MIPFILTER, CDX9Enums::Get(dx9Texture->GetTextureFilterPolicy().minFilter));
			this->m_device->SetSamplerState(unit, D3DSAMP_ADDRESSU, CDX9Enums::Get(dx9Texture->GetTextureFilterPolicy().addressU));
			this->m_device->SetSamplerState(unit, D3DSAMP_ADDRESSV, CDX9Enums::Get(dx9Texture->GetTextureFilterPolicy().addressV));
			this->m_device->SetSamplerState(unit, D3DSAMP_MAXANISOTROPY, dx9Texture->GetTextureFilterPolicy().maxAnisotropy);
		} else {
			this->m_device->SetTexture(unit, nullptr);
		}
	}

	void CDX9Renderer::SetFrameBuffer(IFrameBufferBase * frameBuffer) {
		LOG_TRACE();

		// Default values for the optional render targets
		for(int i = 1; i < 4; ++i)
			this->m_device->SetRenderTarget(i, nullptr);

		if(frameBuffer != nullptr) {
			// Finalize if necessary
			if(!frameBuffer->IsReady())
				frameBuffer->Finalize();

			// Attach the render targets
			CDX9FrameBuffer * fb = static_cast<CDX9FrameBuffer*>(frameBuffer);
			for(int i = 0; i < fb->GetAttachmentSize(); ++i) {
				this->m_device->SetRenderTarget(fb->GetAttachment(i)->attachment, static_cast<CDX9RenderTarget*>(fb->GetRenderTarget(fb->GetAttachment(i)->name))->GetRenderTarget());
			}

			// Attach the depth stencil
			this->m_device->SetDepthStencilSurface(static_cast<CDX9RenderTarget*>(fb->GetDepthRenderTarget())->GetRenderTarget());
		} else {
			// Default frame buffer
			this->m_device->SetRenderTarget(0, static_cast<CDX9RenderTarget*>(this->m_mainFrameBuffer->GetRenderTarget("default"))->GetRenderTarget());
			this->m_device->SetDepthStencilSurface(static_cast<CDX9RenderTarget*>(this->m_mainFrameBuffer->GetDepthRenderTarget("default"))->GetRenderTarget());
		}
	}

	void CDX9Renderer::SetRenderState(ERenderStateType state, const bool value) {
		LOG_TRACE();

		this->m_device->SetRenderState(CDX9Enums::Get(state), value);
	}

	void CDX9Renderer::SetDepthFunction(ECompare depth) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_ZFUNC, CDX9Enums::Get(depth));
	}

	void CDX9Renderer::SetBlendFunction(ERenderStateBlendFunction src, ERenderStateBlendFunction dest) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_SRCBLEND, CDX9Enums::Get(src));
		this->m_device->SetRenderState(D3DRS_DESTBLEND, CDX9Enums::Get(dest));
	}

	void CDX9Renderer::SetBlendOperation(ERenderStateBlendOperation operation) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_BLENDOP, CDX9Enums::Get(operation));
	}

	void CDX9Renderer::SetBlendColor(const CColor & color) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR(color.ToARGB()));
	}

	void CDX9Renderer::SetAlphaReference(const unsigned long value) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_ALPHAREF, value);
	}

	void CDX9Renderer::SetAlphaFunction(ECompare function) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_ALPHAFUNC, CDX9Enums::Get(function));
	}

	void CDX9Renderer::SetCullType(ERenderStateCullType cullType) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_CULLMODE, CDX9Enums::Get(cullType));
	}

	void CDX9Renderer::SetFillMode(ERenderStateFillMode fillMode) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_FILLMODE, CDX9Enums::Get(fillMode));
	}

	void CDX9Renderer::SetStencilFunction(ECompare function) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_STENCILFUNC, CDX9Enums::Get(function));
	}

	void CDX9Renderer::SetStencilRef(const int ref) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_STENCILREF, ref);
	}

	void CDX9Renderer::SetStencilMask(const int mask) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_STENCILMASK, mask);
	}

	void CDX9Renderer::SetStencilFail(EStencilOperation operation) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_STENCILFAIL, CDX9Enums::Get(operation));
	}

	void CDX9Renderer::SetStencilZFail(EStencilOperation operation) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_STENCILZFAIL, CDX9Enums::Get(operation));
	}

	void CDX9Renderer::SetStencilPass(EStencilOperation operation) {
		LOG_TRACE();

		this->m_device->SetRenderState(D3DRS_STENCILPASS, CDX9Enums::Get(operation));
	}

	void CDX9Renderer::SetScissor(const CRectangleI & rectangle) {
		LOG_TRACE();

		RECT rect = { rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetRight(), rectangle.GetBottom() };
		this->m_device->SetScissorRect(&rect);
	}

	void CDX9Renderer::SetViewport(const CRectangleI & viewport) {
		LOG_TRACE();

		D3DVIEWPORT9 vp;
		vp.X = viewport.GetLeft();
		vp.Y = viewport.GetTop();
		vp.Width = viewport.GetWidth();
		vp.Height = viewport.GetHeight();
		vp.MinZ = 0.0F;
		vp.MaxZ = 1.0F;

		this->m_device->SetViewport(&vp);
	}

	void CDX9Renderer::CopyBackBufferToTexture(ITexture2DBase * texture2D) {
		LOG_TRACE();
		
		const CDX9Texture2D * dx9Texture = static_cast<const CDX9Texture2D*>(texture2D);
		const CDX9RenderTarget * mainBuffer = static_cast<const CDX9RenderTarget*>(this->m_mainFrameBuffer->GetRenderTarget("default"));
		IDirect3DSurface9 * textureSurface = nullptr;
		dx9Texture->GetTexture()->GetSurfaceLevel(0, &textureSurface);

		this->m_device->StretchRect(mainBuffer->GetRenderTarget(), nullptr, textureSurface, nullptr, D3DTEXF_LINEAR);
	}

	/**
	* Copy the frame buffer depth to the backbuffer
	* @param frameBuffer the source frame buffer
	*/
	void CDX9Renderer::CopyDepthBufferToBackBuffer(IFrameBufferBase * frameBuffer) {
		LOG_TRACE();

		if (frameBuffer != nullptr) {
			CDX9RenderTarget * mainBufferDepth = static_cast<CDX9RenderTarget*>(this->m_mainFrameBuffer->GetDepthRenderTarget("default"));
			const CDX9RenderTarget * frameBufferDepth = static_cast<const CDX9RenderTarget*>(frameBuffer->GetDepthRenderTarget("depth"));

			this->m_device->StretchRect(frameBufferDepth->GetRenderTarget(), nullptr, mainBufferDepth->GetRenderTarget(), nullptr, D3DTEXF_LINEAR);
		}
	}

	/**
	* Copy a texture to another one
	* @param source The source texture
	* @param destination The destination texture
	*/
	void CDX9Renderer::CopyTextureToTexture(ITexture2DBase * source, ITexture2DBase * destination) {
		LOG_TRACE();


		IDirect3DSurface9 * sourceTextureSurface = nullptr;
		static_cast<const CDX9Texture2D*>(source)->GetTexture()->GetSurfaceLevel(0, &sourceTextureSurface);

		IDirect3DSurface9 * destinationTextureSurface = nullptr;
		static_cast<const CDX9Texture2D*>(destination)->GetTexture()->GetSurfaceLevel(0, &destinationTextureSurface);

		this->m_device->StretchRect(sourceTextureSurface, nullptr, destinationTextureSurface, nullptr, D3DTEXF_LINEAR);
	}

	void CDX9Renderer::BeginScene() {
		LOG_TRACE();

		this->m_device->BeginScene();
	}

	void CDX9Renderer::EndScene() {
		LOG_TRACE();

		this->m_device->EndScene();
		this->m_device->Present(nullptr, nullptr, nullptr, nullptr);
	}

	void CDX9Renderer::Clear(const unsigned int surfaceFlags, const CColor & color) {
		LOG_TRACE();

		this->m_device->Clear(0, nullptr, CDX9Enums::SurfaceFlags(surfaceFlags), color.ToARGB(), 1.0F, 0);
	}

	void CDX9Renderer::DrawPrimitives(EPrimitiveType type, unsigned long offset, unsigned long count) const {
		LOG_TRACE();

		this->m_device->DrawPrimitive(CDX9Enums::Get(type), offset, count);
	}

	void CDX9Renderer::DrawIndexedPrimitives(EPrimitiveType type, unsigned long offset, unsigned long count) const {
		LOG_TRACE();

		this->m_device->DrawIndexedPrimitive(CDX9Enums::Get(type), 0, this->m_minVertexIndex, this->m_numVertices, offset, count);
	}

	const bool CDX9Renderer::CheckFormat(D3DFORMAT format, D3DRESOURCETYPE resourceType, const unsigned long usage) const {
		LOG_TRACE();

		return SUCCEEDED(this->m_d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, usage, resourceType, format));
	}

	IBufferBase * CDX9Renderer::CreateVB(const unsigned long stride, const unsigned long count, const unsigned long flags) const {
		LOG_TRACE();

		LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
		if(FAILED(this->m_device->CreateVertexBuffer(count * stride, CDX9Enums::BufferFlags(flags), 0, D3DPOOL_DEFAULT, &vertexBuffer, nullptr))) {
			LOG_ERROR("Unable to create a vertex buffer");
			ILogger::Kill();

			throw CDX9Exception("CreateVertexBuffer", "CreateVB");
		}
		return new CDX9VertexBuffer(count * stride, vertexBuffer);
	}

	IBufferBase * CDX9Renderer::CreateIB(const unsigned long stride, const unsigned long count, const unsigned long flags) const {
		LOG_TRACE();

		LPDIRECT3DINDEXBUFFER9 indexBuffer;
		if(FAILED(this->m_device->CreateIndexBuffer(count * stride, CDX9Enums::BufferFlags(flags), D3DFMT_INDEX32, D3DPOOL_DEFAULT, &indexBuffer, nullptr))) {
			LOG_ERROR("Unable to create an index buffer");
			ILogger::Kill();

			throw CDX9Exception("CreateIndexBuffer", "CreateIB");
		}

		return new CDX9IndexBuffer(count * stride, indexBuffer);
	}

	DeclarationPtr CDX9Renderer::CreateVD(const SDeclarationElement * elements, const std::size_t size) const {
		LOG_TRACE();

		std::vector<D3DVERTEXELEMENT9> dx9Elements;
		std::vector<int> offset(size, 0);

		for(std::size_t i = 0; i < size; ++i) {
			D3DVERTEXELEMENT9 currentElement;

			currentElement.Stream = elements[i].stream;
			currentElement.Offset = offset[elements[i].stream];
			currentElement.Method = D3DDECLMETHOD_DEFAULT;

			// Usage
			switch(elements[i].usage) {
			case DECLARATION_USAGE_POSITION0:  currentElement.Usage = D3DDECLUSAGE_POSITION; currentElement.UsageIndex = 0; break;
			case DECLARATION_USAGE_POSITION1:  currentElement.Usage = D3DDECLUSAGE_POSITION; currentElement.UsageIndex = 1; break;
			case DECLARATION_USAGE_POSITION2:  currentElement.Usage = D3DDECLUSAGE_POSITION; currentElement.UsageIndex = 2; break;
			case DECLARATION_USAGE_POSITION3:  currentElement.Usage = D3DDECLUSAGE_POSITION; currentElement.UsageIndex = 3; break;
			case DECLARATION_USAGE_NORMAL0:    currentElement.Usage = D3DDECLUSAGE_NORMAL;   currentElement.UsageIndex = 0; break;
			case DECLARATION_USAGE_NORMAL1:    currentElement.Usage = D3DDECLUSAGE_NORMAL;   currentElement.UsageIndex = 1; break;
			case DECLARATION_USAGE_NORMAL2:    currentElement.Usage = D3DDECLUSAGE_NORMAL;   currentElement.UsageIndex = 2; break;
			case DECLARATION_USAGE_NORMAL3:    currentElement.Usage = D3DDECLUSAGE_NORMAL;   currentElement.UsageIndex = 3; break;
			case DECLARATION_USAGE_TEXCOORD0:	currentElement.Usage = D3DDECLUSAGE_TEXCOORD; currentElement.UsageIndex = 0; break;
			case DECLARATION_USAGE_TEXCOORD1:	currentElement.Usage = D3DDECLUSAGE_TEXCOORD; currentElement.UsageIndex = 1; break;
			case DECLARATION_USAGE_TEXCOORD2:	currentElement.Usage = D3DDECLUSAGE_TEXCOORD; currentElement.UsageIndex = 2; break;
			case DECLARATION_USAGE_TEXCOORD3:	currentElement.Usage = D3DDECLUSAGE_TEXCOORD; currentElement.UsageIndex = 3; break;
			case DECLARATION_USAGE_COLOR0:		currentElement.Usage = D3DDECLUSAGE_COLOR;    currentElement.UsageIndex = 0; break;
			case DECLARATION_USAGE_COLOR1:		currentElement.Usage = D3DDECLUSAGE_COLOR;    currentElement.UsageIndex = 1; break;
			case DECLARATION_USAGE_COLOR2:		currentElement.Usage = D3DDECLUSAGE_COLOR;    currentElement.UsageIndex = 2; break;
			case DECLARATION_USAGE_COLOR3:		currentElement.Usage = D3DDECLUSAGE_COLOR;    currentElement.UsageIndex = 3; break;
			default: break;
			}

			// Type
			switch(elements[i].type) {
			case DECLARATION_TYPE_FLOAT1: currentElement.Type = D3DDECLTYPE_FLOAT1;	offset[elements[i].stream] += 4;  break;
			case DECLARATION_TYPE_FLOAT2: currentElement.Type = D3DDECLTYPE_FLOAT2;	offset[elements[i].stream] += 8;  break;
			case DECLARATION_TYPE_FLOAT3: currentElement.Type = D3DDECLTYPE_FLOAT3;	offset[elements[i].stream] += 12; break;
			case DECLARATION_TYPE_FLOAT4: currentElement.Type = D3DDECLTYPE_FLOAT4;	offset[elements[i].stream] += 16; break;
			case DECLARATION_TYPE_COLOR: currentElement.Type = D3DDECLTYPE_FLOAT4;  offset[elements[i].stream] += 16; break;
			default: break;
			}

			dx9Elements.push_back(currentElement);
		}

		D3DVERTEXELEMENT9 EndElement = D3DDECL_END();
		dx9Elements.push_back(EndElement);

		LPDIRECT3DVERTEXDECLARATION9 declaration = nullptr;
		if(FAILED(this->m_device->CreateVertexDeclaration(&dx9Elements[0], &declaration))) {
			LOG_ERROR("Unable to create a vertex declaration");
			ILogger::Kill();

			throw CDX9Exception("CreateVertexDeclaration", "CreateVD");
		}

		return std::shared_ptr<IDeclaration>(new CDX9Declaration(declaration));
	}

	IRenderTargetBase * CDX9Renderer::CreateRT(const CVector2I & size, EPixelFormat format) const {
		LOG_TRACE();

		IDirect3DSurface9 * dss = nullptr;
		ERenderTargetType type;
		CDX9RenderTarget * RT;
		switch(format) {
		case PIXEL_FORMAT_DEPTH24S8:
		case PIXEL_FORMAT_DEPTH16:
		case PIXEL_FORMAT_DEPTH32:
			type = RENDER_TARGET_TYPE_DEPTH;
			this->m_device->CreateDepthStencilSurface(size.x, size.y, CDX9Enums::Get(format), D3DMULTISAMPLE_NONE, 0, TRUE, &dss, NULL);
			RT = new CDX9RenderTarget(dss, type);
			break;

		default:
			type = RENDER_TARGET_TYPE_COLOR;
			RT = new CDX9RenderTarget(type);
			break;
		}

		return RT;
	}

	void CDX9Renderer::CheckCapabilities() {
		LOG_TRACE();

		D3DCAPS9 capabilities;
		this->m_device->GetDeviceCaps(&capabilities);

		SetCapability(CAPABILITY_HARDWARE_MIPMAP, (capabilities.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) != 0);
		SetCapability(CAPABILITY_HARDWARE_DXT_COMPRESSION, CheckFormat(D3DFMT_DXT1, D3DRTYPE_TEXTURE) &&
			CheckFormat(D3DFMT_DXT3, D3DRTYPE_TEXTURE) &&
			CheckFormat(D3DFMT_DXT5, D3DRTYPE_TEXTURE));
		SetCapability(CAPABILITY_HARDWARE_NON_POW2_TEXTURES, ((capabilities.TextureCaps & D3DPTEXTURECAPS_POW2) == 0) &&
			((capabilities.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) == 0));
		SetCapability(CAPABILITY_HARDWARE_MRT, (capabilities.NumSimultaneousRTs > 1) &&
			((capabilities.PrimitiveMiscCaps & D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS) != 0) &&
			((capabilities.PrimitiveMiscCaps & D3DPMISCCAPS_MRTPOSTPIXELSHADERBLENDING) != 0));
		SetCapability(CAPABILITY_HARDWARE_MULTITEXTURE, capabilities.MaxSimultaneousTextures > 1);
		SetCapability(CAPABILITY_HARDWARE_ADVANCED_BUFFERS, true);
	}

	void CDX9Renderer::Initialize() {
		LOG_TRACE();

		// Options
		SiniP options = CConfigurationManager::GetInstance()->GetConfiguration(APPLICATION_OPTIONS);

		CMediaManager::GetInstance()->AddLoader(new CDX9PixelShadersLoader(), "psh");
		CMediaManager::GetInstance()->AddLoader(new CDX9VertexShadersLoader(), "vsh");
		CMediaManager::GetInstance()->AddSearchPath("Resources/Shaders/DX9/");

		ZeroMemory(&this->m_d3dpp, sizeof(this->m_d3dpp));

		RECT rect;
		GetClientRect(this->m_hwnd, &rect);

		UINT adapterToUse = D3DADAPTER_DEFAULT;
		D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;

		this->m_d3dpp.Windowed = true;
		this->m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		this->m_d3dpp.hDeviceWindow = this->m_hwnd;
		this->m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		this->m_d3dpp.BackBufferWidth = rect.right - rect.left;
		this->m_d3dpp.BackBufferHeight = rect.bottom - rect.top;
		this->m_d3dpp.BackBufferCount = options.GetValueInt(APPLICATION_OPTION_SECTION_GENERAL, APPLICATION_OPTION_GENERAL_BACKBUFFERS, 2);
		this->m_d3dpp.EnableAutoDepthStencil = true;
		this->m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

		this->m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

		if(IsVerticalSynchro())
			this->m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		else
			this->m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


#ifdef PERFHUD_VERSION
		for(UINT Adapter = 0; Adapter < this->m_d3d->GetAdapterCount(); Adapter++) {
			D3DADAPTER_IDENTIFIER9  Identifier;
			HRESULT Res;

			Res = this->m_d3d->GetAdapterIdentifier(Adapter, 0, &Identifier);
			if(strstr(Identifier.Description, "PerfHUD") != 0) {
				adapterToUse = Adapter;
				deviceType = D3DDEVTYPE_REF;
				break;
			}
		}
#endif

		this->m_d3d = Direct3DCreate9(D3D_SDK_VERSION);

		if(FAILED(this->m_d3d->CreateDevice(adapterToUse, deviceType, this->m_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_MULTITHREADED, &this->m_d3dpp, &this->m_device))) {
			LOG_ERROR("Unable to create the device");
			ILogger::Kill();

			throw CDX9Exception("CreateDevice", "Initialize");
		}

		this->m_device->SetRenderState(D3DRS_DITHERENABLE, true);
		this->m_device->SetRenderState(D3DRS_LIGHTING, false);
		this->m_device->SetRenderState(D3DRS_ZENABLE, true);
		this->m_device->SetRenderState(D3DRS_FOGENABLE, false);
		this->m_device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		this->m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		this->m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		this->m_device->SetRenderState(D3DRS_STENCILENABLE, false);
		this->m_device->SetRenderState(D3DRS_COLORWRITEENABLE , D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED);

		this->m_minVertexIndex = 0;
		this->m_numVertices = 0;

		IDirect3DSurface9 * mainRenderTarget;
		IDirect3DSurface9 * mainDSS;
		this->m_device->GetRenderTarget(0, &mainRenderTarget);
		this->m_device->GetDepthStencilSurface(&mainDSS);
		SFrameBufferElement element = { nullptr, new CDX9RenderTarget(mainRenderTarget, RENDER_TARGET_TYPE_COLOR) };

		// Get the default frame buffer
		this->m_mainFrameBuffer = new CDX9FrameBuffer();
		this->m_mainFrameBuffer->AddElement("default", element);
		this->m_mainFrameBuffer->AddDepth("default", new CDX9RenderTarget(mainDSS, RENDER_TARGET_TYPE_DEPTH));

		// Canvas resolution
		CRectangleI viewport = GetViewport();
		SetResolution(CVector2I(viewport.GetWidth(), viewport.GetHeight()));

		// Screen quad information
		SScreenQuadElement sq[] =
		{
			{ CVector3F(1, -1, 0), CVector2F(1, 1) },
			{ CVector3F(-1, -1, 0), CVector2F(0, 1) },
			{ CVector3F(-1, 1, 0), CVector2F(0, 0) },
			{ CVector3F(1, 1, 0), CVector2F(1, 0) }
		};
		memcpy(GetScreenQuad(), sq, sizeof(SScreenQuadElement) * 4);

		// Quad Index Buffer
		int indices[] = { 0, 1, 2, 2, 3, 0 };
		SetCurrentScreenQuadIB(CreateIndexBuffer(6, 0, indices));
	}
}
